/**
 *******************************************************************************
 * \file        comms.cpp
 * \brief       comms library source file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Includes ***/
#include "comms.h"

/*** Constructor implementation ***/
Comms::Comms() {
    // Initialize deviceId or other constructor logic here
    txBuf = new uint8_t[2 + TELEMETRY_SIZE];
    rxBuf = new uint8_t[2 + TELEMETRY_SIZE];
    for (int i = 0; i < (TELEMETRY_SIZE + 2); ++i) {
        txBuf[i] = 0;
        rxBuf[i] = 0;
    }

    errorCode = 0;
}

/*** Destructor implementation ***/
Comms::~Comms() {
    // Cleanup resources if any
    delete[] txBuf;
    delete[] rxBuf;
}

/*** Public Functions definitions ***/
int Comms::init() {

    LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN);
    LoRa.setSPI(SPI1);

    if (!LoRa.begin(LORA_FREQ)) {
        Serial.println("LoRa init failed!");
        //while (true);                       // if failed, do nothing
    }

    Serial.println("LoRa init succeeded.");

    LoRa.onReceive(Comms::onReceive);
    LoRa.onTxDone(Comms::onTxDone);
    LoRa.receive();

    

    return 0;
}

int Comms::parseRx(message_s& message) {
    if(*rxBuf) {
        if((rxBuf[0] & 0b00001111) == RXID) {                                           //check rx ID
            //Serial.println("Mine!");
            message.messagetype = (messagetype_t)rxBuf[1];                              //check message type
            //Serial.print("messagetype: ");
            //Serial.println(message.messagetype, BIN);
            memcpy(message.pData, rxBuf+2, (size_t)TELEMETRY_SIZE);                     //load data
            //Serial.println("memcopy done");
            // for (int i = 0; i < TELEMETRY_SIZE; ++i) {
            //     Serial.println(message.pData[i], BIN);
            // }
            for (int i = 0; i < (TELEMETRY_SIZE + 2); ++i) {                            //clear buf
                rxBuf[i] = 0;
            }
            //Serial.println("Buffer cleared");
            message.message_available = 1;
            //Serial.println(message.message_available);
        } else {
            Serial.println("Incorrect RxID, clearing buffer");
            for (int i = 0; i < (TELEMETRY_SIZE + 2); ++i) {
                rxBuf[i] = 0;
            }
        }
    }
    return 0;
}

int Comms::setTelemetry(float accel[], float gyro[], float magn[], float baro_raw, float baro_filt, uint32_t time, uint8_t pyroStates, float batVolts) {
    txBuf[0] = (RXID << 4 ) | TXID;

    txBuf[1] = TELEMETRY_PT;

    txBuf[2] = ((int16_t)(accel[0]*100)>> 8) & 0xFF;
    txBuf[3] = ((int16_t)(accel[0]*100)) & 0xFF;
    txBuf[4] = ((int16_t)(accel[1]*100)>> 8) & 0xFF;
    txBuf[5] = ((int16_t)(accel[1]*100)) & 0xFF;
    txBuf[6] = ((int16_t)(accel[2]*100)>> 8) & 0xFF;
    txBuf[7] = ((int16_t)(accel[2]*100)) & 0xFF;

    txBuf[8] = ((int16_t)(gyro[0]*100)>> 8) & 0xFF;
    txBuf[9] = ((int16_t)(gyro[0]*100)) & 0xFF;
    txBuf[10] = ((int16_t)(gyro[1]*100)>> 8) & 0xFF;
    txBuf[11] = ((int16_t)(gyro[1]*100)) & 0xFF;
    txBuf[12] = ((int16_t)(gyro[2]*100)>> 8) & 0xFF;
    txBuf[13] = ((int16_t)(gyro[2]*100)) & 0xFF;

    txBuf[14] = ((int16_t)(magn[0]*1000)>> 8) & 0xFF;
    txBuf[15] = ((int16_t)(magn[0]*1000)) & 0xFF;
    txBuf[16] = ((int16_t)(magn[1]*1000)>> 8) & 0xFF;
    txBuf[17] = ((int16_t)(magn[1]*1000)) & 0xFF;
    txBuf[18] = ((int16_t)(magn[2]*1000)>> 8) & 0xFF;
    txBuf[19] = ((int16_t)(magn[2]*1000)) & 0xFF;

    txBuf[20] = ((uint16_t)(baro_raw*10)>> 8) & 0xFF;
    txBuf[21] = ((uint16_t)(baro_raw*10)) & 0xFF;
    txBuf[22] = ((uint16_t)(baro_filt*10)>> 8) & 0xFF;
    txBuf[23] = ((uint16_t)(baro_filt*10)) & 0xFF;

    txBuf[24] = (time >> 24) & 0xFFF;
    txBuf[25] = (time >> 16) & 0xFF;
    txBuf[26] = (time >> 8) & 0xFF;
    txBuf[27] = (time) & 0xFF;

    txBuf[28] = pyroStates;

    txBuf[29] = (uint8_t)(batVolts*10);

    txBuf[30] = errorCode; //global variable.

    return 0;
}

int Comms::sendMsg() {
    if(txBuffHasData()) {
        LoRa.idle();
        LoRa.beginPacket();
        LoRa.write(txBuf, TELEMETRY_SIZE+2);
        LoRa.endPacket(true);

        for (int i = 0; i < (TELEMETRY_SIZE + 2); ++i) {
            txBuf[i] = 0;
        }
    }
    return 0;
}
/*** Private Functions definitions ***/

bool Comms::txBuffHasData() {
    int dataCount = 0;
    for (int i = 0; i < (TELEMETRY_SIZE+2); ++i) {
        //Serial.println(txBuf[i], BIN);                                                   //DEBUG
        if (txBuf[i] != 0) {
            ++dataCount;
        }
    }
    return dataCount;
}

void Comms::onReceive(int packetSize) {
    int index = 0;
    while (LoRa.available() && index < (2 + TELEMETRY_SIZE)) {
        m_comms.rxBuf[index++] = LoRa.read();
    }
    //TODO: TEST IF THIS WORKS. It might be nice to use packetSize to have better control of packet size!
    // for (int i = 0; i < packetSize; i++) {
    //     m_comms.rxBuf[index++] = LoRa.read();
    // }

  Serial.println("Message Received!");
}

void Comms::onTxDone() {
  Serial.println("Packet Sent Done");
  LoRa.receive();
}

Comms m_comms;