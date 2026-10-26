/******************** (C) COPYRIGHT 2022 EDS INDIA *****************************
* File Name     : Display_Driver.c					       *
* Author        : SAHIL ALI                                                    *
* Date          : 05-02-2022                                                   *
* Description   : This file contains all Display_Driver                        *
* Revision	: Rev0    						       *
*******************************************************************************/

/* ---------------------------- HOW TO USE -------------------------------------

This file contain display Driver and it is very easy to use . you just need to 
call the function and pass the parameter value to display.

call this function every milisecond LCD_Drive();

SPEED   :-  To display the speed call this function and pass the input parameter    Disp_Speed(speed) 
ODOMETER:-  To display the odometer call this function and pass the input parameter Disp_OdoMeter(ODOMETER) 
Battery :-  To display the Battery call this function and pass the input parameter  status_batt(Battery) 
GEAR    :-  To display the GEAR call this function and pass the input parameter     DISP_GEAR(GEAR) 

595IC Driving :- function "Set_Data(val)" is used to send data over 595IC. 

*/

#include "main.h"
uint32_t LCD_Reg[5] = {0};      // LCD Segment registers

uint8_t LCD_Duty = 0;


/*******************************************************************************
* Function Name  : Disp_BattVolt
* Description    : Display The Battery Voltage
* Input          : (float )Battery_voltage 
* Output         : None
* Return         : None
*******************************************************************************/
void Disp_BattVolt(float Batt_Voltage)
{
  /* Display  Voltage icon */
  LCD_Reg[0] |=  BIT(21); 
  LCD_Reg[0] &= ~BIT(23);/* remove '.'*/
  
    /* CLEAR ODO - METER SEGMENT*/
  CLEAR_BITS(LCD_Reg[0],16,18,20,22,24,26);
  CLEAR_BITS(LCD_Reg[1],15,16,17,18,19);
  CLEAR_BITS(LCD_Reg[1],20,21,22,23,24,25,26);
  CLEAR_BITS(LCD_Reg[2],15,16,17,18,19);
  CLEAR_BITS(LCD_Reg[2],20,21,22,23,24,25,26);
  CLEAR_BITS(LCD_Reg[3],15,16,17,18,19);
  CLEAR_BITS(LCD_Reg[3],20,21,22,23,24,25,26);
   
  /* Convert battery voltage to string format (e.g., "12.3") */
  char BV_Buffer[5];
  if(Batt_Voltage >= 100){sprintf(BV_Buffer, "%d",(int)(Batt_Voltage * 10));}
  else
  {
    LCD_Reg[0] |= BIT(23);
    {sprintf(BV_Buffer, "%d", (int)(Batt_Voltage * 10));} 
  }
  
  switch(BV_Buffer[0])
  {
  
  case '0' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26);        LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '1' : LCD_Reg[0] &= ~BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(25);          break;
  case '2' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26);          break;
  case '3' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '4' : LCD_Reg[0] &= ~BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(25);          break;
  case '5' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break; 
  case '6' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '7' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(25);          break;
  case '8' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '9' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break; 
  default:break;
  }
  
  switch(BV_Buffer[1])
  {
  case '0' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24);        LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '1' : LCD_Reg[0] &= ~BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(23);          break;
  case '2' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24);          break;
  case '3' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '4' : LCD_Reg[0] &= ~BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(23);          break;
  case '5' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break; 
  case '6' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '7' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(23);          break;
  case '8' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '9' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break; 
  default:break;
  }
  
  switch(BV_Buffer[2])
  { 
  case '0' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '1' : LCD_Reg[0] &= ~BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(21);          break;
  case '2' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22);          break;
  case '3' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '4' : LCD_Reg[0] &= ~BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(21);          break;
  case '5' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break; 
  case '6' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '7' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(21);          break;
  case '8' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '9' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;

  default:break;
  }
}
 

void Batt_Temperature(float Batt_Temp)
{
  /* Display  Celsius icon */
  LCD_Reg[0] |=  BIT(19);
  
  /* remove '.'*/
  LCD_Reg[0] &= ~BIT(17); 
  
  /* CLEAR Battery temp segment */
  CLEAR_BITS(LCD_Reg[0],26,24,22);
  CLEAR_BITS(LCD_Reg[1],26,25,24,23,22,21);
  CLEAR_BITS(LCD_Reg[2],26,25,24,23,22,21);
  CLEAR_BITS(LCD_Reg[3],26,25,24,23,22,21);

  char Temp_Buffer[5];
  if(Batt_Temp >= 100){sprintf(Temp_Buffer, "%3d",(int)(Batt_Temp * 10));}
  else
  {
    LCD_Reg[0] |= BIT(23);
    {sprintf(Temp_Buffer, "%3d", (int)(Batt_Temp * 10));} 
  }
 
  switch(Temp_Buffer[0])
  {
  
  case '0' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26);        LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '1' : LCD_Reg[0] &= ~BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(25);          break;
  case '2' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26);          break;
  case '3' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '4' : LCD_Reg[0] &= ~BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(25);          break;
  case '5' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break; 
  case '6' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '7' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(25);          break;
  case '8' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '9' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break; 
  default:break;
  }
  
  switch(Temp_Buffer[1])
  {
  case '0' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24);        LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '1' : LCD_Reg[0] &= ~BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(23);          break;
  case '2' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24);          break;
  case '3' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '4' : LCD_Reg[0] &= ~BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(23);          break;
  case '5' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break; 
  case '6' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '7' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(23);          break;
  case '8' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '9' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break; 
  default:break;
  }
  
  switch(Temp_Buffer[2])
  { 
  case '0' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '1' : LCD_Reg[0] &= ~BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(21);          break;
  case '2' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22);          break;
  case '3' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '4' : LCD_Reg[0] &= ~BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(21);          break;
  case '5' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break; 
  case '6' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '7' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(21);          break;
  case '8' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '9' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;

  default:break;
  }
}


 
/*******************************************************************************
* Function Name  : Disp_Speed
* Description    : Display Vehicle _Speed(Km/h)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Disp_Speed(float Vehicle_SPEED)
{
  /*  Clear Speed Segment */
  /* Digit 1 */
  CLEAR_BITS(LCD_Reg[0],29);
  CLEAR_BITS(LCD_Reg[1],29,2); 
  CLEAR_BITS(LCD_Reg[2],29,2); 
  CLEAR_BITS(LCD_Reg[3],29,2); 
  
  /* Digit 2 */
  CLEAR_BITS(LCD_Reg[0],27);
  CLEAR_BITS(LCD_Reg[1],28,27); 
  CLEAR_BITS(LCD_Reg[2],28,27); 
  CLEAR_BITS(LCD_Reg[3],28,27); 
 
