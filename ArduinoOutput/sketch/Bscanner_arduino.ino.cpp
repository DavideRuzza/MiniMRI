#include <Arduino.h>
#line 1 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino"
#include<ADS1115_WE.h> 
#include<Wire.h>


ADS1115_WE adc = ADS1115_WE(0x48);
#define CH0 ADS1115_COMP_0_GND

#include "Motor.h"

#define X_HOME_DIR 1
#define Y_HOME_DIR 1
#define Z_HOME_DIR 0

#define X_AXIS_DIR 1
#define Y_AXIS_DIR 0
#define Z_AXIS_DIR 1

#define X_LIM_PIN A2
#define Y_LIM_PIN A1
#define Z_LIM_PIN A0

#define MAX_CMD 30

Motor x_axis(HALF_STEP, 94, X_AXIS_DIR, 10.);
Motor y_axis(HALF_STEP, 94, Y_AXIS_DIR, 10.);
Motor z_axis(HALF_STEP, 512, Z_AXIS_DIR, 10.);


#line 29 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino"
void setup();
#line 55 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino"
void loop();
#line 83 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino"
float readChannel(ADS1115_MUX channel);
#line 29 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino"
void setup() {
  Serial.begin(115200);
  
  // Wire.begin();

  // if(!adc.init()){
  //   Serial.println("ADS1115 not connected!");
  // }

  // adc.setVoltageRange_mV(ADS1115_RANGE_2048);

  // x_axis.attach(2, 3, 4, 5);
  // x_axis.set_lim(X_LIM_PIN, 125.0, X_HOME_DIR, 0, 125.0);

  // y_axis.attach(6, 7, 8, 9);
  // y_axis.set_lim(Y_LIM_PIN, 0, Y_HOME_DIR, 0, 125.0);
  
  // z_axis.attach(10, 11, 12, 13);
  // z_axis.set_lim(Z_LIM_PIN, 0, Z_HOME_DIR, 0, 103.9);

  // x_axis.release();
  // y_axis.release();
  // z_axis.release();

}

void loop() {

  char buf[5] = {};
  int i = 0;
  while (true){
    if (Serial.available()){
      char c = Serial.read();
      if (c=='\n'){
        break;
      }
      if (i<=4){
        buf[i] = c;
        i++;
      }
    }
  }

  char cmd = buf[0];
  long value = buf[1]<<24 | buf[2]<<16 | buf[3]<<8 | buf[4];
  float val_fl = * (float *) &value;
  Serial.print(cmd, HEX);
  Serial.print(", ");
  Serial.println(value);
  Serial.print(", ");
  Serial.println(val_fl);
//  
}

float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while(adc.isBusy()){}
  voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}

