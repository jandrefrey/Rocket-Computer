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
#define     USE_PYRO_1
//#define     USE_PYRO_2
#define     PYRO_ON_TIME        500000

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
    //Sensors::measure_s mymeasurements;

    typedef enum {
        IDLE = 0,
        SLOW_PULSE,
        QUICK_BEEPS,
        SWEEPING_PULSE
    } buzzerMode_t;
    buzzerMode_t buzzerMode;

    float m_batteryCheck();
    uint8_t m_pyroCheck();

    static void pyroExpire();

    bool pyro1State;
    bool pyro2State;

    bool pyroDeploy;

private:

    int m_buzzerUpdate();
    int m_pyroUpdate();

    uint32_t pyroTime;

    uint32_t slowPulseTime;
    uint32_t quickBeepsTime;
    uint32_t quickBeepscounter;
    uint32_t sweepingPulseCounter;
    uint32_t sweepingPulseTime;
};

extern Hardware myHardware;

#endif
