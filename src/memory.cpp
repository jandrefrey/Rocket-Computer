/**
 *******************************************************************************
 * \file        memory.cpp
 * \brief       memory library source file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Includes ***/
#include "memory.h"

/*** Constructor implementation ***/
Memory::Memory()
{
    // Initialize deviceId or other constructor logic here
}

/*** Destructor implementation ***/
Memory::~Memory()
{
    // Cleanup resources if any
}

/*** Public Functions definitions ***/
int Memory::init() {
    //SD INIT
    if (!SD.sdfs.begin(SdSpiConfig(SD_CS, SHARED_SPI, SD_SCK_MHZ(16), &SPI2))) {
        Serial.println("SD initialization failed!");
        return 1;
    } else {
        SD.format();
        telemetryFile = SD.open("TELEMETRY.csv", FILE_WRITE);
        if (telemetryFile) {
            telemetryFile.println("Time,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,Raw Baro,Filtered Baro,MagnX,MagY,MagZ");
            telemetryFile.close();
        } else {
            // if the file didn't open, print an error:
            Serial.println("error opening TELEMETRY.csv");
            return 2;
        }

        logFile = SD.open("LOG.txt", FILE_WRITE);
        if (logFile) {
            logFile.println("-------------------------SU ROCKET COMPUTER LOG-------------------------");
            logFile.close();
        } else {
            // if the file didn't open, print an error:
            Serial.println("LOG.txt");
            return 2;
        }
        Serial.println("SD init done");
        return 0;
    }

    //FLASH INIT



    return 0;
}

int Memory::writeFlash() {
    return 0;
}

int Memory::logTelemetry(float accel[], float gyro[], float magn[], float baro_raw, float baro_filt, uint32_t time) {
    telemetryFile = SD.open("TELEMETRY.csv", FILE_WRITE);
    //telemetryFile.printf("%d:%d:%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", 
    //(uint8_t)(time/60000), (uint8_t)(time%60000/1000), (uint8_t)((time%60000)%1000), accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2], baro_raw, baro_filt, magn[0], magn[1], magn[2]);
    telemetryFile.printf("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n", 
    ((float)time)/1000, accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2], baro_raw, baro_filt, magn[0], magn[1], magn[2]);
    telemetryFile.close();

    return 0;
}

int Memory::setFlash() {
    return 0;
}

int Memory::logSD(const char text[]) {
    logFile = SD.open("LOG.txt", FILE_WRITE);
    logFile.printf("%.3f    ", ((float)millis())/1000);
    logFile.println(text);
    logFile.close();
    return 0;    
}
/*** Private Functions definitions ***/



Memory m_mem;