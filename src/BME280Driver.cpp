#include "BME280Driver.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

BME280Driver::BME280Driver():fd(-1){};

void BME280Driver::read_calibration(){
    uint8_t reg = 0x88;
    write(fd, &reg, 1);
    
    uint8_t calib[24];
    ::read(fd, calib, 24);

    dig_T1 = (calib[1]<<8)|calib[0];
    dig_T2 = (calib[3]<<8)|calib[2];
    dig_T3 = (calib[5]<<8)|calib[4];
    dig_P1 = (calib[7]<<8)|calib[6];
    dig_P2 = (calib[9]<<8)|calib[8];
    dig_P3 = (calib[11]<<8)|calib[10];
    dig_P4 = (calib[13]<<8)|calib[12];
    dig_P5 = (calib[15]<<8)|calib[14];
    dig_P6 = (calib[17]<<8)|calib[16];
    dig_P7 = (calib[19]<<8)|calib[18];
    dig_P8 = (calib[21]<<8)|calib[20];
    dig_P9 = (calib[23]<<8)|calib[22];

    uint8_t hreg1 = 0xA1;
    write(fd, &hreg1, 1);
    ::read(fd, &dig_H1, 1);

    uint8_t hreg2 = 0xE1;
    write(fd, &hreg2, 1);
    uint8_t hcalib[7];
    ::read(fd, hcalib, 7);

    dig_H2 = (hcalib[1] << 8) | hcalib[0];
    dig_H3 = hcalib[2];
    dig_H4 = (hcalib[3] << 4) | (hcalib[4] & 0x0F);
    dig_H5 = (hcalib[5] << 4) | (hcalib[4] >> 4);
    dig_H6 = hcalib[6];
}

bool BME280Driver::begin(){
    this ->fd = open("/dev/i2c-1",O_RDWR);
    if(fd==-1){
        return false;
    }
    ioctl(fd, I2C_SLAVE, 0x76);
    char config[] = {0xF4, 0x27};
    write(fd,config,2);
    read_calibration();
    return true;
}

SensorReadings BME280Driver::read(){
    uint8_t reg = 0xF7;
    write(fd,&reg,1);
    uint8_t data[8];
    ::read(fd, data,8);

    int32_t raw_pressure = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) | (data[2] >> 4);
    int32_t raw_temperature = ((int32_t)data[3] << 12) | ((int32_t)data[4] << 4) | (data[5] >> 4);
    int32_t raw_humidity = ((int32_t)data[6] << 8) | data[7];

    double var1 = (raw_temperature / 16384.0 - dig_T1 / 1024.0) * dig_T2;
    double var2 = (raw_temperature / 131072.0 - dig_T1 / 8192.0) * (raw_temperature / 131072.0 - dig_T1 / 8192.0) * dig_T3;
    int32_t t_fine = var1 + var2;
    float temperature = t_fine / 5120.0;

    double p_var1 = t_fine / 2.0 - 64000.0;
    double p_var2 = p_var1 * p_var1 * dig_P6 / 32768.0;
    p_var2 = p_var2 + p_var1 * dig_P5 * 2.0;
    p_var2 = p_var2 / 4.0 + dig_P4 * 65536.0;
    p_var1 = (dig_P3 * p_var1 * p_var1 / 524288.0 + dig_P2 * p_var1) / 524288.0;
    p_var1 = (1.0 + p_var1 / 32768.0) * dig_P1;
    double pressure = 1048576.0 - raw_pressure;
    pressure = (pressure - p_var2 / 4096.0) * 6250.0 / p_var1;
    pressure = pressure + (dig_P9 * pressure * pressure / 2147483648.0 + dig_P8 * pressure / 32768.0 + dig_P7) / 16.0;

    double humidity = t_fine - 76800.0;
    humidity = (raw_humidity - (dig_H4 * 64.0 + dig_H5 / 16384.0 * humidity)) *
            (dig_H2 / 65536.0 * (1.0 + dig_H6 / 67108864.0 * humidity *
            (1.0 + dig_H3 / 67108864.0 * humidity)));
    humidity = humidity * (1.0 - dig_H1 * humidity / 524288.0);
    if(humidity > 100.0) humidity = 100.0;
    if(humidity < 0.0) humidity = 0.0;
    
    SensorReadings result;
    result.temperature = temperature;
    result.pressure = pressure;
    result.humidity = humidity;
    return result;
}


