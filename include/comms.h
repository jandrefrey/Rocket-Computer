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
#include <LoRa.h>

#define         CS_PIN          28
#define         RESET_PIN       30
#define         IRQ_PIN         29

#define         LORA_FREQ       433E6
#define         TXID            0b0001
#define         RXID            0b0010
//messagetype
#define         COMMAND_PT      0b00000000
#define         TELEMETRY_PT    0b00000001
//commands
#define         FIRE_PYRO_C     0b00010001

#define         TELEMETRY_SIZE  29

/*** Class declaration ***/
class Comms {
public:
    typedef enum {      //typedef is public but variable is local.
        COMMAND = 0,
        TELEMETRY,
        STATUS,
        ERROR
    } messagetype_t;

    struct message_s{
        messagetype_t messagetype;
        uint8_t pData[TELEMETRY_SIZE];
        int message_available;
    };

    Comms(); // Constructor
    ~Comms(); // Destructor

    int init();
    int parseRx(message_s& message);
    int setTelemetry(float accel[], float gyro[], float magn[], float baro_raw, float baro_filt, uint32_t time, uint8_t pyroStates, float batVolts);
    int sendMsg();
    uint8_t errorCode;

private:

    bool txBuffHasData();
    static void onReceive(int packetSize);
    static void onTxDone();

    uint8_t* txBuf;
    uint8_t* rxBuf;

};

extern Comms m_comms;


#endif