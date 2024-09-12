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
    pinMode(4, OUTPUT);
    tone(4, 200);
    delay(200);
    tone(4, 400);
    delay(200);
    tone(4, 800);
    delay(200);
    tone(4, 1600);
    delay(200);
    noTone(4);     // Stop sound...

    SPI.begin();

    if(m_batteryCheck() != 0) {
        Serial.println("Battery voltage low. HALT!");
        while(1);
    }
    if (m_pyroCheck() != 0) {
        Serial.println("Pyro continuity failed. HALT!");
        while(1);
    }
    if (m_comms.init() != 0) {  //no send and receive check here. This will be added in the state machine for ground idle.
        Serial.println("Radio init failed. HALT!");
        while(1);
    }
    if (m_sensors.init() != 0) {
        Serial.println("Sensor init failed. HALT!");
        while(1);
    }
    if (m_mem.init() != 0) {
        Serial.println("Memory init failed. HALT!");
        while(1);
    }
    return 0;    
}

void Hardware::update()
{
    //m_sensors.measure(mymeasurements);
    //m_batteryCheck();
    
    m_comms.parseRx(mymessage);
    if(mymessage.message_available == 1) {
        Serial.println("hardware received message");
        switch (mymessage.messagetype)
        {
        case (Comms::COMMAND):
            Serial.println(mymessage.pData[0]);
            Serial.println(mymessage.pData[1]);
            if(mymessage.pData[0] == FIRE_PYRO_C) {
                tone(4, 1000);
                delay(400);
                noTone(4);
                #ifdef USE_PYRO_1
                pinMode(PYRO_PIN_1, OUTPUT);
                digitalWrite(PYRO_PIN_1, HIGH);
                delay(500);                 //Rather put this on a timer as not to delay the main loop.
                digitalWrite(PYRO_PIN_1, LOW);
                #endif
                #ifdef USE_PYRO_2
                pinMode(PYRO_PIN_2, OUTPUT);
                digitalWrite(PYRO_PIN_2, HIGH);
                delay(500);                 //Rather put this on a timer as not to delay the main loop.
                digitalWrite(PYRO_PIN_2, LOW);
                #endif
            }
            else {
                Serial.println("Command not recognised");
            }
            break;
        default:
            Serial.println("Message type not recognised");
            break;
        }
        mymessage.message_available = 0;
    }
    
    //m_comms.sendMsg();
    //m_mem.writeFlash();
    //m_mem.writeSD();
    //m_buzzerUpdate();
    //m_pyroUpdate();
}

/*** Private Functions definitions ***/

int Hardware::m_pyroCheck() {
    #ifdef USE_PYRO_1
    int sensorValue = analogRead(A6);
    float voltage = sensorValue * (3.3 / 1023.0) * (150+47)/47;

    if(voltage > (BATTERY_MINIMUM-3)) {
        Serial.println("Pyro 1 continuity TRUE");
    } else {
        return 1;
    }
    #endif

    #ifdef USE_PYRO_2
    int sensorValue = analogRead(A8);
    float voltage = sensorValue * (3.3 / 1023.0) * (150+47)/47;

    if(voltage > (BATTERY_MINIMUM-3)) {
        Serial.println("Pyro 2 continuity TRUE");
    } else {
        return 1;
    }
    #endif

    return 0;
}

int Hardware::m_batteryCheck() {
    int sensorValue = analogRead(A1);
    float voltage = sensorValue * (3.3 / 1023.0) * (15000+5600)/5600;
    Serial.print("Battery Voltage: ");
    Serial.println(voltage);

    if(voltage > BATTERY_MINIMUM) {
        return 0;
    } else {
        return 1;
    }
}

int Hardware::m_buzzerUpdate() {
    return 0;
}

int Hardware::m_pyroUpdate() {
    return 0;
}


Hardware myHardware;