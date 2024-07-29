/**
 *******************************************************************************
 * \file        hardware.h
 * \brief       hardware library include file
 * \date        2024/07/21
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Prevent Recursive Inclusion ***/
#ifndef HARDWARE_H
#define HARDWARE_H

/*** Includes ***/
#include <Arduino.h>

/*** Class declaration ***/
class Hardware
{
public:
    Hardware();  // Constructor
    ~Hardware(); // Destructor

    void init();
    void update();

private:
};

#endif