//  /* Clear Speed Bar */
  CLEAR_BITS(LCD_Reg[0],6,5,4);
  CLEAR_BITS(LCD_Reg[1],6,5,4); 
  CLEAR_BITS(LCD_Reg[2],6,5,4,3); 
  CLEAR_BITS(LCD_Reg[3],6,5,4,3); 
  
  
  /* Display (mph km/h mile km) icon */ 
  LCD_Reg[0]  |= BIT(15);
  
  /* Display Speed bar */
  LCD_Reg[0]  |= BIT(1);

  DISP_SPEEDBAR(Vehicle_SPEED);
      char BV_Buffer[3];
  sprintf(BV_Buffer,"%2d",(char)Vehicle_SPEED);
   switch(BV_Buffer[0])
  {
  case '0' : LCD_Reg[0] |= BIT(29);LCD_Reg[1] |= BIT(29)|BIT(2);LCD_Reg[2] |= BIT(29);       LCD_Reg[3] |= BIT(29)|BIT(2);break;
  case '1' : LCD_Reg[0] |= 0x00;LCD_Reg[1] |= BIT(29);       LCD_Reg[2] |= BIT(29);          LCD_Reg[3] |= 0x00;          break;
  case '2' : LCD_Reg[0] |= BIT(29);LCD_Reg[1] |= BIT(2);        LCD_Reg[2] |= BIT(29)|BIT(2);LCD_Reg[3] |= BIT(29);       break;
  case '3' : LCD_Reg[0] |= BIT(29);LCD_Reg[1] |= BIT(29);       LCD_Reg[2] |= BIT(29)|BIT(2);LCD_Reg[3] |= BIT(29);       break;
  case '4' : LCD_Reg[0] |= 0x00;   LCD_Reg[1] |= BIT(29);       LCD_Reg[2] |= BIT(29)|BIT(2);LCD_Reg[3] |= BIT(2);        break;    
  case '5' : LCD_Reg[0] |= BIT(29);LCD_Reg[1] |= BIT(29);       LCD_Reg[2] |= BIT(2);        LCD_Reg[3] |= BIT(29)|BIT(2);break;
  case '6' : LCD_Reg[0] |= BIT(29);LCD_Reg[1] |= BIT(29)|BIT(2);LCD_Reg[2] |= BIT(2);        LCD_Reg[3] |= BIT(29)|BIT(2);break;
  case '7' : LCD_Reg[0] |= 0X00;   LCD_Reg[1] |= BIT(29);       LCD_Reg[2] |= BIT(29);       LCD_Reg[3] |= BIT(29);       break;
  case '8' : LCD_Reg[0] |= BIT(29);LCD_Reg[1] |= BIT(29)|BIT(2);LCD_Reg[2] |= BIT(29)|BIT(2);LCD_Reg[3] |= BIT(29)|BIT(2);break;
  case '9' : LCD_Reg[0] |= BIT(29);LCD_Reg[1] |= BIT(29);LCD_Reg[2] |= BIT(29)|BIT(2);LCD_Reg[3] |= BIT(29)|BIT(2);break;
  }
  
  switch(BV_Buffer[1])
  {
  case '0' : LCD_Reg[0] |= BIT(27);LCD_Reg[1] |= BIT(28)|BIT(27);LCD_Reg[2] |= BIT(27);        LCD_Reg[3] |= BIT(28)|BIT(27);break;
  case '1' : LCD_Reg[0] |= 0x00;   LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(27);        LCD_Reg[3] |= 0x00;           break;
  case '2' : LCD_Reg[0] |= BIT(27);LCD_Reg[1] |= BIT(28);        LCD_Reg[2] |= BIT(28)|BIT(27);LCD_Reg[3] |= BIT(27);        break;
  case '3' : LCD_Reg[0] |= BIT(27);LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(28)|BIT(27);LCD_Reg[3] |= BIT(27);        break;
  case '4' : LCD_Reg[0] |= 0X00;   LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(28)|BIT(27);LCD_Reg[3] |= BIT(28);        break;
  case '5' : LCD_Reg[0] |= BIT(27);LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(28);        LCD_Reg[3] |= BIT(28)|BIT(27);break;
  case '6' : LCD_Reg[0] |= BIT(27);LCD_Reg[1] |= BIT(28)|BIT(27);LCD_Reg[2] |= BIT(28);LCD_Reg[3] |= BIT(28)|BIT(27);break;
  case '7' : LCD_Reg[0] |= 0x00;   LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(27);        LCD_Reg[3] |= BIT(27);        break;
  case '8' : LCD_Reg[0] |= BIT(27);LCD_Reg[1] |= BIT(28)|BIT(27);LCD_Reg[2] |= BIT(28)|BIT(27);LCD_Reg[3] |= BIT(28)|BIT(27);break;
  case '9' : LCD_Reg[0] |= BIT(27);LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(28)|BIT(27);LCD_Reg[3] |= BIT(28)|BIT(27);break;
  }
}


