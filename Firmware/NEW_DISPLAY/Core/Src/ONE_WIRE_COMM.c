/******************** (C) COPYRIGHT 2022 EDS INDIA *****************************
* File Name     : ONE_WIRE_COMM.c					       *
* Author        : SAHILALI                                                     *
* Date          : 28-3-2025                                                    *
* Description   : This file contains Motor Control receive Data Functionality  *
* Revision	: Rev0    						       *
********************************************************************************/
#include "main.h"

#include "string.h"
/* --------------------------- HOW TO USE -------------------------------------
 Note :- This Driver File based on LL.

Step1 :- Initialize the timer and add ONE_WIRE_ISR(); inside TIM2_IRQHandler().

Step2 :- in ONE_WIRE_COMM.h File you need to choose the MOTOR Controller interface type.

Step3 :- Set Timer to Input Capture Mode with both-edge polarity. For minimum frequency measurement, 
         e.g., 20 Hz: With TIMER_CLOCK = 64 MHz, 
                                  ARR  = 65535, 
            minimum readable frequency = 64 MHz / 64 = 1 MHz, 
                                       = 1 MHz / 65535
                                       =  15.2 Hz. 
        Note: input Frequency belo 15.2 hz may result inaccurate ON/OFF time .

Step4 :- in ONE_WIRE_ISR(); Change Timer and Channel based on your Configuration.

Step5 :- int State_val input function provide timer Pin and port.

*/

/* enum */
typedef_1WIRE_COMM ONE_WIRE;
uint8_t Reg[15];
 

static void ProcessFrequencyAndDutyCycle(void);
static void ProcessDataBits(void);
__STATIC_INLINE uint32_t Get_IC_Value(const TIM_TypeDef *TIMx,uint16_t channels);
__STATIC_INLINE uint32_t Get_ActiveFlag_Status(const TIM_TypeDef *TIMx,uint32_t channels);
void Clear_ActiveFlag(const TIM_TypeDef *TIMx,uint32_t channels);
int
count = 0,
State_val = 0;

uint32_t 
IC_Val1 = 0,                                                                    // First Input Capture_Val at first Falling Edge
IC_Val2 = 0,                                                                    // Second Input Capture_Val at first Rasing Edge
IC_Val3 = 0,                                                                    // Third Input Capture_Val at Second Falling Edge
Difference = 0,                                                                 // Time Differnce b/w first two input Capture Value
Difference_2=0,                                                                 // Time Differnce b/w Second and third input Capture Value
Difference_3=0;                                                                // Index 0-4 for 5 byte speed buffer

FlagStatus
Flag_startbit;
float refClock = 0;
uint16_t frequency;

uint8_t 
bit_index = 0,
data_index = 0,
sum = 0,
current_byte = 0,
checksum = 0;

int
On_time,
Off_time;

  /*initialize variable */
  uint8_t first_high = 0;
  uint8_t StartBit_Frequency = 0;
  

/*******************************************************************************
* Function Name  : ONE_WIRE_ISR
* Description    : This function is used as a TIM interrupt service routine 
                   (must be called inside the Timer interrupt handler).
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ONE_WIRE_ISR(TIM_TypeDef *TIMx , uint32_t Channel)
{
  if (Get_ActiveFlag_Status(TIMx,Channel)) {
    
    /* Clear the active Flag */
    Clear_ActiveFlag(TIMx,Channel);
    
    /* Check the EDGE of pulse */
    State_val = HAL_GPIO_ReadPin(TIMER_PORT, TIMER_PIN);
    
#if defined WABOUR
    first_high = 1;  /* First state transition is HIGH */
    StartBit_Frequency = 50;
#elif defined (EMF) || defined (CHINESE)
    first_high = 0;  /* First state transition is LOW */
    StartBit_Frequency = 35;
#elif defined (ANAND_MINDO)
    first_high = 0;  /* First state transition is LOW */
    StartBit_Frequency = 65; 
#elif defined (CHINESE)
    first_high = 0;  /* First state transition is LOW */
    StartBit_Frequency = 5; 
