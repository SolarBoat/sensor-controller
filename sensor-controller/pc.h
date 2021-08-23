/*
 * pc.h
 */

#ifndef PC_H_
#define PC_H_

unsigned int buckPWM;
unsigned int cellVoltage1, cellVoltage2, cellVoltage3, cellVoltage4;
unsigned int solarVoltageN, solarVoltageP;
unsigned int motorCurrent1, motorCurrent2;
unsigned int buckCurrent;
unsigned int systemCurrent;

void pc_init();


#endif /* PC_H_ */
