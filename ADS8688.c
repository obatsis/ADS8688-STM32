#include "ADS8688.h"

/*
 * INITIALISATION
 */
uint8_t ADS8688_Init(ADS8688 *ads, SPI_HandleTypeDef *spiHandle, GPIO_TypeDef *csPinBank, uint16_t csPin) {
/* Store interface parameters in struct */
	ads->spiHandle 		= spiHandle;
	ads->csPinBank 	= csPinBank;
	ads->csPin 		= csPin;

	uint8_t ads_data[2] = {0};
	uint8_t state = 0;
	// reset all registers to default
	state += ADS_Cmd_Write(ads, RST, ads_data);
	HAL_Delay(100);
	// send a no_op message to the ADS to enter IDLE mode
	state += ADS_Cmd_Write(ads, CONT, ads_data);
	HAL_Delay(10);
	// enable auto transmit for all inputs(datasheet page 54) or as many as you want
	// if you want only some of the inputs enabled, make sure to power down the unused ones
	ads_data[0] = 0xff;
	state += ADS_Prog_Write(ads, AUTO_SEQ_EN, ads_data);
	HAL_Delay(10);
	// set the desired features such as device id (if multiple devices are used), alarm enable/disable and output format
	ads_data[0] = 0x03; // here i chose id = 0, alarm = disabled and SDO_format = 3 (datasheet page 56)
	state += ADS_Prog_Write(ads, FEATURE_SELECT, ads_data);
	HAL_Delay(10);
	// set all channels ranges(page 57)
	// 0x05 -> Input range is set to 0 to 2.5 x VREF (for VREF=5 volts, this means 0-10 volts range)
	// 0x06 -> Input range is set to 0 to 1.25 x VREF (for VREF=5 volts, this means 0-5 volts range)
	ads_data[0] = 0x06;
	state += ADS_Prog_Write(ads, CHN_0_RANGE, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0x06;
	state += ADS_Prog_Write(ads, CHN_1_RANGE, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0x05;
	state += ADS_Prog_Write(ads, CHN_2_RANGE, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0x05;
	state += ADS_Prog_Write(ads, CHN_3_RANGE, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0x05;
	state += ADS_Prog_Write(ads, CHN_4_RANGE, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0x05;
	state += ADS_Prog_Write(ads, CHN_5_RANGE, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0x06;
	state += ADS_Prog_Write(ads, CHN_6_RANGE, ads_data);
	HAL_Delay(10);
	ads_data[0] = 0x06;
	state += ADS_Prog_Write(ads, CHN_7_RANGE, ads_data);
	HAL_Delay(10);
	// start the auto transmission by entering the appropriate state
	state += ADS_Cmd_Write(ads, AUTO_RST, ads_data);

	return state;
}



// after the read, data contains the data from the addressed register
HAL_StatusTypeDef ADS_Prog_Read(ADS8688 *ads, uint8_t addr, uint8_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t txbuf[2] = {0x00, (addr<<1 & 0xfe)}; // [15:9]->address, [8]->0, [7:0]->don't care (0x00) (stm32 uses little endian so reverse it)
	uint8_t rxbuf[4];

	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_RESET);
	ret = HAL_SPI_TransmitReceive(ads->spiHandle, txbuf, rxbuf, 2, 10);
	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_SET);

	data[0] = rxbuf[2];
	data[1] = rxbuf[3];
	return ret;
}

// after the write, data should contain the data (byte) written to the addressed register (check equality for evaluation)
HAL_StatusTypeDef ADS_Prog_Write(ADS8688 *ads, uint8_t addr, uint8_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t txbuf[2] = {data[0], (addr << 1 | 0x01)}; // [15:9]->address[6:0], [8]->1, [7:0]->data[7:0] (stm32 uses little endian so reverse it)
	uint8_t rxbuf[4];

	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_RESET);
	ret = HAL_SPI_TransmitReceive(ads->spiHandle, txbuf, rxbuf, 2, 10);
	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_SET);

	data[0] = rxbuf[3];
	data[1] = 0x00;
	return ret;
}

HAL_StatusTypeDef ADS_Cmd_Write(ADS8688 *ads, uint8_t cmd, uint8_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t txbuf[2] = {0x00,cmd}; // [15:9]->address[6:0], [8]->1, [7:0]->data[7:0] (stm32 uses little endian so reverse it)
	uint8_t rxbuf[4];

	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_RESET);
	ret = HAL_SPI_TransmitReceive(ads->spiHandle, txbuf, rxbuf, 2, 10);
	HAL_GPIO_WritePin(ads->csPinBank, ads->csPin, GPIO_PIN_SET);

	data[0] = rxbuf[2];
	data[1] = rxbuf[3];
	return ret;
}

HAL_StatusTypeDef ADS_Read_All_Raw(ADS8688 *ads, uint16_t *data) {
	HAL_StatusTypeDef ret;
	uint8_t ads_raw[2];
	for(int i=0; i<CHNS_NUM_READ; i++) {
	  ret = ADS_Cmd_Write(ads, CONT, ads_raw);
	  data[i] = (int)((uint16_t)(ads_raw[1]<<8 | ads_raw[0]) >> 4);
	}
	return ret;
}