/*******************************************************************************
* Function Name  : Disp_OdoMeter
* Description    : Display The OdoMeter
* Output         : None
* Return         : None
*******************************************************************************/
void Disp_OdoTrip_Meter(float Vehicle_ODO_TRIP_Meter)
{
  /* CLEAR ODO - METER SEGMENT*/
  CLEAR_BITS(LCD_Reg[0],16,18,20,22,24,26);
  CLEAR_BITS(LCD_Reg[1],15,16,17,18,19);
  CLEAR_BITS(LCD_Reg[1],20,21,22,23,24,25,26);
  CLEAR_BITS(LCD_Reg[2],15,16,17,18,19);
  CLEAR_BITS(LCD_Reg[2],20,21,22,23,24,25,26);
  CLEAR_BITS(LCD_Reg[3],15,16,17,18,19);
  CLEAR_BITS(LCD_Reg[3],20,21,22,23,24,25,26);
  
  
  char ODO_Buffer[7];
  int ODO_MTR = 0;
  
  if(Vehicle_ODO_TRIP_Meter < 1000.0){ ODO_MTR = (uint32_t)(Vehicle_ODO_TRIP_Meter *10); LCD_Reg[0] |= BIT(17);}  // Odo Resolution 100 meters
  else { ODO_MTR = (uint32_t)Vehicle_ODO_TRIP_Meter; LCD_Reg[0] &= ~BIT(17);}                        // Odo Resolution 1KM
  sprintf(ODO_Buffer,"%06d",(uint32_t)ODO_MTR);
  if(ODO_Buffer[3] == ' ')ODO_Buffer[3] = '0';
  
  switch(ODO_Buffer[0])
  {
  
  case '0' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26);        LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '1' : LCD_Reg[0] &= ~BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(25);          break;
  case '2' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26);          break;
  case '3' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '4' : LCD_Reg[0] &= ~BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(25);          break;
  case '5' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break; 
  case '6' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '7' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(25);          break;
  case '8' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26)|BIT(25);LCD_Reg[3] |= BIT(26)|BIT(25);  break;
  case '9' : LCD_Reg[0] |=  BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);        LCD_Reg[3] |= BIT(26)|BIT(25);  break; 
  default:break;
  }
  
  switch(ODO_Buffer[1])
  {
  case '0' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24);        LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '1' : LCD_Reg[0] &= ~BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(23);          break;
  case '2' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24);          break;
  case '3' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '4' : LCD_Reg[0] &= ~BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(23);          break;
  case '5' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break; 
  case '6' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '7' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(23);          break;
  case '8' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24)|BIT(23);LCD_Reg[3] |= BIT(24)|BIT(23);  break;
  case '9' : LCD_Reg[0] |=  BIT(24);  LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);        LCD_Reg[3] |= BIT(24)|BIT(23);  break; 
  default:break;
  }
  
  switch(ODO_Buffer[2])
  { 
  case '0' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '1' : LCD_Reg[0] &= ~BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(21);          break;
  case '2' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22);          break;
  case '3' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '4' : LCD_Reg[0] &= ~BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(21);          break;
  case '5' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break; 
  case '6' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '7' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(21);          break;
  case '8' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22)|BIT(21);LCD_Reg[3] |= BIT(22)|BIT(21);  break;
  case '9' : LCD_Reg[0] |=  BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);        LCD_Reg[3] |= BIT(22)|BIT(21);  break;

  default:break;
  }
  
  switch(ODO_Buffer[3])
  {
  case '0' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(20);        LCD_Reg[3] |= BIT(20)|BIT(19);  break;
  case '1' : LCD_Reg[0] &= ~BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(19);          break;
  case '2' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= BIT(20)|BIT(19);LCD_Reg[3] |= BIT(20);          break;
  case '3' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= BIT(19);        LCD_Reg[3] |= BIT(20)|BIT(19);  break;
  case '4' : LCD_Reg[0] &= ~BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(19);        LCD_Reg[3] |= BIT(19);          break;
  case '5' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(20);        LCD_Reg[2] |= BIT(19);        LCD_Reg[3] |= BIT(20)|BIT(19);  break; 
  case '6' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(20);        LCD_Reg[2] |= BIT(20)|BIT(19);LCD_Reg[3] |= BIT(20)|BIT(19);  break;
  case '7' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(19);          break;
  case '8' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(20)|BIT(19);LCD_Reg[3] |= BIT(20)|BIT(19);  break;
  case '9' : LCD_Reg[0] |=  BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(19);        LCD_Reg[3] |= BIT(20)|BIT(19);  break; 
  default:break;
  }
  
  switch(ODO_Buffer[4])
  {
  case '0' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(18)|BIT(17);LCD_Reg[2] |= BIT(18);        LCD_Reg[3] |= BIT(18)|BIT(17);  break;
  case '1' : LCD_Reg[0] &= ~BIT(18); LCD_Reg[1] |= BIT(17);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(17);          break;
  case '2' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(17);        LCD_Reg[2] |= BIT(18)|BIT(17);LCD_Reg[3] |= BIT(18);          break;
  case '3' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(17);        LCD_Reg[2] |= BIT(17);        LCD_Reg[3] |= BIT(18)|BIT(17);  break;
  case '4' : LCD_Reg[0] &= ~BIT(18); LCD_Reg[1] |= BIT(18)|BIT(17);LCD_Reg[2] |= BIT(17);        LCD_Reg[3] |= BIT(17);          break;
  case '5' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(18);        LCD_Reg[2] |= BIT(17);        LCD_Reg[3] |= BIT(18)|BIT(17);  break; 
  case '6' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(18);        LCD_Reg[2] |= BIT(18)|BIT(17);LCD_Reg[3] |= BIT(18)|BIT(17);  break;
  case '7' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(17);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(17);          break;
  case '8' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(18)|BIT(17);LCD_Reg[2] |= BIT(18)|BIT(17);LCD_Reg[3] |= BIT(18)|BIT(17);  break;
  case '9' : LCD_Reg[0] |=  BIT(18); LCD_Reg[1] |= BIT(18)|BIT(17);LCD_Reg[2] |= BIT(17);        LCD_Reg[3] |= BIT(18)|BIT(17);  break;
  default:break;
  } 
  
  switch(ODO_Buffer[5])
  {
  case '0' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(16)|BIT(15);LCD_Reg[2] |= BIT(16);        LCD_Reg[3] |= BIT(16)|BIT(15);  break;
  case '1' : LCD_Reg[0] &= ~BIT(16); LCD_Reg[1] |= BIT(15);        LCD_Reg[2] |= 0x00;           LCD_Reg[3] |= BIT(15);          break;
  case '2' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(15);        LCD_Reg[2] |= BIT(16)|BIT(15);LCD_Reg[3] |= BIT(16);          break;
  case '3' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(15);        LCD_Reg[2] |= BIT(15);        LCD_Reg[3] |= BIT(16)|BIT(15);  break;
  case '4' : LCD_Reg[0] &= ~BIT(16); LCD_Reg[1] |= BIT(16)|BIT(15);LCD_Reg[2] |= BIT(15);        LCD_Reg[3] |= BIT(15);          break;
  case '5' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(16);        LCD_Reg[2] |= BIT(15);        LCD_Reg[3] |= BIT(16)|BIT(15);  break; 
  case '6' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(16);        LCD_Reg[2] |= BIT(16)|BIT(15);LCD_Reg[3] |= BIT(16)|BIT(15);  break;
  case '7' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(15);        LCD_Reg[2] |= 0X00;           LCD_Reg[3] |= BIT(15);          break;
  case '8' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(16)|BIT(15);LCD_Reg[2] |= BIT(16)|BIT(15);LCD_Reg[3] |= BIT(16)|BIT(15);  break;
  case '9' : LCD_Reg[0] |=  BIT(16); LCD_Reg[1] |= BIT(16)|BIT(15);LCD_Reg[2] |= BIT(15);        LCD_Reg[3] |= BIT(16)|BIT(15);  break; 
  default:break;
  } 
}


