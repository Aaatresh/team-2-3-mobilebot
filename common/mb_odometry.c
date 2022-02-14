/*******************************************************************************
* mb_odometry.c
*
* TODO: Implement these functions to add odometry and dead rekoning 
*
*******************************************************************************/

#include "../mobilebot/mobilebot.h"
#include "mb_defs.h"
#include <math.h>
#include <rc/mpu.h>
#define PI 3.14159265358979323846


/*******************************************************************************
* mb_initialize_odometry() 
*
* TODO: initialize odometry
* NOTE: you should initialize from Optitrack data if available
*
*******************************************************************************/

rc_mpu_data_t data;

void mb_initialize_odometry(mb_odometry_t* mb_odometry, float x, float y, float theta){

	rc_mpu_config_t conf = rc_mpu_default_config();
	rc_mpu_initialize(&data, conf);
	rc_mpu_initialize_dmp(&data, conf);

	mb_odometry->x = x;
	mb_odometry->y = y;
	mb_odometry->theta = theta;
	
}

float theta_gyro_old = 0.0;

/*******************************************************************************
* mb_update_odometry() 
*
* TODO: calculate odometry from internal variables
*       publish new odometry to lcm ODOMETRY_CHANNEL
*
*******************************************************************************/
void mb_update_odometry(mb_odometry_t* mb_odometry, mb_state_t* mb_state){

	float enc2meters = (WHEEL_DIAMETER * M_PI) / (GEAR_RATIO * ENCODER_RES);

	float delta_s_left = mb_state->left_encoder_delta * enc2meters;
	float delta_s_right = mb_state->right_encoder_delta * enc2meters;
	float delta_theta_odo = (delta_s_right - delta_s_left) / WHEEL_BASE;
	float delta_s = (delta_s_left + delta_s_right) / 2;

	float delta_theta = 0.0;

	// float theta_gyro_old = data.dmp_TaitBryan[TB_YAW_Z];
	rc_mpu_read_gyro(&data);	

	float theta_gyro = data.dmp_TaitBryan[TB_YAW_Z];
	float delta_theta_gyro = mb_angle_diff_radians(theta_gyro, theta_gyro_old);

	// printf("theta_gyro: %f\ttheta_gyro_old: %f\tdiff: %f\n", theta_gyro, theta_gyro_old, mb_angle_diff_radians(theta_gyro, theta_gyro_old));
	theta_gyro_old = theta_gyro;

	// float delta_G_O = mb_angle_diff_radians(theta_gyro, mb_odometry->theta + delta_theta_odo);
	float delta_G_O = mb_angle_diff_radians(delta_theta_gyro, delta_theta_odo);

	// printf("delta_G_O: %f\n", fabs(delta_G_O));

	if(fabs(delta_G_O) > DELTA_THETA_THRESH && (delta_s_left != 0 || delta_s_right != 0))
		delta_theta = delta_theta_gyro;
		// mb_odometry->theta = theta_gyro;
	else
		delta_theta = delta_theta_odo;
		// mb_odometry->theta = mb_odometry->theta + delta_theta_odo;

	
	float delta_x = delta_s * cos(mb_odometry->theta + (delta_theta / 2.0));
	float delta_y = delta_s * sin(mb_odometry->theta + (delta_theta / 2.0));

	mb_odometry->x = mb_odometry->x + delta_x;
	mb_odometry->y = mb_odometry->y + delta_y;

	// clamp heading 
	mb_odometry->theta = mb_clamp_radians(mb_odometry->theta + delta_theta);

	odometry_t odo_msg;
	odo_msg.x = mb_odometry->x;
	odo_msg.y = mb_odometry->y;
	odo_msg.theta = mb_odometry->theta;
	odo_msg.utime = now;

	printf("PUBLISHING ODOM WITH TIME %i", odo_msg.utime);
	odometry_t_publish(lcm, ODOMETRY_CHANNEL, &odo_msg);
}


/*******************************************************************************
* mb_clamp_radians() 
* clamp an angle from -PI to PI
*******************************************************************************/
float mb_clamp_radians(float angle){

    if(angle < -PI)
    {
        for(; angle < -PI; angle += 2.0*PI);
    }
    else if(angle > PI)
    {
        for(; angle > PI; angle -= 2.0*PI);
    }

    return angle;
}


/*******************************************************************************
* mb_angle_diff_radians() 
* computes difference between 2 angles and wraps from -PI to PI
*******************************************************************************/
float mb_angle_diff_radians(float angle1, float angle2){
    float diff = angle2 - angle1;
    while(diff < -PI) diff+=2.0*PI;
    while(diff > PI) diff-=2.0*PI;
    return diff;
}
