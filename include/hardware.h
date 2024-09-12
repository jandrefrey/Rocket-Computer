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

#define     BATTERY_MINIMUM     5.5
#define     PYRO_PIN_1          21
#define     PYRO_PIN_2          23
//#define     USE_PYRO_1
//#define     USE_PYRO_2

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

    Comms::message_s mymessage;
    Sensors::measure_s mymeasurements;

private:

    int m_pyroCheck();
    int m_batteryCheck();
    int m_buzzerUpdate();
    int m_pyroUpdate();
};

extern Hardware myHardware;

#endif