/*******************************************************************************
* Function Name  : Disp_BattCap
* Description    : Display Battery Capacity "C-XXX"
* Output         : None
* Return         : None
*******************************************************************************/
void Disp_BattCap(int Vehicle_capacity)
{
//  /*turn on KM */
//  LCD_Reg[4] |= 0x000400;                                                        
//  
//  /* Clear ODO meter Segments */
//  LCD_Reg[0] &= ~(0x0C0000);LCD_Reg[1] &= ~(0x0FF000);
//  LCD_Reg[2] &= ~(0x0FF000);LCD_Reg[3] &= ~(0x0FF000);
//  LCD_Reg[4] &= ~(0x057000);
//  
//  /* Write C*/
//  LCD_Reg[0] |= 0x040000; LCD_Reg[2] |= 0x080000;                                //"C-XXX"
//  char CAP_Buffer[4];
//  sprintf(CAP_Buffer,"%3d",(short)Vehicle_capacity);
//  
//  switch(CAP_Buffer[0])
//  {
//  case '0' : LCD_Reg[1] |= 0x030000; LCD_Reg[2] |= 0x010000;LCD_Reg[3] |= 0x030000;LCD_Reg[4] |= 0x010000;break;
//  case '1' : LCD_Reg[1] |= 0X000000; LCD_Reg[2] |= 0X010000;LCD_Reg[3] |= 0x010000;LCD_Reg[4] |= 0x000000;break;
//  case '2' : LCD_Reg[1] |= 0X010000; LCD_Reg[2] |= 0X030000;LCD_Reg[3] |= 0x020000;LCD_Reg[4] |= 0X010000;break;
//  case '3' : LCD_Reg[1] |= 0X010000; LCD_Reg[2] |= 0X030000;LCD_Reg[3] |= 0X010000;LCD_Reg[4] |= 0X010000;break;
//  case '4' : LCD_Reg[1] |= 0x020000; LCD_Reg[2] |= 0X030000;LCD_Reg[3] |= 0X010000;LCD_Reg[4] |= 0x000000;break;
//  case '5' : LCD_Reg[1] |= 0X030000; LCD_Reg[2] |= 0x020000;LCD_Reg[3] |= 0X010000;LCD_Reg[4] |= 0X010000;break;
//  case '6' : LCD_Reg[1] |= 0X030000; LCD_Reg[2] |= 0x020000;LCD_Reg[3] |= 0X030000;LCD_Reg[4] |= 0X010000;break;
//  case '7' : LCD_Reg[1] |= 0X010000; LCD_Reg[2] |= 0X010000;LCD_Reg[3] |= 0X010000;LCD_Reg[4] |= 0x000000;break;
//  case '8' : LCD_Reg[1] |= 0X030000; LCD_Reg[2] |= 0X030000;LCD_Reg[3] |= 0X030000;LCD_Reg[4] |= 0X010000;break;
//  case '9' : LCD_Reg[1] |= 0X030000; LCD_Reg[2] |= 0x030000;LCD_Reg[3] |= 0X010000;LCD_Reg[4] |= 0x010000;break;
//  }
//  
//  switch(CAP_Buffer[1])
//  {
//  case '0' : LCD_Reg[1] |= 0x00C000; LCD_Reg[2] |= 0x004000;LCD_Reg[3] |= 0x00C000;LCD_Reg[4] |= 0x004000;break;
//  case '1' : LCD_Reg[1] |= 0x000000; LCD_Reg[2] |= 0x004000;LCD_Reg[3] |= 0x004000;LCD_Reg[4] |= 0x000000;break;
//  case '2' : LCD_Reg[1] |= 0x004000; LCD_Reg[2] |= 0x00C000;LCD_Reg[3] |= 0x008000;LCD_Reg[4] |= 0x004000;break;
//  case '3' : LCD_Reg[1] |= 0x004000; LCD_Reg[2] |= 0x00C000;LCD_Reg[3] |= 0x004000;LCD_Reg[4] |= 0x004000;break;
//  case '4' : LCD_Reg[1] |= 0x008000; LCD_Reg[2] |= 0x00C000;LCD_Reg[3] |= 0x004000;LCD_Reg[4] |= 0x000000;break;
//  case '5' : LCD_Reg[1] |= 0x00C000; LCD_Reg[2] |= 0x008000;LCD_Reg[3] |= 0x004000;LCD_Reg[4] |= 0x004000;break;
//  case '6' : LCD_Reg[1] |= 0x00C000; LCD_Reg[2] |= 0x008000;LCD_Reg[3] |= 0x00C000;LCD_Reg[4] |= 0x004000;break;
//  case '7' : LCD_Reg[1] |= 0x004000; LCD_Reg[2] |= 0x004000;LCD_Reg[3] |= 0x004000;LCD_Reg[4] |= 0x000000;break;
//  case '8' : LCD_Reg[1] |= 0x00C000; LCD_Reg[2] |= 0x00C000;LCD_Reg[3] |= 0x00C000;LCD_Reg[4] |= 0x004000;break;
//  case '9' : LCD_Reg[1] |= 0x00C000; LCD_Reg[2] |= 0x00C000;LCD_Reg[3] |= 0x004000;LCD_Reg[4] |= 0x004000;break;
//  }
//  
//  switch(CAP_Buffer[2])
//  {
//  case '0' : LCD_Reg[1] |= 0x003000; LCD_Reg[2] |= 0x001000;LCD_Reg[3] |= 0x003000;LCD_Reg[4] |= 0x001000;break;
//  case '1' : LCD_Reg[1] |= 0X000000; LCD_Reg[2] |= 0X001000;LCD_Reg[3] |= 0x001000;LCD_Reg[4] |= 0x000000;break;
//  case '2' : LCD_Reg[1] |= 0X001000; LCD_Reg[2] |= 0X003000;LCD_Reg[3] |= 0x002000;LCD_Reg[4] |= 0X001000;break;
//  case '3' : LCD_Reg[1] |= 0X001000; LCD_Reg[2] |= 0X003000;LCD_Reg[3] |= 0X001000;LCD_Reg[4] |= 0X001000;break;
//  case '4' : LCD_Reg[1] |= 0x002000; LCD_Reg[2] |= 0X003000;LCD_Reg[3] |= 0X001000;LCD_Reg[4] |= 0x000000;break;
//  case '5' : LCD_Reg[1] |= 0X003000; LCD_Reg[2] |= 0x002000;LCD_Reg[3] |= 0X001000;LCD_Reg[4] |= 0X001000;break;
//  case '6' : LCD_Reg[1] |= 0X003000; LCD_Reg[2] |= 0x002000;LCD_Reg[3] |= 0X003000;LCD_Reg[4] |= 0X001000;break;
//  case '7' : LCD_Reg[1] |= 0X001000; LCD_Reg[2] |= 0X001000;LCD_Reg[3] |= 0X001000;LCD_Reg[4] |= 0x000000;break;
//  case '8' : LCD_Reg[1] |= 0X003000; LCD_Reg[2] |= 0X003000;LCD_Reg[3] |= 0X003000;LCD_Reg[4] |= 0X001000;break;
//  case '9' : LCD_Reg[1] |= 0X003000; LCD_Reg[2] |= 0x003000;LCD_Reg[3] |= 0X001000;LCD_Reg[4] |= 0x001000;break;
//  }  
}



