/**
 *******************************************************************************
 * \file        app.h
 * \brief       app library header file
 * \date        2024/07/22
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Prevent Recursive Inclusion ***/
#ifndef APP_H
#define APP_H

#define     TX_FREQ     1000
#define     LOG_FREQ    1
#define     DEBUG

/*** Includes ***/
#include <Arduino.h>
#include "sensors.h"
#include "comms.h"
#include "memory.h"
#include "hardware.h"

/*** Class declaration ***/
class App {
public:
    App(); // Constructor
    ~App(); // Destructor

    void init();
    void update();

    typedef enum {
        IDLE,
        LAUNCH,
        BURNOUT,
        APOGEE,
        LANDED
    } flightStage_t;
    flightStage_t m_flightStage;

    uint32_t timeDebug;

private:

    uint32_t prevTickTX;
    uint32_t prevTickSD;

    float pressurePrevPrev;
    float pressurePrev;
    bool apogeeDone;
    uint32_t apogeeCounter;

    int m_setLaunchReady();
    int m_detectLaunch();
    int detectBurnout();
    int detectApogee(float pressure);
    int m_deployCharges();
    int m_detectLanding();
};

extern App myApp;

#endif
