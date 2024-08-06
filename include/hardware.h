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
#include "sensors.h"
#include "comms.h"
#include "memory.h"

/*** Class declaration ***/
class Hardware
{
public:
    Hardware();  // Constructor
    ~Hardware(); // Destructor

    int init();
    void update();

private:

    int m_pyroCheck();
    int m_batteryCheck();
    int m_buzzerUpdate();
    int m_pyroUpdate();
};

extern Hardware myHardware;

#endif
