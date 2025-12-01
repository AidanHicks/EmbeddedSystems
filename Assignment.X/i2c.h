/* 
 * File:   i2c.h
 * Author: 767905
 *
 * Created on 01 December 2025, 15:47
 */

#ifndef I2C_H
#define	I2C_H

void I2C1_Init_Master(unsigned long i2c_hz);
unsigned char I2C1_Write(unsigned char b);
void I2C1_Stop(void);
void I2C1_Start(void);
unsigned char I2C1_Read_ACK(void);
unsigned char I2C1_Read_NACK(void);
void I2C1_Restart(void);

#endif	/* I2C_H */

