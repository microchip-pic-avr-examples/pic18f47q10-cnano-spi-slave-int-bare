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

/* WDT operating mode->WDT Disabled */
#pragma config WDTE = OFF
/* Low voltage programming enabled, RE3 pin is MCLR */
#pragma config LVP = ON

#include <xc.h>
#include <stdint.h>

#define PPS_CONFIG_RA5_SPI_SS       0x05
#define PPS_CONFIG_RC3_SPI_SCK      0x0F
#define PPS_CONFIG_RC4_SPI_SDI      0x14
#define PPS_CONFIG_RC5_SPI_SDO      0x10

static void CLK_init(void);
static void PPS_init(void);
static void PORT_init(void);
static void SPI1_init(void);
static void INTERRUPT_init(void);
static uint8_t SPI1_exchangeByte(uint8_t data);
static void MSSP1_interruptHandler(void);

volatile uint8_t receiveData;        /* Data that will be received */
volatile uint8_t writeData = 1;      /* Data that will be transmitted */

static void CLK_init(void)
{
    OSCCON1 = _OSCCON1_NOSC1_MASK
            | _OSCCON1_NOSC2_MASK;              /* HFINTOSC Oscillator */
    
    OSCFRQ = _OSCFRQ_FRQ1_MASK;                 /* HFFRQ 4 MHz */
}

static void PPS_init(void)
{
    SSP1SSPPS = PPS_CONFIG_RA5_SPI_SS;             /* SS channel on RA5 */
    
    RC3PPS = PPS_CONFIG_RC3_SPI_SCK;               /* SCK channel on RC3 */
 
    SSP1DATPPS = PPS_CONFIG_RC4_SPI_SDI;           /* SDI channel on RC4 */
    
    RC5PPS = PPS_CONFIG_RC5_SPI_SDO;               /* SDO channel on RC5 */
}

static void PORT_init(void)
{
    TRISC |= _TRISC_TRISC3_MASK;            /* SCK channel as input */
    TRISC |= _TRISC_TRISC4_MASK;            /* SDI channel as input */
    TRISC &= ~_TRISC_TRISC5_MASK;           /* SDO channel as output */
    TRISA |= _TRISA_TRISA5_MASK;            /* SS channel as input */
    
    ANSELA &= ~_ANSELA_ANSELA5_MASK;        /* Set RA5 as digital */
    ANSELC = ~_ANSELC_ANSELC3_MASK 
           & ~_ANSELC_ANSELC4_MASK;         /* Set RC3 and RC4 pins as digital */
}

static void SPI1_init(void)
{
    /* Enable module, SPI Slave Mode */
    SSP1CON1 = _SSP1CON1_SSPEN_MASK
             | _SSP1CON1_SSPM2_MASK;         
    
    /* Enable MSSP interrupts */
    PIE3 |= _PIE3_SSPIE_MASK;               
}

static void INTERRUPT_init(void)
{
    INTCON = _INTCON_GIE_MASK               /* Enable Global Interrupts */
           | _INTCON_PEIE_MASK;             /* Enable Peripheral interrupts */
}

static uint8_t SPI1_exchangeByte(uint8_t data)
{
    SSP1BUF = data;

    /* Wait until the end of transmission */
    while(!(PIR3 & _PIR3_SSP1IF_MASK))      
    {
        ;
    }

    /* Clear the flag */
    PIR3 &= ~_PIR3_SSP1IF_MASK;             

    return SSP1BUF;  
}

static void MSSP1_interruptHandler(void)
{
    receiveData = SPI1_exchangeByte(writeData);
}

void __interrupt() INTERRUPT_InterruptManager(void)
{
    if(INTCON & _INTCON_PEIE_MASK)
    {
        if((PIE3 & _PIE3_SSP1IE_MASK) && (PIR3 & _PIR3_SSP1IF_MASK))
        {
            MSSP1_interruptHandler();
        }
    }
}

int main(void)
{
    CLK_init();
    PPS_init();
    PORT_init();
    SPI1_init();
    INTERRUPT_init();
    
    while(1)
    {
        ;
    }
}
