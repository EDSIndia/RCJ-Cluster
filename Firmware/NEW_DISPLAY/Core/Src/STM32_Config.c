/******************************************************************************
  * File Name          : STM32_Config.h
  * Description        : This file provides code for the configuration MCU
  * Author             : Santanu Roy
  * Date               : 25th Jan 2022
********************************************************************************/

#include "main.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* LSI configuration and activation */
  LL_RCC_LSI_Enable();
  while(LL_RCC_LSI_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
  
//  /* Configure SysTick for 1 ms interrupt (assuming 64 MHz clock) */
//  SysTick_Config((64000) - 1);
//  NVIC_SetPriority(SysTick_IRQn, 0);
//  NVIC_EnableIRQ(SysTick_IRQn);
//
//   /* Update the time base */
//  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
//  {
//    Error_Handler();
//  }
}


/** 
*@brief   The configuration of the TIM instances.
*/

void MX_TIM2_Init(void)
{
  
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**TIM2 GPIO Configuration
  PA2   ------> TIM2_CH3
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;                                          // Data Capture Pin
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;                               // FREQ. LOW
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;                                       // PULL UP
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);

  TIM_InitStruct.Prescaler = 64-1;                                              // 64/64=1MHZ                     
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = ARR;                                              // 1000,000 Hz /65535=15.25 Hz Minimum Capture Freq.          
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);                             // TRIGO RESET
  LL_TIM_DisableMasterSlaveMode(TIM2);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_IC_POLARITY_BOTHEDGE);

  
    LL_TIM_EnableCounter(TIM2);                                                   //Start the Timer Counting
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);                            //Channel 3 Enable of Timer 2 for capture
  LL_TIM_EnableIT_CC3(TIM2);                                                    //Enables the interrupt for Timer 2, Channel 3.
  
}



