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
  Serial.begin(9600);
  delay(1000);

  myHardware.init();
  myApp.init();
}

/*** Loop ***/
void loop() {
  myHardware.update();
  myApp.update();  
}