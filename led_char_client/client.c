#include "client.h"

static char in_message[INPUT_LEN];
static char out_message[INPUT_LEN];
int client_state = STATE_RUNNING;

int main(void)
{
	int socket_id;
	struct sockaddr_in led_client;

	/* establish socket */
	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id == -1)
	{
		printf("[led_client] could not create socket\n");
	}

	led_client.sin_addr.s_addr = inet_addr(LED_SERVER_HOST);
	led_client.sin_family = AF_INET;
	led_client.sin_port = htons(LED_SERVER_PORT);
	
	printf("[led_client] Connecting to %s:%d\n", LED_SERVER_HOST, LED_SERVER_PORT);

	if (connect(socket_id, (struct sockaddr *)&led_client, sizeof(led_client)) < 0)
	{
		printf("[led_client] Failed to connect to remote server\n");
		return 1;
	}
	while (client_state == STATE_RUNNING)
	{
		printf("What would you like to send?\n");
		scanf("%s", out_message);
		if(send(socket_id, out_message, INPUT_LEN, 0) < 0)
		{
			printf("[led_client] Failed to send to remote server");
			client_state = STATE_ERROR;
		}
		if(recv(socket_id, in_message, INPUT_LEN, 0) < 0)
		{
			printf("[led_client] Failed to receive from remote server");
			client_state = STATE_ERROR;
		}
	}
	
	printf("[led_client] Terminating client");
	close(socket_id);
	return client_state;
}



int req_driver_var(int var_id)
{
	switch (var_id)
	{
		case VAR_1:
			printf("[led_client] Would have read var 1\n");
			break;
		case VAR_2:
			printf("[led_client] Would have read var 2\n");
			break;
		case VAR_3:
			printf("[led_client] Would have read var 3\n");
			break;
		default:
			printf("[led_client] Invalid input!\n");
			break;
	}
	return 0;
}

int read_all_driver_vars(void)
{
	/* TODO read all vars one-at-a-time */
	return 0;
}

int set_led_state(int state)
{
	int retval;

	if (state == 1)
	{
		retval = write(file_id, LED_ON_STR, strlen(LED_ON_STR));
		if (retval < 0 )
		{
			printf("[led_client] Failed to write to driver!\n");
			return errno;
		}
	}
	else if (state == 0)
	{
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_client] Failed to write to driver!\n");
			return errno;
		}
	}
	else
	{
		printf("[led_client] Invalid LED state\n");
		return 1;
	}
	return 0;

}


int set_led_freq(int freq)
{
	int retval;

	if (freq == 0)
	{
		printf("[led_client] Freq is 0, turning LED off\n");
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_client] Failed to write to driver!\n");
			return errno;
		}
		return 0;	
	}
	else if (freq > 1000 || freq < 1)
	{
		printf("[led_client] Invalid freq %d: Valid range between 1-1000\n", freq);
		return 1;	
	}
	else
	{
		/* TODO FIXME make this do freq */
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_client] Failed to write to driver!\n");
			return errno;
		}
		return 0;
	}

}

int set_led_duty(int duty)
{
	int retval;
	if (duty == 100)
	{
		printf("[led_client] Duty cycle is 1, leaving LED on\n");
		retval = write(file_id, LED_ON_STR, strlen(LED_ON_STR));
		if (retval < 0 )
		{
			printf("[led_client]Failed to write to driver!\n");
			return errno;
		}
		return 0;	
	}
	else if (duty == 0)
	{
		printf("[led_client] Duty cycle is 0, turning LED off\n");
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_client] Failed to write to driver!\n");
			return errno;
		}
		return 0;	
		
	}
	else if ((duty > 100) || (duty < 1))
	{
		/* TODO do duty cycle correctly*/
		retval = write(file_id, LED_ON_STR, strlen(LED_ON_STR));
		if (retval < 0 )
		{
			printf("[led_client] Failed to write to driver!\n");
			return errno;
		}
	}
	else
	{
		printf("[led_client] Invalid LED duty cycle\n");
		return 1;
	}
	return 0;
}

