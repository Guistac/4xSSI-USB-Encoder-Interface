#include "Leds.h"

Leds::Leds(EncoderInterface* ei_ptr){
  encoderInterface_ptr = ei_ptr;
}

void Leds::startup(){
  for(int c = 0; c < ENCODERCOUNT; c++){
    pinMode(pins[c],OUTPUT);
    digitalWrite(pins[c],LOW);
  }
  startupTime = millis();
}

void Leds::update(){
  if(millis() - lastUpdateTime >= updateInterval){
    lastUpdateTime = millis();
    for(int c = 0; c < ENCODERCOUNT; c++){
      //Startup Animation on interface boot
      if(millis() - startupTime < animationTime){
        uint32_t wave = 400;
        uint32_t ramp = wave / 2;
        uint32_t offset = 100;
        int32_t time = millis() - startupTime - c * offset;
        int32_t period = (time / ramp) % 2;
        if(period == 0){
          brightness[c] = time % ramp;
        }else if(period == 1){
          brightness[c] = ramp - (time % ramp);
        }
        brightness[c] *= (255.0 / (float)ramp);
        if(time < 0){
          brightness[c] = 0;
        }else if(time / ramp > 5){
          brightness[c] = 0;
        }
      //Flashing animation when encoder whas connected and is booting
      }else if(encoderInterface_ptr->ssiEncoders_ptr->waitingForStartup[c]){
        uint32_t time = millis() - encoderInterface_ptr->ssiEncoders_ptr->detectionTimeMillis[c];
        uint32_t period = (time / 142) % 2;
        if(period == 0){
          brightness[c] = time % 142;
        }else if(period == 1){
          brightness[c] = 142 - (time % 142);
        }
        brightness[c] *= 255.0 / 142.0;
      //LED on when encoder connected and not moving, slightly flashing when moving
      }else if(encoderInterface_ptr->ssiEncoders_ptr->connected[c]){
        if(millis() - encoderInterface_ptr->ssiEncoders_ptr->lastMoveTimeMillis[c] < 100
        && millis() - encoderInterface_ptr->ssiEncoders_ptr->detectionTimeMillis[c] > 1100){
          if(millis() % 100 < 50){
            brightness[c] = 255;
          }else{
            brightness[c] = 127;
          }
        }else{
          brightness[c] = 255;
        }
      //LED off when no encoder connected
      }else{
        brightness[c] = 0;
      }
      //Write PWM value to LED
      analogWrite(pins[c],min(brightness[c],255));
    }
  }
}
