/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern FDCAN_HandleTypeDef hfdcan1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  Flag_mSec = SET;
  
  //if(!Flag_BatteryLow)                                                           
  
    Ticks++;
    if((Ticks % 2) == 0){Flag_Display_Update = SET;}
    if((Ticks % 100) == 0)Flag_Send_Frame = SET;
    if((Ticks % 499) == 0)Flag_TGL ^= 1;
    if(Ticks > 999)
    {
      Ticks = 0;
      Flag_Second = SET;
      if(wait_Time)wait_Time--;
      if(Init_Time)Init_Time--;
    }
 
  HAL_IncTick();
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */

void TIM2_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_CC3(TIM2))                                           //Wait for Timer 2,Channel 3 Interrupt
  {
    LL_TIM_ClearFlag_CC3(TIM2);                                                // Clear IC_flag
    
    State_val=LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_2);                     //Check Current Stage Level                  
    
    if((State_val==1)&&(count==0))                                             //Read Timer at First Falling Edge                                           
    {
      IC_Val1 = LL_TIM_IC_GetCaptureCH3(TIM2);                               //                          
      count++;
    }
    else if((State_val==0)&&(count==1))                                        //Read Timer at First Raising Edge                                      
    {
      IC_Val2 = LL_TIM_IC_GetCaptureCH3(TIM2);  
      count++;
    }
    else if ((State_val==1)&&(count==2))                                      // Read Timer at Second Falling Edge                             
    {
      IC_Val3 = LL_TIM_IC_GetCaptureCH3(TIM2);                                                                                 
      count=1;        
      
      if (IC_Val3 > IC_Val1){Difference_3 = IC_Val3-IC_Val1;}                         // Checking IC_Val1 is less than or greater
      else if (IC_Val1 > IC_Val3){Difference_3 = ((ARR - IC_Val1) + IC_Val3)+1;}       
      
      refClock = ((TIMCLOCK)/(PRESCALAR));                                   // 1mhz                             
      
      frequency  = (uint16_t)(refClock / Difference_3);                                                                               // Get Frequency/Cycle in Hz    
      
      if (IC_Val2 > IC_Val1){Difference = IC_Val2-IC_Val1;}
      else if (IC_Val1 > IC_Val2){Difference =((ARR - IC_Val1) + IC_Val2)+1;}
      
      Off_time = (int)((Difference*0.000001) * frequency*100) ;               // Off Duty Cycle              
      
      if (IC_Val3 > IC_Val2){Difference_2 = IC_Val3-IC_Val2;}
      else if (IC_Val2 > IC_Val3){Difference_2 =((ARR - IC_Val2) + IC_Val3)+1;}
      
      On_time = (int)((Difference_2*0.000001) * frequency*100) ;              // On Duty Cycle          
      
      IC_Val1=IC_Val3;// Assign value 3 to value 1 because we are doing value high and low between val 3 and val 1         
      
      if((frequency<50)&&(Flag_startbit==RESET))                              
      {                              //Wait for Start Bit 35 frequency
        Flag_startbit=SET;
      }           //28hz-36.8ms
      else// if(frequency > 150)
      {
        
        if(Flag_startbit==SET)//
        {
          if((Off_time<((ONE_HIGH_TIME + ONE_LOW_TIME) / 2))&&(On_time>((ONE_HIGH_TIME + ONE_LOW_TIME) / 2))) //Check Condition against duty cycle for read 1 / 0 Bit
          {
            current_byte <<= 1;                                                        
            current_byte |= 0x01;//Set Bit-1                   
            bit_index++;
            
            if(bit_index == 8)                                     // After 8 bit of data it will reset to then repeatedly store value from to zero
            {
              data[data_index] = current_byte;
              data_index++;
              bit_index = 0;
              current_byte = 0;
              
              if(data_index>11)
              {
                sum=0;
                for(int i=0;i<11;i++){sum^=data[i];} 
                
                checksum=(sum-data[11]) ;   
                if(checksum==0)                           // Verify Checksum
                {
                  for( uint8_t i =0; i < sizeof(data); i++){ONE_WIRE.Reg[i]=data[i];}
                }	
                else 
                {
                  memset(ONE_WIRE.Reg,0,sizeof(ONE_WIRE.Reg));
                }
                
                
                data_index = 0;
                Flag_startbit=RESET;
              }
            } 
          }
          else if((Off_time>((ONE_HIGH_TIME + ONE_LOW_TIME) / 2))&&(On_time<((ONE_HIGH_TIME + ONE_LOW_TIME) / 2)))//Check Condition against duty cycle for read 1 / 0 Bit
          {
            current_byte <<= 1;                                                 // Set Bit-0                    
             bit_index++;
            
            if(bit_index == 8)                                                  // Check Index 8 Bit/1 Byte Complete
            {
              data[data_index] = current_byte;
              data_index++;
              bit_index = 0;
              current_byte = 0;
              
              if(data_index>11)
              { 
                sum=0;
                for(int i=0;i<11;i++){sum^=data[i];}
                checksum=(sum-data[11]) ;    
                if(checksum==0)                                                 // Verify Checksum
                {
                  for( uint8_t i =0; i < sizeof(data); i++){ONE_WIRE.Reg[i]=data[i];}
                }	
                else 
                {
                  memset(ONE_WIRE.Reg,0,sizeof(ONE_WIRE.Reg));
                }
                
                data_index = 0;
                Flag_startbit=RESET;
              }
            }
          }
        }
      }
    }
  }
  
}





/**
  * @brief This function handles TIM16, FDCAN1_IT0 and FDCAN2_IT0 Interrupt.
  */
void TIM16_FDCAN_IT0_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_FDCAN_IT0_IRQn 0 */

  /* USER CODE END TIM16_FDCAN_IT0_IRQn 0 */
  HAL_FDCAN_IRQHandler(&hfdcan1);
  /* USER CODE BEGIN TIM16_FDCAN_IT0_IRQn 1 */

  /* USER CODE END TIM16_FDCAN_IT0_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void EXTI4_15_IRQHandler(void)
{
  /* check the falling interrupt */
  if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_15) != RESET)
  {
    LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_15);
 
    /* increase the button press Count */
    Button_Pressed++;
    if(Button_Pressed > 3)Button_Pressed = 0;
  }
  
}

/* USER CODE END 1 */
