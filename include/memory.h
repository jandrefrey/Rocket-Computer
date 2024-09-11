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

/*** Class declaration ***/
class Memory
{
public:
    Memory();  // Constructor
    ~Memory(); // Destructor

    int init();
    int writeFlash();
    int writeSD();
    int setFlash();
    int setSD();

private:
    File myFile;
};

extern Memory m_mem;

#endif