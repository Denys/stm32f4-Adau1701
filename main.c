/**
 *	Keil project for LIS302DL or LIS3DSH accelerometer on STM32F4-Discovery board
 *
 *  Designed to work with STM32F4-Discovery board with onboard device
 *
 *	@author		Borja Ponz
 *	@email		
 *	@website	
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 *
 */
/* Include core modules */
#include "stm32f4xx.h"
#include <stm32f4xx_i2c.h>
/* Include my libraries here */
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"
#include "tm_stm32f4_i2c.h"


/* Accelerometer data structure */
TM_LIS302DL_LIS3DSH_t Axes_Data;

#define ADDRESS	0x68 
void escribiri2c_mute(void){
	uint8_t addr[]={0x00,0x01};
	uint8_t addr2[]={0x00,0x00};
	uint8_t mapped_mute[4]={0x00,0x00,0x00,0x00};
	int i;
	TM_I2C_Start(I2C1,0x68, 0, 0);// el primer 0 significa modo TRANSMISOR, el segundo 0 es sin ACK final.
	TM_I2C_WriteData(I2C1,addr[0]);
	TM_I2C_WriteData(I2C1,addr[1]);
	for(i=0; i<4; i++){
		TM_I2C_WriteData(I2C1,mapped_mute[i]);
	}
	TM_I2C_Stop(I2C1);
	TM_I2C_Start(I2C1,0x68, 0, 0);// el primer 0 significa modo TRANSMISOR, el segundo 0 es sin ACK final.
	TM_I2C_WriteData(I2C1,addr2[0]);
	TM_I2C_WriteData(I2C1,addr2[1]);
	for(i=0; i<4; i++){
		TM_I2C_WriteData(I2C1,mapped_mute[i]);
	}
	TM_I2C_Stop(I2C1);
}
void escribiri2c_unmute(void){
	int i;
	uint8_t addr[]={0x00,0x01};
	uint8_t addr2[]={0x00,0x00};
	uint8_t mapped_unmute[4]={0x00,0x80,0x00,0x00};
	TM_I2C_Start(I2C1,0x68, 0, 0);// el primer 0 significa modo TRANSMISOR, el segundo 0 es sin ACK final.
	TM_I2C_WriteData(I2C1,addr[0]);
	TM_I2C_WriteData(I2C1,addr[1]);
	for(i=0; i<4; i++){
		TM_I2C_WriteData(I2C1,mapped_unmute[i]);
	}
	TM_I2C_Stop(I2C1);
	TM_I2C_Start(I2C1,0x68, 0, 0);// el primer 0 significa modo TRANSMISOR, el segundo 0 es sin ACK final.
	TM_I2C_WriteData(I2C1,addr2[0]);
	TM_I2C_WriteData(I2C1,addr2[1]);
	for(i=0; i<4; i++){
		TM_I2C_WriteData(I2C1,mapped_unmute[i]);
	}
	TM_I2C_Stop(I2C1);
}
void cambiarvolumeni2c(int posicion_vol){
	int i;
	uint8_t mapped_vol[21][4]={{0x00,0x80,0x00,0x00},
														 {0x00,0x65,0xAC,0x8C},
														 {0x00,0x50,0xC3,0x36},
														 {0x00,0x40,0x26,0xE7},
														 {0x00,0x32,0xF5,0x2D},
														 {0x00,0x28,0x7A,0x27},
														 {0x00,0x20,0x26,0xF3},
														 {0x00,0x19,0x8A,0x13},
														 {0x00,0x14,0x49,0x61},
														 {0x00,0x10,0x1D,0x3F},
														 {0x00,0x0C,0xCC,0xCD},
														 {0x00,0x0A,0x2A,0xDB},
														 {0x00,0x08,0x13,0x85},
														 {0x00,0x06,0x6A,0x4A},
														 {0x00,0x05,0x18,0x84},
														 {0x00,0x04,0x0C,0x37},
														 {0x00,0x03,0x37,0x18},
														 {0x00,0x02,0x80,0xCF},
														 {0x00,0x02,0x07,0x56},
														 {0x00,0x01,0x9C,0x86},
														 {0x00,0x01,0x47,0xAE}};
	uint8_t addr[]={0x00,0x02};
	uint8_t addr2[]={0x00,0x03};
	//Iniciamos transmision.
	TM_I2C_Start(I2C1,0x68, 0, 0);// el primer 0 significa TRANSMISOR, el segundo 0 es sin ACK final.
	TM_I2C_WriteData(I2C1,addr[0]);
	TM_I2C_WriteData(I2C1,addr[1]);
	for(i=0; i<4; i++){
		TM_I2C_WriteData(I2C1,mapped_vol[posicion_vol][i]);
	}
	TM_I2C_Stop(I2C1);
	TM_I2C_Start(I2C1,0x68, 0, 0);// el primer 0 significa TRANSMISOR, el segundo 0 es sin ACK final.
	TM_I2C_WriteData(I2C1,addr2[0]);
	TM_I2C_WriteData(I2C1,addr2[1]);
	for(i=0; i<4; i++){
		TM_I2C_WriteData(I2C1,mapped_vol[posicion_vol][i]);
	}
	TM_I2C_Stop(I2C1);
}
int main(void) {	
	/* Initialize system */
	int pos_volumen=0;
	SystemInit();
	
	/* Init delay */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_1, 50000);
	
	
	

	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {

		TM_DISCO_LedOn(LED_GREEN | LED_RED);

		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {

		TM_DISCO_LedOn(LED_BLUE | LED_ORANGE);

		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	} else {

		TM_DISCO_LedOn(LED_GREEN | LED_RED | LED_BLUE | LED_ORANGE);

		while (1);
	}
	
	Delayms(2000);

	while (1) {

		
		TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
		
		if (Axes_Data.X > 200) {
							
			pos_volumen--;
			if(pos_volumen<0)pos_volumen=0;
			cambiarvolumeni2c(pos_volumen);
		  TM_DISCO_LedOn(LED_RED);
			Delayms(200);
		} else {
			TM_DISCO_LedOff(LED_RED);
		}
		
		if (Axes_Data.X < -200) {

			pos_volumen++;
			if(pos_volumen>20)pos_volumen=20;
			cambiarvolumeni2c(pos_volumen);			  
			TM_DISCO_LedOn(LED_GREEN);
			Delayms(200);
			
		} else {
			TM_DISCO_LedOff(LED_GREEN);
		}
		
		if (Axes_Data.Y > 200) {
			TM_DISCO_LedOn(LED_ORANGE);
			escribiri2c_mute();
			Delayms(200);
		} else {
			TM_DISCO_LedOff(LED_ORANGE);
		}
		if (Axes_Data.Y < -200) {
			escribiri2c_unmute();
			TM_DISCO_LedOn(LED_BLUE);
			Delayms(200);
		} else {
			TM_DISCO_LedOff(LED_BLUE);
		}
	}
}
