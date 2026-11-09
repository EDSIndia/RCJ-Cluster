/******************** (C) COPYRIGHT 2022 EDS INDIA *****************************
* File Name     : Misc_Functions.c					       *
* Author        : SANTANU ROY                                                  *
* Date          : 02-05-2022                                                   *
* Description   : This file contains declaration of all misc functions         *
* Revision	: Rev0    						       *
********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __Functions_H
#define __Functions_H

void FLASH_ProgramHalfWord(uint32_t Address, uint64_t Data);
void mSec_Delay(unsigned int);
void GP_Timer(void);
 void LCD_Drive(void);
 void CONFIG_BUZZ(void);
void Write_OdoMeter(void);
unsigned short Extract_Val(unsigned short *(Ptr));
void Gear (void);
void Cal_speed(void);
void Cal_odo(void);
void Dis_revgear(void);
void Dis_Gear(void);
void dis_alert(void);
void LOW_Voltage_Detection(void) ;
void SOKUDO_BATT_REMOTE_FRAME(void);
void App_Init(void);
void Get_ADC(void);
void FDCAN_Config(void);
void Read_BatteryVolt(void);
void Mode_Management(void);
void Management_Process(void);

void Display_Management(void);
#endif
