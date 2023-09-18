# SPI-Master-Receive-Only-Test
Firmware for the various tests performed with two PIC18F26K83 microcontrollers for the SPI module where the master device begins the communication and then only receives the data being sent by the slave device. Within this firmware there are a couple of test variations for receiving a different number of bytes through different methods, with the objective of observing the generated signals with an oscilloscope.

## Setup
A breadboard with two PIC18F26K83 microcontrollers, where the microcontroller labelled '1' operates as the master device and the microcontroller labelled '3' operates as the slave device, was used as a test bench.

![broadboard_can_bus_top_viewjpg](https://github.com/AlbertoRodriguezSanz/CAN-Bus-Test/assets/95371514/c0f4a20e-199d-4b0a-b0b2-8a69f7578277) 

For this test the PICkit4 in-circuit debugger/programmer will be used to load the firmware into the microcontroller. This needs to be connected to the microcontroller with the following pins.
- MCLR (needs to be connected through two series pull-up resistors: 10kΩ and 100-470Ω to the power supply)
- PGD
- PGC
- VDD
- VSS

In the schematic of the breadboard the two connectors represent the PICKIT4 pins required for programming each of the two microcontrollers. 

![spi_can_combined_breadboard_schematic](https://github.com/AlbertoRodriguezSanz/SPI-Master-Transmit-Only-Test/assets/95371514/68131bdd-db41-4513-8ce9-3f52d5cdf47f)


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
  -  Slave Select -> RA5 (output)
  -  SPI clock -> RC3 (output)
  -  SPI Data Out (SDO) -> RC5 (output)
  -  SPI Data In (SDI) -> RC4 (input)

## SPI module configuration

For this test, the SPI module was programmed through two different abstraction methods: the Foundation Service Libraries (FSL) integrated in MCC for the master device and through "bare metal code" for the slave device. The specifics for each microcontroller are described on the respective README files.

## Requirements

Install MPLAB X IDE tool for Windows, Linux or MAC from the following link ([download link](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide#tabs)).

## Operation

Once MPLAB is opened, load the project through *File > Open Project* and then select the file `FSL_SPI_MASTER.mc3`.
This will open the work environment, where `main.c` is the code file that will be compiled into the PIC. The project properties are accessed through *Production > Set Project Configuration > Customize...*, where the PICkit4 needs to be selected in the *Connected Hardware Tool* menu.
![Screenshot from 2023-09-01 14-19-52](https://github.com/AlbertoRodriguezSanz/CAN-Bus-Test/assets/95371514/248a38f8-ebf5-4f62-97c1-47c6fd496216)

Modify the following options from the default parameters for the PICkit4 programmer from the Option categories dropdown menu.
-Power
  - Power target circuit from PICkit4.
  - Voltage Level: 5V.
- PICkit4 Tool Options
  - Program Speed: Low (Otherwise an error will pop up when trying to load the firmware).

Then, follow the next steps:
* Compile: `Production> Build Main Project`
* Program: `Production> Make and Program Device Main Project`
