/**
 *******************************************************************************
 * \file        app.cpp
 * \brief       app library source file
 * \date        2024/07/22
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Includes ***/
#include "app.h"

/*** Constructor implementation ***/
App::App() {
    // Initialize constructor logic here
}

/*** Destructor implementation ***/
App::~App() {
    // Cleanup resources if any
}

/*** Public Functions definitions ***/
void App::init() {
    calibrateSensors();
    setLaunchReady();
}

void App::update() {
    filterMeasurements();
    setrxTelemetry();
    setTxTelemetry();

    if (m_flightStage == IDLE) {

        detectLaunch();
    }
    if (m_flightStage == LAUNCH) {
        setFlash();

        detectBurnout();
    }
    if (m_flightStage == BURNOUT) {
        setFlash();

        detectApogee();
    }
    if (m_flightStage == APOGEE) {
        deployCharges();

        detectLanding();        
    }
    if (m_flightStage == LANDED) {
        setSD();

    }
}

/*** Private Functions definitions ***/