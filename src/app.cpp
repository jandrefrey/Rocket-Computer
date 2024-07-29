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
app_c::app_c() {
    // Initialize constructor logic here
}

/*** Destructor implementation ***/
app_c::~app_c() {
    // Cleanup resources if any
}

/*** Public Functions definitions ***/
void app_c::init() {
    calibrateSensors();
    setLaunchReady();
}

void app_c::update() {
    filterMeasurements();
    setrxTelemetry();
    setTxTelemetry();

    if (flightStage == IDLE) {

        detectLaunch();
    }
    if (flightStage == LAUNCH) {
        setFlash();

        detectBurnout();
    }
    if (flightStage == BURNOUT) {
        setFlash();

        detectApogee();
    }
    if (flightStage == APOGEE) {
        deployCharges();

        detectLanding();        
    }
    if (flightStage == LANDED) {
        setSD();

    }
}

/*** Private Functions definitions ***/