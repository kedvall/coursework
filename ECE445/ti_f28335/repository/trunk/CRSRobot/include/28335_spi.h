#ifndef __28335_SPI_H_
#define __28335_SPI_H_

#include <DSP2833x_Device.h>


// Function prototypes
void init_SPI(void);
void SPI_RXint(void);
void start_SPI(void);
void writeDAC7564(float dac1,float dac2);
void sendPWM(float u1,float u2,float u3,float u4,float u5);

#endif /*28335_SPI_H_*/
