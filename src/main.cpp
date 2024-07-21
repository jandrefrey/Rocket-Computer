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


hardware_c myHardware;

void setup() {
  myHardware.init();
  appInit();
}

void loop() {
  hardwareUpdate()
  appUpdate();
}

