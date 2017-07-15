#include <xc.h>
#include <pic16lf1459.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "i2c.h"
#include "system.h"

#include "user.h"          /* User funct/params, such as InitApp */

void i2cSetup() //postavke i2c komunikacije
{
        uint8_t bgp = (uint8_t) BGP_BASE;
        
        bgp /= 1;
        bgp -= 1;
        SSPADD = 6;
        // SSPADD values of 0, 1 or 2 are not supported for I2C mode.
        // I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
        SSPCON1bits.SSP1M3 = 1; //strana 253 u manualu
        SSPCON1bits.SSP1M2 = 0;
        SSPCON1bits.SSP1M1 = 0;
        SSPCON1bits.SSP1M0 = 0; 
        
 }

//funkcije za pisanje i citanje

int i2cXfer(
	uint8_t* wdata, int wsize, //pointer na podatak za pisanje i veli?ina podatka
	uint8_t* rdata, int rsize )
    
    {
    int temp = 0;
    uint8_t SENSOR_ADDRESS = 0x40;
    uint8_t TEMP_ADDRESS = 0x00;
    uint8_t CONFIG_ADDRESS = 0x02;
    uint8_t addressRW = 0x00;
    
    // sensor configuration
    temp = i2cStart();
    addressRW = ( SENSOR_ADDRESS << 1 ) & 0xFE; // write
    i2cSetAddress(addressRW);
    
    
    i2cPointerAddressSensor(CONFIG_ADDRESS); 
    i2cAddressSensorWriteData(wdata,wsize); 
    
    // initiate conversion and read data  
    i2cRepeatedStart();
    addressRW = ( SENSOR_ADDRESS << 1 ) & 0xFE; // write
    i2cSetAddress(addressRW);
    i2cPointerAddressSensor(TEMP_ADDRESS); 
    // TEMP conversion: 11 bit accuracy 3.65 ms
    // TEMP conversion: 14 bit accuracy 6.35 ms   
    __delay_ms(7);     
    addressRW = ( SENSOR_ADDRESS << 1 ) | 0x01; // READ
    i2cSetAddress(addressRW);   
    i2cAddressSensorReadData(rdata,rsize );  
    i2cStop();
    
    temp = rdata[0]; //u temp spremi gornjih 8 bitova iz registra temperature
    temp = temp << 8; //pomakni tih 8 gornjih bitova registra temperature u gornjih 8 od temp
    temp |= rdata[1]; //OR sa donjih 8 bitova registra temperature, sad je u temp spremljen citav podatak
      //POGLEDATI PROTOKOL OD i2c senzora kojeg koristimo!
    return temp; 
    }

    //funkcija za resetiranje I2C modula
    void i2cReset(void)
    {
    // first make sure I2C module is disabled
    //SSPCON1bits.SSPEN = 0;    

    // hold SCL low for 100 ms to reset I2C bus
    // 
    //MSSP_SCL_TRIS = 0; //
    //MSSP_SCL_LAT = 0;
    //__delay_ms( 10 );
     //tri-date SCL and SDA

    MSSP_SCL_TRIS = 1;
    MSSP_SDA_TRIS = 1;
    //__delay_ms( 20 );
    } 
    int i2cStop (void)
    {
        int i = 0;
        // Generiraj stop uvjet
        PIR1bits.SSP1IF = 0; //ocisti zatavice
        PIR2bits.BCL1IF = 0;
        SSPCON2bits.PEN = 1; //STOP CONDITION
        
        while ( 1 ) {
            if ( PIR2bits.BCL1IF ) return -13;
            if ( PIR1bits.SSP1IF ) break;
            i = i + 1;
            if (i == 1000) return -99;
        }
        return 0;
    }
    int i2cStart (void)
    {
        int i = 0;
        PIR1bits.SSP1IF = 0; //strana 99 u manualu, Synchronous Serial Port (MSSP) Interrupt Flag bit, interrupt is not pending
        PIR2bits.BCL1IF = 0; //strana 100 u manualu, MSSP Bus Collision Interrupt Flag bit, interrupt is not pending
        SSPCON2bits.SEN = 1; //strana 254 u manualu, set SEN, Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware, 
        while ( 1 ) {
            if ( PIR2bits.BCL1IF ) return -2; //ako je bus collision, vrati -2
            if ( PIR1bits.SSP1IF ) break; //ako do?e do interrupta, zeljenoga, prekini ovu while petlju i nastavi sa izvo?enjem programa
            i = i + 1;
            if (i == 1000) return -99;            
        }
        return 0;
    }
    int i2cSetAddress(uint8_t address)
    {
        int i = 0;
        PIR1bits.SSP1IF = 0; //strana 99 u manualu, Synchronous Serial Port (MSSP) Interrupt Flag bit, interrupt is not pending
        PIR2bits.BCL1IF = 0; //strana 100 u manualu, MSSP Bus Collision Interrupt Flag bit, interrupt is not pending
        SSPBUF = address; 
        while ( 1 ) {
            if ( PIR2bits.BCL1IF ) return -3; //ako je kolizija, vrati -3
            if ( PIR1bits.SSP1IF ) break; // ako imamo prekid,(prekid izaziva slave ciju smo adresu poslali na sabirnicu), izadji iz petlje i nastavi sa izvo?enjem programa
            i = i + 1;
            if (i == 1000) return -99;    
        }
        if ( SSPCON2bits.ACKSTAT ) return -4; //acknowledge was not received, vrati -4 ,strana 254 u manualu
        return 0;
    }
    
    
    int i2cRepeatedStart (void)
    {    
        int i = 0;
        PIR1bits.SSP1IF = 0; //ocisti zastavicu za prekid
        PIR2bits.BCL1IF = 0; //ocisti zastavicu koja javlja koliziju        
        SSPCON2bits.RSEN = 1; //INITIATE REPEATED START CONDITION, MORA BITI REPEATED!!!
        while ( 1 ) {
            if ( PIR2bits.BCL1IF ) return -7; //ako je kolizija, vrati sme?e
            if ( PIR1bits.SSP1IF ) break; //ako je postavljena zastavica za komunikaciju, prekini while petlju i nastavi s izvo?enjem programa
            i = i + 1;
            if (i == 1000) return -99;
        }
        return 0;
    }
    
    int i2cPointerAddressSensor(uint8_t address)
    {
        int i = 0;
        PIR1bits.SSP1IF = 0;//ocisti zastavicu za prekid
        PIR2bits.BCL1IF = 0; //ocisti zastavicu koja javlja koliziju na sabirnici
        SSPBUF = address; //u buffer upisi pointer REGISTER OD TEMPERATURE
        
        while ( 1 ) {
            
            if ( PIR2bits.BCL1IF ) return -12; //ako ima kolizije, vrati sme?e
            if ( PIR1bits.SSP1IF ) break; //ako je postavljena zastavica komunikacije, nastavi
            i = i + 1;
            if (i == 1000) return -99;
        }
        if ( SSPCON2bits.ACKSTAT ) return -13; //ako nije acknowledge,vrati sme?e
        return 0;
    }
    
    int i2cAddressSensorReadData(uint8_t* rdata, int rsize)
    {    
        int j = 0;
        // Primjer citanja za HDC1080 senzor, http://www.ti.com/lit/ds/symlink/hdc1080.pdf , strana 11
        for ( int i = 0; i < rsize; i++ ) { //dok god ima podatka koje citamo
            // check idle, je li nedefinirano stanje(stanje visoke impedancije)
            if ( SSPCON2 & 0x1F ) return -10; //ako je stanje visoke impedancije, prekini. Pogledaj NOTE1 na strani 254 u manualu mikrokontrolera
            //omoguguci primanje podataka
            PIR1bits.SSP1IF = 0; //ocisti zastavicu za komunikacijski prekid, prekid od slavea
            PIR2bits.BCL1IF = 0; //ocisti zastavicu koja javlja koliziju
            SSPCON2bits.RCEN = 1; //repeated start condition
            while ( 1 ) {
                if ( PIR2bits.BCL1IF ) return -11; //ako je kolizija,vrati smece                
                if ( PIR1bits.SSP1IF ) break; //ako ima prekida od slavea, nastavi s izvodjenjem programa
                j = j + 1;
                if (j == 1000) return -99;
            }
            j = 0;
            // read data byte from buffer
            rdata[ i ] = SSPBUF; //u polje bajtova rdata spremi podatak iz buffera
            // __delay_ms(7);       //AD conversion time
            // send acknowledge (or not acknowledge for final byte)
            PIR1bits.SSP1IF = 0; //ocisti zastavice
            PIR2bits.BCL1IF = 0;
            SSPCON2bits.ACKDT = i == ( rsize - 1 ) ? 1 : 0; //
            SSPCON2bits.ACKEN = 1; //Initiate Acknowledge sequence on SDA and SCL pins, and transmit ACKDT data bit
            while ( 1 ) {
                if ( PIR2bits.BCL1IF ) return -12;
                if ( PIR1bits.SSP1IF ) break;
                j = j + 1;
                if (j == 1000) return -99;
            }
        } return 0 ;
    }
    int i2cAddressSensorWriteData(uint8_t* wdata, int wsize)
    {   
        int j = 0;
        for ( int i = 0; i < wsize; i++ ) { //podatak je spremljen u polje bajtova wdata
            PIR1bits.SSP1IF = 0; //ocisti zatavice
            PIR2bits.BCL1IF = 0;
            SSPBUF = wdata[ i ]; //salji bajt sa sabirnicu i u while(1) petlji provjeri sljedece:
            while ( 1 ) {
                if ( PIR2bits.BCL1IF ) return -5; //ako je data collision, vrati sme?e
                if ( PIR1bits.SSP1IF ) break; //gneriran je prekid od slavea, natavi s izvodjenjem programa
                j = j + 1;
                if (j == 1000) return -99;
            
            }//provjeri jel potvrdio slave
            if ( SSPCON2bits.ACKSTAT ) return -6; //ako nije primljen acknowledge bit, vrati sme?e
        }  return 0;
    }
        
   