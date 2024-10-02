/**
 *******************************************************************************
 * \file        memory.h
 * \brief       memory library include file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Prevent Recursive Inclusion ***/
#ifndef MEMORY_H
#define MEMORY_H

#define     SD_CS       36

/*** Includes ***/
#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>

/*** Class declaration ***/
class Memory
{
public:
    Memory();  // Constructor
    ~Memory(); // Destructor

    int sdInit();
    //int flashInit();
    //int writeFlash();
    int logTelemetry(float accel[], float gyro[], float magn[], float baro_raw, float baro_filt, uint32_t time);
    //int setFlash();
    int logSD(const char text[]);

    File logFile;

private:
    File telemetryFile;
};

extern Memory m_mem;

#endif