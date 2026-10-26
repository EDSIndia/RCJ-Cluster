/******************** (C) COPYRIGHT 2022 EDS INDIA *****************************
* File Name     : Misc_Functions.c					       *
* Author        : SAHIL ALI                                                    *
* Date          : 05-02-2022                                                   *
* Description   : This file contains all misc functions                        *
* Revision	: Rev0    						       *
********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

  uint32_t  FlashAddress = 0;
  uint64_t FlashData = 0;
/*******************************************************************************
* Function Name  : mSec_Delay
* Description    : Insert Delay in mili Seconds
* Input          : mili seconds 
* Output         : None
* Return         : None
*******************************************************************************/
void mSec_Delay(unsigned int nTime)
{
if(!Flag_BatteryLow)
  {   
  while(nTime != 0)                                                             // MILI SECOND DELAY 
  { 
    if(Flag_mSec == SET){Flag_mSec = RESET;nTime--;};                           // MILI SECOND FLAG IS SET BY SYSTICK TIMER 
    if(Flag_BatteryLow){ break;}
  };
  }
}
  

/*******************************************************************************
* Function Name  : GP_Timer
* Description    : Second Activities
* Input          : mili seconds 
* Output         : None
* Return         : None
*******************************************************************************/
void GP_Timer(void)
{
  Flag_Second = RESET;
  
  /* RELOAD WATCHDOG */
  LL_IWDG_ReloadCounter(IWDG);
  
  /* init-time for Odo reset functionality */
  if(Init_Time)Init_Time--;    
  
  /* Calculate Odo-Meter */
  Cal_odo(); 
 
//  if(Flag_parking)                                                                
//  {
//    if((Flag_revgear)&&(ONE_WIRE.Reg[8]!=0))                                                 //Reverse Gear with Speed   
//      Flag_revspeed=SET; 
//    Vehicle.SPEED = speed;
//    if(Old_Speed != speed){Disp_Speed(Vehicle.SPEED);}                                      //Display Vehicle _Speed(Km/h)              
//    if(Flag_TGL){LW_ON;RW_ON;}else {LW_OFF;RW_OFF;}
//  } 
//  else 
//  { 
//    if((!Flag_revgear)&&(!Flag_parking)){Disp_Speed(Vehicle.SPEED);}                         //Display Vehicle _Speed(Km/h)
//    if((Flag_revgear)&&(ONE_WIRE.Reg[8]==0))                                              //Reverse Gear without Speed,stable stage
//    {Flag_revspeed=RESET;LCD_Reg[0] &= ~(0x31F000);                          
//    }
//  }
}




/*******************************************************************************
* Function Name  : Write_OdoMeter
* Description    : Write The OdoMeter
* Output         : None
* Return         : None
*******************************************************************************/
void Write_OdoMeter(void)
{
  Old_ODO = Vehicle.ODO;
  
 /*---- UNLOCK THE FLASH PROGRAM ERASE CONTROLLER ----------------------------*/
  HAL_FLASH_Unlock();								 // UNLOCK FLASH

 /*----- CLEAR ALL PENDING FLAGS ---------------------------------------------*/
 __HAL_FLASH_CLEAR_FLAG(FLASH_SR_OPERR  | FLASH_SR_PROGERR | FLASH_SR_WRPERR | 
                        FLASH_SR_PGAERR | FLASH_SR_SIZERR  | FLASH_SR_PGSERR |  
                        FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_OPTVERR);
 
 FlashAddress = EndAddr-FLASH_PAGE_SIZE;					 //  SET FLASH ADDRESS
 
/*----- ERASE THE FLASH PAGES -----------------------------------------------*/
 FLASH_PageErase(FLASH_BANK_1, ((FlashAddress - FLASH_BASE) / FLASH_PAGE_SIZE)); // ERASE FLASH PAGES 
 if(FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE) == HAL_OK){};

 CLEAR_BIT(FLASH->CR,FLASH_CR_PER); 

 /*----- FLASH HALFWORD PROGRAM ----------------------------------------------*/
  FlashData =(uint64_t)0xED;
  FLASH_ProgramHalfWord(FlashAddress, FlashData);
  FlashAddress += 8;
 /*----- WRITE SOS NUMBER ----------------------------------------------------*/ 

                                                                                // Read back the data from the backup register
                                                                                //read_data = LL_RTC_BKP_GetRegister(&hrtc, LL_RTC_BKP_DR1);
                                                                                // Convert the integer data back to floating-point with scaling
                                                                                // float read_data_float = (float)read_data / 10.0;

    FlashData =  (uint64_t)(Vehicle.ODO *10);                                   //FlashData = (uint64_t)(read_data);        // FlashData =  (uint64_t)(Vehicle.ODO *10); 
    FLASH_ProgramHalfWord(FlashAddress, FlashData );				 // WRITE IN FLASH 
    FlashAddress += 8;
      
 CLEAR_BIT(FLASH->CR,FLASH_CR_PG); 
    
 /*----- LOCK FLASH MEMORY ---------------------------------------------------*/
 HAL_FLASH_Lock();							         // LOCK FLASH MEMORY  

}




