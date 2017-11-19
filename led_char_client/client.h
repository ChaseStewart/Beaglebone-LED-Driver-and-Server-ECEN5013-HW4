
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* general vars*/
#define INPUT_LEN 512

#define LED_SERVER_HOST "127.0.0.1"
#define LED_SERVER_PORT 50010

#define STATE_RUNNING 1
#define STATE_STOPPED 0
#define STATE_ERROR   -1

#define VAR_1 1
#define VAR_2 2
#define VAR_3 3

/* commands */
#define LED_ON_STR  "on"
#define LED_OFF_STR "off"

// TODO FIXME LINE BELOW IS DEFINITELY WRONG
int file_id;

/*prototypes*/
int main(void);

int req_driver_var(int var_id);

int req_all_driver_vars(void);

int set_led_state(int state);

int set_led_freq(int freq);

int set_led_duty(int duty);

