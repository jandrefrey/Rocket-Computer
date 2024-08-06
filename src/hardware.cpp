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
Hardware::Hardware()
{

}

/*** Destructor implementation ***/
Hardware::~Hardware()
{
    // Cleanup resources if any
}

/*** Public Functions definitions ***/
int Hardware::init()
{
    if(m_batteryCheck() != 0) {
        return 1;
    }
    if (m_pyroCheck() != 0) {
        return 2;
    }
    if (m_comms.init() != 0) {
        return 3;
    }
    if (m_sensors.init() != 0) {
        return 4;
    }
    if (m_mem.init() != 0) {
        return 5;
    }
    return 0;    
}

void Hardware::update()
{
    m_sensors.measure();
    m_batteryCheck();
    m_comms.parseRx();
    m_comms.sendMsg();
    m_mem.writeFlash();
    m_mem.writeSD();
    m_buzzerUpdate();
    m_pyroUpdate();
}

/*** Private Functions definitions ***/

int Hardware::m_pyroCheck() {
    return 0;
}

int Hardware::m_batteryCheck() {
    return 0;
}

int Hardware::m_buzzerUpdate() {
    return 0;
}

int Hardware::m_pyroUpdate() {
    return 0;
}


Hardware myHardware;