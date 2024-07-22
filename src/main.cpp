/**
 *******************************************************************************
 * \file        main.cpp
 * \brief       Main program Code
 * \date        2024/07/21
 * \author      Jandre Frey
 *******************************************************************************
 */

#include <Arduino.h>
#include "hardware.h"
#include "app.h"


hardware_c myHardware;
app_c myApp;

void setup() {
  myHardware.init();
  myApp.init();
}

void loop() {
  myHardware.update();
  myApp.update();
}

