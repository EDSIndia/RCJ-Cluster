/**
  ******************************************************************************
  * File Name          : STM32_Config.h
  * Description        : This file provides code for the configuration MCU
  * 
  *
  ******************************************************************************
  */

#ifndef __STM32_H
#define __STM32_H

#include "stm32g0xx_ll_tim.h"

#define TIM2_PRC    64
extern ADC_HandleTypeDef hadc1;
void SystemClock_Config(void);
void MX_IWDG_Init(void);
void MX_GPIO_Init(void);
void MX_ADC1_Init(void);
void MX_TIM2_Init(void);
void Configure_Buzzer(void);
void MX_FDCAN1_Init(void)  ;
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle);
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle);


























#endif

