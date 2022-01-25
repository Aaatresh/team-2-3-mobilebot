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

pid_data_t lw_pid_data, rw_pid_data;

int mb_load_controller_config(){
    FILE* file = fopen(CFG_PATH, "r");
    if (file == NULL){
        printf("Error opening pid.cfg\n");
    }

    // Read PID parameters for left wheel
    fscanf(file, "%f %f %f %f %f %f %f\n",
		    &lw_pid_params.kp,
		    &lw_pid_params.ki,
		    &lw_pid_params.kd,
		    &lw_pid_params.dFilterHz,
		    &lw_pid_params.out_lim,
		    &lw_pid_params.int_lim, 
				&lw_pid_params.int_reset
	  );

    // Read PID parameters for right wheel
    fscanf(file, "%f %f %f %f %f %f %f",
		    &rw_pid_params.kp,
		    &rw_pid_params.ki,
		    &rw_pid_params.kd,
		    &rw_pid_params.dFilterHz,
		    &rw_pid_params.out_lim,
		    &rw_pid_params.int_lim,
				&lw_pid_params.int_reset
	  );


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

int mb_controller_convert_setpoints_to_setpointsLR(mb_setpoints_t* mb_setpoints, mb_setpoints_LR_t* mb_setpoints_LR){


    float diff = mb_setpoints->turn_velocity * WHEEL_BASE;

    mb_setpoints_LR -> left_velocity = mb_setpoints -> fwd_velocity - diff/2;
    mb_setpoints_LR -> right_velocity = mb_setpoints -> fwd_velocity + diff/2;
    
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


	mb_setpoints_LR_t mb_setpoints_LR;
	mb_controller_convert_setpoints_to_setpointsLR(mb_setpoints, &mb_setpoints_LR);

	mb_state->left_cmd = speed_to_duty_cycle(LEFT_MOTOR, mb_setpoints_LR.left_velocity);
	mb_state->right_cmd = speed_to_duty_cycle(RIGHT_MOTOR, mb_setpoints_LR.right_velocity);

	printf("pwm: %f\t vel: %f", mb_state->left_cmd, mb_setpoints_LR.left_velocity);

	//printf("Inside open loop controller!");

	// mb_state->left_cmd = 0.25;
	// mb_state->right_cmd = 0.25;

    return 0;
}

void update_pid_data(float new_error, pid_data_t* pid_data, pid_parameters_t*pid_parameters){

	pid_data -> ierror += new_error;

	// reset integrator error if current error is small
	/*
	if ((new_error < pid_parameters->int_reset) && (new_error > -pid_parameters->int_reset)) {
        printf("RESETING IERR");
        pid_data->ierror = 0;
    }
	*/
	if (pid_data->ierror > pid_parameters->int_lim) pid_data->ierror = pid_parameters->int_lim;
	if (pid_data->ierror < -pid_parameters->int_lim) pid_data->ierror = -pid_parameters->int_lim; 

	pid_data -> derror = new_error - pid_data-> error;
	pid_data -> error = new_error;

}

float compute_pid_control(float feedforward, pid_data_t* pid_data, pid_parameters_t*pid_parameters){

	double out = feedforward;
	out += -pid_parameters->kp * pid_data->error;
	out += -pid_parameters->kd * pid_data->derror;
	out += -pid_parameters->ki * pid_data->ierror;

	if (out > pid_parameters->out_lim) out = pid_parameters->out_lim;
	if (out < -pid_parameters->out_lim) out = -pid_parameters->out_lim;


	return out;
}

int mb_controller_update(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints){  

	mb_setpoints_LR_t mb_setpoints_LR;
	mb_controller_convert_setpoints_to_setpointsLR(mb_setpoints, &mb_setpoints_LR);

	// mb_state->left_velocity = mb_state->left_encoder_delta / lw_pid_params.dFilterHz;
	// mb_state->right_velocity = mb_state->right_encoder_delta / rw_pid_params.dFilterHz;

	// compute errors
	float lw_error = mb_state->left_velocity - mb_setpoints_LR.left_velocity;
	float rw_error = mb_state->right_velocity - mb_setpoints_LR.right_velocity;

	// Update the pid data struct
	update_pid_data(lw_error, &lw_pid_data, &lw_pid_params);
	update_pid_data(rw_error, &rw_pid_data, &rw_pid_params);
    /*
	printf("LPID: des: %f, speed: %f,  err %f, derr: %f, ierr: %f\n", 
			mb_setpoints_LR.left_velocity, 
			mb_state->left_velocity,
			lw_pid_data.error,
			lw_pid_data.derror,
			lw_pid_data.ierror
			);
    */
    
	printf("RPID: des: %f, speed: %f,  err %f, derr: %f, ierr: %f\n", 
			mb_setpoints_LR.right_velocity, 
			mb_state->right_velocity,
			rw_pid_data.error,
			rw_pid_data.derror,
			rw_pid_data.ierror
			);
    
	// compute command
	float lw_cmd_speed = compute_pid_control(mb_setpoints_LR.left_velocity, &lw_pid_data, &lw_pid_params);
	float rw_cmd_speed = compute_pid_control(mb_setpoints_LR.right_velocity, &rw_pid_data, &rw_pid_params);
	
	// send the command
	mb_state->left_cmd = speed_to_duty_cycle(LEFT_MOTOR, lw_cmd_speed);
	mb_state->right_cmd = speed_to_duty_cycle(RIGHT_MOTOR, rw_cmd_speed); 


	// // Right wheel PID
	
	// float rw_correction = rw_pid_params.kp * rw_error; // plus ki and kd related terms...
	// mb_state->right_cmd = mb_state->right_cmd + rw_correction;


	// // Cap max and min values of mb_state->left_cmd and mb_state->right_cmd
	// if(mb_state->left_cmd < lw_pid_params.int_lim)
	// 	mb_state->left_cmd = lw_pid_params.int_lim;
	// else if(mb_state->left_cmd > lw_pid_params.out_lim)
	// 	mb_state->left_cmd = lw_pid_params.out_lim;

	// if(mb_state->right_cmd < rw_pid_params.int_lim)
	// 	mb_state->right_cmd = rw_pid_params.int_lim;
	// else if(mb_state->right_cmd > rw_pid_params.out_lim)
	// 	mb_state->right_cmd = rw_pid_params.out_lim;

	// rc_nanosleep((1.0 / lw_pid_params.dFilterHz) * 1E9);

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
