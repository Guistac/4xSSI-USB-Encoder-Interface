#pragma once

#include "EncoderInterface.h"

#define DATAFREQUENCY 250.0
#define DEBUGFREQUENCY 30.0

class UsbSerial {
  public:

    EncoderInterface* encoderInterface_ptr;

    char connectionRequestMsg = 'c';
    char connectionConfirmationMsg = 'C';
    char dataHeaderMsg = 'd';
    char heartBeatMsg = 'h';

    char sendAngularDebugDataMsg = 'a';
    char sendBinaryDebugDataMsg = 'b';
    char sendErrorDebugDataMsg = 'e';
    char disableDebugMsg = 'n';
    char versionRequestMsg = 'v';

    uint8_t debugMode = 0;

    enum DEBUGMODE {
      NONE,
      ANGLE,
      BINARY,
      ERROR
    };

    bool connected = false;

    uint32_t dataSendIntervalMillis = 1000.0 / DATAFREQUENCY;
    uint32_t lastDataSendTimeMillis = 0;

    uint32_t debugSendIntervalMillis = 1000.0 / DEBUGFREQUENCY;
    uint32_t lastDebugSendTimeMillis = 0;

    uint32_t lastHearbeatReceiveTimeMillis;
    uint32_t timeOutDelay = 100;

    UsbSerial(EncoderInterface* ei_ptr);
    void startup();
    void update();
    void readSerialData();
    void writeSerialData();
    void printDebugData();
    void handleTimeout();
};
