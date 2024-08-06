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
}

/*** Destructor implementation ***/
Comms::~Comms() {
    // Cleanup resources if any
    delete[] txBuf;
}

/*** Public Functions definitions ***/
int Comms::init() {
    Serial.begin(9600);

    const int csPin = 10;          // LoRa radio chip select
    const int resetPin = 9;        // LoRa radio reset
    const int irqPin = 2;          // change for your board; must be a hardware interrupt pin
    LoRa.setPins(csPin, resetPin, irqPin);

    if (!LoRa.begin(LORA_FREQ)) {
        Serial.println("LoRa init failed. Check your connections.");
        //while (true);                       // if failed, do nothing
    }
    Serial.println("LoRa init succeeded.");

    LoRa.onReceive(Comms::onReceive);
    LoRa.onTxDone(Comms::onTxDone);
    LoRa.receive();

    return 0;
}

int Comms::parseRx() {
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

    for (int i = 0; i < (TELEMETRY_SIZE+2); ++i) {                  //DEBUG
        Serial.println(txBuf[i], BIN); 
    }
    Serial.println("Data logged"); 

    return 0;
}

int Comms::sendMsg() {
    if(txBuffHasData()) {
        //LoRa.idle();
        //LoRa.beginPacket();
        //LoRa.write(txBuf, txBuffHasData());
        //LoRa.endPacket();

        for (int i = 0; i < (TELEMETRY_SIZE + 2); ++i) {                                //DEBUG
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
        Serial.println(txBuf[i], BIN);                                                   //DEBUG
        if (txBuf[i] != 0) {
            ++dataCount;
        }
    }
    return dataCount;
}

void Comms::onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Node Receive: ");
  Serial.println(message);
}

void Comms::onTxDone() {
  Serial.println("TxDone");
  LoRa.receive();
}

Comms m_comms;