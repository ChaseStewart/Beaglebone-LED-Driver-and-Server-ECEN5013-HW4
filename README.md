# Beaglebone LED Driver and Server ECEN5013-HW4
Repository for HW4 LED char driver

## Contents
This repository holds a Beaglebone Black LED char driver for USR0 that can set/clear a static LED or freq/duty_cycle flashing on the LED, and can set and return variables.

## Usage
1) First, run the `sudo sh -c "..."` commands for usr0 usr1 usr2 usr3 to ensure all LEDs are controlled by the GPIO only, else you will get EBUSY errno when trying to insert the led_driver module.
2) To run the project, compile server, client, and compile and insert led_driver.ko
3) Run `sudo ./server`, then run `./client`
4) Follow the prompts in ./client to set and query values and when you are done, type `exit`.
