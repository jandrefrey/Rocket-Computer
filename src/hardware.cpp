/**
 *******************************************************************************
 * \file        hardware.cpp
 * \brief       hardware library source file
 * \date        2024/07/21
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Includes ***/
#include "hardware.h"

/*** Constructor implementation ***/
hardware_c::hardware_c() {
    // Initialize deviceId or other constructor logic here
}

/*** Destructor implementation ***/
hardware_c::~hardware_c() {
    // Cleanup resources if any
}

/*** Public Functions definitions ***/
void hardware_c::init() {
    sensorInit();
    batteryInit();
    pyroInit();
    radioInit();
}

void hardware_c::update() {
    measureSensors();
    
}

/*** Private Functions definitions ***/
