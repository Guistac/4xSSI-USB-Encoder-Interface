#include "SsiEncoders.h"

SsiEncoders::SsiEncoders(EncoderInterface* ei_ptr) {
  encoderInterface_ptr = ei_ptr;
}

void SsiEncoders::startup() {
  for(uint8_t c = 0; c < ENCODERCOUNT; c++){
    pinMode(dataPins[c], INPUT);
  }
  pinMode(clockPin, OUTPUT);
  digitalWrite(clockPin, HIGH);
}

void SsiEncoders::update() {
  //only get a reading if enough time has passed since the last reading
  if (micros() - lastReadingTimeMicros > pauseTimeMicroseconds) {
    //take two readings for each encoder channel
    uint32_t reading1[ENCODERCOUNT] = {0,0,0,0};
    uint32_t reading2[ENCODERCOUNT] = {0,0,0,0};

    //pulse the data pin and read the shifted data from each data pin
    for (int i = 0; i < bitCount; i++) {
      digitalWriteFast(clockPin, LOW);
      delayMicroseconds(clockDelayMicroseconds);
      digitalWriteFast(clockPin, HIGH);
      delayMicroseconds(clockDelayMicroseconds);
      for(int c = 0; c < ENCODERCOUNT; c++){
        reading1[c] <<= 1;
        reading1[c] |= digitalReadFast(dataPins[c]);
      }
    }
    //repeat for second reading
    for (int i = 0; i < bitCount; i++) {
      digitalWriteFast(clockPin, LOW);
      delayMicroseconds(clockDelayMicroseconds);
      digitalWriteFast(clockPin, HIGH);
      delayMicroseconds(clockDelayMicroseconds);
      for(int c = 0; c < ENCODERCOUNT; c++){
        reading2[c] <<= 1;
        reading2[c] |= digitalReadFast(dataPins[c]);
      }
    }

    //compare the two readings
    //use results to format data and handle encoder connection status
    for(int c = 0; c < ENCODERCOUNT; c++){
      if (reading1[c] == reading2[c]) {
        rawBinaryData[c] = reading1[c];
        //to check if an encoder is connected
        //the last two bits are verified to be pulled low
        //the differential data lines are pulled high by default when nothing is connected
        if (!(rawBinaryData[c] & B11)) {
          deviceDetected[c] = true;
          //update encoder status
          if (!connected[c] && !waitingForStartup[c]) {
            detectionTimeMillis[c] = millis();
            waitingForStartup[c] = true;
          } else if (waitingForStartup[c] && startupTimeMillis < millis() - detectionTimeMillis[c]) {
            waitingForStartup[c] = false;
            connected[c] = true;
          }
          //do not transmit data collected while the encoder is booting
          if (connected[c]) {
            uint32_t newData = rawBinaryData[c] >> 2;
            if(newData != data[c]){
              lastMoveTimeMillis[c] = millis();
            }
            data[c] = newData;
          }
        } else {
          deviceDetected[c] = false;
          waitingForStartup[c] = false;
          connected[c] = false;
        }
      } else {
        errorCount[c]++;
      }
    }
    lastReadingTimeMicros = micros();
  }
}
