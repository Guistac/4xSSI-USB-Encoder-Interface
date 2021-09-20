#pragma once

#define ENCODERCOUNT 4

class EncoderInterface;
class SsiEncoders;
class UsbSerial;
class Leds;

#include <Arduino.h>
#include "SsiEncoders.h"
#include "UsbSerial.h"
#include "Leds.h"


class EncoderInterface {
  public:

    SsiEncoders* ssiEncoders_ptr;
    UsbSerial* usbSerial_ptr;
    Leds* leds_ptr;

    EncoderInterface();
    void startup();
    void update();
};
