/*******************************************************************************
*
*   i2c.c
*
*   Copyright (c) 2013 Shahrooz Shahparnia (sshahrooz@gmail.com)
*
*   Description:
*   i2c is a command-line utility for executing i2c commands with the 
*   Broadcom bcm2835.  It was developed and tested on a Raspberry Pi single-board
*   computer model B.  The utility is based on the bcm2835 C library developed
*   by Mike McCauley of Open System Consultants, http://www.open.com.au/mikem/bcm2835/.
*
*   Invoking spincl results in a read or write I2C transfer.  Options include the
*   the I2C clock frequency, read/write, address, and port initialization/closing
*   procedures.  The command usage and command-line parameters are described below
*   in the showusage function, which prints the usage if no command-line parameters
*   are included or if there are any command-line parameter errors.  Invoking i2c 
*   requires root privilege.
*
*   This file contains the main function as well as functions for displaying
*   usage and for parsing the command line.
*
*   Open Source Licensing GNU GPLv3
*
*   Building:
* After installing bcm2835, you can build this 
* with something like:
* gcc -o i2c i2c.c -l bcm2835
* sudo ./i2c
*
* Or you can test it before installing with:
* gcc -o i2c -I ../../src ../../src/bcm2835.c i2c.c
* sudo ./i2c
*
*   History:
*   11/05    VERSION 1.0.0: Original
*
*      User input parsing (comparse) and showusage\
*      have been adapted from: http://ipsolutionscorp.com/raspberry-pi-spi-utility/
*      mostly to keep consistence with the spincl tool usage.
*
*      Compile with: gcc -o i2c i2c.c bcm2835.c
*
*      Examples:
*
*           Set up ADC (Arduino: ADC1015)
*           sudo ./i2c -s72 -dw -ib 3 0x01 0x44 0x00 (select config register, setup mux, etc.)
*           sudo ./i2c -s72 -dw -ib 1 0x00 (select ADC data register)
*
*           Bias DAC (Arduino: MCP4725) at some voltage
*           sudo ./i2c -s99 -dw -ib 3 0x60 0x7F 0xF0 (FS output is with 0xFF 0xF0)
*           Read ADC convergence result
*           sudo ./i2c -s72 -dr -ib 2 (FS output is 0x7FF0 with PGA1 = 1)
*  
*      In a DAC to ADC loop back typical results are:
*
*      DAC    VOUT   ADC
*      7FFh   1.6V   677h                    Note ratio is FS_ADC*PGA_GAIN/FS_DAC = 4.096/3.3 = 1.23
*      5FFh   1.2V   4DCh
*      8F0h   1.8V   745h
*      9D0h   2V     7EAh
*      000h   10mV   004h
*
********************************************************************************/

#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MODE_READ 0
#define MODE_WRITE 1

#define MAX_LEN 32

#define RTC_ADDR 104
#define TC74_ADDR 77

#define CLK_DIV 2500

char wbuf[MAX_LEN];

// 01/01/01 Mon 12:00:00 AM	
uint8_t rtc_config[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01};
// DATA: SEC, MIN, HOUR, WEEKDAY, DAY, MONTH, YEAR
uint8_t rtc_buff[7];

uint8_t tc74_buff[1];

typedef enum {
    NO_ACTION,
    I2C_BEGIN,
    I2C_END
} i2c_init;

uint8_t  init = NO_ACTION;
uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148;
uint8_t slave_address = 0x00;
uint32_t len = 0;
uint8_t  mode = MODE_READ;

char buf[MAX_LEN];
int i;
uint8_t data;
int n_records;
char file_buff[3][42];
char file_buff_t[3][28];

int RTC_init();
int RTC_get();
int TC74_get();
void delay_ms();
char* weekday(int);
void logFile();

int main(int argc, char **argv) {

  printf("Running ... \n");

  RTC_init();
  while(1){
    RTC_get();
    TC74_get();
    sprintf(file_buff_t[n_records%3], "RECEIVER> Temperature: %d°C\n", tc74_buff[0]);
    printf("RECEIVER> Temperature: %d°C\n", tc74_buff[0]);
    sprintf(file_buff[n_records%3], "RECEIVER> Record %d: %02X/%02X/%02X %s %02X:%02X:%02X\n", n_records, rtc_buff[4], rtc_buff[5], rtc_buff[6], weekday(rtc_buff[3]), rtc_buff[2], rtc_buff[1], rtc_buff[0]);
    logFile();
    n_records++;
    delay_ms();
  }
    return 0;
}

int RTC_init(){

  if (!bcm2835_init())
  {
    printf("bcm2835_init failed. Are you running as root??\n");
    return 1;
  }

  if (!bcm2835_i2c_begin())
  {
    printf("bcm2835_i2c_begin failed. Are you running as root??\n");
    return 1;
  }

  bcm2835_i2c_setSlaveAddress(RTC_ADDR);
  bcm2835_i2c_setClockDivider(CLK_DIV);

  data = bcm2835_i2c_write(rtc_config, 8);

  bcm2835_i2c_end();
  bcm2835_close();

  return data;

}

int RTC_get(){
  if (!bcm2835_init())
  {
    printf("bcm2835_init failed. Are you running as root??\n");
    return 1;
  }

  if (!bcm2835_i2c_begin())
  {
    printf("bcm2835_i2c_begin failed. Are you running as root??\n");
    return 1;
  }

  bcm2835_i2c_setSlaveAddress(RTC_ADDR);
  bcm2835_i2c_setClockDivider(CLK_DIV);

  data = bcm2835_i2c_write(&rtc_config[0], 1);

  data = bcm2835_i2c_read(rtc_buff, 7);

  bcm2835_i2c_end();
  bcm2835_close();
  printf("RECEIVER> Record %d: %02X/%02X/%02X %s %02X:%02X:%02X\n", n_records, rtc_buff[4], rtc_buff[5], rtc_buff[6], weekday(rtc_buff[3]), rtc_buff[2], rtc_buff[1], rtc_buff[0]);
  return data;
}

int TC74_get(){
  if (!bcm2835_init())
  {
    printf("bcm2835_init failed. Are you running as root??\n");
    return 1;
  }

  if (!bcm2835_i2c_begin())
  {
    printf("bcm2835_i2c_begin failed. Are you running as root??\n");
    return 1;
  }

  bcm2835_i2c_setSlaveAddress(TC74_ADDR);
  bcm2835_i2c_setClockDivider(CLK_DIV);

  data = bcm2835_i2c_read(tc74_buff, 1);

  bcm2835_i2c_end();
  bcm2835_close();

  return tc74_buff[0];
}

void delay_ms()
{
    int milli_seconds = 1000 * 10000;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds){
      if(TC74_get()>30)
        break;
    }
}

char* weekday(int day){
  switch (day)
  {
  case 0:
    return "Mon";
  case 1:
    return "Tue";
  case 2:
    return "Wed";
  case 3:
    return "Thu";
  case 4:
    return "Fri";
  case 5:
    return "Sat";
  case 6:
    return "Sun";
  default:
    return "ERR";
  }
}

char date_log[20], temp_log[28];

void logFile(){
  FILE *fp;
  fp = fopen("output.txt","w");
  if(fp){
    for(int i=0; i<3 && i<= n_records; i++){
      snprintf(date_log, 42, "%s", &file_buff[i]);
      snprintf(temp_log, 28, "%s", &file_buff_t[i]);
      fprintf(fp, "%s\n", temp_log);
      fprintf(fp, "%s\n", date_log);
    }
    fclose(fp);
  }
}

