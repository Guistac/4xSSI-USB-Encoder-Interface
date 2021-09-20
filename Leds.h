#pragma once

#include "EncoderInterface.h"

#define FRAMERATE 250.0

class Leds{
public:
  EncoderInterface* encoderInterface_ptr;
  Leds(EncoderInterface*);

  uint8_t pins[ENCODERCOUNT] = {23,21,22,20};
  uint8_t brightness[ENCODERCOUNT] = {0,0,0,0};

  void startup();
  void update();

  uint32_t startupTime;
  uint32_t animationTime = 2000;

  uint32_t updateInterval = 1000.0 / FRAMERATE;
  uint32_t lastUpdateTime = 0;
};
