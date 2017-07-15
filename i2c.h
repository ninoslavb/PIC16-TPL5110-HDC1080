/* 
 * File:   i2c_nino.h
 * Author: ninos
 *
 * Created on 06 July 2016, 12:51
 */

#ifndef I2C_NINO_H
#define	I2C_NINO_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MSSP_SCL_TRIS   TRISBbits.TRISB6 // scl je na portu RB6
#define MSSP_SCL_LAT    LATBbits.LATB6
#define MSSP_SDA_TRIS   TRISBbits.TRISB4 //sda je na portu RB4 

//zastavice za I2C komunikaciju
#define kI2C_MASTER     (0x01) 
#define kI2C_DISABLE    (0x00)
#define kI2C_100KHZ     (0x00)
#define kI2C_400KHZ     (0x02)

#define BGP_BASE        (_XTAL_FREQ / 400000L)

//#define SENSOR_ADDRESS  0x80
//#define TEMP_ADDRESS    0x00
//#define CONFIG_ADDRESS    0x02    
    
int i2cAddressSensorReadData(uint8_t* rdata, int rsize);
int i2cAddressSensorWriteData(uint8_t* wdata, int wsize);
int i2cStart(void);
int i2cSetAddress(uint8_t address);
void i2cReset(void);
int i2cStop (void);
int i2cRepeatedStart (void);
int i2cPointerAddressSensor(uint8_t address);
int i2cXfer(
    uint8_t* wdata, int wsize, //pointer na podatak za pisanje i veli?ina podatka
	uint8_t* rdata, int rsize ); 
void i2cSetup(void);
void i2cReset(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_NINO_H */

