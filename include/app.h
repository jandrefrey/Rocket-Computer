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
class App {
public:
    App(); // Constructor
    ~App(); // Destructor

    void init();
    void update();

private:
    typedef enum {
        IDLE,
        LAUNCH,
        BURNOUT,
        APOGEE,
        LANDED
    } flightStage_t;

    flightStage_t m_flightStage;
};

#endif
