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

/*** Setup ***/
void setup() {
  myHardware.init();
  myApp.init();

  Serial.begin(9600);
}

/*** Loop ***/
void loop() {
  myApp.update();
  myHardware.update();
  

  Serial.println("SU Rocket Computer");
  delay(1000);
}