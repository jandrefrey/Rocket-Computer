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
    //m_setLaunchReady();

    prevTickTX = millis();
    prevTickSD = millis();

    positiveCount = 0;
}

void App::update() {
    // m_sensors.integrateMeasures();

    //Log Data
    if((millis() - prevTickSD) > LOG_FREQ) {
        m_mem.logTelemetry(m_sensors.mymeasurements.accel, m_sensors.mymeasurements.gyro, m_sensors.mymeasurements.mag,
        m_sensors.mymeasurements.bpressure, m_sensors.mymeasurements.bpresurre_vel, m_sensors.mymeasurements.time);
        prevTickSD = millis();
    }
    //Tx Data
    if((millis() - prevTickTX) > TX_FREQ) {
        m_comms.setTelemetry(m_sensors.mymeasurements.accel, m_sensors.mymeasurements.gyro, m_sensors.mymeasurements.mag,
        m_sensors.mymeasurements.bpressure, m_sensors.mymeasurements.bpresurre_vel, m_sensors.mymeasurements.time, myHardware.m_pyroCheck(), myHardware.m_batteryCheck());
        prevTickTX = millis();
    }

    #ifndef DEBUG
    //STATE MACHINE
    if (m_flightStage == IDLE) {
        myHardware.buzzerMode = Hardware::SLOW_PULSE;

        //Update State
        if(m_detectLaunch()) {
            m_flightStage = LAUNCH;
            myHardware.buzzerMode = Hardware::QUICK_BEEPS;
        }
    }
    if (m_flightStage == LAUNCH) {
        //Update State
        if(detectApogee()) {
            m_flightStage = APOGEE;
            myHardware.pyroDeploy = 1;
            myHardware.buzzerMode = Hardware::QUICK_BEEPS;
        }
    }
    if (m_flightStage == APOGEE) {
        myHardware.pyroDeploy = 1;
        myHardware.buzzerMode = Hardware::QUICK_BEEPS;
        m_flightStage = DESCENT;
    }
    if (m_flightStage == DESCENT) {
        //Update State
        if(m_detectLanding()) {
            m_flightStage = LANDED;
        }    
    }
    if (m_flightStage == LANDED) {
        myHardware.buzzerMode = Hardware::SWEEPING_PULSE;
    }
    #endif

    #ifdef DEBUG
        //STATE MACHINE
    if (m_flightStage == IDLE) {
        myHardware.buzzerMode = Hardware::SLOW_PULSE;

        //Update State
        if(m_detectLaunch()) {
            m_flightStage = LAUNCH;
            myHardware.buzzerMode = Hardware::QUICK_BEEPS;
            timeDebug = millis();
        }
    }
    if (m_flightStage == LAUNCH) {
        //Update State
        if((millis() - timeDebug) > 5000) {
            m_flightStage = APOGEE;
        }
    }
    if (m_flightStage == APOGEE) {
        myHardware.pyroDeploy = 1;
        myHardware.buzzerMode = Hardware::QUICK_BEEPS;
        timeDebug = millis();
        m_flightStage = DESCENT;
    }
    if (m_flightStage == DESCENT) {
        //Update State
        if((millis() - timeDebug) > 5000) {
            m_flightStage = LANDED;
        }
    }
    if (m_flightStage == LANDED) {
        myHardware.buzzerMode = Hardware::SWEEPING_PULSE;
    }
    #endif
}

/*** Private Functions definitions ***/

int App::m_detectLaunch() {
    float accelVector = sqrt((sq(m_sensors.mymeasurements.accel[0]) + sq(m_sensors.mymeasurements.accel[1]) + sq(m_sensors.mymeasurements.accel[2])));
    if((accelVector > 20) || (m_sensors.mymeasurements.accel[1] > 20)) {
        m_mem.logSD("Launch Detected");
        return 1;
    }
    return 0;
}

int App::detectApogee() {
    // if(((pressure - pressurePrev) > 0.4) && (pressurePrev != 0)) {
    //     m_mem.logSD("Apogee Detected");
    //     return 1;
    // }

    // if(apogeeCounter > 10) {
    //     pressurePrev = pressure;
    //     apogeeCounter = 0;
    // }
    // apogeeCounter++;
    if(m_sensors.mymeasurements.bpresurre_vel < -0.1) {        //pressure will increase going down, ie positive velocity
        positiveCount++;
    }
    if(positiveCount > COUNT_THRESHOLD){
        positiveCount = 0;
        m_mem.logSD("Apogee Detected");
        return 1;
    }
    
    return 0;
}

int App::m_detectLanding() {
    // float accelVector = sqrt((sq(m_sensors.mymeasurements.accel[0]) + sq(m_sensors.mymeasurements.accel[1]) + sq(m_sensors.mymeasurements.accel[2])));
    // if ((accelVector > 9) && (accelVector < 11) ) {
    //     m_mem.logSD("Landing Detected");
    //     return 1;
    // }
    if((m_sensors.mymeasurements.bpresurre_vel > -0.02) && (m_sensors.mymeasurements.bpresurre_vel < 0.02)) {        //pressure will increase going down, ie positive velocity
        positiveCount++;
    }
    if(positiveCount > COUNT_THRESHOLD){
        positiveCount = 0;
        m_mem.logSD("Landing Detected");
        return 1;
    }

    return 0;    
}

App myApp;