//void dis_alert(void)
//{
//     LCD_Reg[4] &= 0xf7ffff;
//   
//   //Side Stand Sensor/Brake  APPLIED Function
//    if((Reg_4==0x21)||(Reg_4==0x22)||(Reg_4==0x23))     //||(Reg_4==0x2A)       //Side Stand_Sensor   
//    {
//      
//      Flag_SideStand=SET;
//      LCD_Reg[0] |= 0x020000;
//    }
//    else{ LCD_Reg[0] &= 0xfDffff; Flag_SideStand=RESET; }
//   
//
//   //For Parking "P"
//    if(!Flag_SideStand)
//     {
//             if((Reg_5==0x40)||(Reg_5==0x44))                                   //Parking Mode Activate                 
//             {
//              
//              LCD_Reg[0] &= 0xfff1ff; LCD_Reg[1] &= 0xfff0ff;LCD_Reg[2] &= 0xfff0ff;LCD_Reg[3] &= 0xfff0ff; LCD_Reg[4] &= 0xdff55f;
//              LCD_Reg[1] |= 0x000300; LCD_Reg[2] |= 0x000300;LCD_Reg[3] |= 0x000200;  
//              mSec_Delay(5);
//              Flag_parking=SET;
//
//              if(Flag_TGL)
//              {
//                LW_ON;RW_ON; 
//              }
//              else
//              {
//                LW_OFF;RW_OFF;
//              }
////              LW_ON;RW_ON;   mSec_Delay(500); 
////              LW_OFF;RW_OFF; mSec_Delay(500);  
//             }
//             else {if(Flag_parking){Flag_parking=RESET;} }       
//     }
//  
//  //Motor  Fail
//  if(((Reg_3==0x01)||(Reg_3==0x40)||(Reg_3==0x41)||(Reg_3==0x60))&&(speed_rpm==0)){ LCD_Reg[4] |= 0x020000; } else {LCD_Reg[4] &= 0xfDffff;}                                    
//   
//  //Throter Fail
//  if(((Reg_3==0x20)||(Reg_3==0x60))&&(speed_rpm==0)) {LCD_Reg[4] |= 0x008000; } else {LCD_Reg[4] &= 0xff7fff;}    
//
//  //ECU Fail    
//  //if((Reg_0 != 0x1F)&&(Reg_1 != 0x01)&&(Reg_4==0)) {LCD_Reg[4] |= 0x080000;} else {LCD_Reg[4] &= 0xf7ffff;}                                       
//} 








/*******************************************************************************
* Function Name  : Cal_speed
* Description    : Calculate The Vehicle Speed(Km/h)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Cal_speed(void)
{   
  if((ONE_WIRE.Reg[0]==DEVICE_SIGN1)&&(ONE_WIRE.Reg[1] == DEVICE_SIGN2)){                                            // Device Identify
    speed_high= ONE_WIRE.Reg[7];                                                           // Speed Data Register_Reg 7 & 8
    speed_low= ONE_WIRE.Reg[8];
    speed_rpm= ((uint16_t)speed_high << 8) | speed_low;                         // speed in rpm
    speed=(int)(speed_rpm * 32 * 0.001885);                                     // speed in km/h(/*40-42 max,gear 1 //50-54 max,gear 2  //61-64 max,gear 3*/)                     
    
    R_Speed=speed;
    if(Vehicle.GearNo == 1 && (R_Speed > 34)) R_Speed =  34;                   
    else if(Vehicle.GearNo == 2 && (R_Speed > 53)) R_Speed =  53; 
    else if(Vehicle.GearNo == 3 && (R_Speed > 66)) R_Speed =  66; 
    
    if(Vehicle.GearNo == 1){speed = (uint8_t)(speed *1.13);  if(speed > 40)speed = 40;} 
    if(Vehicle.GearNo == 2){speed = (uint8_t)(speed *1.13);  if(speed > 55)speed = 55;} 
    if(Vehicle.GearNo == 3){speed = (uint8_t)(speed *1.13);  if(speed > 70)speed = 70;}
    Vehicle.SPEED = speed;
  }                                                                                     
} 

