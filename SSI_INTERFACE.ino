#include "EncoderInterface.h"

EncoderInterface encoderInterface;

void setup() {
  encoderInterface.startup();
}

void loop() {
  encoderInterface.update();
}
