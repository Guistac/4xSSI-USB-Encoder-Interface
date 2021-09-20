#include "EncoderInterface.h"

EncoderInterface::EncoderInterface() {
  ssiEncoders_ptr = new SsiEncoders(this);
  usbSerial_ptr = new UsbSerial(this);
  leds_ptr = new Leds(this);
}

void EncoderInterface::startup() {
  ssiEncoders_ptr->startup();
  usbSerial_ptr->startup();
  leds_ptr->startup();
}

void EncoderInterface::update() {
  ssiEncoders_ptr->update();
  usbSerial_ptr->update();
  leds_ptr->update();
}
