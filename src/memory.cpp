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
        myFile = SD.open("DATA.txt", FILE_WRITE);
        if (myFile) {
            myFile.println("SU MODEL ROCKET DATA:");
            // close the file:
            myFile.close();
            Serial.println("SD init done");
        } else {
            // if the file didn't open, print an error:
            Serial.println("error opening test.txt");
            return 2;
        }
    }

    //FLASH INIT



    return 0;
}

int Memory::writeFlash() {
    return 0;
}

int Memory::writeSD() {
    return 0;
}

int Memory::setFlash() {
    return 0;
}

int Memory::setSD() {
    return 0;    
}
/*** Private Functions definitions ***/



Memory m_mem;