/*
 * main.c
 *
 * Created: 01/11/2017 08:47:35 p. m.
 *  Author: L00254193
 */ 

#include "sam.h"
#include "myprintf.h"
#include "RTCControl.h"

void UARTInit(void);
second_type second; minutes_type minute; hour_type hour;
day_type day; date_type date; month_type month;
year_type year;

uint8_t tx_buff[7];
uint8_t rx_buff[7];

int main(void)
{
	
	UARTInit();
	RTCInit();
	
	char buff[31]; buff[30]= '\0';
	myprintf("\nHello World!",buff);
	
	second.reg = 0;
	minute.field.dec_minutes = 4; minute.field.un_minutes = 6;
	hour.t24.field.en_t24 = HOURS_CONFIG24;
	hour.t24.field.dec_hours = 0; hour.t24.field.un_hours = 4;
	day.field.day = 3;
	date.field.dec_date = 0; date.field.un_date = 5;
	month.field.dec_month = 1; month.field.un_month = 0;
	year.field.dec_year = 2; year.field.un_year = 1; 
	tx_buff[0] = second.reg;
	tx_buff[1] = minute.reg;
	tx_buff[2] = hour.t24.reg;
	tx_buff[3] = day.reg;
	tx_buff[4] = date.reg;
	tx_buff[5] = month.reg;
	tx_buff[6] = year.reg;
	sendI2CDataArray(RTC_ADDR, SECONDS_ADDR, tx_buff, 7);
	receiveI2CDataArray(RTC_ADDR, SECONDS_ADDR, rx_buff, 7);
	second.reg = rx_buff[0]; 
	minute.reg = rx_buff[1]; 
	hour.t24.reg = rx_buff[2]; 
	date.reg = rx_buff[4]; 
	month.reg = rx_buff[5]; 
	year.reg = rx_buff[6]; 
	myprintf("\nDate/Month/Year/Hours/Seconds/Minutes");
	myprintf("\n%d%d/%d%d/%d%d/%d%d/%d%d/%d%d",date.field.dec_date, date.field.un_date, month.field.dec_month, 
	month.field.un_month, year.field.dec_year, year.field.un_year, hour.t24.field.dec_hours, 
	hour.t24.field.un_hours, second.field.dec_seconds, second.field.un_seconds, 
	minute.field.dec_minutes, minute.field.un_minutes);
	
	StopCond();
	
	/*
	mysnprintf(buff, sizeof buff, "testing %d %d %07d", 1, 2, 3);
	myprintf("\nBuff=%s",buff);
	mysnprintf(buff, sizeof buff, "faster %s %ccheaper%c", "and", 34, 34);
	myprintf("\nBuff=%s",buff);
	mysnprintf(buff, sizeof buff, "%x %% %X", 0xdeadf00d, 0xdeadf00d);
	myprintf("\nBuff=%s",buff);
	mysnprintf(buff, sizeof buff, "%09d%09d%09d%09d%09d", 1, 2, 3, 4, 5);
	myprintf("\nBuff=%s",buff);
	mysnprintf(buff, sizeof buff, "%d %u %d %u", 50, 50, -50, -50);
	myprintf("\nBuff=%s",buff);
	*/
	return 0;
}

void UARTInit(void) {
	    	/* Initialize the SAM system */
    SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	
	/* port mux configuration*/
	PORT->Group[0].DIR.reg |= (1 << 10);                  /* Pin 10 configured as output */
	PORT->Group[0].PINCFG[PIN_PA11].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	PORT->Group[0].PINCFG[PIN_PA10].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	
	/*PMUX: even = n/2, odd: (n-1)/2 */
	PORT->Group[0].PMUX[5].reg |= 0x02;                   /* Selecting peripheral function C */
	PORT->Group[0].PMUX[5].reg |= 0x20;                   /* Selecting peripheral function C */
	
	/* APBCMASK */
	//PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;			  /* SERCOM 0 enable*/
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

	/*GCLK configuration for sercom0 module: using generic clock generator 0, ID for sercom0, enable GCLK*/

	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) |
	GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

	
	/* configure SERCOM0 module for UART as Standard Frame, 8 Bit size, No parity, BAUDRATE:9600*/

	SERCOM0->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
	SERCOM_USART_CTRLA_RXPO(3/*PAD3*/) | SERCOM_USART_CTRLA_TXPO(1/*PAD2*/);
	
	uint64_t br = (uint64_t)65536 * (8000000 - 16 * 9600) / 8000000;
	
	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);

	SERCOM0->USART.BAUD.reg = (uint16_t)br;

	SERCOM0->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}