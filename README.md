# ADS8688-STM32
TI-ADS8688 SPI Library intended for use within STM32CubeIDE (uses HAL Library)

Library Functionalities:

* Initialization Function for various channel set-ups
* Low Level Register Read/Write and Command Functions
* High level Functions for reading raw data from tha ADC
* Usage Example: `main.c`
-----------------------------------------------------------------------------
MXCube Setup:
* SPI Mode: `Full-Duplex Master`
* Hardware NSS: `Disabled` (Setup a separate GPIO as Output, Very High Speed, Output Level High)
* Frame Format: `Motorola`
* Data Size: `16 Bit`
* First Bit: `MSB`
* Prescaler: `16` (with HCLK @168MHz)
* Clock Polarity: `Low`
* Cock Phase: `2 Edge`
* CRC: `Disabled`
* NSS Signal Type: `Software`
-----------------------------------------------------------------------------
