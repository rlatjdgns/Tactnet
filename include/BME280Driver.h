#ifndef BME280DRIVER_H
#define BME280DRIVER_H
#include <cstdint>
struct SensorReadings{
    float temperature;
    float pressure;
    float humidity; 
};

class BME280Driver{
private:
    int fd;
    uint16_t dig_T1; //0 to 65535
    int16_t dig_T2, dig_T3; //-32768 to 32767 
    uint16_t dig_P1; //0 to 65535
    int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9; //-32768 to 32767
    uint8_t dig_H1, dig_H3; //0 to 255
    int16_t dig_H2, dig_H4, dig_H5; //-32768 to 32767
    int8_t dig_H6; //-128 to 127
    void read_calibration();

public:
    BME280Driver();
    bool begin(); 
    SensorReadings read();
};
#endif