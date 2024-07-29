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
    if (m_memory.init() != 0) {
        return 5;
    }
}

void Hardware::update()
{
    m_sensors.measure();
    m_batteryCheck();
    m_comms.receive();
    m_comms.send();
    m_memory.writeFlash();
    m_memory.writeSD();
    m_buzzerUpdate();
    m_pyroUpdate();
}

/*** Private Functions definitions ***/

int Hardware::m_pyroCheck() {

}

int Hardware::m_batteryCheck() {

}

int Hardware::m_buzzerUpdate() {

}

int Hardware::m_pyroUpdate() {

}