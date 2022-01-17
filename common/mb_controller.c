#include "../mobilebot/mobilebot.h"
#include <signal.h>
#include <rc/motor.h>
#include <rc/encoder.h>
#include <rc/encoder_eqep.h>

/*******************************************************************************
* int mb_initialize()
*
* this initializes all the PID controllers from the configuration file
* you can use this as is or modify it if you want a different format
*
* return 0 on success
*
*******************************************************************************/

int mb_initialize_controller(){
    mb_load_controller_config();
    return 0;
}

/*******************************************************************************
* int mb_load_controller_config()
*
* this provides a basic configuration load routine
* you can use this as is or modify it if you want a different format
*
* return 0 on success
*
*******************************************************************************/

// PID parameters to store left and right wheel data
pid_parameters_t lw_pid_params, rw_pid_params;

int mb_load_controller_config(){
    FILE* file = fopen(CFG_PATH, "r");
    if (file == NULL){
        printf("Error opening pid.cfg\n");
    }

	
    fscanf(file, "%f %f %f %f %f %f\n", 
        &lw_pid_params.kp,
        &lw_pid_params.ki,
        &lw_pid_params.kd,
        &lw_pid_params.dFilterHz,
	&lw_pid_params.out_lim,
	&lw_pid_params.int_lim
        );
    lw_pid_params.prev_error_running_sum = 0;
    lw_pid_params.prev_error = 0;

    fscanf(file, "%f %f %f %f %f %f\n", 
        &rw_pid_params.kp,
        &rw_pid_params.ki,
        &rw_pid_params.kd,
        &rw_pid_params.dFilterHz,
	&rw_pid_params.out_lim,
	&rw_pid_params.int_lim
        );
    rw_pid_params.prev_error_running_sum = 0;
    rw_pid_params.prev_error = 0;


    fclose(file);
    return 0;
}

/*******************************************************************************
* int mb_controller_update()
* 
* TODO: Write your PID controller here
* take inputs from the global mb_state
* write outputs to the global mb_state
*
* return 0 on success
*
*******************************************************************************/

int mb_controller_update(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints){  

	// PID for left wheel
	float lw_error = mb_state->lw_vel - mb_setpoints->lw_vel;
	float lw_int_error = lw_pid_params.prev_error_running_sum + lw_error;
	float lw_d_error = lw_error - lw_pid_params.prev_error;
	lw_pid_params.prev_error = lw_error;

	float lw_correction = (lw_pid_params.kp * lw_error) + (lw_pid_params.ki * lw_int_error) + (lw_pid_params.kd * lw_d_error);

	// PID for right wheel
	float rw_error = mb_state->rw_vel - mb_setpoints->rw_vel;
	float rw_int_error = rw_pid_params.prev_error_running_sum + rw_error;
	float rw_d_error = rw_error - rw_pid_params.prev_error;
	rw_pid_params.prev_error = rw_error;

	float rw_correction = (rw_pid_params.kp * rw_error) + (rw_pid_params.ki * rw_int_error) + (rw_pid_params.kd * rw_d_error);

	mb_state->left_wheel_duty = mb_state->left_wheel_duty + lw_correction;
	if(mb_state->left_wheel_duty < -lw_pid_params.int_lim)
		mb_state->left_wheel_duty = -1;
	else if(mb_state->left_wheel_duty > lw_pid_params.out_lim)
		mb_state->left_wheel_duty = 1;

	mb_state->right_wheel_duty = mb_state->right_wheel_duty - rw_correction;
	if(mb_state->right_wheel_duty < -rw_pid_params.int_lim)
		mb_state->right_wheel_duty = -1;
	else if(mb_state->right_wheel_duty > rw_pid_params.out_lim)
		mb_state->right_wheel_duty = 1;

    return 0;
}


/*******************************************************************************
* int mb_destroy_controller()
* 
* TODO: Free all resources associated with your controller
*
* return 0 on success
*
*******************************************************************************/

int read_wheel_vels(mb_state_t* mb_state, float dFilterHz)
{
	uint64_t lw_ticks = rc_encoder_eqep_read(1);
	mb_state->left_encoder_delta = lw_ticks - mb_state->left_encoder_total;
	mb_state->left_encoder_total = lw_ticks;

	uint64_t rw_ticks = rc_encoder_eqep_read(2);
	mb_state->right_encoder_delta = rw_ticks - mb_state->right_encoder_total;
	mb_state->right_encoder_total = rw_ticks;

	mb_state->lw_vel = mb_state->left_encoder_delta / dFilterHz;
	mb_state->rw_vel = mb_state->right_encoder_delta / dFilterHz;

	return 0;
}

int open_loop_controller(mb_state_t* mb_state)
{
	rc_motor_set(1, mb_state->left_wheel_duty);
	rc_motor_set(2, mb_state->right_wheel_duty);

	return 0;
}

int pid_controller(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints)
{
	rc_motor_set(1, mb_state->left_wheel_duty);
	rc_motor_set(2, mb_state->right_wheel_duty);

	read_wheel_vels(mb_state, lw_pid_params.dFilterHz);

	mb_controller_update(mb_state, mb_setpoints);

	rc_nanosleep(1.0 / lw_pid_params.dFilterHz);

	return 0;
}


int mb_destroy_controller(){

	rc_motor_cleanup();
	rc_encoder_eqep_cleanup();

    return 0;
}

static int running = 0;

static void __signal_handler(__attribute__ ((unused)) int dummy)
{
	running = 0;
	return;
}

int main()
{
	//  Initialize motors and encoders
	rc_motor_init();
	rc_encoder_eqep_init();

	mb_initialize_controller();

	signal(SIGINT, __signal_handler);
	running = 1;

	mb_state_t* mb_state;
	mb_setpoints_t* mb_setpoints;

	mb_state->left_wheel_duty = 0.25;
	mb_state->right_wheel_duty = 0.25;

	open_loop_controller(mb_state);

	while(running)
	{
	}

	mb_destroy_controller();

	return 0;
}