#endif
    
    /* Check First EDGE */
    if (State_val == first_high && count == 0) {IC_Val1 = Get_IC_Value(TIMx,Channel);count++;}
    
    /* Check Second EDGE */
    else if (State_val != first_high && count == 1) {IC_Val2 = Get_IC_Value(TIMx,Channel);count++;}
    
    /* Check Third EDGE */
    else if (State_val == first_high && count == 2) {IC_Val3 = Get_IC_Value(TIMx,Channel);count = 1;
    
    /* extract Frequency ON/OFF Time*/
      ProcessFrequencyAndDutyCycle();
      
      /* Check start bit frequency */
      if (frequency < StartBit_Frequency && Flag_startbit == RESET) {Flag_startbit = SET;}
      
      /* if start bit frequency match then process data bits*/
      else if (Flag_startbit == SET) {ProcessDataBits();}
    }
  }
  else
  {
        /* Clear the active Flag */
    Clear_ActiveFlag(TIMx,Channel);
  }
}



/*******************************************************************************
* Function Name  : ProcessFrequencyAndDutyCycle
* Description    : This function calculates the frequency, ON time, and OFF time 
                   based on input capture values from the timer.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProcessFrequencyAndDutyCycle(void) 
{
  /* Topic :- ((ARR - IC_Val1) + IC_Val3 + 1)


  0 1 2 3 4 5 6 7 8   ->  suppot arr == 8           -> (8 - v1) + v3 + 1 
  3     2   1         v1 = 7 v2 = 5 v3 = 2             (8 - 7)  + 2  + 1 ->  1 + 2 + 1 = 4 Ans
  
  you can see here 
  arr - 4 = 4
  if i count 4 from 0 it will be 3
  */
 
    /* checking the input capture difference */
    Difference_3 = (IC_Val3 >= IC_Val1) ? (IC_Val3 - IC_Val1) : ((ARR - IC_Val1) + IC_Val3 + 1);
    
    /* extract Frequency */
    refClock     = (TIMCLOCK / PRESCALAR);
    frequency    = (uint16_t)(refClock / Difference_3);

    /* extract off time */
    Difference   = (IC_Val2 >= IC_Val1) ? (IC_Val2 - IC_Val1) : ((ARR - IC_Val1) + IC_Val2 + 1);
    Off_time     = (int)((Difference * 0.000001) * frequency * 100);

    /* extract on time */
    Difference_2 = (IC_Val3 >= IC_Val2) ? (IC_Val3 - IC_Val2) : ((ARR - IC_Val2) + IC_Val3 + 1);
    On_time      = (int)((Difference_2 * 0.000001) * frequency * 100);

    IC_Val1      = IC_Val3;
}


/*******************************************************************************
* Function Name  : ProcessDataBits
* Description    : This function processes data bits by analyzing ON/OFF time 
                   and calculates the checksum for one-wire communication.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProcessDataBits(void) 
{
  /* Checking High Bit */
  if (Off_time < 50 && On_time > 50) {current_byte = (current_byte << 1) | 0x01;} 
  
  /* Check Low Bit */
  else if (Off_time > 50 && On_time < 50) {current_byte <<= 1;}
  
  /* increment bit index */
  bit_index++;
  
  /* Check 8 bit collect or not */
  if (bit_index == 8) 
  {
    /* store 8 bit data into buffer index */
    ONE_WIRE.data[data_index++] = current_byte;
    
    /* Clear variable to restore the data */
    bit_index = 0;
    current_byte = 0;
    
    /* Check 12 byte receive or not */
    if (data_index > 11) {
      sum = 0;
      
      /* Calculate the Checksum */
      for (int i = 0; i < 11; i++) {sum ^= ONE_WIRE.data[i];}
      checksum = sum - ONE_WIRE.data[11];
      
      /*checksum receive checksum and calculated checksum difference are zero or not */
      if (!checksum) 
      {
#if defined (WABOUR)  || defined (CHINESE)           
        memcpy(Reg, ONE_WIRE.data, sizeof(Reg));
        
#elif defined (EMF)   || defined (ANAND_MINDO)
        Reg[0]=ONE_WIRE.data[0];
        Reg[1]=ONE_WIRE.data[1];
        Reg[2]=ONE_WIRE.data[2];
        Reg[3]=(ONE_WIRE.data[3]-(0x13));
        Reg[4]=(ONE_WIRE.data[4]-(0x13)); 
        Reg[5]=(ONE_WIRE.data[5]-(0x13)); 
        Reg[6]=(ONE_WIRE.data[6]-(0x13));
        Reg[7]=(ONE_WIRE.data[7]-(0x13));
        Reg[8]=(ONE_WIRE.data[8]-(0x13));
        Reg[9]=(ONE_WIRE.data[9]-(0x13));
        Reg[10]=(ONE_WIRE.data[10]-(0x13));
#endif
      } 
      else {memset(Reg, 0, sizeof(Reg));}
      
      /* Make index zero and reset start bit flag to store new data */
      data_index = 0;
      Flag_startbit = RESET;
    }
  }
}

