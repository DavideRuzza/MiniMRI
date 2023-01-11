# 1 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino"
# 2 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino" 2
# 3 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino" 2


ADS1115_WE adc = ADS1115_WE(0x48);


# 9 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino" 2
# 24 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Bscanner_arduino.ino"
Motor x_axis(2, 94, 1, 10.);
Motor y_axis(2, 94, 0, 10.);
Motor z_axis(2, 512, 1, 10.);


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
  Serial.print(cmd, 16);
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
