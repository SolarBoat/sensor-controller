/*
 * motor.h
 */

#ifndef MOTOR_H_
#define MOTOR_H_


void motor_init();

void motor_enable();

void motor_disable();

// speeds are from -100 to 100
void motor_set_speed(signed char left, signed char right);


#endif /* MOTOR_H_ */
