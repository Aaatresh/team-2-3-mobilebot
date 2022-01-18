/*******************************************************************************
* measure_motor_params.c
*   Template code 
*   Complete this code to automatically measure motor parameters
*   or print out data to be namalyzed in numpy
* 
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <rc/start_stop.h>
#include <rc/encoder_eqep.h>
#include <rc/encoder.h>
#include <rc/adc.h>
#include <rc/time.h>
#include <rc/motor.h>
#include "../common/mb_motor.h"
#include "../common/mb_defs.h"
#include <signal.h>


float enc2meters = (WHEEL_DIAMETER * M_PI) / (GEAR_RATIO * ENCODER_RES);

void test_speed(float du, float dtime_s, FILE *fp);

static int running = 0;

static void __signal_handler(__attribute__ ((unused)) int dummy)
{
	running = 0;
	return;
}

/*******************************************************************************
* int main() 
*
*******************************************************************************/


int main(){

	// make sure another instance isn't running
    // if return value is -3 then a background process is running with
    // higher privaledges and we couldn't kill it, in which case we should
    // not continue or there may be hardware conflicts. If it returned -4
    // then there was an invalid argument that needs to be fixed.
    if(rc_kill_existing_process(2.0)<-2) return -1;

	// start signal handler so we can exit cleanly
    if(rc_enable_signal_handler()==-1){
                fprintf(stderr,"ERROR: failed to start signal handler\n");
                return -1;
    }

#if defined(MRC_VERSION_1v3) || defined(MRC_VERSION_2v1)
    if(mb_motor_init()<0){
        fprintf(stderr,"ERROR: failed to initialze mb_motors\n");
        return -1;
    }
#endif

#if defined(BEAGLEBONE_BLUE)
    if(rc_motor_init()<0){
        fprintf(stderr,"ERROR: failed to initialze motors\n");
        return -1;
    }
#endif

    if(rc_encoder_eqep_init()<0){
        fprintf(stderr,"ERROR: failed to initialze encoders\n");
        return -1;
    }
   
   	FILE *fp; 
	fp = fopen("speed_vs_pwm.txt", "w");

	signal(SIGINT, __signal_handler);
	running = 1;

	float dtime_s = 0.1;
	// fprintf(fp, "Sampling time: %f seconds\n\n", dtime_s);

    	// Initializing motors
	rc_motor_init();
	rc_encoder_eqep_init();

    // make PID file to indicate your project is running
	// due to the check made on the call to rc_kill_existing_process() above
	// we can be fairly confident there is no PID file already and we can
	// make our own safely.
	rc_make_pid_file();

	// done initializing so set state to RUNNING
	rc_set_state(RUNNING);

	// Run motor at given speed until speed settles
	rc_motor_set(1, -0.25);
	rc_motor_set(2, -0.25);
	rc_nanosleep(1E9);

	while(running){
		
		for(float i=-0.25; i <= 0.25; i = i + 0.01)
		{

			test_speed(i, dtime_s, fp);

			if(!running)
				break;
			
		}

		break;

	}
	
	// Stop motor in coast mode
	rc_motor_free_spin(1);
	rc_motor_free_spin(2);

	rc_motor_cleanup();
	rc_encoder_eqep_cleanup();
	
    // remove pid file LAST
    	fclose(fp);
	rc_remove_pid_file();   
	return 0;
}


void test_speed(float duty, float dtime_s, FILE *fp){
	
	long ticks_before = rc_encoder_eqep_read(1);

	rc_motor_set(1, duty);
	rc_motor_set(2, duty);

	rc_nanosleep(dtime_s * 1E9);

	long ticks_after = rc_encoder_eqep_read(1);

	float speed = -1 * ((ticks_after - ticks_before) / dtime_s) * enc2meters;

	fprintf(fp, "%f, %f\n", duty, speed);

}
