#include "../mobilebot/mobilebot.h"

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


int mb_load_controller_config(){
    FILE* file = fopen(CFG_PATH, "r");
    if (file == NULL){
        printf("Error opening pid.cfg\n");
    }

/******
*
*   Example of loading a line from .cfg file:
*
*    fscanf(file, "%f %f %f %f", 
*        &pid_params.kp,
*        &pid_params.ki,
*        &pid_params.kd,
*        &pid_params.dFilterHz
*        );
*
******/

	
    fscanf(file, "%f %f %f %f", 
        &pid_parameters.kp,
        &pid_parameters.ki,
        &pid_parameters.kd,
        &pid_parameters.dFilterHz,
	&pid_parameters.out_lim,
	&pid_parameters.int_lim
        );

    pid_parameters.prev_error_running_sum = 0;
    pid_parameters.prev_error = 0;

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

	error = mb_state.turn_velocity - mb_setpoints.turn_velocity;
	int_error = pid_parameters.prev_error_running_sum + error;
	d_error = error - pid_parameters.prev_error;
	pid_parameters.prev_error = error;

	correction = (pid_parameters.kp * error) + (pid_parameters.ki * int_error) + (pid_parameters.kd * d_error);

	mb_state.left_wheel_duty = mb_state.left_wheel_duty + correction;
	if(mb_state.left_wheel_duty < -pid_paramters.int_lim)
		mb_state.left_wheel_duty = -1;
	else if(mb_state.left_wheel_duty > pid_parameters.out_lim)
		mb_state.left_wheel_duty = 1;

	mb_state.right_wheel_duty = mb_state.right_wheel_duty - correction;
	if(mb_state.right_wheel_duty < -pid_parameters.int_lim)
		mb_state.right_wheel_duty = -1;
	else if(mb_state.right_wheel_duty > pid_parameters.out_lim)
		mb_state.right_wheel_duty = 1;

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

int open_loop_controller(mb_state_t* mb_state)
{
	rc_motor_set(1, mb_state.left_wheel_duty);
	rc_motor_set(2, mb_state.right_wheel_duty);
}

int pid_controller(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints)
{
	rc_motor_set(1, mb_state.left_wheel_duty);
	rc_motor_set(2, mb_state.right_wheel_duty);

	mb_controller_update(mb_state, mb_setpoints);

	rc_nanosleep(1.0 / pid_parameters.dFilterHz);
}


int mb_destroy_controller(){
    return 0;
}
