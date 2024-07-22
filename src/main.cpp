/**
 *******************************************************************************
 * \file        main.cpp
 * \brief       Main program Code
 * \date        2024/07/21
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Includes ***/
#include <Arduino.h>
#include "hardware.h"
#include "app.h"

/*** Declerations ***/
hardware_c myHardware;
app_c myApp;

/*** Setup ***/
void setup() {
  myHardware.init();
  myApp.init();
}

/*** Loop ***/
void loop() {
  myHardware.update();
  myApp.update();
}