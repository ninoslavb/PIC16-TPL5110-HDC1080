/* 
 * File:   spi_nino.h
 * Author: ninos
 *
 * Created on 07 July 2016, 11:38
 */

#ifndef SPI_NINO_H
#define	SPI_NINO_H

#ifdef	__cplusplus
extern "C" {
#endif

//Prototipi funkcija    
void SPI_init();
uint8_t SPI_transfer(uint8_t data);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_NINO_H */

