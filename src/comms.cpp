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

int Comms::setTelemetry(uint8_t *pData) {
    // Copy the 2 bytes of additional data into the beginning of the tx buffer
    //txBuf[TELEMETRY_SIZE+1] = (RXID << 4 ) | TXID;
    //txBuf[TELEMETRY_SIZE+0] = TELEMETRY_PT;
    txBuf[0] = (RXID << 4 ) | TXID;
    txBuf[1] = TELEMETRY_PT;

    // Copy the original data into the tx buffer after the 2 bytes
    memcpy(txBuf+2, pData, TELEMETRY_SIZE);

    // for (int i = 0; i < (TELEMETRY_SIZE+2); ++i) {                  //DEBUG
    //     Serial.println(txBuf[i], BIN); 
    // }
    Serial.println("Data logged"); 

    return 0;
}

int Comms::sendMsg() {
    if(txBuffHasData()) {
        LoRa.idle();
        LoRa.beginPacket();
        LoRa.write(txBuf, txBuffHasData());
        LoRa.endPacket();

        for (int i = 0; i < (TELEMETRY_SIZE + 2); ++i) {
            txBuf[i] = 0;
        }
        Serial.println("Data Sent");
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

  Serial.println("Message Received!");
}

void Comms::onTxDone() {
  Serial.println("");
  LoRa.receive();
}

Comms m_comms;