/*******************************************************************************
* Function Name  : LCD_Drive
* Description    : Drive LCD Display
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 
void LCD_Drive(void)
{
  COM1_OFF; COM2_OFF; COM3_OFF; COM4_OFF;
  static uint16_t Segment = 0x00;
  LCD_Duty++;
  if(LCD_Duty > 7)LCD_Duty = 0;
  
  switch(LCD_Duty)
  {
  case 0 : 
    if (LCD_Reg[0] & BIT(29)) SEG1_SET;  else SEG1_RST;
    if (LCD_Reg[0] & BIT(28)) SEG2_SET;  else SEG2_RST;
    if (LCD_Reg[0] & BIT(27)) SEG3_SET;  else SEG3_RST;
    if (LCD_Reg[0] & BIT(26)) SEG4_SET;  else SEG4_RST;
    if (LCD_Reg[0] & BIT(25)) SEG5_SET;  else SEG5_RST;
    if (LCD_Reg[0] & BIT(24)) SEG6_SET;  else SEG6_RST;
    if (LCD_Reg[0] & BIT(23)) SEG7_SET;  else SEG7_RST;
    if (LCD_Reg[0] & BIT(22)) SEG8_SET;  else SEG8_RST;
    if (LCD_Reg[0] & BIT(21)) SEG9_SET;  else SEG9_RST;
    if (LCD_Reg[0] & BIT(20)) SEG10_SET; else SEG10_RST;
    if (LCD_Reg[0] & BIT(19)) SEG11_SET; else SEG11_RST;
    if (LCD_Reg[0] & BIT(18)) {Segment |=(1<<7);} else {Segment &= ~(1<<7);}
    if (LCD_Reg[0] & BIT(17)) {Segment |=(1<<6);} else {Segment &= ~(1<<6);}
    if (LCD_Reg[0] & BIT(16)) {Segment |=(1<<5);} else {Segment &= ~(1<<5);}
    if (LCD_Reg[0] & BIT(15)) {Segment |=(1<<4);} else {Segment &= ~(1<<4);}
    if (LCD_Reg[0] & BIT(14)) {Segment |=(1<<3);} else {Segment &= ~(1<<3);}
    if (LCD_Reg[0] & BIT(13)) {Segment |=(1<<2);} else {Segment &= ~(1<<2);}
    if (LCD_Reg[0] & BIT(12)) {Segment |=(1<<1);} else {Segment &= ~(1<<1);}
    if (LCD_Reg[0] & BIT(11)) {Segment |=(1<<0);} else {Segment &= ~(1<<0);}
    if (LCD_Reg[0] & BIT(6))  SEG20_SET; else SEG20_RST;
    if (LCD_Reg[0] & BIT(5))  SEG21_SET; else SEG21_RST;
    if (LCD_Reg[0] & BIT(4))  SEG22_SET; else SEG22_RST;
    if (LCD_Reg[0] & BIT(3))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[0] & BIT(2))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[0] & BIT(1))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[0] & BIT(0))  SEG26_SET; else SEG26_RST;
    COM1_SET;
    break;
  case 1 : 
    if (LCD_Reg[1] & BIT(29)) SEG1_SET;  else SEG1_RST;
    if (LCD_Reg[1] & BIT(28)) SEG2_SET;  else SEG2_RST;
    if (LCD_Reg[1] & BIT(27)) SEG3_SET;  else SEG3_RST;
    if (LCD_Reg[1] & BIT(26)) SEG4_SET;  else SEG4_RST;
    if (LCD_Reg[1] & BIT(25)) SEG5_SET;  else SEG5_RST;
    if (LCD_Reg[1] & BIT(24)) SEG6_SET;  else SEG6_RST;
    if (LCD_Reg[1] & BIT(23)) SEG7_SET;  else SEG7_RST;
    if (LCD_Reg[1] & BIT(22)) SEG8_SET;  else SEG8_RST;
    if (LCD_Reg[1] & BIT(21)) SEG9_SET;  else SEG9_RST;
    if (LCD_Reg[1] & BIT(20)) SEG10_SET; else SEG10_RST;
    if (LCD_Reg[1] & BIT(19)) SEG11_SET; else SEG11_RST;
    if (LCD_Reg[1] & BIT(18)) {Segment |=(1<<7);} else {Segment &= ~(1<<7);}
    if (LCD_Reg[1] & BIT(17)) {Segment |=(1<<6);} else {Segment &= ~(1<<6);}
    if (LCD_Reg[1] & BIT(16)) {Segment |=(1<<5);} else {Segment &= ~(1<<5);}
    if (LCD_Reg[1] & BIT(15)) {Segment |=(1<<4);} else {Segment &= ~(1<<4);}
    if (LCD_Reg[1] & BIT(14)) {Segment |=(1<<3);} else {Segment &= ~(1<<3);}
    if (LCD_Reg[1] & BIT(13)) {Segment |=(1<<2);} else {Segment &= ~(1<<2);}
    if (LCD_Reg[1] & BIT(12)) {Segment |=(1<<1);} else {Segment &= ~(1<<1);}
    if (LCD_Reg[1] & BIT(11)) {Segment |=(1<<0);} else {Segment &= ~(1<<0);}
    if (LCD_Reg[1] & BIT(6))  SEG20_SET; else SEG20_RST;
    if (LCD_Reg[1] & BIT(5))  SEG21_SET; else SEG21_RST;
    if (LCD_Reg[1] & BIT(4))  SEG22_SET; else SEG22_RST;
    if (LCD_Reg[1] & BIT(3))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[1] & BIT(2))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[1] & BIT(1))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[1] & BIT(0))  SEG26_SET; else SEG26_RST;
    COM2_SET; 
    break;
    
  case 2 : 
    if (LCD_Reg[2] & BIT(29)) SEG1_SET;  else SEG1_RST;
    if (LCD_Reg[2] & BIT(28)) SEG2_SET;  else SEG2_RST;
    if (LCD_Reg[2] & BIT(27)) SEG3_SET;  else SEG3_RST;
    if (LCD_Reg[2] & BIT(26)) SEG4_SET;  else SEG4_RST;
    if (LCD_Reg[2] & BIT(25)) SEG5_SET;  else SEG5_RST;
    if (LCD_Reg[2] & BIT(24)) SEG6_SET;  else SEG6_RST;
    if (LCD_Reg[2] & BIT(23)) SEG7_SET;  else SEG7_RST;
    if (LCD_Reg[2] & BIT(22)) SEG8_SET;  else SEG8_RST;
    if (LCD_Reg[2] & BIT(21)) SEG9_SET;  else SEG9_RST;
    if (LCD_Reg[2] & BIT(20)) SEG10_SET; else SEG10_RST;
    if (LCD_Reg[2] & BIT(19)) SEG11_SET; else SEG11_RST;
    if (LCD_Reg[2] & BIT(18)) {Segment |=(1<<7);} else {Segment &= ~(1<<7);}
    if (LCD_Reg[2] & BIT(17)) {Segment |=(1<<6);} else {Segment &= ~(1<<6);}
    if (LCD_Reg[2] & BIT(16)) {Segment |=(1<<5);} else {Segment &= ~(1<<5);}
    if (LCD_Reg[2] & BIT(15)) {Segment |=(1<<4);} else {Segment &= ~(1<<4);}
    if (LCD_Reg[2] & BIT(14)) {Segment |=(1<<3);} else {Segment &= ~(1<<3);}
    if (LCD_Reg[2] & BIT(13)) {Segment |=(1<<2);} else {Segment &= ~(1<<2);}
    if (LCD_Reg[2] & BIT(12)) {Segment |=(1<<1);} else {Segment &= ~(1<<1);}
    if (LCD_Reg[2] & BIT(11)) {Segment |=(1<<0);} else {Segment &= ~(1<<0);}
    if (LCD_Reg[2] & BIT(6))  SEG20_SET; else SEG20_RST;
    if (LCD_Reg[2] & BIT(5))  SEG21_SET; else SEG21_RST;
    if (LCD_Reg[2] & BIT(4))  SEG22_SET; else SEG22_RST;
    if (LCD_Reg[2] & BIT(3))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[2] & BIT(2))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[2] & BIT(1))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[2] & BIT(0))  SEG26_SET; else SEG26_RST;
    COM3_SET; 
    break;
    
  case 3 : 
    if (LCD_Reg[3] & BIT(29)) SEG1_SET;  else SEG1_RST;
    if (LCD_Reg[3] & BIT(28)) SEG2_SET;  else SEG2_RST;
    if (LCD_Reg[3] & BIT(27)) SEG3_SET;  else SEG3_RST;
    if (LCD_Reg[3] & BIT(26)) SEG4_SET;  else SEG4_RST;
    if (LCD_Reg[3] & BIT(25)) SEG5_SET;  else SEG5_RST;
    if (LCD_Reg[3] & BIT(24)) SEG6_SET;  else SEG6_RST;
    if (LCD_Reg[3] & BIT(23)) SEG7_SET;  else SEG7_RST;
    if (LCD_Reg[3] & BIT(22)) SEG8_SET;  else SEG8_RST;
    if (LCD_Reg[3] & BIT(21)) SEG9_SET;  else SEG9_RST;
    if (LCD_Reg[3] & BIT(20)) SEG10_SET; else SEG10_RST;
    if (LCD_Reg[3] & BIT(19)) SEG11_SET; else SEG11_RST;
    if (LCD_Reg[3] & BIT(18)) {Segment |=(1<<7);} else {Segment &= ~(1<<7);}
    if (LCD_Reg[3] & BIT(17)) {Segment |=(1<<6);} else {Segment &= ~(1<<6);}
    if (LCD_Reg[3] & BIT(16)) {Segment |=(1<<5);} else {Segment &= ~(1<<5);}
    if (LCD_Reg[3] & BIT(15)) {Segment |=(1<<4);} else {Segment &= ~(1<<4);}
    if (LCD_Reg[3] & BIT(14)) {Segment |=(1<<3);} else {Segment &= ~(1<<3);}
    if (LCD_Reg[3] & BIT(13)) {Segment |=(1<<2);} else {Segment &= ~(1<<2);}
    if (LCD_Reg[3] & BIT(12)) {Segment |=(1<<1);} else {Segment &= ~(1<<1);}
    if (LCD_Reg[3] & BIT(11)) {Segment |=(1<<0);} else {Segment &= ~(1<<0);}
    if (LCD_Reg[3] & BIT(6))  SEG20_SET; else SEG20_RST;
    if (LCD_Reg[3] & BIT(5))  SEG21_SET; else SEG21_RST;
    if (LCD_Reg[3] & BIT(4))  SEG22_SET; else SEG22_RST;
    if (LCD_Reg[3] & BIT(3))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[3] & BIT(2))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[3] & BIT(1))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[3] & BIT(0))  SEG26_SET; else SEG26_RST;
    COM4_SET; 
    break;
 
  case 4 : 
    if (LCD_Reg[0] & BIT(29)) SEG1_RST;  else SEG1_SET;
    if (LCD_Reg[0] & BIT(28)) SEG2_RST;  else SEG2_SET;
    if (LCD_Reg[0] & BIT(27)) SEG3_RST;  else SEG3_SET;
    if (LCD_Reg[0] & BIT(26)) SEG4_RST;  else SEG4_SET;
    if (LCD_Reg[0] & BIT(25)) SEG5_RST;  else SEG5_SET;
    if (LCD_Reg[0] & BIT(24)) SEG6_RST;  else SEG6_SET;
    if (LCD_Reg[0] & BIT(23)) SEG7_RST;  else SEG7_SET;
    if (LCD_Reg[0] & BIT(22)) SEG8_RST;  else SEG8_SET;
    if (LCD_Reg[0] & BIT(21)) SEG9_RST;  else SEG9_SET;
    if (LCD_Reg[0] & BIT(20)) SEG10_RST; else SEG10_SET;
    if (LCD_Reg[0] & BIT(19)) SEG11_RST; else SEG11_SET;
    if (LCD_Reg[0] & BIT(18)) {Segment  &= ~(1<<7);}else {Segment |=(1<<7);}
    if (LCD_Reg[0] & BIT(17)) {Segment  &= ~(1<<6);}else {Segment |=(1<<6);}
    if (LCD_Reg[0] & BIT(16)) {Segment  &= ~(1<<5);}else {Segment |=(1<<5);}
    if (LCD_Reg[0] & BIT(15)) {Segment  &= ~(1<<4);}else {Segment |=(1<<4);}
    if (LCD_Reg[0] & BIT(14)) {Segment  &= ~(1<<3);}else {Segment |=(1<<3);}
    if (LCD_Reg[0] & BIT(13)) {Segment  &= ~(1<<2);}else {Segment |=(1<<2);}
    if (LCD_Reg[0] & BIT(12)) {Segment  &= ~(1<<1);}else {Segment |=(1<<1);}
    if (LCD_Reg[0] & BIT(11)) {Segment  &= ~(1<<0);}else {Segment |=(1<<0);}
    if (LCD_Reg[0] & BIT(6))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[0] & BIT(5))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[0] & BIT(4))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[0] & BIT(3))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[0] & BIT(2))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[0] & BIT(1))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[0] & BIT(0))  SEG26_RST; else SEG26_SET; 
    COM1_RST;
    break;
    
  case 5 : 
    if (LCD_Reg[1] & BIT(29)) SEG1_RST;  else SEG1_SET;
    if (LCD_Reg[1] & BIT(28)) SEG2_RST;  else SEG2_SET;
    if (LCD_Reg[1] & BIT(27)) SEG3_RST;  else SEG3_SET;
    if (LCD_Reg[1] & BIT(26)) SEG4_RST;  else SEG4_SET;
    if (LCD_Reg[1] & BIT(25)) SEG5_RST;  else SEG5_SET;
    if (LCD_Reg[1] & BIT(24)) SEG6_RST;  else SEG6_SET;
    if (LCD_Reg[1] & BIT(23)) SEG7_RST;  else SEG7_SET;
    if (LCD_Reg[1] & BIT(22)) SEG8_RST;  else SEG8_SET;
    if (LCD_Reg[1] & BIT(21)) SEG9_RST;  else SEG9_SET;
    if (LCD_Reg[1] & BIT(20)) SEG10_RST; else SEG10_SET;
    if (LCD_Reg[1] & BIT(19)) SEG11_RST; else SEG11_SET;
    if (LCD_Reg[1] & BIT(18)) {Segment  &= ~(1<<7);}else {Segment |=(1<<7);}
    if (LCD_Reg[1] & BIT(17)) {Segment  &= ~(1<<6);}else {Segment |=(1<<6);}
    if (LCD_Reg[1] & BIT(16)) {Segment  &= ~(1<<5);}else {Segment |=(1<<5);}
    if (LCD_Reg[1] & BIT(15)) {Segment  &= ~(1<<4);}else {Segment |=(1<<4);}
    if (LCD_Reg[1] & BIT(14)) {Segment  &= ~(1<<3);}else {Segment |=(1<<3);}
    if (LCD_Reg[1] & BIT(13)) {Segment  &= ~(1<<2);}else {Segment |=(1<<2);}
    if (LCD_Reg[1] & BIT(12)) {Segment  &= ~(1<<1);}else {Segment |=(1<<1);}
    if (LCD_Reg[1] & BIT(11)) {Segment  &= ~(1<<0);}else {Segment |=(1<<0);}
    if (LCD_Reg[1] & BIT(6))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[1] & BIT(5))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[1] & BIT(4))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[1] & BIT(3))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[1] & BIT(2))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[1] & BIT(1))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[1] & BIT(0))  SEG26_RST; else SEG26_SET; 
    COM2_RST; 
    break;
    
  case 6 : 
    if (LCD_Reg[2] & BIT(29)) SEG1_RST;  else SEG1_SET;
    if (LCD_Reg[2] & BIT(28)) SEG2_RST;  else SEG2_SET;
    if (LCD_Reg[2] & BIT(27)) SEG3_RST;  else SEG3_SET;
    if (LCD_Reg[2] & BIT(26)) SEG4_RST;  else SEG4_SET;
    if (LCD_Reg[2] & BIT(25)) SEG5_RST;  else SEG5_SET;
    if (LCD_Reg[2] & BIT(24)) SEG6_RST;  else SEG6_SET;
    if (LCD_Reg[2] & BIT(23)) SEG7_RST;  else SEG7_SET;
    if (LCD_Reg[2] & BIT(22)) SEG8_RST;  else SEG8_SET;
    if (LCD_Reg[2] & BIT(21)) SEG9_RST;  else SEG9_SET;
    if (LCD_Reg[2] & BIT(20)) SEG10_RST; else SEG10_SET;
    if (LCD_Reg[2] & BIT(19)) SEG11_RST; else SEG11_SET;
    if (LCD_Reg[2] & BIT(18)) {Segment  &= ~(1<<7);}else {Segment |=(1<<7);}
    if (LCD_Reg[2] & BIT(17)) {Segment  &= ~(1<<6);}else {Segment |=(1<<6);}
    if (LCD_Reg[2] & BIT(16)) {Segment  &= ~(1<<5);}else {Segment |=(1<<5);}
    if (LCD_Reg[2] & BIT(15)) {Segment  &= ~(1<<4);}else {Segment |=(1<<4);}
    if (LCD_Reg[2] & BIT(14)) {Segment  &= ~(1<<3);}else {Segment |=(1<<3);}
    if (LCD_Reg[2] & BIT(13)) {Segment  &= ~(1<<2);}else {Segment |=(1<<2);}
    if (LCD_Reg[2] & BIT(12)) {Segment  &= ~(1<<1);}else {Segment |=(1<<1);}
    if (LCD_Reg[2] & BIT(11)) {Segment  &= ~(1<<0);}else {Segment |=(1<<0);}
    if (LCD_Reg[2] & BIT(6))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[2] & BIT(5))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[2] & BIT(4))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[2] & BIT(3))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[2] & BIT(2))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[2] & BIT(1))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[2] & BIT(0))  SEG26_RST; else SEG26_SET;  
    COM3_RST; 
    break;
    
  case 7 : 
    if (LCD_Reg[3] & BIT(29)) SEG1_RST;  else SEG1_SET;
    if (LCD_Reg[3] & BIT(28)) SEG2_RST;  else SEG2_SET;
    if (LCD_Reg[3] & BIT(27)) SEG3_RST;  else SEG3_SET;
    if (LCD_Reg[3] & BIT(26)) SEG4_RST;  else SEG4_SET;
    if (LCD_Reg[3] & BIT(25)) SEG5_RST;  else SEG5_SET;
    if (LCD_Reg[3] & BIT(24)) SEG6_RST;  else SEG6_SET;
    if (LCD_Reg[3] & BIT(23)) SEG7_RST;  else SEG7_SET;
    if (LCD_Reg[3] & BIT(22)) SEG8_RST;  else SEG8_SET;
    if (LCD_Reg[3] & BIT(21)) SEG9_RST;  else SEG9_SET;
    if (LCD_Reg[3] & BIT(20)) SEG10_RST; else SEG10_SET;
    if (LCD_Reg[3] & BIT(19)) SEG11_RST; else SEG11_SET;
    if (LCD_Reg[3] & BIT(18)) {Segment  &= ~(1<<7);}else {Segment |=(1<<7);}
    if (LCD_Reg[3] & BIT(17)) {Segment  &= ~(1<<6);}else {Segment |=(1<<6);}
    if (LCD_Reg[3] & BIT(16)) {Segment  &= ~(1<<5);}else {Segment |=(1<<5);}
    if (LCD_Reg[3] & BIT(15)) {Segment  &= ~(1<<4);}else {Segment |=(1<<4);}
    if (LCD_Reg[3] & BIT(14)) {Segment  &= ~(1<<3);}else {Segment |=(1<<3);}
    if (LCD_Reg[3] & BIT(13)) {Segment  &= ~(1<<2);}else {Segment |=(1<<2);}
    if (LCD_Reg[3] & BIT(12)) {Segment  &= ~(1<<1);}else {Segment |=(1<<1);}
    if (LCD_Reg[3] & BIT(11)) {Segment  &= ~(1<<0);}else {Segment |=(1<<0);}
    if (LCD_Reg[3] & BIT(6))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[3] & BIT(5))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[3] & BIT(4))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[3] & BIT(3))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[3] & BIT(2))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[3] & BIT(1))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[3] & BIT(0))  SEG26_RST; else SEG26_SET;  
    COM4_RST; 
    break;
    
    default:break;
  } 
  Set_Data(Segment);
}





/*******************************************************************************
* Function Name  : status_batt
* Description    : Display Battery Voltage Level
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void status_batt(uint8_t Batt_SOC)
{   
  if(Batt_SOC >= BATT_LOW)                                                  
  { 
    /* CLEAR SEGMENTS */
    CLEAR_BITS(LCD_Reg[0],28,13);
    CLEAR_BITS(LCD_Reg[1],13,12);
    CLEAR_BITS(LCD_Reg[2],13);
    CLEAR_BITS(LCD_Reg[3],13);
    
    /* Turn on battery bar */
    LCD_Reg[0] |= BIT(12); 
    
    if(Batt_SOC  <= BATT_L1){LCD_Reg[3] |= BIT(13);}
    else if((Batt_SOC <= BATT_L2)&&(Batt_SOC > BATT_L1)){LCD_Reg[2] |= BIT(13);LCD_Reg[3] |= BIT(13);}
    else if((Batt_SOC <= BATT_L3)&&(Batt_SOC > BATT_L2)){LCD_Reg[1] |= BIT(13);LCD_Reg[2] |= BIT(13);LCD_Reg[3] |= BIT(13);}
    else if(Batt_SOC  <= BATT_L4){LCD_Reg[0] |= BIT(13);LCD_Reg[1]  |= BIT(13);LCD_Reg[2] |= BIT(13);LCD_Reg[3] |= BIT(13);}
    else if(Batt_SOC  >  BATT_L4){LCD_Reg[0] |= BIT(13);LCD_Reg[1]  |= BIT(13)|BIT(12);LCD_Reg[2] |= BIT(13);LCD_Reg[3] |= BIT(13);}
  }
  else
  {
    /* CLEAR SEGMENTS */
    CLEAR_BITS(LCD_Reg[0],28,13);
    CLEAR_BITS(LCD_Reg[1],13,12);
    CLEAR_BITS(LCD_Reg[2],13);
    CLEAR_BITS(LCD_Reg[3],13);
    /* turn on */
    LCD_Reg[0] |= BIT(28);
    LCD_Reg[0] ^= BIT(12);    
  }
}