/*******************************************************************************
* Function Name  : Cal_odo
* Description    : Calculate The Vehicle ODOMETER_Val(Km)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Cal_odo(void)
{
  /* Checking Signature */
  if((ONE_WIRE.Reg[0] == DEVICE_SIGN1)&&(ONE_WIRE.Reg[1] == DEVICE_SIGN2) && (speed != 0))                                 
  {    
    /* make it zero to restore the value */
    avg_speed = 0;
    
    /* Store speed */
    speeds[speed_index] = R_Speed;
    
    /* increment index */
    speed_index = (speed_index + 1) % 5;
    
    /* Taking Sample of speed */
    for (int i = 0; i < 5; i++) {avg_speed += speeds[i];}
    
    /* Take average */
    avg_speed /= 5.0;                                                          
    
    /************  10 % INCREMENT ******/
    Vehicle.ODO += ((avg_speed / 3.6)/1000)*1.1;                                         
    /***********************************/
  }                                                                   
}

/*******************************************************************************
* Function Name  : LOW_Voltage_Detection
* Description    : Check and Verify Battery Volt Status 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LOW_Voltage_Detection(void)  
{       
  /* Read Battery Voltage */
  Read_BatteryVolt(); 
         
  /* Check Battery Voltage is less than 60 or Not */
  if(Vehicle.EBV < 55)                                                                   
  {
    LW_OFF;RW_OFF;//HL_OFF;LGO_OFF;                                              
    Flag_BatteryLow=SET;
  }
}
 
/*******************************************************************************
* Function Name  : App_Init
* Description    : Initialize ODOMETER and LCD Display.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void App_Init(void)
{
  Init_Time = 11;                                                               // Initialize TIme_ODOMETER
  FlashAddress = EndAddr-FLASH_PAGE_SIZE;
  FlashData =(*(uint64_t*)FlashAddress);
  if(FlashData == 0xED)
  {
    FlashAddress += 8;
    FlashData = (*(uint64_t*)FlashAddress);
    Vehicle.ODO = ((float)FlashData)/10;                                        //Initialize ODOMETER
  }
  
  /* Turn on all segment*/
  LCD_Reg[0] = ALL_SEGS_ON;                                                        //LCD Display Reg R4_speed bar,TEMP *C
  LCD_Reg[1] = ALL_SEGS_ON;
  LCD_Reg[2] = ALL_SEGS_ON;
  LCD_Reg[3] = ALL_SEGS_ON;                                                      //LCD Display Reg R19_Km/h
  
  /* Turn on backlight */
  BKL_ON; 
  
  /* Hold display for 2 second */
  int i = 2000;
  while(i)
  {
    if(Flag_mSec){Flag_mSec = RESET;i--; }
    if(Flag_Display_Update){Flag_Display_Update = RESET;LCD_Drive();}
  }
  
  /* Turn off all segment*/
  LCD_Reg[0] = ALL_SEGS_OFF;                                                        //LCD Display Reg R4_speed bar,TEMP *C
  LCD_Reg[1] = ALL_SEGS_OFF;
  LCD_Reg[2] = ALL_SEGS_OFF;
  LCD_Reg[3] = ALL_SEGS_OFF;                                                      //LCD Display Reg R19_Km/h
}

