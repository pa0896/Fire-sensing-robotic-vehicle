//use PORTB for PWM, use PB6 and PB7 for PWM, PB5 for the flame sensor, PB4 for the event capture of HCSR04
/*
 * for PB7 pin, the PWM signal is M0PWM1. Do not use PWM1 generator, instead use the PWM0 generator
 * the comparator and the generator differentiates the two
 * use CMPB for PB7 and CMPA for PB6
 */
//PA2 is the trigger pin and PB4 is the echo pin
// use PA5 and PA6 as the enable pins of L298N motor driver
//TIMER B is 16 bit and TIMER A is 32 bit -- REVERSE

//use better variable names

#include <stdint.h>
#include <stdbool.h>
#include <pindef.h>

void portA_init(void);
void portB_init(void);
void timer0A_init(void);
void timer0B_init(void);
void Systick_Init(void);
void pwm_init(void);
uint32_t measure_distance(uint32_t time);
uint32_t signal(void);


void portA_init(void){
	SYSCTL_RCGCGPIO_R	|=	 0x00000001;		// Port A clock
	while((SYSCTL_PRGPIO_R&0x00000001)	==	0){
		//wait for Port A clock to be stabilised
	};
	GPIO_PORTA_DIR_R	|=	 0x00000064;		// PA2,PA5,PA6 output
	GPIO_PORTA_DIR_R	&=	~0x00000008;		// PA3 input
	GPIO_PORTA_DIR_R	|=	 0x00000010;		// PA4 output------ remove
	GPIO_PORTA_DEN_R	|=	 0x0000006C;		// make PA2, PA3, PA5, PA6 digital
	GPIO_PORTA_DEN_R	|=	 0x00000010;		// PA4 digital----- remove
	GPIO_PORTA_AMSEL_R	&=	~0x0000006C;		// disable analog function on all the 4 pins
	GPIO_PORTA_AFSEL_R	&=	~0x0000006C;		// disable alternate function on PA2, PA3, PA5, PA6 because they are general purpose
	GPIO_PORTA_PCTL_R	 =	 0x00000000;		// no special function required
}


void portB_init(){
	SYSCTL_RCGCGPIO_R	|=	 0x00000002;		// Port B clock
	while((SYSCTL_PRGPIO_R&0x00000002)	==	0){
		//wait for Port B clock to be stabilised
	};
	GPIO_PORTB_DIR_R	|=	 0x000000C0;		// PB6 and PB7 output
	GPIO_PORTB_DIR_R	&=	~0x00000020;		// PB5 input
	GPIO_PORTB_DEN_R	|=	 0x000000E0;		// make PB5, PB6 and PB7 digital pins
	GPIO_PORTB_AMSEL_R	&=	~0x000000E0;	    // disable analog function on all the pins
	GPIO_PORTB_AFSEL_R	|=	 0x000000C0;		// enable alternate function on the pins PB6 and PB7
	GPIO_PORTB_AFSEL_R	&=	~0x00000020;		// disable alternate function on PB5
	GPIO_PORTB_PCTL_R	 =   0x44070000;		// insert value 4 to get PWM functionality on the pins PB6 and PB7 and value 7 to get timer capture functionality for PB4
}


void pwm_init(void){
	SYSCTL_RCGCPWM_R    |=	 0x00000001;		// enable PWM Module 0
	while((SYSCTL_PRPWM_R&0x00000001)  ==  0){
		//wait for the PWM clock to be stabilized
	};
	/*
	 * set bit 20 of the register to enable the frequency divider as well as choose /64 configuration
	 * to get the clock down to 250 KHz. The XTAL frequency of TM4C is 16 MHz
	*/
	SYSCTL_RCC_R        |=	 0x00100000;		// use PWM clock divider for the frequency
	SYSCTL_RCC_R		|=	 0x000E0000;		// use /64 value for dividing the frequency
	PWM0_CTL_R			&=	~0x00000001;		// disable entire PWM0 block
	PWM0_CTL_R			&=	~0x00000002;		// count down mode to be used with PWM0
	PWM0_LOAD_R		     =		25000;			// (1/10)/(1/250000)  [required frequency/TM4C actual frequency when 64 is divided by 16]
	PWM0_CMPA_R		 	 =		12500;			// duty cycle
	PWM0_CMPB_R		 	 =		12500;			// duty cycle
	PWM0_GENA_R			|=	 0x0000008C;		// put PWM signal to low on matching comparator value and drive PWM high when counter = load
	PWM0_GENB_R			|=	 0x0000080C;		// put PWM signal to low on matching comparator value and drive PWM high when counter = load
	PWM0_CTL_R	  		|=	 0x00000001;		// start PWM0 generator
	PWM0_ENABLE_R		|=	 0x00000003;		// enable M0PWM0 and M0PWM1
}


