/**
 *******************************************************************************
 * \file        sensors.h
 * \brief       sensors library include file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Prevent Recursive Inclusion ***/
#ifndef SENSORS_H
#define SENSORS_H

/*** Includes ***/
#include <Arduino.h>

/*** Class declaration ***/
class Sensors
{
public:
    Sensors();  // Constructor
    ~Sensors(); // Destructor

    int init();
    int calib();
    int measure();
    int filter();

private:

};

#endif