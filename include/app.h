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
};

#endif
