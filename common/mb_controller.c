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

int mb_controller_update_open_loop(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints){  

	// m_forward, b_forward => slope and y-intercept for forward movement
	// m_backward, b_backward => slope and y-intercept for forward movement

	mb_state->left_cmd = speed_to_duty_cycle(...);
	mb_state->right_cmd = speed_to_duty_cycle(...);

    return 0;
}

int mb_controller_update(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints){  

	// Left wheel PID
	lw_error = mb_state->left_velocity - mb_setpoints->fwd_velocity;
	lw_correction = lw_pid_params.kp * lw_error; // plus ki and kd related terms...
	mb_state->left_cmd = mb_state->left_cmd + lw_correction;

	// Right wheel PID
	rw_error = mb_state->right_velocity - mb_setpoints->fwd_velocity;
	rw_correction = rw_pid_params.kp * rw_error; // plus ki and kd related terms...
	mb_state->right_cmd = mb_state->right_cmd + rw_correction;


	// Cap max and min values of mb_state->left_cmd and mb_state->right_cmd
	if(mb_state->left_cmd < lw_pid_params.int_lim)
		mb_state->left_cmd = lw_pid_params.int_lim;
	else if(mb_state->left_cmd > lw_pid_params.out_lim)
		mb_state->left_cmd = lw_pid_params.out_lim

	if(mb_state->right_cmd < rw_pid_params.int_lim)
		mb_state->right_cmd = rw_pid_params.int_lim;
	else if(mb_state->right_cmd > rw_pid_params.out_lim)
		mb_state->right_cmd = rw_pid_params.out_lim

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

int mb_destroy_controller(){
    return 0;
}
