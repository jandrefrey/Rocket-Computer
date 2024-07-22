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

/*** Includes ***/
#include <Arduino.h>

/*** Class declaration ***/
class app_c {
public:
    app_c(); // Constructor
    ~app_c(); // Destructor

    void init();
    void update();

private:
    int altitude;
    typedef enum {
        IDLE,
        LAUNCH,
        BURNOUT,
        APOGEE,
        LANDED
    } flightStage_t;

    flightStage_t flightStage;

    void measureSensors();
    void filterMeasurements();
    void rxTelemetry();
    void txTelemetry();
    void detectLaunch();
    void storeFlash();
    void detectBurnout();
    void detectApogee();
    void deployCharges();
    void detectLanding();
    void storeSD();
    void calibrateSensors();
    void setLaunchReady();
};

#endif