/*******************************************************************************
* Function Name  : Read_BatteryVolt
* Description    : Read the value of Battery Voltage
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Read_BatteryVolt(void)
{
  ADC_ConvertedData = HAL_ADC_GetValue(&hadc1);
  Vehicle.EBV = DC_Voltage_Calibration(12,ADC_ConvertedData);                      
}

/*******************************************************************************
* Function Name  : Gear
* Description    : Gear Data extract
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Gear(void)
{
       if(ONE_WIRE.Reg[4] == 1 || ONE_WIRE.Reg[4] == 9  || ONE_WIRE.Reg[4] == 65 )Vehicle.GearNo = 1;
  else if(ONE_WIRE.Reg[4] == 2 || ONE_WIRE.Reg[4] == 10 || ONE_WIRE.Reg[4] == 66 )Vehicle.GearNo = 2;
  else if(ONE_WIRE.Reg[4] == 3 || ONE_WIRE.Reg[4] == 11 || ONE_WIRE.Reg[4] == 67 )Vehicle.GearNo = 3; 
}

/*******************************************************************************
* Function Name  : Indicator_Management
* Description    : This Function is used to Manage the indication 
                   (Left,Right,High beam).
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Indicator_Management(void)
{
  /* Check if parking and reverse gear flags are cleared */
  if ((!Flag_parking) && (!Flag_revgear)) 
  {
 
    /* LEFT WINKER */
    if (LL_GPIO_IsInputPinSet(IN_LW_SENSE_GPIO_Port, IN_LW_SENSE_Pin)) {LW_OFF;}else{LW_ON;}
    
    
    /* RIGHT WINKER */
    if (LL_GPIO_IsInputPinSet(IN_RW_SENSE_GPIO_Port, IN_RW_SENSE_Pin)) {RW_OFF;}else{RW_ON;}
  }
 }

/*******************************************************************************
* Function Name  : dis_alert
* Description    : Display The Alerts of all Failure Section
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void dis_alert(void)
{
 
  /* Side Stand Sensor/Brake APPLIED Function */
  if (SIDE_STAND){Flag_SideStand = SET;DIS_STAND;BRAKE_LED_ON;}else { CLR_STAND; Flag_SideStand = RESET;BRAKE_LED_OFF;}
  
  /* Motor Fail */
  if (MOTOR_FAIL) {DIS_MOTOR;}else {CLR_MOTOR;}
  
  /* Throttle Fail */
  if (THROTTLE_FAIL) {DIS_THROTTLE;}else {CLR_THROTTLE;}
  
  /* For Parking "P" */
  if (!Flag_SideStand && PARKING_MODE)
  {
    Flag_parking = SET; 
    if (Flag_TGL) {LW_ON; RW_ON;} else {LW_OFF; RW_OFF;}
  }
  else{ Flag_parking = RESET;}
  
  if(Vehicle.Charging){DIS_CHG;} else {CLR_CHG;}
  /* ECU Fail */
  /* Uncomment if ECU failure logic is required */
  // if ((Reg_0 != 0x1F) && (Reg_1 != 0x01) && (Reg_4 == 0)) { DIS_ECU;} 
  // else { CLR_ECU; } 
}


