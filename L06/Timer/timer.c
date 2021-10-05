#include "sam.h"
 
#define PINCFG_CONFIG_VALUE 0b00000000
#define GENCTRL_CONFIG_VALUE 0x10600
#define CLKCTRL_CONFIG_VALUE 0x401B
#define TIMER_INIT_COUNT 65307
 
int main(void) {

   	/* Initialize the SAM system */
	SystemInit();
 
    //PORT configuration for general-purpose PIN
   	PORT->Group[0].PINCFG[14].reg = PINCFG_CONFIG_VALUE;
   	PORT->Group[0].DIRSET.reg = PORT_PA14;
   	//TIMER configuration
   	//Configure the POWER MANAGER to enable the TC3 module
        	PM->APBCMASK.reg |= PM_APBCMASK_TC3;
   	//Configure the GENERIC CLOCK CONTROL used by the TC3 module
			GCLK->GENDIV.reg = GCLK_GENDIV_RESETVALUE;
			GCLK->GENCTRL.reg = GENCTRL_CONFIG_VALUE;
			GCLK->CLKCTRL.reg = CLKCTRL_CONFIG_VALUE;
	//Determine the initial count
    //Time = Toscillator * (2^TIMERMODE - COUNT) * Prescaler
			TC3->COUNT16->COUNT.reg = TIMER_INIT_COUNT;
	//Configure the TC3 peripheral
			TC3->COUNT16->CTRLA.reg = TC_CTRLA_RESETVALUE;
			TC3->COUNT8->CTRLBSET.reg = TC_CTRLBSET_RESETVALUE;
			TC3->COUNT8->INTFLAG.reg = TC_INTENCLR_OVF;
	//Enabling the TC3 peripheral
			TC3->COUNT16->CTRLA.reg |= TC_CTRLA_ENABLE;
	//Loop to sync the TC clock with the processor/main clock
			while(TC3->COUNT8.STATUS.reg == TC_STATUS_SYNCBUSY);
	//***************************************************************************
			//Loop to generate the square-wave signal
			//EXTRA cycles to ADD to the initial count---it will be still a 
			//polling-error of +/- 7 us in the period of the signal
			//LDRB(4) + ANDS(1) + BEQ(2) + LDRB(4) + ANDS(1) + BEQ(1) + STR(4) + 
			//STRB(4) + STRH(4) + B(2) = 27 cycles
		while(1){
			if(TC3->COUNT8->INTFLAG.reg != TC_INTENCLR_OVF){
				PORT->Group[0].OUTTGL.reg = PORT_PA14;
				TC3->COUNT8->INTFLAG.reg = TC_INTENCLR_OVF;
				TC3->COUNT16->COUNT.reg = TIMER_INIT_COUNT;
			}
		}
}