void DISP_GEAR(uint8_t Gear)
{

    
  /* Clear Gear number */
  CLEAR_BITS(LCD_Reg[0],0,2);
  CLEAR_BITS(LCD_Reg[1],0);
  CLEAR_BITS(LCD_Reg[2],0);
  CLEAR_BITS(LCD_Reg[3],0);
  
  /* Turn on GEAR symbol*/
  LCD_Reg[2] |= BIT(1);
  
  switch(Gear)
  {
  case 1:{LCD_Reg[3] |= BIT(0);}break;
  case 2:{LCD_Reg[2] |= BIT(0);}break;
  case 3:{LCD_Reg[1] |= BIT(0);}break;
  case 4:{LCD_Reg[0] |= BIT(0);}break;
  case 5:{LCD_Reg[0] |= BIT(2);}break;/*Reverse Gear */
  }
}
 

void Set_Data(uint16_t Data_Val)
{
  unsigned int  SegMult = 0x80;
  unsigned char Seg_Counter;
  for(Seg_Counter = 0;Seg_Counter < 8; Seg_Counter++)                          // check 24 bits
  {
    if((SegMult & Data_Val)) DATA_595_RESET;
    else    DATA_595_SET;
    /**************** creating clock ************************/
    CLK_595_SET;
    for(int i = 0;i < 100;i++){__NOP();}
//    mSec_Delay(1);
    CLK_595_RESET;
    for(int i = 0;i < 100;i++){__NOP();}
    //mSec_Delay(1);            // Set Clock Pulse
    /*******************************************************/  
    
    /*************** bit shifting ***********************/
    SegMult = SegMult >> 1;				                        // Right Shift Multilplier
    /****************************************************/
  } 					                        	        // Delay to remove ghost Image
  LAT_595_SET;
  for(int i = 0;i < 100;i++){__NOP();}
  LAT_595_RESET;
  for(int i = 0;i < 100;i++){__NOP();}
}




