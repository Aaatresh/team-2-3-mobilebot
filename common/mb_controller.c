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

double speed_to_duty_cycle(int ch, double speed){

   if (speed == 0.0) return 0.0;

   if (ch == LEFT_MOTOR){
       if (speed > 0){
           
           double duty = MOTOR_CALIB_LEFT_POS_M * speed + MOTOR_CALIB_LEFT_POS_B);
       }
       else{
           double duty = MOTOR_CALIB_LEFT_NEG_M * speed + MOTOR_CALIB_LEFT_NEG_B);
       }
   }

   if (ch == RIGHT_MOTOR){
       if (speed > 0){
           double duty = MOTOR_CALIB_RIGHT_POS_M * speed + MOTOR_CALIB_RIGHT_POS_B);
       }
       else{
           double duty =  MOTOR_CALIB_RIGHT_NEG_M * speed + MOTOR_CALIB_RIGHT_NEG_B);
       }
   }

   if (duty >= 1.0) duty = 1.0;
   if (duty <= -1.0) duty = 1.0;

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

int mb_controller_update(mb_state_t* mb_state, mb_setpoints_t* mb_setpoints){  
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
