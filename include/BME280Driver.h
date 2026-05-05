#ifndef BME280DRIVER_H
#define BME280DRIVER_H

#include <cstdint>

// Raw sensor readings returned by BME280Driver::read()
struct SensorReadings {
    float temperature;  
    float humidity;     
    float pressure;   
};

class BME280Driver {
private:
    int fd;  // file descriptor for /dev/i2c-1

    // Bosch factory calibration coefficients
    uint16_t dig_T1;
    int16_t  dig_T2, dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
    uint8_t  dig_H1, dig_H3;
    int16_t  dig_H2, dig_H4, dig_H5;
    int8_t   dig_H6;

    void read_calibration(); 

public:
    BME280Driver();
    bool          begin();
    SensorReadings read();
};

#endif