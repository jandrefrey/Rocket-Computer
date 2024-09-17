/**
 *******************************************************************************
 * \file        sensors.h
 * \brief       sensors library include file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Prevent Recursive Inclusion ***/
#ifndef SENSORS_H
#define SENSORS_H

/*** Includes ***/
#include <Arduino.h>
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <SparkFun_MMC5983MA_Arduino_Library.h>


#define LSM_CS 8
#define LSM_SCK 13
#define LSM_MISO 12
#define LSM_MOSI 11

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

#define SENSOR_READ_RATE 10000 //Microseconds

#define MMC_CS 9
//Other pins?

/*** Class declaration ***/
class Sensors
{
public:

    struct measure_s {
        float accel[3];
        float gyro[3];
        float mag[3];
        float bpressure;
        int32_t time; 
    };

    struct telemetry_s {
        int16_t position[3];    //xyz
        int16_t velocity[3];
        int16_t attitude[3];
        int32_t time;           //millis
        uint8_t pyroStates;
        uint8_t batVoltage;
        uint8_t errorCode;
    };

    measure_s mymeasurements;
    telemetry_s myTelemetry;
    IntervalTimer myTimer;

    Sensors();  // Constructor
    ~Sensors(); // Destructor

    int init();
    int calib();
    int measure();
    float filter();
    float bpresurre_filtered;
    float bpresurre_filtered_prev;
    // int integrateMeasures();
    static void getMeasures();


private:

    int lsm32Init();
    int bmp390Init();
    int mmc598Init();

    Adafruit_LSM6DSO32 dso32;
    Adafruit_BMP3XX bmp;
    SFE_MMC5983MA myMag;

    // uint32_t _time_old;
    // uint32_t _time_new;
    // float _accel_new[3];
    // float _accel_old[3];
    // float _vel_old[3];
    // float _gyro_old[3];

};

extern Sensors m_sensors;

#endif