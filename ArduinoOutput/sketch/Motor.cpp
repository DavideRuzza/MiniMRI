#line 1 "c:\\D\\MiniMRI\\BScanner\\Bscanner_arduino\\Motor.cpp"
#include "Motor.h"

Motor::Motor(int mode, int step_per_mm, bool axis_dir, float vel){
  _mode = mode;
  _step_per_mm = step_per_mm;
  _ax_dir = axis_dir;
  if (_mode == 0 || _mode == 1){
    _step_per_rev = 2048;
    delay_step = (uint32_t)((1e6/mm_to_step(vel))); // in microseconds
  } else {
    _step_per_rev = 4096;
    delay_step = (uint32_t)((5e5/mm_to_step(vel))); // in microseconds
  }
}

void Motor::attach(int IN1, int IN2, int IN3, int IN4) {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  _IN1 = IN1;
  _IN2 = IN2;
  _IN3 = IN3;
  _IN4 = IN4;

}

void Motor::set_lim(int home_pin, float home_offset, bool home_dir, int lim_default_value, float ax_length){
  pinMode(home_pin, INPUT);
  _home_pin = home_pin;
  _home_dir = home_dir;
  _home_offset = home_offset;
  _lim_def_val = lim_default_value;
  _ax_length = ax_length;
}

void Motor::home(){
  _dir = _home_dir;
  
  while (digitalRead(_home_pin)==_lim_def_val){
    Stepping();
  }

  pos = _home_offset;
  release();
}

void Motor::move_steps(uint32_t nsteps, bool dir){
  
  _dir = (dir) ? _ax_dir : ! _ax_dir;

  step_left = nsteps;
  while(step_left>0){
    Stepping();
    step_left--;
  }
  release();
}

void Motor::move_mm(float mm){
  _dir = (mm<0) ? !_ax_dir : _ax_dir;

  step_left = mm_to_step(mm);
  float add_pos = abs(mm)*step_to_mm(1)/mm;

  while(step_left>0){
    if (pos+add_pos<0 || pos+add_pos>_ax_length)
      break;
    Stepping();
    pos += add_pos;
    step_left--;
  }
  release();
}

void Motor::move_to(float target_pos){
  float mm_to_do = target_pos-pos;
  move_mm(mm_to_do);
} 

float Motor::step_to_mm(uint32_t nstep){
  return (float)(nstep/(float)_step_per_mm);
}

uint32_t Motor::mm_to_step(float mm){
  return (uint32_t)((float)(abs(mm)*(float)_step_per_mm));
}

void Motor::SetStep() {
  
  if(_dir){ 
    _curr_step++;
  } else {
   _curr_step--; 
  }
  
  if(_curr_step>7){_curr_step=0;}
  if(_curr_step<0){_curr_step=7;}
}

void Motor::Stepping(){
  
  digitalWrite(_IN1, _step_sequence[_mode][_curr_step][0]); 
  digitalWrite(_IN2, _step_sequence[_mode][_curr_step][1]);
  digitalWrite(_IN3, _step_sequence[_mode][_curr_step][2]);
  digitalWrite(_IN4, _step_sequence[_mode][_curr_step][3]);
  
  SetStep();
  delayMicroseconds(800); //needed

  //delay set by velocity
  if (delay_step > 65000){
      delay(delay_step/1000);
    } else {
      delayMicroseconds(delay_step);
    }
}

void Motor::release(){
  digitalWrite(_IN1, LOW);
  digitalWrite(_IN2, LOW);
  digitalWrite(_IN3, LOW);
  digitalWrite(_IN4, LOW);
}





