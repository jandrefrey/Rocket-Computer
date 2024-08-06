/**
 *******************************************************************************
 * \file        sensors.cpp
 * \brief       Sensors library source file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Includes ***/
#include "sensors.h"

/*** Constructor implementation ***/
Sensors::Sensors()
{
    // Initialize constructor logic here
}

/*** Destructor implementation ***/
Sensors::~Sensors()
{
    // Cleanup resources if any
}

/*** Public Functions definitions ***/
int Sensors::init() {
    return 0;
}

int Sensors::calib() {
    return 0;
}

int Sensors::measure() {
    return 0;
}

int Sensors::filter() {
    return 0;
}


/*** Private Functions definitions ***/


Sensors m_sensors;