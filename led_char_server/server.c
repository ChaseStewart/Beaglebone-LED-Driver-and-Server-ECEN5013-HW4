
#include "server.h"

//static char from_driver[BUFFER_LEN];

int main(void)
{
	int retval, fileptr;

	fileptr = open(DRIVER_PATH, O_RDWR);
	if( fileptr < 0)
	{
		printf("[led_server] Failed to open device at %s\n", DRIVER_PATH);
		return errno;
	}

	/* establish socket */

	retval = 0;
	return retval;
}



int read_driver_var(int fileptr, int var_id)
{
	switch (var_id)
	{
		case VAR_1:
			printf("[led_server] Would have read var 1\n");
			break;
		case VAR_2:
			printf("[led_server] Would have read var 2\n");
			break;
		case VAR_3:
			printf("[led_server] Would have read var 3\n");
			break;
		default:
			printf("[led_server] Invalid input!\n");
			break;
	}
	return 0;
}

int read_all_driver_vars(int fileptr)
{
	/* TODO read all vars one-at-a-time */
	return 0;
}

int set_led_state(int fileptr, int state)
{
	int retval;

	if (state == 1)
	{
		retval = write(fileptr, LED_ON_STR, strlen(LED_ON_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
	}
	else if (state == 0)
	{
		retval = write(fileptr, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
	}
	else
	{
		printf("[led_server] Invalid LED state\n");
		return 1;
	}
	return 0;

}


int set_led_freq(int fileptr, int freq)
{
	int retval;

	if (freq == 0)
	{
		printf("[led_server] Freq is 0, turning LED off\n");
		retval = write(fileptr, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
		return 0;	
	}
	else if (freq > 1000 || freq < 1)
	{
		printf("[led_server] Invalid freq %d: Valid range between 1-1000\n", freq);
		return 1;	
	}
	else
	{
		/* TODO FIXME make this do freq */
		retval = write(fileptr, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
		return 0;
	}

}

int set_led_duty(int fileptr, int duty)
{
	int retval;
	if (duty == 100)
	{
		printf("[led_server] Duty cycle is 1, leaving LED on\n");
		retval = write(fileptr, LED_ON_STR, strlen(LED_ON_STR));
		if (retval < 0 )
		{
			printf("[led_server]Failed to write to driver!\n");
			return errno;
		}
		return 0;	
	}
	else if (duty == 0)
	{
		printf("[led_server] Duty cycle is 0, turning LED off\n");
		retval = write(fileptr, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
		return 0;	
		
	}
	else if ((duty > 100) || (duty < 1))
	{
		/* TODO do duty cycle correctly*/
		retval = write(fileptr, LED_ON_STR, strlen(LED_ON_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
	}
	else
	{
		printf("[led_server] Invalid LED duty cycle\n");
		return 1;
	}
	return 0;
}

