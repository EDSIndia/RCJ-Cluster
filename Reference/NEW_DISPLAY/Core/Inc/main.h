/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_iwdg.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_crs.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "adc.h"
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#include "ONE_WIRE_COMM.h"
#include "APP_RAM.h"
#include "STM32_Config.h"
#include "Misc_Functions.h"
#include "FDCAN_DRIVER.h"
#include "Display_Driver.h"
#include "VOLTAGE_CAL.h"
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define BV_FACTOR                             90.0/3.3
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern uint16_t ADC_BUFFER[1];
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
  #define ONE_HIGH_TIME   70                                                    // time for high cycle of logical 1
  #define ONE_LOW_TIME    30                                                    // time for low cycle of logical 1
  #define ZERO_HIGH_TIME  30                                                    // time for high cycle of logical 0
  #define ZERO_LOW_TIME   70                                                    // time for low cycle of logical 0
 
 
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* ------------------------------ */
/*          INPUT PINS            */
/* ------------------------------ */
#define IN_LW_SENSE_Pin      LL_GPIO_PIN_13
#define IN_LW_SENSE_GPIO_Port GPIOC

#define IN_RW_SENSE_Pin      LL_GPIO_PIN_14
#define IN_RW_SENSE_GPIO_Port GPIOC

#define BUTTON_Pin           LL_GPIO_PIN_15
#define BUTTON_GPIO_Port     GPIOC

/* ------------------------------ */
/*        LCD BACKLIGHT           */
/* ------------------------------ */
#define LCD_BKL_PIN_Pin      LL_GPIO_PIN_0
#define LCD_BKL_PIN_GPIO_Port GPIOF

/* ------------------------------ */
/*           INDICATORS           */
/* ------------------------------ */
#define IN_LW_LED_Pin        LL_GPIO_PIN_1
#define IN_LW_LED_GPIO_Port  GPIOF

#define IN_RW_LED_Pin        LL_GPIO_PIN_13
#define IN_RW_LED_GPIO_Port  GPIOA

#define LOW_SOC_LED_Pin      LL_GPIO_PIN_6
#define LOW_SOC_LED_GPIO_Port GPIOB

#define BRAKE_LED_Pin        LL_GPIO_PIN_7
#define BRAKE_LED_GPIO_Port  GPIOB

/* ------------------------------ */
/*         BUZZER CONTROL         */
/* ------------------------------ */
#define BUZZER_ON_Pin        LL_GPIO_PIN_1
#define BUZZER_ON_GPIO_Port  GPIOA

/* ------------------------------ */
/*       COMMUNICATION PINS       */
/* ------------------------------ */
#define COM1_RST_Pin        LL_GPIO_PIN_3
#define COM1_RST_GPIO_Port  GPIOA

#define COM1_Pin            LL_GPIO_PIN_5
#define COM1_GPIO_Port      GPIOB

#define COM2_RST_Pin        LL_GPIO_PIN_0
#define COM2_RST_GPIO_Port  GPIOB

#define COM2_Pin            LL_GPIO_PIN_4
#define COM2_GPIO_Port      GPIOA

#define COM3_RST_Pin        LL_GPIO_PIN_2
#define COM3_RST_GPIO_Port  GPIOB

#define COM3_Pin            LL_GPIO_PIN_1
#define COM3_GPIO_Port      GPIOB

#define COM4_RST_Pin        LL_GPIO_PIN_11
#define COM4_RST_GPIO_Port  GPIOB

#define COM4_Pin            LL_GPIO_PIN_10
#define COM4_GPIO_Port      GPIOB

/* ------------------------------ */
/*        LCD SEGMENT PINS        */
/* ------------------------------ */
#define LCD_SEG1_PIN_Pin     LL_GPIO_PIN_12
#define LCD_SEG1_PIN_GPIO_Port GPIOA

#define LCD_SEG2_PIN_Pin     LL_GPIO_PIN_11
#define LCD_SEG2_PIN_GPIO_Port GPIOA

#define LCD_SEG3_PIN_Pin     LL_GPIO_PIN_10
#define LCD_SEG3_PIN_GPIO_Port GPIOA

#define LCD_SEG4_PIN_Pin     LL_GPIO_PIN_7
#define LCD_SEG4_PIN_GPIO_Port GPIOC

#define LCD_SEG5_PIN_Pin     LL_GPIO_PIN_6
#define LCD_SEG5_PIN_GPIO_Port GPIOC

