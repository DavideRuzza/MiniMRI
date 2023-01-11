#ifndef St
#define St

#include "arduino.h"

#define WAVE_DRIVE 0
#define FULL_STEP 1
#define HALF_STEP 2


class Motor {
  public:
    
    Motor(int mode, int step_per_mm, bool axis_dir, float vel);  // [vel] = mm/sec
    void attach(int IN1, int IN2, int IN3, int IN4);
    void set_lim(int home_pin, float home_offset, bool home_dir, int lim_default_value, float _ax_length);
    void home();
    void move_steps(uint32_t nsteps, bool dir);
    void move_mm(float mm);
    void move_to(float position); 
    void release();

    float step_to_mm(uint32_t nstep);
    uint32_t mm_to_step(float mm);

    void SetStep();
    void Stepping();
    
    uint32_t delay_step;  
    float pos;
    uint32_t step_left;
    bool busy = false;
  private:

    int _IN1, _IN2, _IN3, _IN4;
    bool _dir;
    int _mode;
    int _curr_step;
    int _step_per_rev;
    int _step_per_mm;
    int _step_sequence[3][8][4] = {
      {{LOW, LOW, LOW,  HIGH}, {LOW, LOW,  HIGH, LOW}, {LOW,  HIGH, LOW,  LOW}, {HIGH, LOW,  LOW,  LOW}, {LOW, LOW,  LOW,  HIGH}, {LOW,  LOW,  HIGH, LOW}, {LOW,  HIGH, LOW, LOW}, {HIGH, LOW, LOW, LOW}},
      {{LOW, LOW, HIGH, HIGH}, {LOW, HIGH, HIGH, LOW}, {HIGH, HIGH, LOW,  LOW}, {HIGH, LOW,  LOW,  HIGH},{LOW, LOW,  HIGH, HIGH}, {LOW,  HIGH, HIGH, LOW}, {HIGH, HIGH, LOW, LOW}, {HIGH, LOW, LOW, HIGH}},
      {{LOW, LOW, LOW,  HIGH}, {LOW, LOW,  HIGH, HIGH},{LOW,  LOW,  HIGH, LOW}, {LOW,  HIGH, HIGH, LOW}, {LOW, HIGH, LOW,  LOW},  {HIGH, HIGH, LOW,  LOW}, {HIGH, LOW,  LOW, LOW}, {HIGH, LOW, LOW, HIGH}}
    };

    bool _ax_dir;
    float _ax_length;

    int _home_pin;
    float _home_offset;
    bool _home_dir;
    int _lim_def_val; 


};

#endif