/*******************************************************************************
* Function Name  : ProcessDataBits
* Description    : This function processes data bits by analyzing ON/OFF time 
                   and calculates the checksum for one-wire communication.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__STATIC_INLINE uint32_t Get_IC_Value(const TIM_TypeDef *TIMx,uint16_t channels)
{
  uint32_t Get_Value = 0;
  switch(channels)
  {
  case LL_TIM_CHANNEL_CH1: Get_Value= (READ_REG(TIMx->CCR1));break;
  case LL_TIM_CHANNEL_CH2: Get_Value= (READ_REG(TIMx->CCR2));break;
  case LL_TIM_CHANNEL_CH3: Get_Value= (READ_REG(TIMx->CCR3));break;
  case LL_TIM_CHANNEL_CH4: Get_Value= (READ_REG(TIMx->CCR4));break;
  case LL_TIM_CHANNEL_CH5: Get_Value= (READ_REG(TIMx->CCR5));break;
  case LL_TIM_CHANNEL_CH6: Get_Value= (READ_REG(TIMx->CCR6));break;
  }
  return Get_Value;
}



/*******************************************************************************
* Function Name  : Get_ActiveFlag_Status
* Description    : This function checks the status of the capture/compare 
                  interrupt flag (CCxIF) for the specified timer channel.
                  It returns 1 if the flag is set, otherwise returns 0.
* Input          : TIMx     - Pointer to the timer peripheral (TIM_TypeDef)
                  channels - Specifies the timer channel (LL_TIM_CHANNEL_CHx)
* Output         : None
* Return         : 1UL if the flag is active, 0UL otherwise
*******************************************************************************/
__STATIC_INLINE uint32_t Get_ActiveFlag_Status(const TIM_TypeDef *TIMx,uint32_t channels)
{
  switch(channels)
  {
  case LL_TIM_CHANNEL_CH1:
    {
      return ((READ_BIT(TIMx->SR, TIM_SR_CC1IF) == (TIM_SR_CC1IF)) ? 1UL : 0UL);
    }
    break;
  case LL_TIM_CHANNEL_CH2:
    {
      return ((READ_BIT(TIMx->SR, TIM_SR_CC2IF) == (TIM_SR_CC2IF)) ? 1UL : 0UL);
    }
    break;
  case LL_TIM_CHANNEL_CH3:
    {
      return ((READ_BIT(TIMx->SR, TIM_SR_CC3IF) == (TIM_SR_CC3IF)) ? 1UL : 0UL);
    }
    break;
  case LL_TIM_CHANNEL_CH4:
    {
      return ((READ_BIT(TIMx->SR, TIM_SR_CC4IF) == (TIM_SR_CC4IF)) ? 1UL : 0UL);
    }
    break;
  case LL_TIM_CHANNEL_CH5:
    {
      return ((READ_BIT(TIMx->SR, TIM_SR_CC5IF) == (TIM_SR_CC5IF)) ? 1UL : 0UL);
    }
    break;
  case LL_TIM_CHANNEL_CH6:
    {
      return ((READ_BIT(TIMx->SR, TIM_SR_CC6IF) == (TIM_SR_CC6IF)) ? 1UL : 0UL);
    }
    break;
  }
  return 0;
}
 

/*******************************************************************************
* Function Name  : Clear_ActiveFlag
* Description    : This function clears the capture/compare interrupt flag (CCxIF)
                  for the specified timer channel in the given timer instance.
* Input          : TIMx     - Pointer to the timer peripheral (TIM_TypeDef)
                  channels - Specifies the timer channel (LL_TIM_CHANNEL_CHx) 
* Output         : None
* Return         : None
*******************************************************************************/
void Clear_ActiveFlag(const TIM_TypeDef *TIMx, uint32_t channels) 
{
    TIM_TypeDef *Timer = (TIM_TypeDef *)TIMx; // Cast away const (not ideal)
    switch (channels)  
    {
        case LL_TIM_CHANNEL_CH1: LL_TIM_ClearFlag_CC1(Timer); break;
        case LL_TIM_CHANNEL_CH2: LL_TIM_ClearFlag_CC2(Timer); break;
        case LL_TIM_CHANNEL_CH3: LL_TIM_ClearFlag_CC3(Timer); break;
        case LL_TIM_CHANNEL_CH4: LL_TIM_ClearFlag_CC4(Timer); break;
        case LL_TIM_CHANNEL_CH5: LL_TIM_ClearFlag_CC5(Timer); break;
        case LL_TIM_CHANNEL_CH6: LL_TIM_ClearFlag_CC6(Timer); break;
        default: break;
    }
}




