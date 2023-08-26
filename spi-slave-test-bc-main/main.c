
/**
 * \file main.c
 *
 * \brief Main source file.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.
    Subject to your compliance with these terms, you may use this software and
    any derivatives exclusively with Microchip products. It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

// CONFIG1L
#pragma config FEXTOSC = OFF    // External Oscillator Selection->Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_64MHZ    // Reset Oscillator Selection->HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1

// CONFIG1H
#pragma config CLKOUTEN = OFF    // Clock out Enable bit->CLKOUT function is disabled
#pragma config PR1WAY = ON    // PRLOCKED One-Way Set Enable bit->PRLOCK bit can be cleared and set only once
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable bit->Fail-Safe Clock Monitor enabled

// CONFIG2L
#pragma config MCLRE = EXTMCLR    // MCLR Enable bit->If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR 
#pragma config PWRTS = PWRT_OFF    // Power-up timer selection bits->PWRT is disabled
#pragma config MVECEN = OFF    // Multi-vector enable bit->Interrupt contoller does not use vector table to prioritze interrupts
#pragma config IVT1WAY = ON    // IVTLOCK bit One-way set enable bit->IVTLOCK bit can be cleared and set only once
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit->ULPBOR disabled
#pragma config BOREN = SBORDIS    // Brown-out Reset Enable bits->Brown-out Reset enabled , SBOREN bit is ignored

// CONFIG2H
#pragma config BORV = VBOR_2P45    // Brown-out Reset Voltage Selection bits->Brown-out Reset Voltage (VBOR) set to 2.45V
#pragma config ZCD = OFF    // ZCD Disable bit->ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON
#pragma config PPS1WAY = ON    // PPSLOCK bit One-Way Set Enable bit->PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config DEBUG = OFF    // Debugger Enable bit->Background debugger disabled
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Extended Instruction Set and Indexed Addressing Mode disabled

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31    // WDT Period selection bits->Divider ratio 1:65536; software control of WDTPS
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled; SWDTEN is ignored

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7    // WDT Window Select bits->window always open (100%); software control; keyed access not required
#pragma config WDTCCS = SC    // WDT input clock selector->Software Control

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512    // Boot Block Size selection bits->Boot Block size is 512 words
#pragma config BBEN = OFF    // Boot Block enable bit->Boot block disabled
#pragma config SAFEN = OFF    // Storage Area Flash enable bit->SAF disabled
#pragma config WRTAPP = OFF    // Application Block write protection bit->Application Block not write protected

// CONFIG4H
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block not write-protected
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration registers (300000-30000Bh) not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not write-protected
#pragma config WRTSAF = OFF    // SAF Write protection bit->SAF not Write Protected
#pragma config LVP = ON    // Low Voltage Programming Enable bit->Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored

// CONFIG5L
#pragma config CP = OFF    // PFM and Data EEPROM Code Protection bit->PFM and Data EEPROM code protection disabled


#include <xc.h>
#include <stdint.h>

static void CLK_Initialize(void);
static void PPS_Initialize(void);
static void PORT_Initialize(void);
static void SPI1_Initialize(void);
static void INTERRUPT_Initialize(void);
static uint8_t SPI1_exchangeByte(uint8_t data);
//static void MSSP1_InterruptHandler(void);

volatile uint8_t receiveData = 0x00;        /* Data that will be received */
volatile uint8_t writeData = 32;      /* Data that will be transmitted */

static void CLK_Initialize(void)
{
     // NOSC HFINTOSC; NDIV 1; 
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power; 
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
    OSCEN = 0x00;
    // HFFRQ 16_MHz; 
    OSCFRQ = 0x05;
    // TUN 0; 
    OSCTUNE = 0x00;
}


static void PPS_Initialize(void)
{
    RA5PPS = 0b100000 ;     //RA5->SPI1:SS;
    SPI1SSPPS = 0b00101;    //RA5->SPI1:SS;
   
    
	
    SPI1SCKPPS = 0x13;   //RC3->SPI1:SCK1;    
    //RC3PPS = 0x1E;   //RC3->SPI1:SCK1;    
    RC5PPS = 0x1F;   //RC5->SPI1:SDO1;    
    SPI1SDIPPS = 0x14;   //RC4->SPI1:SDI1; 
}

static void PORT_Initialize(void)
{
    /* SDO as output; SDI, SCK as input */
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 0;
    TRISAbits.TRISA5 = 1;
    /* SS as digital pin */
    ANSELA = 0x00;  
    
    /* SCK, SDI, SDO as digital pins */
    ANSELC = 0x00;         
}

static void SPI1_Initialize(void)
{
    //EN disabled; LSBF MSb first; bus slave; BMODE last byte; 
    SPI1CON0 = 0x00;    
    //SMP Middle; CKE Active to idle; CKP Idle:Low, Active:High; FST disabled; SSP active low; SDIP active high; SDOP active high; 
    SPI1CON1 = 0x04;    
    //SSET disabled; TXR required for a transfer; RXR data is not stored in the FIFO; 
    SPI1CON2 = 0x03;
    //CLKSEL FOSC; 
    SPI1CLK = 0x00;
    //BAUD 0; 
    SPI1BAUD = 0x1F;        //0x1F para 64MHz, 0x01 para 16MHz, 0x1F para 500KHz
    TRISCbits.TRISC3 = 1;           
}

static void INTERRUPT_Initialize(void)
{
    INTCON0bits.IPEN = 0;
}

static uint8_t SPI1_exchangeByte(uint8_t data)
{
    SPI1TCNTL = 1;
    SPI1TXB = data;
    //SPI1TXB = 0x55;
    while(!PIR2bits.SPI1RXIF);
    return SPI1RXB; 
}

static void SPI1_RxInterruptHandler(void)
{
    //receiveData = SPI1_exchangeByte(writeData);
}

void __interrupt() INTERRUPT_InterruptManager(void)
{
   
        if(PIE2bits.SPI1RXIE == 1 && PIR2bits.SPI1RXIF == 1)
    {
        SPI1_RxInterruptHandler();
    }
}

int main(void)
{
    CLK_Initialize();
    PPS_Initialize();
    PORT_Initialize();
    SPI1_Initialize();
    INTERRUPT_Initialize();
    SPI1CON0bits.EN = 1;
    while(1)
    {
        if(LATAbits.LA5 == 0){
        
            receiveData = SPI1_exchangeByte(writeData);     //Even though the master does not send useful stuff the slave needs to use this instruction to sycnhronize with the master, simply SPI1_WriteByte does not work
        }
        
    }
}