/*******************************************************************************
* Function Name  : Dis_Gear
* Description    : Display Gear(1,2,3) as Manually Selected.
Configuration for Max. Speed per Gear.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Dis_SpeedBar(void)
{
  switch(ONE_WIRE.Reg[4])                                                       //Gear Register_Reg 4                    
  {     
  case 1 : if(speed>=45) {speed=45;}                                            //max speed=45km/h,Gear=1 ,Non-load                         
  
  if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*1.5);}                         //150KM-100% SOC_72v 42AH
  else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*1.0);}                    //100KM-100% SOC_72v 30AH
  
  Flag_revspeed=RESET;
  LCD_Reg[0] &= 0xfff9FF;                                                    
  LCD_Reg[0] |= 0x000000; LCD_Reg[1] |= 0x000000; 
  LCD_Reg[2] |= 0x000000; LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x0000A0;  
  
  break;                          
  case 2 :  if(speed>=55) {speed=55;}                                           //max speed=55km/h,Gear=2 ,Non-load
  
  if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*1.2);}                         //120KM-100% SOC_72v 42AH
  else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*0.8);}                    //80KM-100% SOC_72v 30AH
  
   Flag_revspeed=RESET;
  LCD_Reg[0] &= 0xfffBff; LCD_Reg[4] &= 0xffff7f;
  LCD_Reg[0] |= 0x000200; LCD_Reg[1] |= 0x000000; 
  LCD_Reg[2] |= 0x000000;LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x000020; 
  
  break;                         
  case 3 :  if(speed>=70) {speed=70;}                                           //max speed=70km/h,Gear=3 ,Non-load
  
  if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*0.9);}                         //90KM-100% SOC_72v 42AH
  else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*0.6);}                    //60KM-100% SOC_72v 30AH
  
  Flag_revspeed=RESET;
  LCD_Reg[0] &= 0xfffDff; LCD_Reg[4] &= 0xffff7f;          
  LCD_Reg[0] |= 0x000400; LCD_Reg[1] |= 0x000000;
  LCD_Reg[2] |= 0x000000; LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x000020;  
  
  break;  
  
  case 33 :                          
    if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*1.5);}                       //150KM-100% SOC_72v 42AH
    else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*1.0);}                  //100KM-100% SOC_72v 30AH
    
    LCD_Reg[0] &= 0xfff9FF;                                                    
    LCD_Reg[0] |= 0x000000; LCD_Reg[1] |= 0x000000; 
    LCD_Reg[2] |= 0x000000; LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x0000A0;  
    
    break;                          
  case 34 :  
    if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*1.2);}                       //120KM-100% SOC_72v 42AH
    else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*0.8);}                  //80KM-100% SOC_72v 30AH
    
    LCD_Reg[0] &= 0xfffBff; LCD_Reg[4] &= 0xffff7f;
    LCD_Reg[0] |= 0x000200; LCD_Reg[1] |= 0x000000; 
    LCD_Reg[2] |= 0x000000;LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x000020; 
    
    break;                         
    //    case 35 :  
    //              if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*0.9);}              //90KM-100% SOC_72v 42AH
    //              else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*0.6);}         //60KM-100% SOC_72v 30AH
    //              
    //              LCD_Reg[0] &= 0xfffDff; LCD_Reg[4] &= 0xffff7f;          
    //              LCD_Reg[0] |= 0x000400; LCD_Reg[1] |= 0x000000;
    //              LCD_Reg[2] |= 0x000000; LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x000020;  
    //
    //    break;  
    
  case 65 : if(speed>=45) {speed=45;}                                           //max speed=45km/h,Gear=1 ,loaded
  if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*1.5);}                         //150KM-100% SOC_72v 42AH
  else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*1.0);}                    //100KM-100% SOC_72v 30AH
  
  LCD_Reg[0] &= 0xfff9FF;             
  LCD_Reg[0] |= 0x000000; LCD_Reg[1] |= 0x000000; 
  LCD_Reg[2] |= 0x000000; LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x0000A0;  
  
  break;                          
  case 66 : if(speed>=55) {speed=55;}                                           //max speed=55km/h,Gear=2 ,loaded
  if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*1.2);}                         //120KM-100% SOC_72v 42AH
  else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*0.8);}                    //80KM-100% SOC_72v 30AH
  
  LCD_Reg[0] &= 0xfffBff; LCD_Reg[4] &= 0xffff7f;
  LCD_Reg[0] |= 0x000200; LCD_Reg[1] |= 0x000000; 
  LCD_Reg[2] |= 0x000000;LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x000020; 
  
  break;                          
  case 67 : if(speed>=70) {speed=70;}                                           //max speed=70km/h,Gear=3 ,loaded
  if(Vehicle.BC==42){CAP_MTR=(int)((Vehicle.SOC)*0.9);}                         //90KM-100% SOC_72v 42AH
  else if(Vehicle.BC==30){CAP_MTR=(int)((Vehicle.SOC)*0.6);}                    //60KM-100% SOC_72v 30AH
  
  LCD_Reg[0] &= 0xfffDff; LCD_Reg[4] &= 0xffff7f;          
  LCD_Reg[0] |= 0x000400; LCD_Reg[1] |= 0x000000;
  LCD_Reg[2] |= 0x000000; LCD_Reg[3] |= 0x000000; LCD_Reg[4] |= 0x000020; 
  
  break;                          
  default :  if(speed>=70) {speed=70;}                                          //default speed in km/h                                 
  break;                          
  }
  Vehicle.SPEED = speed;
} 

/*******************************************************************************
* Function Name  : Dis_revgear
* Description    : Display Reverse Gear "R"
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Dis_revgear(void)                                                           
{ 
  /* Rerverse Gear */
  Vehicle.GearNo = 5;
  
  DISP_REVERSE();
  /* Toggle indicator */
  if(Flag_TGL){LW_ON;RW_ON;}else {LW_OFF;RW_OFF;}
}


