/**
 *******************************************************************************
 * \file        hardware.h
 * \brief       hardware library include file
 * \date        2024/07/21
 * \author      Jandre Frey
 *******************************************************************************
 */


/*** Prevent Recursive Inclusion ***/
#ifndef HARDWARE_H
#define HARDWARE_H


/*** Includes ***/
#include <Arduino.h>

/*** Class declaration ***/
class hardware_c {
public:
    hardware_c(); // Constructor
    ~hardware_c(); // Destructor

    void init();

private:
    int deviceId;
};

#endif