#define LCD_SEG6_PIN_Pin     LL_GPIO_PIN_9
#define LCD_SEG6_PIN_GPIO_Port GPIOA

#define LCD_SEG7_PIN_Pin     LL_GPIO_PIN_8
#define LCD_SEG7_PIN_GPIO_Port GPIOA

#define LCD_SEG8_PIN_Pin     LL_GPIO_PIN_15
#define LCD_SEG8_PIN_GPIO_Port GPIOB

#define LCD_SEG9_PIN_Pin     LL_GPIO_PIN_14
#define LCD_SEG9_PIN_GPIO_Port GPIOB

#define LCD_SEG10_PIN_Pin    LL_GPIO_PIN_13
#define LCD_SEG10_PIN_GPIO_Port GPIOB

#define LCD_SEG11_Pin        LL_GPIO_PIN_12
#define LCD_SEG11_GPIO_Port  GPIOB

#define LCD_SEG20_PIN_Pin    LL_GPIO_PIN_4
#define LCD_SEG20_PIN_GPIO_Port GPIOB

#define LCD_SEG21_PIN_Pin    LL_GPIO_PIN_3
#define LCD_SEG21_PIN_GPIO_Port GPIOB

#define LCD_SEG22_PIN_Pin    LL_GPIO_PIN_3
#define LCD_SEG22_PIN_GPIO_Port GPIOD

#define LCD_SEG23_PIN_Pin    LL_GPIO_PIN_2
#define LCD_SEG23_PIN_GPIO_Port GPIOD

#define LCD_SEG24_PIN_Pin    LL_GPIO_PIN_1
#define LCD_SEG24_PIN_GPIO_Port GPIOD

#define LCD_SEG25_PIN_Pin    LL_GPIO_PIN_0
#define LCD_SEG25_PIN_GPIO_Port GPIOD

#define LCD_SEG26_PIN_Pin    LL_GPIO_PIN_15
#define LCD_SEG26_PIN_GPIO_Port GPIOA

/* ------------------------------ */
/*         CONTROL SIGNALS        */
/* ------------------------------ */
#define DI_Pin              LL_GPIO_PIN_5
#define DI_GPIO_Port        GPIOA

#define LAT_Pin             LL_GPIO_PIN_6
#define LAT_GPIO_Port       GPIOA

#define CLK_Pin             LL_GPIO_PIN_7
#define CLK_GPIO_Port       GPIOA


/* USER CODE BEGIN Private defines */
#define ODO_RST_PORT          GPIOA
#define ODO_RST_PIN           LL_GPIO_PIN_1


#define LED_LW_PORT           GPIOF
#define LED_LW_PIN            LL_GPIO_PIN_1

#define LED_RW_PORT           GPIOA  /** Debug Pin **/
#define LED_RW_PIN            LL_GPIO_PIN_13         
 

#define LW_ON           LED_LW_PORT->BSRR  = LED_LW_PIN
#define LW_OFF          LED_LW_PORT->BRR   = LED_LW_PIN
#define LW_TGL          LED_LW_PORT->ODR  ^= LED_LW_PIN

#define RW_ON           LED_RW_PORT->BSRR = LED_RW_PIN
#define RW_OFF          LED_RW_PORT->BRR  = LED_RW_PIN
#define RW_TGL          LED_RW_PORT->ODR  ^= LED_RW_PIN
 
#define BRAKE_LED_ON       BRAKE_LED_GPIO_Port->BSRR = BRAKE_LED_Pin
#define BRAKE_LED_OFF      BRAKE_LED_GPIO_Port->BRR  = BRAKE_LED_Pin
#define BRAKE_LED_TGL      BRAKE_LED_GPIO_Port->ODR ^= BRAKE_LED_Pin


#define LOW_SOC_LED_ON       LOW_SOC_LED_GPIO_Port->BSRR = LOW_SOC_LED_Pin
#define LOW_SOC_LED_OFF      LOW_SOC_LED_GPIO_Port->BRR  = LOW_SOC_LED_Pin
#define LOW_SOC_LED_TGL      LOW_SOC_LED_GPIO_Port->ODR ^= LOW_SOC_LED_Pin


#define BKL_ON  LCD_BKL_PIN_GPIO_Port-> BSRR = LCD_BKL_PIN_Pin
#define BKL_OFF LCD_BKL_PIN_GPIO_Port-> BRR = LCD_BKL_PIN_Pin



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
