#ifndef __28335_PWM_H__
#define __28335_PWM_H__

#define PWM_CARRIER_HZ	20000L

#define PWM_TPS			0x0
#define PWM_TCLK		(HISPCLK_HZ >> (PWM_TPS))
#define PWM_PR			(PWM_TCLK / PWM_CARRIER_HZ)
#define PWM_DUTY50		(PWM_PR >> 1)

void init_PWM(enum epwm ep);
void init_PWMandDIR(enum epwm ep);
void PWMandDIR_out(enum epwm ep, float u);
void PWM_out(enum epwm ep, float u);
void init_dualPWM(enum epwm ep);
void dualPWM_out(enum epwm ep, float u);
void RCSERVO_out(enum epwm ep, float u);
void init_PWM_AS_RCSERVO(enum epwm ep);

#endif /* __2808_PWM_H__ */

