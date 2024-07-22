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
    app_c::calibrateSensors();
    app_c::setLaunchReady();
}

void app_c::update() {
    app_c::measureSensors();
    app_c::filterMeasurements();
    app_c::rxTelemetry();
    app_c::txTelemetry();
    if (flightStage == IDLE) {

        app_c::detectLaunch();
    }
    if (flightStage == LAUNCH) {
        app_c::storeFlash();

        app_c::detectBurnout();
    }
    if (flightStage == BURNOUT) {
        app_c::storeFlash();

        app_c::detectApogee();
    }
    if (flightStage == APOGEE) {
        app_c::deployCharges();

        app_c::detectLanding();        
    }
    if (flightStage == LANDED) {
        app_c::storeSD();

    }
}

/*** Private Functions definitions ***/
void calibrateSensors() {

}

void setLaunchReady() {

}