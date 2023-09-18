### SPI Slave Device

This device's firmware is programmed through "bare metal coding", which is a low-level type of programming (C language), without any abstraction or superior software layers, that is hardcoded to the device at a hardware level and optimizes code execution. The same definition and implementation of the functions used in the master device are done for the slave device in the `main.c` file directly, removing two abstraction layers.

## Device configuration

- Configuration bits (CONFIG1-5H/L registers)
  - Oscillator source at power-up: High frequency internall oscillator with no clock division applied
  - Master Clear and Low Voltage Programming: MCLR and LVP are enabled, making the MCLR pin work for as a master clear for programming.
  - Brown-out Reset: Disabled, device will not reset when voltage drops under a certain threshold.
  - Watchdog Timer: Disabled, the Windowed Watchdog Timer will not be enabled for this test, to check for timing inaccuracies while executing instructions.
- Clock Manager (OSCCON1 and OSCFRQ registers)
  -   Clock Frequency: 16MHz, proceeding from a 64MHz base High-Frequency Oscillator Clock
- Pin Manager:
  -  LED pin -> RC2 (output)
  -  Slave Select -> RA5 (input)
  -  SPI clock -> RC3 (input)
  -  SPI Data Out (SDO) -> RC5 (output)
  -  SPI Data In (SDI) -> RC4 (input)


## SPI module configuration

- SPI Mode 0
  - Bit Count Mode (BMODE): 0
  - Bus: Slave
  - MSB transmitted first
  - Sample Data Inout: Middle
  - Clock Transition: Active to Idle
  - Idle State of Clock: Low Level
  - Slave Select Active: Low Level
  - SDI Active: High Level
  - SDO Active: High Level
- Transfer Settings
  - SDO transfers: according to the level of Slave Select
  - Transmit: Enabled
  - Receive: Disabled
- Clock Settings
  - Clock Source: High Frequency Internall Oscillator
  - Baud Clock: 4MHz
- Implemented Functions
  - ExchangeByte  
 
The message reception is done through the ByteExchange function, even though the master is not meant to send any data out. This function already implements the process for reading the receive buffer when a byte is stored and the interrupt flag sets. As for the transmitted message from the master device, the user can send a 0x00 byte to avoid sending anything if the TXR bit was to be set.

