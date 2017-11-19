
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/* general vars*/
#define BUFFER_LEN 256
#define DRIVER_PATH "/dev/myledchar"

/* commands */
#define LED_ON_STR  "on"
#define LED_OFF_STR "off"

#define VAR_1 1
#define VAR_2 2
#define VAR_3 3


/*prototypes*/
int main(void);

int read_driver_var(int fileptr, int var_id);

int read_all_driver_vars(int fileptr);

int set_led_state(int fileptr, int state);

int set_led_freq(int fileptr, int freq);

int set_led_duty(int fileptr, int duty);