void DISP_PARKING(void)
{
    /*  Clear Speed Segment */
  /* Digit 1 */
  CLEAR_BITS(LCD_Reg[0],29);
  CLEAR_BITS(LCD_Reg[1],29,2); 
  CLEAR_BITS(LCD_Reg[2],29,2); 
  CLEAR_BITS(LCD_Reg[3],29,2); 
  
  /* Digit 2 */
  CLEAR_BITS(LCD_Reg[0],27);
  CLEAR_BITS(LCD_Reg[1],28,27); 
  CLEAR_BITS(LCD_Reg[2],28,27); 
  CLEAR_BITS(LCD_Reg[3],28,27); 
 
//  /* Clear Speed Bar */
  CLEAR_BITS(LCD_Reg[0],6,5,4);
  CLEAR_BITS(LCD_Reg[1],6,5,4); 
  CLEAR_BITS(LCD_Reg[2],6,5,4,3); 
  CLEAR_BITS(LCD_Reg[3],6,5,4,3); 
  
    /* Display Speed bar */
  LCD_Reg[0]  |= BIT(1);
  
  /* Write P */
  LCD_Reg[0] &= ~BIT(27);
  LCD_Reg[1] |= BIT(28);
  LCD_Reg[2] |= BIT(28)|BIT(27);
  LCD_Reg[3] |= BIT(28)|BIT(27);
}


