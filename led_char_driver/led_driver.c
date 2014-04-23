/*****************************************************
 * Redistribution, modification or use of this software in source or binary forms 
 * is permitted as long as the files maintain this copyright. Users are permitted to 
 * modify this and use it to learn about the field of embedded software but don't copy 
 * my (Chase E Stewart's) work for class, I worked really hard on this. Alex Fosdick and 
 * the University of Colorado and Chase E Stewart are not liable for any misuse of this material. 
 * License copyright (C) 2017 originally from Alex Fosdick, code by Chase E Stewart.
 *****************************************************/
/**
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
#define DEVICE_NAME "led_driver"
#define CLASS_NAME  "ebb"

/* GPIO vars */
#define LED_GPIO 56
#define LED_ON   1
#define LED_OFF  0 


/* module info */
MODULE_LICENSE("GPL")
MODULE_AUTHOR("Chase E Stewart")
MODULE_DESCRIPTION("A dead simple char driver for BBB LEDs")
MODULE_VERSION("0.1") 


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
static bool led_state = 0;
static int  driver_number;
static char   message[256] = {0};           
static short  size_of_message;              
static int    numberOpens = 0;              
static struct class*  ledDriverClass  = NULL; ///< The device-driver class struct pointer
static struct device* ledDriverDevice = NULL; ///< The device-driver device struct pointer



static int __init init_led_driver(void){
	int retval;
	printk(KERN_INFO "[led_driver] Initiating LED Driver\n");
	driver_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (driver_number < 0)
	{
		printk(KERN_ERROR "[led_driver] could not register the driver!\n");
		return driver_number;
	}
	printk(KERN_INFO "[led_driver] registered the driver\n");
	
	
	if (!gpio_is_valid(LED_GPIO))
	{
		printk(KERN_ERROR "[led_driver] Invalid GPIO pin provided\n");
		return 1;
	}
	gpio_request(LED_GPIO, "sysfs");
	led_state = LED_ON;
	gpio_direction_output(LED_GPIO, LED_ON);
	gpio_export(LED_GPIO, false);
	
	printk(KERN_INFO "[led_driver] LED GPIO pins Initialized\n");
	return retval;

}

static int __init exit_led_driver(void){
	printk(KERN_INFO "[led_driver] Closing  LED Driver\n");
	gpio_set_value(LED_GPIO, LED_OFF);
	gpio_unexport(LED_GPIO);
	gpio_free(LED_GPIO);
	printk(KERN_INFO "[led_driver] LED Driver terminated!\n");


}

static int dev_open(struct inode *inodep, struct file *filep)
{
	count_access++;
	printk(KERN_INFO "[led_driver] LED Driver has been accessed %d times\n", count_access );
	return 0;
}

static int dev_read(struct file *, char *, size_t, loff_t *)
{

}

static int dev_write(struct file *, const char *, size_t, loff_t *)
{

}

static int dev_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "Closed the LED driver device\n");
	return 0;
}


/* use macros to setup init and teardown functions */
module_init(init_led_driver);
module_exit(exit_led_driver);