void Timer_ISR_Handler(TIM_TypeDef *TIMx)
{
  {
   if (LL_TIM_IsActiveFlag_CC1(TIMx))                                           //Wait for Timer 2,Channel 3 Interrupt
   {
  
      LL_TIM_ClearFlag_CC1(TIMx);                                                // Clear IC_flag
      
      
      
    State_val = HAL_GPIO_ReadPin(TIMER_PORT, TIMER_PIN);     
      
      
      
      if((State_val==1)&&(count==0))                                             //Read Timer at First Falling Edge                                           
       {
         IC_Val1 = LL_TIM_IC_GetCaptureCH1(TIMx);                               //                          
         count++;
       }
      else if((State_val==0)&&(count==1))                                        //Read Timer at First Raising Edge                                      
       {
         IC_Val2 = LL_TIM_IC_GetCaptureCH1(TIMx);  
         count++;
       }
      else if ((State_val==1)&&(count==2))                                      // Read Timer at Second Falling Edge                             
       {
         IC_Val3 = LL_TIM_IC_GetCaptureCH1(TIMx);                                                                                 
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
                   
                   if((Off_time<((100) / 2))&&(On_time>((100) / 2))) //Check Condition against duty cycle for read 1 / 0 Bit
                   {
                          current_byte <<= 1;                                                        
                                                                    current_byte |= 0x01;
                    //Set Bit-1                   
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
                                      Reg_0=data[0]; Reg_1=data[1]; Reg_2=data[2];
                                      Reg_3=data[3]; Reg_4=data[4]; Reg_5=data[5];
                                      Reg_6=data[6]; Reg_7=data[7]; Reg_8=data[8];
                                      Reg_9=data[9]; Reg_10=data[10];
                                      }	
                                      else 
                                      {
                                       Reg_0=0; Reg_1=0; Reg_2=0; Reg_3=0;
                                       Reg_4=0; Reg_5=0; Reg_6=0; Reg_7=0;
                                       Reg_8=0; Reg_9=0; Reg_10=0;
                                      }
                                      
        
                                      data_index = 0;
                                      Flag_startbit=RESET;
                                   }
                             } 
                       }
                    else if((Off_time>((100) / 2))&&(On_time<((100) / 2)))//Check Condition against duty cycle for read 1 / 0 Bit
                     {
                            current_byte <<= 1;                                 // Set Bit-0                    
                                                                                // current_byte |= 0x01;
                                     bit_index++;

                            if(bit_index == 8)                                  // Check Index 8 Bit/1 Byte Complete
                              {
                                data[data_index] = current_byte;
                                data_index++;
                                bit_index = 0;
                                current_byte = 0;
                            
                                if(data_index>11)
                                  { 
                                    sum=0;
                                    for(int i=0;i<11;i++)
                                    {sum^=data[i];}
                                    
                                    checksum=(sum-data[11]) ;    
                                    if(checksum==0)                             // Verify Checksum
                                    {
                                      Reg_0=data[0]; Reg_1=data[1]; Reg_2=data[2];
                                      Reg_3=data[3]; Reg_4=data[4]; Reg_5=data[5];
                                      Reg_6=data[6]; Reg_7=data[7]; Reg_8=data[8];
                                      Reg_9=data[9]; Reg_10=data[10];
                                            
                                      
                                      
//                                       Reg_0=data[0]; Reg_1=data[1]; Reg_2=data[2]; Reg_3=(data[3]-(0x13));
//                                       Reg_4=(data[4]-(0x13)); Reg_5=(data[5]-(0x13)); Reg_6=(data[6]-(0x13)); Reg_7=(data[7]-(0x13));
//                                       Reg_8=(data[8]-(0x13)); Reg_9=(data[9]-(0x13)); Reg_10=(data[10]-(0x13));
                                    }	
                                    else 
                                    {
                                     Reg_0=0; Reg_1=0; Reg_2=0; Reg_3=0;
                                     Reg_4=0; Reg_5=0; Reg_6=0; Reg_7=0;
                                     Reg_8=0; Reg_9=0; Reg_10=0;
                                    }
                                    data_index = 0;
                                    Flag_startbit=RESET;
                                  }
                              }
                       }
                 }
            }
     
//       else
//       {
//       __NOP();
//       }
       }
   }

}



  
  
}




