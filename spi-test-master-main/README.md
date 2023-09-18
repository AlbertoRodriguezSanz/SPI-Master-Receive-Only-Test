### SPI Master Device

This device's firmware is programmed through Foundation Service libraries found in the Microchip Code Configurator (MCC), which generates all of the required functions for initialization and operation. 
The communication steps are implemented in `spi_master_example.c` file and called in the `main.c` file.

## Device configuration

- Configuration bits (CONFIG1-5H/L registers)
  - Oscillator source at power-up: High frequency internall oscillator with no clock division applied
  - Master Clear and Low Voltage Programming: MCLR and LVP are enabled, making the MCLR pin work for as a master clear for programming.
  - Brown-out Reset: Disabled, device will not reset when voltage drops under a certain threshold.
  - Watchdog Timer: Disabled, the Windowed Watchdog Timer will not be enabled for this test, to check for timing inaccuracies while executing instructions.
- Clock Manager (OSCCON1 and OSCFRQ registers)
  -   Clock Frequency: 16MHz, proceeding from a 64MHz base High-Frequency Oscillator Clock
- Pin Manager:
  -  Slave Select -> RA5 (output)
  -  SPI clock -> RC3 (output)
  -  SPI Data Out (SDO) -> RC5 (output)
  -  SPI Data In (SDI) -> RC4 (input)

## SPI module configuration

![spi_Master_fsl](https://github.com/AlbertoRodriguezSanz/SPI-Master-Transmit-Only-Test/assets/95371514/3aa8eacd-1583-4173-91d7-bd830cbe2b16)

- SPI Mode 0
  - Bit Count Mode (BMODE): 0
  - Bus: Master
  - MSB transmitted first
  - Sample Data Inout: Middle
  - Clock Transition: Active to Idle
  - Idle State of Clock: Low Level
  - Slave Select Active: Low Level
  - SDI Active: High Level
  - SDO Active: High Level
- Transfer Settings
  - Slave Select value: driven to the active state while transmit counter is not zero
  - Transmit: Enabled
  - Receive: Enabled
- Clock Settings
  - Clock Source: High Frequency Internall Oscillator
  - Baud Clock: 4MHz
 
The message reception is done through the SPI1_ByteExchange function, even though the master is not meant to send any data out. This function already implements the process for reading the receive buffer when a byte is stored and the interrupt flag sets. As for the transmitted message from the master device, the user can send a 0x00 byte to avoid sending anything if the TXR bit was to be set.