/*******************************************************************************
* Function Name  : ONE_WIRE_COMM
* Description    : This Function is used to process functionality based on data receive from controller
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ONE_WIRE_COMM(void) 
{ 
  /* Validate ECU_Status by Device ID and checksum */
  if ((ONE_WIRE.Reg[0] == DEVICE_SIGN1) && (ONE_WIRE.Reg[1] == DEVICE_SIGN2)) 
  {
    /* Check Side Stand status */
    if (!Flag_SideStand) 
    {
      /* Check Parking status */
      if(!Flag_parking) 
      {
        /* Check Reverse Gear status */
        if (ONE_WIRE.Reg[5] == REVERSE_ID) 
        {
          /* Display Reverse Gear if speed flag is reset and ticks are divisible by 200 */
          if (/*!Flag_revspeed &&*/ ((Ticks % 200) == 0)) 
          {
            /* Set reverse Gear Flag */
            Flag_revgear=SET;
            
            Dis_revgear();
          }
        } 
        else 
        {
          /* Reset Reverse Gear flag if necessary */
          if (Flag_revgear) 
          {
            Flag_revgear = RESET;
          }
          
          /* Display Gear or Speed based on Parking and Reg_8 value */
          if((Flag_parking) || (ONE_WIRE.Reg[8] != 0)) 
          {
            //Dis_SpeedBar();                                                         /* Display Gear */
          }
        }
      }
    } 
    else 
    {
      //Dis_SpeedBar(); /* Display Gear with Side Stand active */
    }
    
    dis_alert(); /* Display Alerts */
  }
}
/*******************************************************************************
* Function Name  : FLASH_ProgramHalfWord
* Description    : Programs a 64-bit half-word into Flash memory at a specified address.
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_ProgramHalfWord(uint32_t Address, uint64_t Data)
{
  /* Set PG bit */
  SET_BIT(FLASH->CR, FLASH_CR_PG);
  
  /* Program first word */
  *(uint32_t *)Address = (uint32_t)Data;
  
  /* Barrier to ensure programming is performed in 2 steps, in right order
  (independently of compiler optimization behavior) */
  __ISB();
  
  /* Program second word */
  *(uint32_t *)(Address + 4U) = (uint32_t)(Data >> 32U);
}


/*******************************************************************************
* Function Name  : CONFIG_BUZZ
* Description    : Configures the buzzer GPIO pin with predefined settings.
*                  Initializes the pin as an output with low-speed frequency,
*                  push-pull output type, and no pull-up/pull-down resistors.
* Output         : None
* Return         : None
*******************************************************************************/
void CONFIG_BUZZ(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  GPIO_InitStruct.Pin = BUZZER_ON_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(BUZZER_ON_GPIO_Port, &GPIO_InitStruct);
  
}

/*******************************************************************************
* Function Name  : Mode_Management
* Description    : Handles mode selection based on button press input.
*                  Determines the display mode and calls the appropriate function
*                  to present vehicle-related information such as trip distance,
*                  odometer readings, battery temperature, and battery voltage.
* Output         : None
* Return         : None
*******************************************************************************/
void Mode_Management(void)
{
  switch(Button_Pressed)
  {
  case 0:Disp_OdoTrip_Meter(Vehicle.ODO);    break;/*Trip*/
  case 1:Disp_OdoTrip_Meter(Vehicle.ODO);     break;/*Odo*/
  case 2:Batt_Temperature(Vehicle.Batt_TEMP); break;/*Battery Temperature*/
  case 3:Disp_BattVolt(Vehicle.BV);           break;/*Battery voltage*/
  default:break;
  }
}


/*******************************************************************************
* Function Name  : Management_Process
* Description    : Handles various management tasks in the system, including:
*                  - Button functionality via Mode_Management()
*                  - Status indication using Indicator_Management()
*                  - CAN communication through CAN_Management()
*                  - 1-Wire communication if checksum validation passes
* Output         : None
* Return         : None
*******************************************************************************/
void Management_Process(void)
{
  /* This Function is responisble for Button Functionality*/
  Mode_Management();
  
  /* This Function is responsible for Indication */
  Indicator_Management();
  
  
  LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_3);
  /* This Function is responsible for CAN Process*/
  CAN_Management();
  
  /*1-Wire communication */
  if(!checksum)ONE_WIRE_COMM();
  
  /* Display Management */
  Display_Management();
}





void Display_Management(void)
{
  /* Display Speed if it has changed */
  if(Flag_parking){DISP_PARKING();}
  else if (Flag_revgear){DISP_REVERSE();}
  else{Disp_Speed(Vehicle.SPEED);}
  
  /* Display Gear */
  DISP_GEAR(Vehicle.GearNo);
  
  /* Display ODO meter */
  Disp_OdoTrip_Meter(Vehicle.ODO);
  
  /* Display SOC using Bar */
  status_batt(Vehicle.SOC);
}




