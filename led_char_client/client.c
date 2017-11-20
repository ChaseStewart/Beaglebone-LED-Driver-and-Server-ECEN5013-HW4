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

	printf("[led_client] Connected!\n");

	while (client_state == STATE_RUNNING)
	{
		/* get line of input from user */
		printf("[led_client]What would you like to send?\n");
		printf("\t\tType \'state\' to set LED state\n");		
		printf("\t\tType \'read\'  to read LED driver vars\n");		
		printf("\t\tType \'freq\'  to set LED flashing freq\n");
		printf("\t\tType \'duty\'  to set LED duty cycle\n");		
		printf("\t\tType \'exit\'  to close the program\n");		
		scanf("%s", out_message);

		/* parse user input and format message*/
		if (strcmp(out_message, "state") == 0)
		{
			int correct_input = 0;
			while(correct_input == 0)
			{
				printf("\t\tSet LED \'on\' or \'off\'?\n");
				scanf("%s", out_message);
				if (strcmp(out_message, "on") == 0)
				{
					sprintf(out_message, "state:on");
					correct_input = 1;
				}
				else if (strcmp(out_message, "off") == 0)
				{
					sprintf(out_message, "state:off");
					correct_input = 1;
				}
				else
				{
					printf("\t\tInvalid selection!\n");
					continue;
				}
			}
		}
		else if (strcmp(out_message, "read") == 0)
		{
			int correct_input = 0;
			while(correct_input == 0)
			{
				printf("\t\tType \'all\' to get all vars \n");
				printf("\t\tType \'state\' to get LED state \n");
				printf("\t\tType \'freq\'  to get LED freq \n");
				printf("\t\tType \'duty\'  to get LED duty \n");
				printf("\t\tType \'exit\'  to return \n");
				scanf("%s", out_message);
				if (strcmp(out_message, "all") == 0)
				{
					sprintf(out_message, "read:%d", VAR_ALL);
					correct_input = 1;
				}
				else if (strcmp(out_message, "state") == 0)
				{
					sprintf(out_message, "read:%d", VAR_STATE);
					correct_input = 1;
				}
				else if (strcmp(out_message, "freq") == 0)
				{
					sprintf(out_message, "read:%d", VAR_FREQ);
					correct_input = 1;
				}
				else if (strcmp(out_message, "duty") == 0)
				{
					sprintf(out_message, "read:%d", VAR_DUTY);
					correct_input = 1;
				}
				else if (strcmp(out_message, "exit") == 0)
				{
					continue;
				}
				else
				{
					printf("\t\tInvalid selection!\n");
					continue;
				}
			}
		
		}
		else if (strcmp(out_message, "freq") == 0)
		{
			int freq_val = 0;
			while(freq_val == 0)
			{
				printf("\t\tEnter freq between 1-1000Hz\n");
				scanf("%s", out_message);
				if (sscanf(out_message, "%d", &freq_val) >= 0)
				{
					/*set freq val*/
					if (freq_val <= 0 || freq_val >= 1000)
					{
						printf("\t\tInvalid selection!\n");
						freq_val = 0;
					}
					else
					{
						sprintf(out_message, "freq:%d", freq_val);
					}
				}
				else
				{
					printf("\t\tInvalid selection!\n");
					freq_val = 0;
				}
			}
		}
		else if (strcmp(out_message, "duty") == 0)
		{
			int duty_val = 0;
			while(duty_val == 0)
			{
				printf("\t\tEnter duty cycle between 1-100\n");
				scanf("%s", out_message);
				if (sscanf(out_message, "%d", &duty_val) >= 0)
				{
					/*set duty val*/
					if (duty_val =< 0 || duty_val >= 100)
					{
						printf("\t\tInvalid selection!\n");
						duty_val = 0;
					}
					else
					{
						sprintf(out_message, "duty:%d", duty_val);
					}
				}
				else
				{
					printf("\t\tInvalid selection!\n");
					duty_val = 0;
				}
			}
		}
		else if (strcmp(out_message, "exit") == 0)
		{
			client_state = STATE_STOPPED;
			break;
		}
		else
		{
			printf("[led_client] Invalid input %s!\n", out_message);
			continue;
		}
		printf("[led_client] Sending \"%s\"\n", out_message);

		/* send message and listen for ACK/NACK */
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
		printf("[led_client] Received <%s>\n", in_message);
	}
	
	if (client_state == STATE_ERROR)
	{
		printf("[led_client] EXIT WITH ERROR\n");
	}

	printf("[led_client] Terminating client\n");
	close(socket_id);
	return client_state;
}

