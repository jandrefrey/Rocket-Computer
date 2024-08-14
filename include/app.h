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

#define DEBUG

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

private:
    typedef enum {
        IDLE,
        LAUNCH,
        BURNOUT,
        APOGEE,
        LANDED
    } flightStage_t;
    flightStage_t m_flightStage;

    int m_setLaunchReady();
    int m_detectLaunch();
    int detectBurnout();
    int detectApogee();
    int m_deployCharges();
    int m_detectLanding();
};

extern App myApp;

#endif
