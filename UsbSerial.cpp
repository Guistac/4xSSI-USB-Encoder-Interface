#include "UsbSerial.h"

UsbSerial::UsbSerial(EncoderInterface* ei_ptr) {
  encoderInterface_ptr = ei_ptr;
}

void UsbSerial::startup() {
  Serial.begin(115200);
}

void UsbSerial::update() {
  readSerialData();
  handleTimeout();
  writeSerialData();
  printDebugData();
}

void UsbSerial::readSerialData() {
  if (Serial.available()) {
    char msg = Serial.read();
    if (msg == connectionRequestMsg) {
      Serial.write(connectionConfirmationMsg);
      connected = true;
      debugMode = DEBUGMODE::NONE;
      lastHearbeatReceiveTimeMillis = millis();
      lastDataSendTimeMillis = millis();
    } else if (msg == heartBeatMsg) {
      lastHearbeatReceiveTimeMillis = millis();
    } else if (msg == sendAngularDebugDataMsg) {
      debugMode = DEBUGMODE::ANGLE;
    } else if (msg == sendBinaryDebugDataMsg) {
      debugMode = DEBUGMODE::BINARY;
    } else if (msg == sendErrorDebugDataMsg) {
      debugMode = DEBUGMODE::ERROR;
    } else if (msg == disableDebugMsg) {
      debugMode = DEBUGMODE::NONE;
    } else if(msg == versionRequestMsg){
      Serial.println("Firmware V1.0 - 14 Oct 2020 - Leo Becker");
      debugMode = DEBUGMODE::NONE;
    }
  }
}

void UsbSerial::writeSerialData() {
  if (connected && dataSendIntervalMillis < millis() - lastDataSendTimeMillis) {
    uint8_t byteCount = 2 + ENCODERCOUNT * 3;
    byte transmission[byteCount];
    transmission[0] = dataHeaderMsg;
    byte connectionByte = 0x00;
    for(int c = 0; c < ENCODERCOUNT; c++){
      connectionByte <<= 1;
      connectionByte |= encoderInterface_ptr->ssiEncoders_ptr->connected[c];
    }
    transmission[1] = connectionByte;
    for(int c = 0; c < ENCODERCOUNT; c++){
      transmission[2+c*3+0] = encoderInterface_ptr->ssiEncoders_ptr->data[c] >> 16;
      transmission[2+c*3+1] = encoderInterface_ptr->ssiEncoders_ptr->data[c] >> 8;
      transmission[2+c*3+2] = encoderInterface_ptr->ssiEncoders_ptr->data[c];
    }
    Serial.write(transmission,byteCount);
    Serial.send_now();
    lastDataSendTimeMillis = millis();
  }
}

void UsbSerial::printDebugData(){
  if (debugSendIntervalMillis < millis() - lastDebugSendTimeMillis) {
    if (debugMode == DEBUGMODE::ANGLE) {
     for(int c = 0; c < ENCODERCOUNT; c++){
       Serial.print(c);
       Serial.print(": ");
       if (encoderInterface_ptr->ssiEncoders_ptr->connected[c]) {
         Serial.print((float)encoderInterface_ptr->ssiEncoders_ptr->data[c] * 360.0 / 4096.0);
         Serial.print("°");
       } else if (encoderInterface_ptr->ssiEncoders_ptr->waitingForStartup[c]) {
         Serial.print("Encoder Booting");
       } else {
         Serial.print("NC");
       }
       Serial.print("  ");
     }
     Serial.println();
   } else if (debugMode == DEBUGMODE::BINARY) {
     for(int c = 0; c < ENCODERCOUNT; c++){
       Serial.print(encoderInterface_ptr->ssiEncoders_ptr->rawBinaryData[c], BIN);
       Serial.print("   ");
     }
     Serial.println();
   } else if (debugMode == DEBUGMODE::ERROR) {
     for(int c = 0; c < ENCODERCOUNT; c++){
       Serial.print(encoderInterface_ptr->ssiEncoders_ptr->errorCount[c]);
       Serial.print("   ");
     }
     Serial.println();
   }
   lastDebugSendTimeMillis = millis();
  }
}

void UsbSerial::handleTimeout() {
  if (connected && timeOutDelay < millis() - lastHearbeatReceiveTimeMillis) {
    connected = false;
  }
}
