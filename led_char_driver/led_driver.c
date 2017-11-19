/*****************************************************
 * Redistribution, modification or use of this software in source or binary forms 
 * is permitted as long as the files maintain this copyright. Users are permitted to 
 * modify this and use it to learn about the field of embedded software but don't copy 
 * my (Chase E Stewart's) work for class, I worked really hard on this. Alex Fosdick and 
 * the University of Colorado and Chase E Stewart are not liable for any misuse of this material. 
 * License copyright (C) 2017 originally from Alex Fosdick, code by Chase E Stewart.
 *****************************************************/

/*
 * @file led_driver.c
 * @brief A driver to control Beaglebone LEDs in userspace 
 * 
 * A driver with an init, an exit, and an IRQ handler
 * 
 *
 * @author Chase E Stewart
 * @date November 11 2017
 * @version 1.0
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/fs.h>

/* device vars*/
#define DEVICE_NAME "myledchar"
#define CLASS_NAME  "myled"

/* GPIO vars */
#define LED_GPIO 56
#define LED_ON   1
#define LED_OFF  0 

/* module info */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chase E Stewart");
MODULE_DESCRIPTION("A dead simple char driver for BBB LEDs");
MODULE_VERSION("0.1");

/* prototypes */
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static int dev_read(struct file *, char *, size_t, loff_t *);
static int dev_write(struct file *, const char *, size_t, loff_t *);

/* file ops -> file pointers structs */
static struct file_operations fops = 
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release

};

/* state vars */
static bool led_state      = 0;
static int  driver_number  = 0;
static char   message[256] = {0};           
static int    numberOpens  = 0;              
static struct class*  ledDriverClass  = NULL; 
static struct device* ledDriverDevice = NULL; 



static int __init init_led_driver(void){
	
	/* register the gpio */
	if (!gpio_is_valid(LED_GPIO))
	{
		printk(KERN_ERR "[led_driver] Invalid GPIO pin provided\n");
		return 1;
	}
	gpio_request(LED_GPIO, "sysfs");
	led_state = LED_ON;
	gpio_direction_output(LED_GPIO, LED_ON);
	gpio_export(LED_GPIO, false);
	printk(KERN_INFO "[led_driver] LED GPIO pins Initialized\n");

	/* grab a major number for the device driver */
	printk(KERN_INFO "[led_driver] Initiating LED Driver\n");
	driver_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (driver_number < 0)
	{
		printk(KERN_ERR "[led_driver] could not register the driver!\n");
		return driver_number;
	}
	printk(KERN_INFO "[led_driver] registered the driver with driver number %d\n", driver_number);

	/* attempt to register the device with the driver number */
	ledDriverClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(ledDriverClass))
	{
		unregister_chrdev(driver_number, DEVICE_NAME);
		printk(KERN_ERR "Failed to register device class\n");
		return PTR_ERR(ledDriverClass);
	}
	printk(KERN_INFO "Successfully registered device class");

	/* register device driver */
	ledDriverDevice = device_create(ledDriverClass, NULL, MKDEV(driver_number, 0), NULL, DEVICE_NAME);
	if (IS_ERR(ledDriverDevice))
	{
		class_destroy(ledDriverClass);
		unregister_chrdev(driver_number, DEVICE_NAME);
		printk(KERN_ERR "Failed to create device!\n");
		return PTR_ERR(ledDriverDevice);
	}
	return 0;
}

static int dev_open(struct inode *inodep, struct file *filep)
{
	numberOpens++;
	printk(KERN_INFO "[led_driver] LED Driver has been accessed %d times\n", numberOpens );
	return 0;
}

static int dev_read(struct file *myfile, char *mybuffer, size_t len, loff_t *myoffset)
{
	switch(led_state)
	{
		case LED_OFF:
			printk(KERN_INFO "[led_driver] LED State is off\n");
			break;
		case LED_ON:
			printk(KERN_INFO "[led_driver] LED State is on\n");
			break;
		default:
			printk(KERN_ALERT "[led_driver] What happened?\n");
			break;
	}
	return 0;
}

static int dev_write(struct file *myfile, const char *mybuffer, size_t len, loff_t *myoffset)
{
	sprintf(message, "%s(%zu letters)", mybuffer, len);
	if (strcmp(message, "on"))
	{
		led_state = LED_ON;
		gpio_set_value(LED_GPIO, LED_ON);
	}
	else if (strcmp(message, "On"))
	{
		led_state = LED_ON;
		gpio_set_value(LED_GPIO, LED_ON);
	}
	else if (strcmp(message, "ON"))
	{
		led_state = LED_ON;
		gpio_set_value(LED_GPIO, LED_ON);
	}
	else if (strcmp(message, "1"))
	{
		led_state = LED_ON;
		gpio_set_value(LED_GPIO, LED_ON);
	}
	else if (strcmp(message, "off"))
	{
		led_state = LED_OFF;
		gpio_set_value(LED_GPIO, LED_OFF);
	}
	else if (strcmp(message, "Off"))
	{
		led_state = LED_OFF;
		gpio_set_value(LED_GPIO, LED_OFF);
	}
	else if (strcmp(message, "OFF"))
	{
		led_state = LED_OFF;
		gpio_set_value(LED_GPIO, LED_OFF);
	}
	else if (strcmp(message, "0"))
	{
		led_state = LED_OFF;
		gpio_set_value(LED_GPIO, LED_OFF);
	}
	else
	{
		printk(KERN_ALERT "[led_driver] Driver received invalid response\n");
	}

	printk(KERN_INFO "[led_driver] LED Driver received message \'%s\' \n", message);
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "[led_driver] Closed the LED driver device\n");
	return 0;
}

static int __init exit_led_driver(void){
	printk(KERN_INFO "[led_driver] Closing  LED Driver\n");

	/* First, free GPIO resources */
	gpio_set_value(LED_GPIO, LED_OFF);
	gpio_unexport(LED_GPIO);
	gpio_free(LED_GPIO);
	printk(KERN_INFO "[led_driver] GPIO links terminated\n");

	/* Now destroy driver */
	device_destroy(ledDriverClass, MKDEV(driver_number, 0));
	class_unregister(ledDriverClass);
	class_destroy(ledDriverClass);
	unregister_chrdev(driver_number, DEVICE_NAME);
	printk(KERN_INFO "[led_driver] LED Driver exit... Goodbye!\n");
	return 0;
}


/* use macros to setup init and teardown functions */
module_init(init_led_driver);
module_exit(exit_led_driver);


