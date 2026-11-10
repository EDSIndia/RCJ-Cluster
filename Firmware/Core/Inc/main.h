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
#include "adc.h"
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
  /* ----------------------------------------------------- */
  /*          INPUT PINS                                   */
  /* ----------------------------------------------------- */
#define IN_LW_SENSE_Pin                  LL_GPIO_PIN_9
#define IN_LW_SENSE_GPIO_Port            GPIOA
  
#define IN_RW_SENSE_Pin                  LL_GPIO_PIN_8
#define IN_RW_SENSE_GPIO_Port             GPIOA
  
#define HL_SENSE_Pin                     LL_GPIO_PIN_14
#define HL_SENSE_GPIO_Port               GPIOB
  
  //#define BUTTON_Pin           LL_GPIO_PIN_15
  //#define BUTTON_GPIO_Port     GPIOC
  
  /* ----------------------------------------------------- */
  /*        LCD BACKLIGHT                                  */
  /* ----------------------------------------------------- */
#define LCD_BKL_PIN_Pin                  LL_GPIO_PIN_15
#define LCD_BKL_PIN_GPIO_Port            GPIOB
  
  /* ----------------------------------------------------- */
  /*           INDICATORS                                  */
  /* ----------------------------------------------------- */
#define LED_LW_PIN                      LL_GPIO_PIN_6
#define LED_LW_PORT                     GPIOC
  
#define LED_RW_PIN                      LL_GPIO_PIN_4
#define LED_RW_PORT                     GPIOA
  
#define HL_Pin                          LL_GPIO_PIN_1
#define HL_GPIO_Port                    GPIOA
  
  /* ----------------------------------------------------- */
  /*         BUZZER CONTROL                                */
  /* ----------------------------------------------------- */
#define BUZZER_ON_Pin                   LL_GPIO_PIN_10
#define BUZZER_ON_GPIO_Port             GPIOB
  
  /* ------------------------------- */
  /* USER CODE BEGIN Private defines */
  /* ------------------------------- */
#define ODO_RST_PORT         GPIOA
#define ODO_RST_PIN          LL_GPIO_PIN_14
  
#define LOGO_ON              LOGO_LED_Pin_Port->BSRR  = LOGO_LED_Pin
#define LOGO_OFF             LOGO_LED_Pin_Port->BRR   = LOGO_LED_Pin  
  
#define HL_ON                HL_GPIO_Port->BSRR  = HL_Pin
#define HL_OFF               HL_GPIO_Port->BRR   = HL_Pin  
  
#define BUZZER_ON            BUZZER_ON_GPIO_Port->BSRR  = BUZZER_ON_Pin
#define BUZZER_OFF           BUZZER_ON_GPIO_Port->BRR   = BUZZER_ON_Pin  
  
#define LW_ON                LED_LW_PORT->BSRR  = LED_LW_PIN
#define LW_OFF               LED_LW_PORT->BRR   = LED_LW_PIN
#define LW_TGL               LED_LW_PORT->ODR  ^= LED_LW_PIN
  
#define RW_ON                LED_RW_PORT->BSRR = LED_RW_PIN
#define RW_OFF               LED_RW_PORT->BRR  = LED_RW_PIN
#define RW_TGL               LED_RW_PORT->ODR  ^= LED_RW_PIN
  
#define BRAKE_LED_ON         RED_LED_Pin_Port->BSRR = RED_LED_Pin
#define BRAKE_LED_OFF        RED_LED_Pin_Port->BRR  = RED_LED_Pin
#define BRAKE_LED_TGL        RED_LED_Pin_Port->ODR ^= RED_LED_Pin
  
#define CHRG_ON              GRN_LED_Pin_Port->BSRR = GRN_LED_Pin
#define CHRG_OFF             GRN_LED_Pin_Port->BRR  = GRN_LED_Pin
  
//#define BRAKE_LED_TGL        RED_LED_Pin_Port->ODR ^= RED_LED_Pin  
  
#define MOTOR_ON              ORAN_LED_Pin_Port->BSRR = ORAN_LED_Pin
#define MOTOR_OFF             ORAN_LED_Pin_Port->BRR  = ORAN_LED_Pin
  
#define THROTTLE_ON           YLW_LED_Pin_Port->BSRR = YLW_LED_Pin
#define THROTTLE_OFF          YLW_LED_Pin_Port->BRR  = YLW_LED_Pin
  
#define LOW_SOC_LED_ON        LOW_SOC_LED_GPIO_Port->BSRR = LOW_SOC_LED_Pin
#define LOW_SOC_LED_OFF       LOW_SOC_LED_GPIO_Port->BRR  = LOW_SOC_LED_Pin
#define LOW_SOC_LED_TGL       LOW_SOC_LED_GPIO_Port->ODR ^= LOW_SOC_LED_Pin
  
  
#define BKL_ON                LCD_BKL_PIN_GPIO_Port-> BSRR = LCD_BKL_PIN_Pin
#define BKL_OFF               LCD_BKL_PIN_GPIO_Port-> BRR = LCD_BKL_PIN_Pin
  
  
  /* ------------------------------ */
  /*        EXTRA LED PINS          */
  /* ------------------------------ */
  
#define ORAN_LED_Pin           LL_GPIO_PIN_0
#define ORAN_LED_Pin_Port      GPIOB
  
#define RED_LED_Pin            LL_GPIO_PIN_1
#define RED_LED_Pin_Port       GPIOB
  
#define YLW_LED_Pin            LL_GPIO_PIN_11
#define YLW_LED_Pin_Port       GPIOB
  
#define GRN_LED_Pin            LL_GPIO_PIN_12
#define GRN_LED_Pin_Port       GPIOB
  
#define LOGO_LED_Pin           LL_GPIO_PIN_2
#define LOGO_LED_Pin_Port      GPIOB
  
#define CAN_STB                LL_GPIO_PIN_3
#define CAN_STB_Port           GPIOA
  
  /* USER CODE END Private defines */
  
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
