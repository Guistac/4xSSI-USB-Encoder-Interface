#pragma once

#include "EncoderInterface.h"

class SsiEncoders {
  public:
    EncoderInterface* encoderInterface_ptr;
                                  
    uint8_t dataPins[ENCODERCOUNT] = {4,2,3,1};
    uint8_t clockPin = 0;

    uint8_t bitCount = 26;
    uint32_t clockDelayMicroseconds = 2;
    uint32_t pauseTimeMicroseconds = 30;
    uint32_t lastReadingTimeMicros = 0;

    uint32_t errorCount[ENCODERCOUNT] = {0,0,0,0};

    bool deviceDetected[ENCODERCOUNT] = {0,0,0,0};
    bool waitingForStartup[ENCODERCOUNT] = {0,0,0,0};
    uint32_t detectionTimeMillis[ENCODERCOUNT] = {0,0,0,0};
    uint32_t startupTimeMillis = 1000;

    bool validZeroDetected[ENCODERCOUNT] = {0,0,0,0};

    uint32_t data[ENCODERCOUNT] = {0,0,0,0};
    bool connected[ENCODERCOUNT] = {0,0,0,0};
    uint32_t lastMoveTimeMillis[ENCODERCOUNT] = {0,0,0,0};

    uint32_t rawBinaryData[ENCODERCOUNT] = {0,0,0,0};

    SsiEncoders(EncoderInterface* ei_ptr);
    void startup();
    void update();
};
