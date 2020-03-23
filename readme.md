<div id="readme" class="Box-body readme blob js-code-block-container">
<article class="markdown-body entry-content p-3 p-md-6" itemprop="text"><p><a href="https://www.microchip.com" rel="nofollow"><img src="Images/MicrochipLogo.png" alt="MCHP" style="max-width:100%;"></a></p>


# PIC18F47Q10 Exchanging Data as a Slave SPI Device Using Interrupts

## Objective:
The PIC18F47Q10 features two MSSP modules, which can be configured in SPI mode.
In this demo, the SPI is configured as slave and it is used to exchange data. The data transfer is triggered by interrupts.

## Resources:
- Technical Brief Link [(linkTBD)](http://www.microchip.com/)
- MPLAB® X IDE 5.30 or newer [(microchip.com/mplab/mplab-x-ide)](http://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC8 2.10 or newer compiler [(microchip.com/mplab/compilers)](http://www.microchip.com/mplab/compilers)
- PIC18F47Q10 Curiosity Nano [(DM182029)](https://www.microchip.com/Developmenttools/ProductDetails/DM182029)
- [PIC18F47Q10 datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/40002043D.pdf) for more information or specifications.

## Hardware Configuration:
The PIC18F47Q10 Curiosity Nano Development Board [(DM182029)](https://www.microchip.com/Developmenttools/ProductDetails/DM182029) is used as the test platform.
The following configurations must be made for this project:
- RA5 pin - SS
- RC3 pin - SCK
- RC4 pin - SDI
- RC5 pin - SDO

## Demo:
Run the code, connect an oscilloscope or logic analyzer to the pins. In order to see the results, a master SPI device must be connected with the slave device.