void DISP_REVERSE(void)
{
  /*  Clear Speed Segment */
  /* Digit 1 */
  CLEAR_BITS(LCD_Reg[0],29);
  CLEAR_BITS(LCD_Reg[1],29,2); 
  CLEAR_BITS(LCD_Reg[2],29,2); 
  CLEAR_BITS(LCD_Reg[3],29,2); 
  
  /* Digit 2 */
  CLEAR_BITS(LCD_Reg[0],27);
  CLEAR_BITS(LCD_Reg[1],28,27); 
  CLEAR_BITS(LCD_Reg[2],28,27); 
  CLEAR_BITS(LCD_Reg[3],28,27); 
  
  //  /* Clear Speed Bar */
  CLEAR_BITS(LCD_Reg[0],6,5,4);
  CLEAR_BITS(LCD_Reg[1],6,5,4); 
  CLEAR_BITS(LCD_Reg[2],6,5,4,3); 
  CLEAR_BITS(LCD_Reg[3],6,5,4,3); 
//  
//  DISP_SPEEDBAR();
//  
//  
  if(Vehicle.SPEED == 0)
  {
  /* Write R */
  LCD_Reg[0] &= ~BIT(27);
  LCD_Reg[1] |= BIT(28)|BIT(27);
  LCD_Reg[2] |= BIT(28)|BIT(27);
  LCD_Reg[3] |= BIT(28)|BIT(27);
  }
  else
  {
   Disp_Speed(Vehicle.SPEED); 
  }
}




void DISP_SPEEDBAR (float Vehicle_SPEED)
{
    if(Vehicle_SPEED > 0) 
  { 
    if(Vehicle_SPEED > 5.0)
    {
      LCD_Reg[0] |= BIT(6);
    } 
    
    if(Vehicle_SPEED > 14.0)
    {
      LCD_Reg[0] |= BIT(6);
      LCD_Reg[1] |= BIT(6);
    } 
    if(Vehicle_SPEED > 15.0)
    {
      LCD_Reg[0] |= BIT(6);
      LCD_Reg[1] |= BIT(6);
      LCD_Reg[2] |= BIT(6);
    }
    if(Vehicle_SPEED > 20.0)
    {
      LCD_Reg[0] |= BIT(6);
      LCD_Reg[1] |= BIT(6);
      LCD_Reg[2] |= BIT(6);
      LCD_Reg[3] |= BIT(6);
    }
    if(Vehicle_SPEED > 29.0)
    {
    LCD_Reg[0] |= BIT(6)|BIT(5);
    LCD_Reg[1] |= BIT(6);
    LCD_Reg[2] |= BIT(6);
    LCD_Reg[3] |= BIT(6);
    
    } 
    if(Vehicle_SPEED > 30.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(5);
      LCD_Reg[1] |= BIT(6)|BIT(5);
      LCD_Reg[2] |= BIT(6);
      LCD_Reg[3] |= BIT(6);
    } 
    if(Vehicle_SPEED > 40.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(5);
      LCD_Reg[1] |= BIT(6)|BIT(5);
      LCD_Reg[2] |= BIT(6)|BIT(5);
      LCD_Reg[3] |= BIT(6);
    } 
    if(Vehicle_SPEED > 44.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(5);
      LCD_Reg[1] |= BIT(6)|BIT(5);
      LCD_Reg[2] |= BIT(6)|BIT(5);
      LCD_Reg[3] |= BIT(6)|BIT(5);
    }  
    if(Vehicle_SPEED > 45.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(5);
      LCD_Reg[1] |= BIT(6)|BIT(5);
      LCD_Reg[2] |= BIT(6)|BIT(5);
      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
    } 
    if(Vehicle_SPEED > 50.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(5);
      LCD_Reg[1] |= BIT(6)|BIT(5);
      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4);
      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);

    } 
 
    if(Vehicle_SPEED > 59.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(5);
      LCD_Reg[1] |= BIT(6)|BIT(5)|BIT(4);
      LCD_Reg[2] |= BIT(6)|BIT(5);
      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
    } 

    if(Vehicle_SPEED > 60.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(4);
      LCD_Reg[1] |= BIT(6)|BIT(5);
      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4);
      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
    } 
 
    if(Vehicle_SPEED > 74.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(4);
      LCD_Reg[1] |= BIT(6)|BIT(5);
      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4)|BIT(3);
      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);

    } 

    if(Vehicle_SPEED > 89.0)
    {
      LCD_Reg[0] |= BIT(6)|BIT(5)|BIT(4);
      LCD_Reg[1] |= BIT(6)|BIT(5)|BIT(4);
      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4)|BIT(3); 
      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4)|BIT(3);
    }
  }

}








