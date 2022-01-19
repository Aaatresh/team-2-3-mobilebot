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

pid_parameters_t lw_pid_params, rw_pid_params;

int mb_load_controller_config(){
    FILE* file = fopen(CFG_PATH, "r");
    if (file == NULL){
        printf("Error opening pid.cfg\n");
    }

    // Read PID parameters for left wheel
    fscanf(file, "%f %f %f %f %f %f\n",
		    &lw_pid_params.kp,
		    &lw_pid_params.ki,
		    &lw_pid_params.kd,
		    &lw_pid_params.dFilterHz,
		    &lw_pid_params.out_lim,
		    &lw_pid_params.int_lim
	  );

    // Read PID parameters for right wheel
    fscanf(file, "%f %f %f %f %f %f",
		    &rw_pid_params.kp,
		    &rw_pid_params.ki,
		    &rw_pid_params.kd,
		    &rw_pid_params.dFilterHz,
		    &rw_pid_params.out_lim,
		    &rw_pid_params.int_lim
	  );

	printf("rw pid params: %f %f %f\n", lw_pid_params.kp, lw_pid_params.ki, lw_pid_params.kd);
	printf("rw pid params: %f %f %f\n", lw_pid_params.dFilterHz, lw_pid_params.out_lim, lw_pid_params.int_lim);

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

double speed_to_duty_cycle(int ch, double speed){

   if (speed == 0.0) return 0.0;

	 double duty = 0.0;

   if (ch == LEFT_MOTOR){
       if (speed > 0){
           
          duty = MOTOR_CALIB_LEFT_POS_M * speed + MOTOR_CALIB_LEFT_POS_B;
       }
       else{
        	duty = MOTOR_CALIB_LEFT_NEG_M * speed + MOTOR_CALIB_LEFT_NEG_B;
       }
   }

   if (ch == RIGHT_MOTOR){
       if (speed > 0){
           duty = MOTOR_CALIB_RIGHT_POS_M * speed + MOTOR_CALIB_RIGHT_POS_B;
       }
       else{
           duty =  MOTOR_CALIB_RIGHT_NEG_M * speed + MOTOR_CALIB_RIGHT_NEG_B;
       }
   }

   if (duty >= 1.0) duty = 1.0;
   if (duty <= -1.0) duty = -1.0;

   return duty;

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

	// mb_state->left_cmd = speed_to_duty_cycle(...);
	// mb_state->right_cmd = speed_to_duty_cycle(...);
	mb_state->left_cmd = 0.25;
	mb_state->right_cmd = 0.25;

    return 0;
}

int mb_controller_update(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints){  

	// Left wheel PID
	float lw_error = mb_state->left_velocity - mb_setpoints->fwd_velocity;
	float lw_correction = lw_pid_params.kp * lw_error; // plus ki and kd related terms...
	mb_state->left_cmd = mb_state->left_cmd + lw_correction;

	// Right wheel PID
	float rw_error = mb_state->right_velocity - mb_setpoints->fwd_velocity;
	float rw_correction = rw_pid_params.kp * rw_error; // plus ki and kd related terms...
	mb_state->right_cmd = mb_state->right_cmd + rw_correction;


	// Cap max and min values of mb_state->left_cmd and mb_state->right_cmd
	if(mb_state->left_cmd < lw_pid_params.int_lim)
		mb_state->left_cmd = lw_pid_params.int_lim;
	else if(mb_state->left_cmd > lw_pid_params.out_lim)
		mb_state->left_cmd = lw_pid_params.out_lim;

	if(mb_state->right_cmd < rw_pid_params.int_lim)
		mb_state->right_cmd = rw_pid_params.int_lim;
	else if(mb_state->right_cmd > rw_pid_params.out_lim)
		mb_state->right_cmd = rw_pid_params.out_lim;

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
