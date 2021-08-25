/*
 * i2c.h
 */

#ifndef I2C_H_
#define I2C_H_


void i2c_init();

unsigned char i2c_write(unsigned char address, unsigned char length, unsigned char * txData, unsigned char stop);

unsigned char i2c_read(unsigned char address, unsigned char length, unsigned char * rxData);

#endif /* I2C_H_ */
