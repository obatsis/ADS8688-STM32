#ifndef ADS8688_H
#define ADS8688_H

// include the correct file according to you MCU
#include "stm32f4xx_hal.h"


// datasheet link
// https://www.ti.com/lit/ds/symlink/ads8668.pdf?ts=1620233637463&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS8668

// ----------------------- ADS Commands and Registers ------------------------ //
// Commands
#define CONT 0x00				// continue operation in previous mode
#define STBY 0x82				// device is placed into standby mode
#define PWDN 0x83				// device is powered down
#define RST 0x85				// program register is reset to default
#define AUTO_RST 0xa0			// Auto mode enabled following a reset
#define MAN_0 0xc0				// Channel 0 manual input selected
#define MAN_1 0xc4				// Channel 1 manual input selected
#define MAN_2 0xc8				// Channel 2 manual input selected
#define MAN_3 0xcc				// Channel 3 manual input selected
#define MAN_4 0xd0				// Channel 4 manual input selected
#define MAN_5 0xd4				// Channel 5 manual input selected
#define MAN_6 0xd8				// Channel 6 manual input selected
#define MAN_7 0xdc				// Channel 7 manual input selected

// Register addresses
#define AUTO_SEQ_EN 0x01		// Auto Scan Sequencing Control
#define CHN_PWRDN 0x02			// Channel Power Down
#define FEATURE_SELECT 0x03		// Feature Select
#define CHN_0_RANGE 0x05		// Channel 0 Input Range
#define CHN_1_RANGE 0x06		// Channel 1 Input Range
#define CHN_2_RANGE 0x07		// Channel 2 Input Range
#define CHN_3_RANGE 0x08		// Channel 3 Input Range
#define CHN_4_RANGE 0x09		// Channel 4 Input Range
#define CHN_5_RANGE 0x0a		// Channel 5 Input Range
#define CHN_6_RANGE 0x0b		// Channel 6 Input Range
#define CHN_7_RANGE 0x0c		// Channel 7 Input Range
#define ALRM_OVRVW_TRIP 0x10	// ALARM Overview Tripped-Flag
#define ALRM_0_3_TRIP 0x11		// ALARM Ch 0-3 Tripped-Flag
#define ALRM_0_3_ACT 0x12		// ALARM Ch 0-3 Active-Flag
#define ALRM_4_7_TRIP 0x13		// ALARM Ch 4-7 Tripped-Flag
#define ALRM_4_7_ACT 0x14		// ALARM Ch 4-7 Active-Flag
/*
	Here place the rest of the registers you want to use...
*/
#define CMD_RD_BCK 0x3f			// COMMAND READ BACK (Read-Only)

// --------------------------------------------------------------------------- //

#define CHNS_NUM_READ 8			// the number of channel you want to get the raw data (you also have to adjust the AUTO_SEQ_EN register value to match with the number of channel you like to read)

typedef struct {

	/* SPI */
	SPI_HandleTypeDef *spiHandle;
	GPIO_TypeDef 	  *csPinBank;
	uint16_t 		   csPin;

} ADS8688;


/*
 *
 * INITIALISATION
 *
 */
uint8_t ADS8688_Init(ADS8688 *ads, SPI_HandleTypeDef *spiHandle, GPIO_TypeDef *csPinBank, uint16_t csPin);

/*
 *
 * REGISTER AND COMAND LOW LEVEL FUNCTIONS
 *
 */
HAL_StatusTypeDef ADS_Prog_Read(ADS8688 *ads, uint8_t addr, uint8_t *data);
HAL_StatusTypeDef ADS_Prog_Write(ADS8688 *ads, uint8_t addr, uint8_t *data);
HAL_StatusTypeDef ADS_Cmd_Write(ADS8688 *ads, uint8_t cmd, uint8_t *data);


/*
 *
 * HIGHER LEVEL FUNCTIONS
 *
 */

HAL_StatusTypeDef ADS_Read_All_Raw(ADS8688 *ads, uint16_t *data);





#endif
