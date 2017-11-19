
#include "server.h"

static char in_message[INPUT_LEN];
int server_state = STATE_RUNNING;

int main(void)
{
	int socket_id, client_id, file_id, retval, client_sockaddr_len;
	struct sockaddr_in led_server;

	file_id = open(DRIVER_PATH, O_RDWR);
	if( file_id < 0)
	{
		printf("[led_server] Failed to open device at %s\n", DRIVER_PATH);
		return errno;
	}

	/* establish socket */
	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id == -1)
	{
		printf("[led_server] could not create socket\n");
	}

	led_server.sin_addr.s_addr = inet_addr(LED_SERVER_HOST);
	led_server.sin_family = AF_INET;
	led_server.sin_port = htons(LED_SERVER_PORT);
	

	if (bind(socket_id, (struct sockaddr *)&led_server, sizeof(led_server)) < 0)
	{
		printf("[led_server] Failed to bind to port %d\n", LED_SERVER_PORT);
		return 1;
	}
	listen(socket_id, MAX_NUM_CONNS);
	printf("[led_server] Listening on %s:%d\n", LED_SERVER_HOST, LED_SERVER_PORT);

	client_sockaddr_len = sizeof(struct sockaddr_in);

	client_id = accept(socket_id, (struct sockaddr *)&client_id, (socklen_t *)&client_sockaddr_len );
	if (client_id < 0)
	{
		printf("[led_server] Failed to accept connection\n");
		return 1;
	}
	printf("[led_server] Connected!\n");

	/* Now enter receiving loop from client */
	while (server_state == STATE_RUNNING)
	{
		retval = recv(client_id, in_message, INPUT_LEN, 0);
		if(retval < 0)
		{
			printf("[led_server] Failed to receive from remote server, got value %d and errno %d \n", retval, errno);
			server_state = STATE_ERROR;
		}
		else if (retval == 0 )
		{
			printf("[led_server] Connection closed by client\n");
			server_state = STATE_STOPPED;
		}
		else 
		{
			printf("[led_server] Received <%s>\n", in_message);
		}
	}
	
	printf("[led_server] Terminating server\n");
	close(socket_id);
	return server_state;
}



int read_driver_var(int file_id, int var_id)
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

int read_all_driver_vars(int file_id)
{
	/* TODO read all vars one-at-a-time */
	return 0;
}

int set_led_state(int file_id, int state)
{
	int retval;

	if (state == 1)
	{
		retval = write(file_id, LED_ON_STR, strlen(LED_ON_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
	}
	else if (state == 0)
	{
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
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


int set_led_freq(int file_id, int freq)
{
	int retval;

	if (freq == 0)
	{
		printf("[led_server] Freq is 0, turning LED off\n");
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
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
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
		if (retval < 0 )
		{
			printf("[led_server] Failed to write to driver!\n");
			return errno;
		}
		return 0;
	}

}

int set_led_duty(int file_id, int duty)
{
	int retval;
	if (duty == 100)
	{
		printf("[led_server] Duty cycle is 1, leaving LED on\n");
		retval = write(file_id, LED_ON_STR, strlen(LED_ON_STR));
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
		retval = write(file_id, LED_OFF_STR, strlen(LED_OFF_STR));
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
		retval = write(file_id, LED_ON_STR, strlen(LED_ON_STR));
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

