#include<ADS1115_WE.h> 
#include<Wire.h>


ADS1115_WE adc = ADS1115_WE(0x48);
#define CH0 ADS1115_COMP_0_1

#include "Motor.h"

#define X_HOME_DIR 1
#define Y_HOME_DIR 1
#define Z_HOME_DIR 0

#define X_AXIS_DIR 1
#define Y_AXIS_DIR 0
#define Z_AXIS_DIR 1

#define X_LIM_PIN A0
#define Y_LIM_PIN A1
#define Z_LIM_PIN A2

#define MAX_CMD 30

Motor x_axis(HALF_STEP, 94, X_AXIS_DIR, 30.);
Motor y_axis(HALF_STEP, 94, Y_AXIS_DIR, 30.);
Motor z_axis(HALF_STEP, 512, Z_AXIS_DIR, 30.);

bool st = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, st);
   Wire.begin();
   if(!adc.init()){  
   }
   adc.setVoltageRange_mV(ADS1115_RANGE_2048);
   
   x_axis.attach(31, 33, 35, 37);
   x_axis.set_lim(X_LIM_PIN, 155.0, X_HOME_DIR, 0, 155.0);
   y_axis.attach(39, 41, 43, 45);
   y_axis.set_lim(Y_LIM_PIN, 0, Y_HOME_DIR, 0, 125.0);
   z_axis.attach(47, 49, 51, 53);
   z_axis.set_lim(Z_LIM_PIN, 0, Z_HOME_DIR, 0, 103.9);
   
   x_axis.release();
   y_axis.release();
   z_axis.release();

   //x_axis.home();

   //x_axis.move_mm(10);

}

void loop() {
//   Serial.print(-2048);
//   Serial.print(" , ");
//   Serial.print(readChannel(CH0));
//   Serial.print(" , ");
//   Serial.println(2048);

   
  char buf[5] = {};
  int i = 0;
  while (true){
    if (Serial.available()){
      char c = Serial.read();
      if (c=='\n' && i >4){
        break;
      }
      if (i<=4){
        buf[i] = c;
        i++;
      }
    }
  }
  char cmd = buf[0];
  long value = ((long)buf[1]<<24)&0xff000000 | ((long)buf[2]<<16)&0x00ff0000 | ((long)buf[3]<<8)&0x0000ff00 | (long)buf[4]&0x000000ff;
  float val_fl = * (float *) &value;
  
  switch (cmd){
    case 0x01: //move_mm x
      {
        x_axis.move_mm(val_fl);
        Serial.write(0x01);
        write_float(x_axis.pos);
      }break;
    case 0x02: //move_mm y
      {
        y_axis.move_mm(val_fl);
        Serial.write(0x02);
        write_float(y_axis.pos);
      }break;
    case 0x03: //move_mm z
      {
        z_axis.move_mm(val_fl);
        Serial.write(0x03);
        write_float(z_axis.pos);
      }break;
    case 0x04: //home x
      {
        x_axis.home();
        Serial.write(0x04);
        write_float(x_axis.pos);
      }break;
    case 0x05: //home y
      {
        y_axis.home();
        Serial.write(0x05);
        write_float(y_axis.pos);
      }break;
    case 0x06: //home z
      {
        z_axis.home();
        Serial.write(0x06);
        write_float(z_axis.pos);
      }break;
    case 0x07: //move to x pos
      {
        x_axis.move_to(val_fl);
        Serial.write(0x07);
        write_float(x_axis.pos);
      }break;
    case 0x08: //move to y pos
      {
        y_axis.move_to(val_fl);
        Serial.write(0x08);
        write_float(y_axis.pos);
      }break;
    case 0x09: //move to z pos
      {
        z_axis.move_to(val_fl);
        Serial.write(0x09);
        write_float(z_axis.pos);
        //Serial.write(0x00);Serial.write(0x00);Serial.write(0x00);Serial.write(0x00);
      }break;
      
    case 0x0b: // get_adc_value
      {
        float ad_val = 0;
        int navg = 10;
        for (int i = 0; i<navg;i++){
          ad_val += readChannel(CH0);
        }
        ad_val/=navg;
        Serial.write(0x0b);
        write_float(ad_val);
      }break;
    default:
      {
        Serial.write(0xff);
        write_float(0);
      }break;
  }
}

void write_float(float val){
  long val_int = * (long *) &val;
  Serial.write(val_int>>24&0xff);
  Serial.write(val_int>>16&0xff);
  Serial.write(val_int>>8&0xff);
  Serial.write(val_int&0xff);
}

float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while(adc.isBusy()){}
  voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}
