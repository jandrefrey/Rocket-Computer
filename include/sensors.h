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

#define MMC_CS 9
//Other pins?

/*** Class declaration ***/
class Sensors
{
public:

    struct measure_s {
        float accelx;
        float accely;
        float accelz;
        float gyrox;
        float gyroy;
        float gyroz;
        float magx;
        float magy;
        float magz;
        float alt;
    };

    Sensors();  // Constructor
    ~Sensors(); // Destructor

    int init();
    int calib();
    int measure(measure_s measurement);
    int filter();

private:

    int lsm32Init();
    int bmp390Init();
    int mmc598Init();

    Adafruit_LSM6DSO32 dso32;
    Adafruit_BMP3XX bmp;
    SFE_MMC5983MA myMag;
};

extern Sensors m_sensors;

#endif