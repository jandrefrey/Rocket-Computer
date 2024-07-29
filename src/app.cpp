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
    m_sensors.calib();
    m_setLaunchReady();
}

void App::update() {
    m_sensors.filter();
    m_comms.setTx();

    if (m_flightStage == IDLE) {

        m_detectLaunch();
    }
    if (m_flightStage == LAUNCH) {
        m_mem.setFlash();

        detectBurnout();
    }
    if (m_flightStage == BURNOUT) {
        m_mem.setFlash();

        detectApogee();
    }
    if (m_flightStage == APOGEE) {
        m_deployCharges();
        m_mem.setFlash();

        m_detectLanding();        
    }
    if (m_flightStage == LANDED) {
        m_mem.setSD();

    }
}

/*** Private Functions definitions ***/
int App::m_setLaunchReady() {

}
int App::m_detectLaunch() {

}
int App::detectBurnout() {

}
int App::detectApogee() {

}
int App::m_deployCharges() {

}
int App::m_detectLanding() {
    
}