void timer0A_init(void){
	//initialization of timer
	SYSCTL_RCGCTIMER	|=	 0x00000001;		// use timer 0
	SYSCTL_GPTM_CTL		&=	~0x00000001;		// disable timer A before initialization
	SYSCTL_GPTM_CFG		 = 	 0x00000000;	    // put value of 0x0 so that the timer uses 32-bit config
	SYSCTL_GPTMTAMR		|=	 0x00000003;		// use capture mode
	SYSCTL_GPTMTAMR		|=	 0x00000004;		// update TACMR use edge time mode
	SYSCTL_GPTMTAMR		&=	~0x00000008;		// update TAAMS to use capture mode
	SYSCTL_GPTMTAMR		|=	 0x00000010;		// update TACDIR to use count up mode
	SYSCTL_GPTMTAMR		&=	~0x00000040;		// update TAWOT to start the timer as soon as it is enabled
	SYSCTL_GPTMTAILR	 = 	 608000;			// finish value of the counter (0.038*16000000)
	SYSCTL_GPTM_CTL		|=	 0x00000004;		// update TAEVENT to capture the falling edge
	SYSCTL_GPTMICR		|=	 0x00000001;		// clears the TATORIS bit in GPTMRIS and the TATOMIS bit in GPTMMIS register so that the timeout flag bit can be cleared
	SYSCTL_GPTM_CTL		|=	 0x00000001;		// enable timer 0A
}


void timer0B_init(void){
	SYSCTL_RCGCTIMER	|=	 0x00000001;		// use timer 0
	SYSCTL_GPTM_CTL		&=	~0x00000100;		// disable timer B before initialization
	SYSCTL_GPTM_CFG		 =	 0x00000004; 		// put value of 0x4 so that the timer uses 16-bit config, it is not able to use 32-bit
	SYSCTL_GPTMTBMR		|=	 0x00000001;		// use one-shot timer mode
	SYSCTL_GPTMTBMR		&=	~0x00000010;		// update TBCDIR to use count down mode
	SYSCTL_GPTMTBPR		 = 		3;				// prescale timer by a factor of 3 i.e 16000000/3= 5333333.333Hz
	SYSCTL_GPTMTBMR		&=	~0x00000040;		// update TBWOT to start timer as soon as it is enabled
	SYSCTL_GPTMTBILR	 = 		53334;			// x/(16000000/3) = 10ms -> x = 53334
	SYSCTL_GPTMICR		|=	 0x00000100;		// clears the TBTORIS bit in GPTMRIS and the TBTOMIS bit in GPTMMIS register so that the timeout flag bit can be cleared
	GPIO_PORTA_DATA_R	|= 	 0x00000004;		// trigger PA2 Pin
	SYSCTL_GPTM_CTL		|=	 0x00000100;		// enable timer 0B
}


void Systick_Init(void){
	SYSCTL_STCTRL		&= ~0x00000001;			// disable Systick before initialization
	SYSCTL_STRELOAD	 	 =   16777215;  		// maximum value which can be fit in the free 24 bits of this register
	SYSCTL_SYSPRI3		|= 	0x30000000;  		// set priority of the handler
	SYSCTL_STCTRL		|=	0x00000003;			// enable the Systick and use frequencz of PIOSC ie. 16Mhz/4 = 4Mhz
	SYSCTL_STCURRENT     = 		0;				// load current value of counter
}


uint32_t signal(void){
	uint32_t time;
	timer0B_init();
	if(SYSCTL_GPTMRIS&0x00000100){
		SYSCTL_GPTMICR		|=	 0x00000100;	// reset timer flag
		GPIO_PORTA_DATA_R	&=	~0x00000004;	// turn PA2 off
		GPIO_PORTA_DATA_R	^=	 0x00000010;    // remove
	}
	timer0A_init();
	//BIG PROBLEM HERE
	while((SYSCTL_GPTMTBR) != 608000){
		if((SYSCTL_GPTMRIS&0x00000100) == 1 && (GPIO_PORTA_DATA_R&0x00000008) == 0){
		time = SYSCTL_GPTMTBR;
		return time;
		}
	}
	return 608000;
}


uint32_t measure_distance(uint32_t time){
	uint32_t distance;
	distance = 34000 * time;
	distance /= 2;
	return distance;
}


/**
 * main.c
 */
int main(void)
{
	uint32_t time;
	uint32_t distance;
	portA_init();
	portB_init();
	Systick_Init();
	pwm_init();
	//timer0A_init();
	timer0B_init();
	while(1){
		//time = signal();
		//distance = measure_distance(time);
		if(SYSCTL_GPTMRIS&0x00000100){
			GPIO_PORTA_DATA_R ^= 0x00000010;   // --REMOVE LATER
			SYSCTL_GPTMICR |= 0x00000100;
			timer0B_init();
		}
		if(SYSCTL_GPTMRIS&0x00000001){
			GPIO_PORTA_DATA_R ^= 0x00000020;
			SYSCTL_GPTMICR |= 0x00000001;
			timer0B_init();
		}
		//if(!(GPIO_PORTB_DATA_R&0x00000020)){
			// generate interrupt to start the pump
		//}
	}
	return 0;
}
