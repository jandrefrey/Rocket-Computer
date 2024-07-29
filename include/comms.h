/**
 *******************************************************************************
 * \file        comms.h
 * \brief       comms library include file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */


/*** Prevent Recursive Inclusion ***/
#ifndef COMMS_H
#define COMMS_H


/*** Includes ***/
#include <Arduino.h>

/*** Class declaration ***/
class Comms {
public:
    Comms(); // Constructor
    ~Comms(); // Destructor

    int init();
    int receive();
    int send();
    int setTx();

private:

};

#endif