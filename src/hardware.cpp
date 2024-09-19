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
#include "app.h"

IntervalTimer pyroTimer;


/*** Constructor implementation ***/
Hardware::Hardware()
{
    pyro1State = 0;
    pyro2State = 0;
    pyroDeploy = 0;
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

    pinMode(PYRO_PIN_1, OUTPUT);
    digitalWrite(PYRO_PIN_1, LOW);
    pinMode(PYRO_PIN_2, OUTPUT);
    digitalWrite(PYRO_PIN_2, LOW);

    SPI.begin();

    Serial.print("Battery Voltage: ");
    Serial.println(m_batteryCheck());
    if(m_batteryCheck() < BATTERY_MINIMUM) {
        Serial.println("Battery voltage low. HALT!");
        while(1);
    }

    #ifdef USE_PYRO_1
    if(m_pyroCheck() != 0b00001000 || m_pyroCheck() != 0b00001100) {
        Serial.println("Pyro 1 Continuity Failed!");
        while(1);
    }
    #endif
    #ifdef USE_PYRO_2
    if(m_pyroCheck() != 0b00000100 || m_pyroCheck() != 0b00001100) {
        Serial.println("Pyro 2 Continuity Failed!");
        while(1);
    }
    #endif

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
    //int time = millis();
    m_sensors.measure();
    //int measuredtime = millis() - time;
    // Serial.print("Measuredtime:");
    // Serial.println(measuredtime);    //Takes 9ms to measure
    
    // Serial.print("AccelX");
    // Serial.println(mymeasurements.accelx);
    // Serial.print("AccelY");
    // Serial.println(mymeasurements.accely);
    // Serial.print("AccelZ");
    // Serial.println(mymeasurements.accelz);
    // Serial.print("GyroX");
    // Serial.println(mymeasurements.gyrox);
    // Serial.print("GyroY");
    // Serial.println(mymeasurements.gyroy);
    // Serial.print("GyroZ");
    // Serial.println(mymeasurements.gyroz);
    // Serial.print("MagnX");
    // Serial.println(mymeasurements.magx);
    // Serial.print("MagnY");
    // Serial.println(mymeasurements.magy);
    // Serial.print("MagnZ");
    // Serial.println(mymeasurements.magz);
    // Serial.print("Baro");
    // Serial.println(mymeasurements.bpressure);
    // delay(500);
    //m_batteryCheck();
    
    m_comms.parseRx(mymessage);
    if(mymessage.message_available == 1) {
        //Serial.println("hardware received message");
        switch (mymessage.messagetype)
        {
        case (Comms::COMMAND):
            //Serial.println(mymessage.pData[0]);
            //Serial.println(mymessage.pData[1]);
            if(mymessage.pData[0] == FIRE_PYRO_C) {
                myApp.m_flightStage = App::APOGEE;      //DONT LIKE THIS IMPLEMENTATION
                pyroDeploy = 1;
                myHardware.buzzerMode = Hardware::QUICK_BEEPS;
                myApp.timeDebug = millis();
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
    
    m_comms.sendMsg();

    m_buzzerUpdate();
    m_pyroUpdate();
}

/*** Private Functions definitions ***/

uint8_t Hardware::m_pyroCheck() {
    uint8_t pyroStatus = 0b00000000;

    float voltage = analogRead(A6) * (3.3 / 1023.0) * (150+47)/47;
    if(voltage > (BATTERY_MINIMUM-3)) {
        pyroStatus = pyroStatus | 0b00001000;
    }

    voltage = analogRead(A8) * (3.3 / 1023.0) * (150+47)/47;
    if(voltage > (BATTERY_MINIMUM-3)) {
        pyroStatus = pyroStatus | 0b00000100;
    }

    if(pyro1State) {
        pyroStatus = pyroStatus | 0b00000010;
    }
    if(pyro2State) {
        pyroStatus = pyroStatus | 0b00000001;
    }

    return pyroStatus;
}

float Hardware::m_batteryCheck() {
    float voltage = analogRead(A1) * (3.3 / 1023.0) * (15000+5600)/5600;
    return voltage;
}

int Hardware::m_buzzerUpdate() {
    switch (buzzerMode)
    {
    case IDLE:
        //noTone(4);
        break;
    case SLOW_PULSE:
        if((millis() - slowPulseTime) > 1000) {
            tone(4, 1000, 500);
            slowPulseTime = millis();
        }
        break;
    case QUICK_BEEPS:
        if((millis() - quickBeepsTime) > 200) {
            tone(4, 2000, 100);
            quickBeepsTime = millis();
            quickBeepscounter++;
        }
        if((quickBeepscounter) > 2) {
            buzzerMode = IDLE;
            quickBeepscounter = 0;
        }
        break; 
    case SWEEPING_PULSE:
        if((millis() - sweepingPulseTime) > 3000) {
            sweepingPulseCounter++;
            tone(4, sweepingPulseCounter);
        }
        if(sweepingPulseCounter > 2500) {
            noTone(4);
            sweepingPulseCounter = 500;
            sweepingPulseTime = millis();
        }
        break;    
    default:
    buzzerMode = IDLE;
        break;
    }

    return 0;
}

int Hardware::m_pyroUpdate() {
    if(pyroDeploy) {
        pyroTimer.begin(Hardware::pyroExpire, PYRO_ON_TIME);
        m_mem.logSD("Pyro fired!");
        #ifdef USE_PYRO_1
        digitalWrite(PYRO_PIN_1, HIGH);
        #endif
        #ifdef USE_PYRO_2
        digitalWrite(PYRO_PIN_2, HIGH);
        #endif
        pyroDeploy = 0;
    }
    // if((millis() - pyroTime) > 500) {
    // pyroExpire();
    // }
    return 0;
}

void Hardware::pyroExpire() {
    digitalWrite(PYRO_PIN_1, LOW);
    digitalWrite(PYRO_PIN_2, LOW);
}


Hardware myHardware;