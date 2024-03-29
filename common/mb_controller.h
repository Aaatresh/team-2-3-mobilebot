#ifndef MB_CONTROLLER_H
#define MB_CONTROLLER_H


#include "../mobilebot/mobilebot.h"
#define CFG_PATH "/home/debian/mobilebot/common/pid.cfg"
#include "MOTOR_CALIBRATION.h"



int mb_initialize_controller();
int mb_load_controller_config();
int mb_controller_setpoints_to_setpoints_LR(mb_setpoints_t*, mb_setpoints_LR_t*);
int mb_controller_update(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints);
int mb_destroy_controller();
double speed_to_duty_cycle(int ch, double speed);

/************
* Add your PID and other SISO filters here.
* examples:
* rc_filter_t left_wheel_speed_pid;
* rc_filter_t fwd_vel_sp_lpf;
*************/

// rc_filter_t low_pass_left_filt = RC_FILTER_INITIALIZER;
// rc_filter_t low_pass_right_filt  = RC_FILTER_INITIALIZER; 

/***********
* For each PID filter you want to load from settings
* add a pid_parameter_t or filter_parameter_t
* example:
* pid_parameters_t left_wheel_speed_params;
* filter_parameters_t fwd_vel_sp_lpf_params;
************/

#endif

