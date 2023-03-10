/*
 * pindef.h
 *
 *  Created on: 8 Mar 2023
 *      Author: kreisch
 */

#ifndef PINDEF_H_

#define PINDEF_H_
#define GPIO_PORTA_DATA_R		(*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DEN_R		(*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_DIR_R		(*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R		(*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_AMSEL_R		(*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R		(*((volatile unsigned long *)0x4000452C))
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGCGPIO_R 		(*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R			(*((volatile unsigned long *)0x400FEA08))
#define SYSCTL_RCC_R 			(*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCGCPWM_R		(*((volatile unsigned long *)0x400FE640))
#define SYSCTL_PRPWM_R			(*((volatile unsigned long *)0x400FEA40))
#define PWM0_CTL_R				(*((volatile unsigned long *)0x40028040))
#define PWM0_LOAD_R 			(*((volatile unsigned long *)0x40028050))
#define PWM0_CMPA_R 			(*((volatile unsigned long *)0x40028058))
#define PWM0_CMPB_R 			(*((volatile unsigned long *)0x4002805C))
#define PWM0_GENA_R 			(*((volatile unsigned long *)0x40028060))
#define PWM0_GENB_R 			(*((volatile unsigned long *)0x40028064))
#define PWM0_ENABLE_R 			(*((volatile unsigned long *)0x40028008))
#define SYSCTL_RCGCTIMER		(*((volatile unsigned long *)0x400FE604))
#define SYSCTL_GPTM_CTL			(*((volatile unsigned long *)0x4003000C))
#define SYSCTL_GPTM_CFG			(*((volatile unsigned long *)0x40030000))
#define SYSCTL_GPTMTAMR			(*((volatile unsigned long *)0x40030004))
#define SYSCTL_GPTMTBMR			(*((volatile unsigned long *)0x40030008))
#define SYSCTL_GPTMTAILR		(*((volatile unsigned long *)0x40030028))
#define SYSCTL_GPTMTBR			(*((volatile unsigned long *)0x4003004C))
#define SYSCTL_GPTMICR			(*((volatile unsigned long *)0x40030024))
#define SYSCTL_GPTMRIS			(*((volatile unsigned long *)0x4003001C))
#define SYSCTL_GPTMTBILR		(*((volatile unsigned long *)0x4003002C))
#define SYSCTL_GPTMTBPR			(*((volatile unsigned long *)0x4003003C))
#define SYSCTL_STCTRL			(*((volatile unsigned long *)0xE000E010))
#define SYSCTL_STRELOAD			(*((volatile unsigned long *)0xE000E014))
#define SYSCTL_STCURRENT		(*((volatile unsigned long *)0xE000E018))
#define SYSCTL_SYSPRI3			(*((volatile unsigned long *)0xE000ED20))

#endif /* PINDEF_H_ */