/** Configure pins as
         * Input
        * Output
*/
void MX_GPIO_Init(void)
{
 LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);
 
  // LCD COMMON
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
   GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
 
  GPIO_InitStruct.Pin = LCD_COM1_PIN;LL_GPIO_Init(LCD_COM1_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_COM2_PIN;LL_GPIO_Init(LCD_COM2_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_COM3_PIN;LL_GPIO_Init(LCD_COM3_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_COM4_PIN;LL_GPIO_Init(LCD_COM4_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin = LCD_COM1N_PIN;LL_GPIO_Init(LCD_COM1N_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_COM2N_PIN;LL_GPIO_Init(LCD_COM2N_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_COM3N_PIN;LL_GPIO_Init(LCD_COM3N_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_COM4N_PIN;LL_GPIO_Init(LCD_COM4N_PORT, &GPIO_InitStruct);
   
  // LCD Segment
  GPIO_InitStruct.Pin = LCD_SEG17_PIN; LL_GPIO_Init(LCD_SEG17_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG18_PIN;LL_GPIO_Init(LCD_SEG18_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG19_PIN;LL_GPIO_Init(LCD_SEG19_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG20_PIN;LL_GPIO_Init(LCD_SEG20_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG21_PIN;LL_GPIO_Init(LCD_SEG21_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG22_PIN;LL_GPIO_Init(LCD_SEG22_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG23_PIN;LL_GPIO_Init(LCD_SEG23_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG24_PIN;LL_GPIO_Init(LCD_SEG24_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG25_PIN;LL_GPIO_Init(LCD_SEG25_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LCD_SEG26_PIN;LL_GPIO_Init(LCD_SEG26_PORT, &GPIO_InitStruct);
   GPIO_InitStruct.Pin = LCD_SEG27_PIN;LL_GPIO_Init(LCD_SEG27_PORT, &GPIO_InitStruct);
  
  /**/
  GPIO_InitStruct.Pin = IN_LW_SENSE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(IN_LW_SENSE_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = IN_RW_SENSE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(IN_RW_SENSE_GPIO_Port, &GPIO_InitStruct);
 
  /**/
  GPIO_InitStruct.Pin = LCD_BKL_PIN_Pin ;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LCD_BKL_PIN_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =  HL_SENSE_Pin ;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LCD_BKL_PIN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_LW_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_LW_PORT, &GPIO_InitStruct);

//  /**/
//  GPIO_InitStruct.Pin = ODO_RST_PIN;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
//  LL_GPIO_Init(ODO_RST_PORT, &GPIO_InitStruct);
// 
//  /**/
  GPIO_InitStruct.Pin = DI_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DI_GPIO_Port, &GPIO_InitStruct);
  
  
/* ------------------------------ */
/*        EXTRA LED PINS          */
/* ------------------------------ */

   
  GPIO_InitStruct.Pin = ORAN_LED_Pin;LL_GPIO_Init(ORAN_LED_Pin_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = RED_LED_Pin;LL_GPIO_Init(RED_LED_Pin_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = YLW_LED_Pin;LL_GPIO_Init(YLW_LED_Pin_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GRN_LED_Pin;LL_GPIO_Init(GRN_LED_Pin_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LOGO_LED_Pin;LL_GPIO_Init(LOGO_LED_Pin_Port, &GPIO_InitStruct);
   

  /**/
  GPIO_InitStruct.Pin = LAT_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LAT_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CLK_Pin| CAN_STB;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CLK_GPIO_Port, &GPIO_InitStruct);

//  /**/
  GPIO_InitStruct.Pin = LED_RW_PIN | HL_Pin;                                           // debug pin
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_RW_PORT, &GPIO_InitStruct);
   
//  /**/
////  GPIO_InitStruct.Pin = LOW_SOC_LED_Pin;
////  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
////  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
////  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
////  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
////  LL_GPIO_Init(LOW_SOC_LED_GPIO_Port, &GPIO_InitStruct);
////
////  /**/
//  GPIO_InitStruct.Pin = BRAKE_LED_Pin;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//  LL_GPIO_Init(BRAKE_LED_GPIO_Port, &GPIO_InitStruct);
//
//  
  
//  /**/
//  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTC, LL_EXTI_CONFIG_LINE15);
//
//  /**/
//  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_15;
//  EXTI_InitStruct.LineCommand = ENABLE;
//  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
//  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
//  LL_EXTI_Init(&EXTI_InitStruct);
//
//  /**/
//  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_15, LL_GPIO_PULL_UP);
//
//  /**/
//  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_15, LL_GPIO_MODE_INPUT);
//
//  /* EXTI interrupt init*/
//  NVIC_SetPriority(EXTI4_15_IRQn, 5);
//  NVIC_EnableIRQ(EXTI4_15_IRQn);
}



/**
  * Enable ADC controller 
  */

ADC_HandleTypeDef hadc1;
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
 
  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_IN0
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}
                   

/**
  * Enable IWDG controller 
  */
void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);
  LL_IWDG_SetReloadCounter(IWDG, 4095);
  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }

  LL_IWDG_ReloadCounter(IWDG);
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

void Configure_Buzzer(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = BUZZER_ON_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(BUZZER_ON_GPIO_Port, &GPIO_InitStruct);
}



FDCAN_HandleTypeDef hfdcan1;

/* FDCAN1 init function */
void MX_FDCAN1_Init(void)                                                       //64MHZ,500KBPS_/2=250KBPS,5000,200ns
{

  hfdcan1.Instance                  = FDCAN1;
  hfdcan1.Init.ClockDivider         = FDCAN_CLOCK_DIV1;                         //64MHZ FDCAN Peripheral Clock
  hfdcan1.Init.FrameFormat          = FDCAN_FRAME_FD_NO_BRS;
  hfdcan1.Init.Mode                 = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission   = ENABLE;
  hfdcan1.Init.TransmitPause        = ENABLE;
  hfdcan1.Init.ProtocolException    = DISABLE;
  hfdcan1.Init.NominalPrescaler     = 1;                                        //1_500kbps AND 2_250kbps
  hfdcan1.Init.NominalSyncJumpWidth = 17;
  hfdcan1.Init.NominalTimeSeg1      = 110;
  hfdcan1.Init.NominalTimeSeg2      = 17;
  hfdcan1.Init.DataPrescaler        = 1;                                        //1_500kbps AND 2_250kbps
  hfdcan1.Init.DataSyncJumpWidth    = 63;
  hfdcan1.Init.DataTimeSeg1         = 64;
  hfdcan1.Init.DataTimeSeg2         = 63;
  hfdcan1.Init.StdFiltersNbr        = 0;
  hfdcan1.Init.ExtFiltersNbr        = 0;
  hfdcan1.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* USER CODE BEGIN FDCAN1_Init 2 */
  /* USER CODE END FDCAN1_Init 2 */
  
}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */
    LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PCLK1);

    /* FDCAN1 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PB8     ------> FDCAN1_RX
    PB9     ------> FDCAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_FDCAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(TIM16_FDCAN_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM16_FDCAN_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN1 GPIO Configuration
    PB8     ------> FDCAN1_RX
    PB9     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM16_FDCAN_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
}