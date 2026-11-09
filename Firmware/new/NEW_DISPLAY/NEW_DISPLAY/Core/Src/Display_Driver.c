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
/*******************************************************************************
* Function Name  : Disp_Speed
* Description    : Display Vehicle_Speed (Km/h)
* Input          : Vehicle_SPEED
* Output         : None
* Return         : None
*******************************************************************************/
void Disp_Speed(float Vehicle_SPEED)
{
  /* Clear both speed digits (SEG15..SEG18) */
  CLEAR_BITS(LCD_Reg[0],17,15);
  CLEAR_BITS(LCD_Reg[1],18,17,16,15);
  CLEAR_BITS(LCD_Reg[2],18,17,16,15);
  CLEAR_BITS(LCD_Reg[3],18,17,16,15);
 
  /* Display (mph km/h mile km) icon */
//  LCD_Reg[0] |= BIT(15);   /* <-- see note 1 */
// 
//  /* Display Speed bar */
//  LCD_Reg[0] |= BIT(1);
 
//  DISP_SPEEDBAR(Vehicle_SPEED);
 
  char BV_Buffer[3];
  sprintf(BV_Buffer,"%2d",(char)Vehicle_SPEED);
 
  /* Tens digit : SEG17 / SEG18 */
  switch(BV_Buffer[0])
  {
  case '0' : LCD_Reg[0] |= BIT(17); LCD_Reg[1] |= BIT(18)|BIT(17); LCD_Reg[2] |= BIT(17);         LCD_Reg[3] |= BIT(18)|BIT(17); break;
  case '1' :                        LCD_Reg[1] |= BIT(17);         LCD_Reg[2] |= BIT(17);                                        break;
  case '2' : LCD_Reg[0] |= BIT(17); LCD_Reg[1] |= BIT(18);         LCD_Reg[2] |= BIT(17)|BIT(18); LCD_Reg[3] |= BIT(17);         break;
  case '3' : LCD_Reg[0] |= BIT(17); LCD_Reg[1] |= BIT(17);         LCD_Reg[2] |= BIT(17)|BIT(18); LCD_Reg[3] |= BIT(17);         break;
  case '4' :                        LCD_Reg[1] |= BIT(17);         LCD_Reg[2] |= BIT(17)|BIT(18); LCD_Reg[3] |= BIT(18);         break;
  case '5' : LCD_Reg[0] |= BIT(17); LCD_Reg[1] |= BIT(17);         LCD_Reg[2] |= BIT(18);         LCD_Reg[3] |= BIT(17)|BIT(18); break;
  case '6' : LCD_Reg[0] |= BIT(17); LCD_Reg[1] |= BIT(17)|BIT(18); LCD_Reg[2] |= BIT(18);         LCD_Reg[3] |= BIT(17)|BIT(18); break;
  case '7' :                        LCD_Reg[1] |= BIT(17);         LCD_Reg[2] |= BIT(17);         LCD_Reg[3] |= BIT(17);         break;
  case '8' : LCD_Reg[0] |= BIT(17); LCD_Reg[1] |= BIT(17)|BIT(18); LCD_Reg[2] |= BIT(17)|BIT(18); LCD_Reg[3] |= BIT(17)|BIT(18); break;
  case '9' : LCD_Reg[0] |= BIT(17); LCD_Reg[1] |= BIT(17);         LCD_Reg[2] |= BIT(17)|BIT(18); LCD_Reg[3] |= BIT(17)|BIT(18); break;
  }
 
  /* Units digit : SEG15 / SEG16 */
  switch(BV_Buffer[1])
  {
  case '0' : LCD_Reg[0] |= BIT(15); LCD_Reg[1] |= BIT(16)|BIT(15); LCD_Reg[2] |= BIT(15);         LCD_Reg[3] |= BIT(16)|BIT(15); break;
  case '1' :                        LCD_Reg[1] |= BIT(15);         LCD_Reg[2] |= BIT(15);                                        break;
  case '2' : LCD_Reg[0] |= BIT(15); LCD_Reg[1] |= BIT(16);         LCD_Reg[2] |= BIT(16)|BIT(15); LCD_Reg[3] |= BIT(15);         break;
  case '3' : LCD_Reg[0] |= BIT(15); LCD_Reg[1] |= BIT(15);         LCD_Reg[2] |= BIT(16)|BIT(15); LCD_Reg[3] |= BIT(15);         break;
  case '4' :                        LCD_Reg[1] |= BIT(15);         LCD_Reg[2] |= BIT(16)|BIT(15); LCD_Reg[3] |= BIT(16);         break;
  case '5' : LCD_Reg[0] |= BIT(15); LCD_Reg[1] |= BIT(15);         LCD_Reg[2] |= BIT(16);         LCD_Reg[3] |= BIT(16)|BIT(15); break;
  case '6' : LCD_Reg[0] |= BIT(15); LCD_Reg[1] |= BIT(16)|BIT(15); LCD_Reg[2] |= BIT(16);         LCD_Reg[3] |= BIT(16)|BIT(15); break;
  case '7' :                        LCD_Reg[1] |= BIT(15);         LCD_Reg[2] |= BIT(15);         LCD_Reg[3] |= BIT(15);         break;
  case '8' : LCD_Reg[0] |= BIT(15); LCD_Reg[1] |= BIT(16)|BIT(15); LCD_Reg[2] |= BIT(16)|BIT(15); LCD_Reg[3] |= BIT(16)|BIT(15); break;
  case '9' : LCD_Reg[0] |= BIT(15); LCD_Reg[1] |= BIT(15);         LCD_Reg[2] |= BIT(16)|BIT(15); LCD_Reg[3] |= BIT(16)|BIT(15); break;
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
uint16_t Segment = 0;
void LCD_Drive(void)
{
  COM4_OFF; COM3_OFF; COM2_OFF; COM1_OFF;
  
  //for(int i = 0;i < 500 ; i++);
  
  LCD_Duty++;
  if(LCD_Duty > 7 )LCD_Duty = 0;
  
  switch(LCD_Duty)
  {
  case 0 :
   
    if (LCD_Reg[0] & BIT(30)) {SET_BITS(Segment,7);}  else {CLEAR_BITS(Segment,7); }// seg
    if (LCD_Reg[0] & BIT(29)) {SET_BITS(Segment,6);}  else {CLEAR_BITS(Segment,6); }// seg
    if (LCD_Reg[0] & BIT(28)) {SET_BITS(Segment,5);}  else {CLEAR_BITS(Segment,5); }// seg
    if (LCD_Reg[0] & BIT(27)) {SET_BITS(Segment,4);}  else {CLEAR_BITS(Segment,4); }// seg
    if (LCD_Reg[0] & BIT(26)) {SET_BITS(Segment,3);}  else {CLEAR_BITS(Segment,3); }// seg
    if (LCD_Reg[0] & BIT(25)) {SET_BITS(Segment,2);}  else {CLEAR_BITS(Segment,2); }// seg
    if (LCD_Reg[0] & BIT(24)) {SET_BITS(Segment,1);}  else {CLEAR_BITS(Segment,1); }// seg
    if (LCD_Reg[0] & BIT(23)) {SET_BITS(Segment,0);}  else {CLEAR_BITS(Segment,0); }// seg
    if (LCD_Reg[0] & BIT(22)) {SET_BITS(Segment,12);} else {CLEAR_BITS(Segment,12);}// seg
    if (LCD_Reg[0] & BIT(21)) {SET_BITS(Segment,13);} else {CLEAR_BITS(Segment,13);}// seg
    if (LCD_Reg[0] & BIT(20)) {SET_BITS(Segment,14);} else {CLEAR_BITS(Segment,14);}// seg
    if (LCD_Reg[0] & BIT(19)) {SET_BITS(Segment,15);} else {CLEAR_BITS(Segment,15);}// seg
    if (LCD_Reg[0] & BIT(18)) {SET_BITS(Segment,11);} else {CLEAR_BITS(Segment,11);}// seg
    if (LCD_Reg[0] & BIT(17)) {SET_BITS(Segment,10);} else {CLEAR_BITS(Segment,10);}// seg3
    if (LCD_Reg[0] & BIT(16)) {SET_BITS(Segment,9);}  else {CLEAR_BITS(Segment,9);}/// seg
    if (LCD_Reg[0] & BIT(15)) {SET_BITS(Segment,8);}  else {CLEAR_BITS(Segment,8);}/// seg1
    if (LCD_Reg[0] & BIT(14)) SEG17_SET; else SEG17_RST;
    if (LCD_Reg[0] & BIT(13)) SEG18_SET; else SEG18_RST;
    if (LCD_Reg[0] & BIT(12)) SEG19_SET; else SEG19_RST;
    if (LCD_Reg[0] & BIT(11)) SEG20_SET; else SEG20_RST;
    if (LCD_Reg[0] & BIT(10)) SEG21_SET; else SEG21_RST;
    if (LCD_Reg[0] & BIT(5))  SEG22_SET; else SEG22_RST;  
    if (LCD_Reg[0] & BIT(4))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[0] & BIT(3))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[0] & BIT(2))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[0] & BIT(1))  SEG26_SET; else SEG26_RST;
    if (LCD_Reg[0] & BIT(0))  SEG27_SET; else SEG27_RST;
    
    Set_Data(Segment);
    COM1_SET;
    break;
  case 1 : 
    if (LCD_Reg[1] & BIT(30)) {SET_BITS(Segment,7);}  else {CLEAR_BITS(Segment,7);}//1
    if (LCD_Reg[1] & BIT(29)) {SET_BITS(Segment,6);}  else {CLEAR_BITS(Segment,6);}//2
    if (LCD_Reg[1] & BIT(28)) {SET_BITS(Segment,5);}  else {CLEAR_BITS(Segment,5);}//3
    if (LCD_Reg[1] & BIT(27)) {SET_BITS(Segment,4);}  else {CLEAR_BITS(Segment,4);}//4
    if (LCD_Reg[1] & BIT(26)) {SET_BITS(Segment,3);}  else {CLEAR_BITS(Segment,3);}//5  
    if (LCD_Reg[1] & BIT(25)) {SET_BITS(Segment,2);}  else {CLEAR_BITS(Segment,2);}//6
    if (LCD_Reg[1] & BIT(24)) {SET_BITS(Segment,1);}  else {CLEAR_BITS(Segment,1);}//7
    if (LCD_Reg[1] & BIT(23)) {SET_BITS(Segment,0);}  else {CLEAR_BITS(Segment,0);}//8
    if (LCD_Reg[1] & BIT(22)) {SET_BITS(Segment,12);} else {CLEAR_BITS(Segment,12);}//9
    if (LCD_Reg[1] & BIT(21)) {SET_BITS(Segment,13);} else {CLEAR_BITS(Segment,13);}//10
    if (LCD_Reg[1] & BIT(20)) {SET_BITS(Segment,14);} else {CLEAR_BITS(Segment,14);}//11
    if (LCD_Reg[1] & BIT(19)) {SET_BITS(Segment,15);} else {CLEAR_BITS(Segment,15);}//12
    if (LCD_Reg[1] & BIT(18)) {SET_BITS(Segment,11);} else {CLEAR_BITS(Segment,11);}//  seg2
    if (LCD_Reg[1] & BIT(17)) {SET_BITS(Segment,10);} else {CLEAR_BITS(Segment,10);}//  seg3
    if (LCD_Reg[1] & BIT(16)) {SET_BITS(Segment,9);}  else {CLEAR_BITS(Segment,9);}//   seg
    if (LCD_Reg[1] & BIT(15)) {SET_BITS(Segment,8);}  else {CLEAR_BITS(Segment,8);}//   seg1
    if (LCD_Reg[1] & BIT(14)) SEG17_SET; else  SEG17_RST;
    if (LCD_Reg[1] & BIT(13)) SEG18_SET; else  SEG18_RST;
    if (LCD_Reg[1] & BIT(12)) SEG19_SET; else SEG19_RST;
    if (LCD_Reg[1] & BIT(11)) SEG20_SET; else SEG20_RST;
    if (LCD_Reg[1] & BIT(10)) SEG21_SET; else SEG21_RST;
    if (LCD_Reg[1] & BIT(5))  SEG22_SET; else SEG22_RST;  
    if (LCD_Reg[1] & BIT(4))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[1] & BIT(3))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[1] & BIT(2))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[1] & BIT(1))  SEG26_SET; else SEG26_RST;
    if (LCD_Reg[1] & BIT(0))  SEG27_SET; else SEG27_RST;
    Set_Data(Segment);
      COM2_SET; 
    break;
    
  case 2 : 
    if (LCD_Reg[2] & BIT(30)) {SET_BITS(Segment,7);}  else {CLEAR_BITS(Segment,7);}//1
    if (LCD_Reg[2] & BIT(29)) {SET_BITS(Segment,6);}  else {CLEAR_BITS(Segment,6);}//2
    if (LCD_Reg[2] & BIT(28)) {SET_BITS(Segment,5);}  else {CLEAR_BITS(Segment,5);}//3
    if (LCD_Reg[2] & BIT(27)) {SET_BITS(Segment,4);}  else {CLEAR_BITS(Segment,4);}//4
    if (LCD_Reg[2] & BIT(26)) {SET_BITS(Segment,3);}  else {CLEAR_BITS(Segment,3);}//5  
    if (LCD_Reg[2] & BIT(25)) {SET_BITS(Segment,2);}  else {CLEAR_BITS(Segment,2);}//6
    if (LCD_Reg[2] & BIT(24)) {SET_BITS(Segment,1);}  else {CLEAR_BITS(Segment,1);}//7
    if (LCD_Reg[2] & BIT(23)) {SET_BITS(Segment,0);}  else {CLEAR_BITS(Segment,0);}//8
    if (LCD_Reg[2] & BIT(22)) {SET_BITS(Segment,12);} else {CLEAR_BITS(Segment,12);}//9
    if (LCD_Reg[2] & BIT(21)) {SET_BITS(Segment,13);} else {CLEAR_BITS(Segment,13);}//10
    if (LCD_Reg[2] & BIT(20)) {SET_BITS(Segment,14);} else {CLEAR_BITS(Segment,14);}//11
    if (LCD_Reg[2] & BIT(19)) {SET_BITS(Segment,15);} else {CLEAR_BITS(Segment,15);}//12
    if (LCD_Reg[2] & BIT(18)) {SET_BITS(Segment,11);} else {CLEAR_BITS(Segment,11);}//13
    if (LCD_Reg[2] & BIT(17)) {SET_BITS(Segment,10);} else {CLEAR_BITS(Segment,10);}//14
    if (LCD_Reg[2] & BIT(16)) {SET_BITS(Segment,9);}  else {CLEAR_BITS(Segment,9);}//15
    if (LCD_Reg[2] & BIT(15)) {SET_BITS(Segment,8);}  else {CLEAR_BITS(Segment,8);}//16
    if (LCD_Reg[2] & BIT(14)) SEG17_SET; else  SEG17_RST;
    if (LCD_Reg[2] & BIT(13)) SEG18_SET; else  SEG18_RST;
    if (LCD_Reg[2] & BIT(12)) SEG19_SET; else SEG19_RST;
    if (LCD_Reg[2] & BIT(11)) SEG20_SET; else SEG20_RST;
    if (LCD_Reg[2] & BIT(10))  SEG21_SET; else SEG21_RST;
    if (LCD_Reg[2] & BIT(5))  SEG22_SET; else SEG22_RST;  
    if (LCD_Reg[2] & BIT(4))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[2] & BIT(3))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[2] & BIT(2))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[2] & BIT(1))  SEG26_SET; else SEG26_RST;
    if (LCD_Reg[2] & BIT(0))  SEG27_SET; else SEG27_RST;
    Set_Data(Segment);
    COM3_SET; 
    break;
    
  case 3 : 
    if (LCD_Reg[3] & BIT(30)) {SET_BITS(Segment,7);}  else {CLEAR_BITS(Segment,7);}//1
    if (LCD_Reg[3] & BIT(29)) {SET_BITS(Segment,6);}  else {CLEAR_BITS(Segment,6);}//2
    if (LCD_Reg[3] & BIT(28)) {SET_BITS(Segment,5);}  else {CLEAR_BITS(Segment,5);}//3
    if (LCD_Reg[3] & BIT(27)) {SET_BITS(Segment,4);}  else {CLEAR_BITS(Segment,4);}//4
    if (LCD_Reg[3] & BIT(26)) {SET_BITS(Segment,3);}  else {CLEAR_BITS(Segment,3);}//5  
    if (LCD_Reg[3] & BIT(25)) {SET_BITS(Segment,2);}  else {CLEAR_BITS(Segment,2);}//6
    if (LCD_Reg[3] & BIT(24)) {SET_BITS(Segment,1);}  else {CLEAR_BITS(Segment,1);}//7
    if (LCD_Reg[3] & BIT(23)) {SET_BITS(Segment,0);}  else {CLEAR_BITS(Segment,0);}//8
    if (LCD_Reg[3] & BIT(22)) {SET_BITS(Segment,12);} else {CLEAR_BITS(Segment,12);}//9
    if (LCD_Reg[3] & BIT(21)) {SET_BITS(Segment,13);} else {CLEAR_BITS(Segment,13);}//10
    if (LCD_Reg[3] & BIT(20)) {SET_BITS(Segment,14);} else {CLEAR_BITS(Segment,14);}//11
    if (LCD_Reg[3] & BIT(19)) {SET_BITS(Segment,15);} else {CLEAR_BITS(Segment,15);}//12
    if (LCD_Reg[3] & BIT(18)) {SET_BITS(Segment,11);} else {CLEAR_BITS(Segment,11);}//13
    if (LCD_Reg[3] & BIT(17)) {SET_BITS(Segment,10);} else {CLEAR_BITS(Segment,10);}//14
    if (LCD_Reg[3] & BIT(16)) {SET_BITS(Segment,9);}  else {CLEAR_BITS(Segment,9);}//15
    if (LCD_Reg[3] & BIT(15)) {SET_BITS(Segment,8);}  else {CLEAR_BITS(Segment,8);}//16
    if (LCD_Reg[3] & BIT(14)) SEG17_SET; else  SEG17_RST;
    if (LCD_Reg[3] & BIT(13)) SEG18_SET; else  SEG18_RST;
    if (LCD_Reg[3] & BIT(12)) SEG19_SET; else SEG19_RST;
    if (LCD_Reg[3] & BIT(11)) SEG20_SET; else SEG20_RST;
    if (LCD_Reg[3] & BIT(10)) SEG21_SET; else SEG21_RST;
    if (LCD_Reg[3] & BIT(5))  SEG22_SET; else SEG22_RST;  
    if (LCD_Reg[3] & BIT(4))  SEG23_SET; else SEG23_RST;
    if (LCD_Reg[3] & BIT(3))  SEG24_SET; else SEG24_RST;
    if (LCD_Reg[3] & BIT(2))  SEG25_SET; else SEG25_RST;
    if (LCD_Reg[3] & BIT(1))  SEG26_SET; else SEG26_RST;
    if (LCD_Reg[3] & BIT(0))  SEG27_SET; else SEG27_RST;
    Set_Data(Segment);
    COM4_SET; 
    break;
    
  case 4 : 
    if (LCD_Reg[0] & BIT(30)) {CLEAR_BITS(Segment,7);}  else {SET_BITS(Segment,7);}//1
    if (LCD_Reg[0] & BIT(29)) {CLEAR_BITS(Segment,6);}  else {SET_BITS(Segment,6);}//2
    if (LCD_Reg[0] & BIT(28)) {CLEAR_BITS(Segment,5);}  else {SET_BITS(Segment,5);}//3
    if (LCD_Reg[0] & BIT(27)) {CLEAR_BITS(Segment,4);}  else {SET_BITS(Segment,4);}//4
    if (LCD_Reg[0] & BIT(26)) {CLEAR_BITS(Segment,3);}  else {SET_BITS(Segment,3);}//5                                                                   
    if (LCD_Reg[0] & BIT(25)) {CLEAR_BITS(Segment,2);}  else {SET_BITS(Segment,2);}//6
    if (LCD_Reg[0] & BIT(24)) {CLEAR_BITS(Segment,1);}  else {SET_BITS(Segment,1);}//7
    if (LCD_Reg[0] & BIT(23)) {CLEAR_BITS(Segment,0);}  else {SET_BITS(Segment,0);}//8
    if (LCD_Reg[0] & BIT(22)) {CLEAR_BITS(Segment,12);} else {SET_BITS(Segment,12);}//9
    if (LCD_Reg[0] & BIT(21)) {CLEAR_BITS(Segment,13);} else {SET_BITS(Segment,13);}//10
    if (LCD_Reg[0] & BIT(20)) {CLEAR_BITS(Segment,14);} else {SET_BITS(Segment,14);}//11
    if (LCD_Reg[0] & BIT(19)) {CLEAR_BITS(Segment,15);} else {SET_BITS(Segment,15);}//12
    if (LCD_Reg[0] & BIT(18)) {CLEAR_BITS(Segment,11);} else {SET_BITS(Segment,11);}//13
    if (LCD_Reg[0] & BIT(17)) {CLEAR_BITS(Segment,10);} else {SET_BITS(Segment,10);}//14
    if (LCD_Reg[0] & BIT(16)) {CLEAR_BITS(Segment,9);}  else {SET_BITS(Segment,9);}//15
    if (LCD_Reg[0] & BIT(15)) {CLEAR_BITS(Segment,8);}  else {SET_BITS(Segment,8);}//16
    if (LCD_Reg[0] & BIT(14)) SEG17_RST; else SEG17_SET;
    if (LCD_Reg[0] & BIT(13))  SEG18_RST; else SEG18_SET;
    if (LCD_Reg[0] & BIT(12))  SEG19_RST; else SEG19_SET;
    if (LCD_Reg[0] & BIT(11))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[0] & BIT(10))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[0] & BIT(5))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[0] & BIT(4))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[0] & BIT(3))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[0] & BIT(2))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[0] & BIT(1))  SEG26_RST; else SEG26_SET;
    if (LCD_Reg[0] & BIT(0))  SEG27_RST; else SEG27_SET;
    Set_Data(Segment);
    COM1_RST;
    break;
    
  case 5 : 
    if (LCD_Reg[1] & BIT(30)) {CLEAR_BITS(Segment,7);}  else {SET_BITS(Segment,7);}//1
    if (LCD_Reg[1] & BIT(29)) {CLEAR_BITS(Segment,6);}  else {SET_BITS(Segment,6);}//2
    if (LCD_Reg[1] & BIT(28)) {CLEAR_BITS(Segment,5);}  else {SET_BITS(Segment,5);}//3
    if (LCD_Reg[1] & BIT(27)) {CLEAR_BITS(Segment,4);}  else {SET_BITS(Segment,4);}//4
    if (LCD_Reg[1] & BIT(26)) {CLEAR_BITS(Segment,3);}  else {SET_BITS(Segment,3);}//5                                                                   
    if (LCD_Reg[1] & BIT(25)) {CLEAR_BITS(Segment,2);}  else {SET_BITS(Segment,2);}//6
    if (LCD_Reg[1] & BIT(24)) {CLEAR_BITS(Segment,1);}  else {SET_BITS(Segment,1);}//7
    if (LCD_Reg[1] & BIT(23)) {CLEAR_BITS(Segment,0);}  else {SET_BITS(Segment,0);}//8
    if (LCD_Reg[1] & BIT(22)) {CLEAR_BITS(Segment,12);} else {SET_BITS(Segment,12);}//9
    if (LCD_Reg[1] & BIT(21)) {CLEAR_BITS(Segment,13);} else {SET_BITS(Segment,13);}//10
    if (LCD_Reg[1] & BIT(20)) {CLEAR_BITS(Segment,14);} else {SET_BITS(Segment,14);}//11
    if (LCD_Reg[1] & BIT(19)) {CLEAR_BITS(Segment,15);} else {SET_BITS(Segment,15);}//12
    if (LCD_Reg[1] & BIT(18)) {CLEAR_BITS(Segment,11);} else {SET_BITS(Segment,11);}//13
    if (LCD_Reg[1] & BIT(17)) {CLEAR_BITS(Segment,10);} else {SET_BITS(Segment,10);}//14
    if (LCD_Reg[1] & BIT(16)) {CLEAR_BITS(Segment,9);}  else {SET_BITS(Segment,9);}//15
    if (LCD_Reg[1] & BIT(15)) {CLEAR_BITS(Segment,8);}  else {SET_BITS(Segment,8);}//16
    if (LCD_Reg[1] & BIT(14)) SEG17_RST; else SEG17_SET;
    if (LCD_Reg[1] & BIT(13))  SEG18_RST; else SEG18_SET;
    if (LCD_Reg[1] & BIT(12))  SEG19_RST; else SEG19_SET;
    if (LCD_Reg[1] & BIT(11))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[1] & BIT(10))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[1] & BIT(5))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[1] & BIT(4))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[1] & BIT(3))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[1] & BIT(2))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[1] & BIT(1))  SEG26_RST; else SEG26_SET;
    if (LCD_Reg[1] & BIT(0))  SEG27_RST; else SEG27_SET;
    Set_Data(Segment);
    COM2_RST; 
    break;
    
  case 6 : 
    if (LCD_Reg[2] & BIT(30)) {CLEAR_BITS(Segment,7);}  else {SET_BITS(Segment,7);}//1
    if (LCD_Reg[2] & BIT(29)) {CLEAR_BITS(Segment,6);}  else {SET_BITS(Segment,6);}//2
    if (LCD_Reg[2] & BIT(28)) {CLEAR_BITS(Segment,5);}  else {SET_BITS(Segment,5);}//3
    if (LCD_Reg[2] & BIT(27)) {CLEAR_BITS(Segment,4);}  else {SET_BITS(Segment,4);}//4
    if (LCD_Reg[2] & BIT(26)) {CLEAR_BITS(Segment,3);}  else {SET_BITS(Segment,3);}//5                                                                   
    if (LCD_Reg[2] & BIT(25)) {CLEAR_BITS(Segment,2);}  else {SET_BITS(Segment,2);}//6
    if (LCD_Reg[2] & BIT(24)) {CLEAR_BITS(Segment,1);}  else {SET_BITS(Segment,1);}//7
    if (LCD_Reg[2] & BIT(23)) {CLEAR_BITS(Segment,0);}  else {SET_BITS(Segment,0);}//8
    if (LCD_Reg[2] & BIT(22)) {CLEAR_BITS(Segment,12);} else {SET_BITS(Segment,12);}//9
    if (LCD_Reg[2] & BIT(21)) {CLEAR_BITS(Segment,13);} else {SET_BITS(Segment,13);}//10
    if (LCD_Reg[2] & BIT(20)) {CLEAR_BITS(Segment,14);} else {SET_BITS(Segment,14);}//11
    if (LCD_Reg[2] & BIT(19)) {CLEAR_BITS(Segment,15);} else {SET_BITS(Segment,15);}//12
    if (LCD_Reg[2] & BIT(18)) {CLEAR_BITS(Segment,11);} else {SET_BITS(Segment,11);}//13
    if (LCD_Reg[2] & BIT(17)) {CLEAR_BITS(Segment,10);} else {SET_BITS(Segment,10);}//14
    if (LCD_Reg[2] & BIT(16)) {CLEAR_BITS(Segment,9);}  else {SET_BITS(Segment,9);}//15
    if (LCD_Reg[2] & BIT(15)) {CLEAR_BITS(Segment,8);}  else {SET_BITS(Segment,8);}//16
    if (LCD_Reg[2] & BIT(14)) SEG17_RST; else SEG17_SET;
    if (LCD_Reg[2] & BIT(13))  SEG18_RST; else SEG18_SET;
    if (LCD_Reg[2] & BIT(12))  SEG19_RST; else SEG19_SET;
    if (LCD_Reg[2] & BIT(11))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[2] & BIT(10))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[2] & BIT(5))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[2] & BIT(4))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[2] & BIT(3))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[2] & BIT(2))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[2] & BIT(1))  SEG26_RST; else SEG26_SET;
    if (LCD_Reg[2] & BIT(0))  SEG27_RST; else SEG27_SET;
    Set_Data(Segment);
    COM3_RST; 
    break;
    
  case 7 : 
    if (LCD_Reg[3] & BIT(30)) {CLEAR_BITS(Segment,7);}  else {SET_BITS(Segment,7);}//1
    if (LCD_Reg[3] & BIT(29)) {CLEAR_BITS(Segment,6);}  else {SET_BITS(Segment,6);}//2
    if (LCD_Reg[3] & BIT(28)) {CLEAR_BITS(Segment,5);}  else {SET_BITS(Segment,5);}//3
    if (LCD_Reg[3] & BIT(27)) {CLEAR_BITS(Segment,4);}  else {SET_BITS(Segment,4);}//4
    if (LCD_Reg[3] & BIT(26)) {CLEAR_BITS(Segment,3);}  else {SET_BITS(Segment,3);}//5                                                                   
    if (LCD_Reg[3] & BIT(25)) {CLEAR_BITS(Segment,2);}  else {SET_BITS(Segment,2);}//6
    if (LCD_Reg[3] & BIT(24)) {CLEAR_BITS(Segment,1);}  else {SET_BITS(Segment,1);}//7
    if (LCD_Reg[3] & BIT(23)) {CLEAR_BITS(Segment,0);}  else {SET_BITS(Segment,0);}//8
    if (LCD_Reg[3] & BIT(22)) {CLEAR_BITS(Segment,12);} else {SET_BITS(Segment,12);}//9
    if (LCD_Reg[3] & BIT(21)) {CLEAR_BITS(Segment,13);} else {SET_BITS(Segment,13);}//10
    if (LCD_Reg[3] & BIT(20)) {CLEAR_BITS(Segment,14);} else {SET_BITS(Segment,14);}//11
    if (LCD_Reg[3] & BIT(19)) {CLEAR_BITS(Segment,15);} else {SET_BITS(Segment,15);}//12
    if (LCD_Reg[3] & BIT(18)) {CLEAR_BITS(Segment,11);} else {SET_BITS(Segment,11);}//13
    if (LCD_Reg[3] & BIT(17)) {CLEAR_BITS(Segment,10);} else {SET_BITS(Segment,10);}//14
    if (LCD_Reg[3] & BIT(16)) {CLEAR_BITS(Segment,9);}  else {SET_BITS(Segment,9);}//15
    if (LCD_Reg[3] & BIT(15)) {CLEAR_BITS(Segment,8);}  else {SET_BITS(Segment,8);}//16
    if (LCD_Reg[3] & BIT(14)) SEG17_RST; else SEG17_SET;
    if (LCD_Reg[3] & BIT(13))  SEG18_RST; else SEG18_SET;
    if (LCD_Reg[3] & BIT(12))  SEG19_RST; else SEG19_SET;
    if (LCD_Reg[3] & BIT(11))  SEG20_RST; else SEG20_SET;
    if (LCD_Reg[3] & BIT(10))  SEG21_RST; else SEG21_SET;
    if (LCD_Reg[3] & BIT(5))  SEG22_RST; else SEG22_SET;
    if (LCD_Reg[3] & BIT(4))  SEG23_RST; else SEG23_SET;
    if (LCD_Reg[3] & BIT(3))  SEG24_RST; else SEG24_SET;
    if (LCD_Reg[3] & BIT(2))  SEG25_RST; else SEG25_SET;
    if (LCD_Reg[3] & BIT(1))  SEG26_RST; else SEG26_SET;
    if (LCD_Reg[3] & BIT(0))  SEG27_RST; else SEG27_SET;
    Set_Data(Segment);
    COM4_RST; 
    break;
    
  default:break;
  } 
  for(int i = 0;i < 10000; i++);
 // Set_Data(Segment);
}

//uint16_t Segment = 0xFFFF;
//
//void LCD_Drive(void)
//{
//  uint32_t data;
//  uint8_t  i;
//
//  /* SEG1..SEG16 -> Segment bit position (bit30 down to bit15 of LCD_Reg) */
//  static const uint8_t SegMap[16] = {7,6,5,4,3,2,1,0,12,13,14,15,11,10,9,8};
//
//  LCD_Duty++;
//  if(LCD_Duty > 7) LCD_Duty = 0;
//
//  /* Duty 0-3: normal phase, duty 4-7: inverted phase of the same COM */
//  data = LCD_Reg[LCD_Duty & 3];
//  if(LCD_Duty > 3) data = ~data;
//
//  /* SEG1..SEG16 (written through Set_Data) */
//  for(i = 0; i < 16; i++)
//  {
//    if(data & BIT(30 - i)) Segment |=  (uint16_t)(1u << SegMap[i]);
//    else                   Segment &= ~(uint16_t)(1u << SegMap[i]);
//  }
//
//  /* SEG17..SEG27 (direct pin macros) */
//  if(data & BIT(14)) SEG17_SET; else SEG17_RST;
//  if(data & BIT(13)) SEG18_SET; else SEG18_RST;
//  if(data & BIT(12)) SEG19_SET; else SEG19_RST;
//  if(data & BIT(11)) SEG20_SET; else SEG20_RST;
//  if(data & BIT(10)) SEG21_SET; else SEG21_RST;
//  if(data & BIT(5))  SEG22_SET; else SEG22_RST;
//  if(data & BIT(4))  SEG23_SET; else SEG23_RST;
//  if(data & BIT(3))  SEG24_SET; else SEG24_RST;
//  if(data & BIT(2))  SEG25_SET; else SEG25_RST;
//  if(data & BIT(1))  SEG26_SET; else SEG26_RST;
//  if(data & BIT(0))  SEG27_SET; else SEG27_RST;
//
//  /* Put all SEG1..SEG16 on the pins BEFORE the COM edge */
//  Set_Data(Segment);
//
//  /* Now switch the COM lines: all segments are stable at this point */
//  COM1_OFF; COM2_OFF; COM3_OFF; COM4_OFF;
//
//  switch(LCD_Duty)
//  {
//    case 0: COM1_SET; break;
//    case 1: COM2_SET; break;
//    case 2: COM3_SET; break;
//    case 3: COM4_SET; break;
//    case 4: COM1_RST; break;
//    case 5: COM2_RST; break;
//    case 6: COM3_RST; break;
//    case 7: COM4_RST; break;
//    default: break;
//  }
//}

/*******************************************************************************
* Function Name  : status_batt
* Description    : Display Battery Voltage Level
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void status_batt(uint8_t Batt_SOC)
//{   
//  if(Batt_SOC >= BATT_LOW)                                                  
//  { 
//    /* CLEAR SEGMENTS */
//    CLEAR_BITS(LCD_Reg[0],14,18);
//    CLEAR_BITS(LCD_Reg[1],14);
//    CLEAR_BITS(LCD_Reg[2],14);
//    CLEAR_BITS(LCD_Reg[3],14);
//    
////    /* Turn on battery bar */
////    LCD_Reg[0] |= BIT(12);            check and add
////    
//    if(Batt_SOC  <= BATT_L1){LCD_Reg[3] |= BIT(14);}
//    else if((Batt_SOC <= BATT_L2)&&(Batt_SOC > BATT_L1)){LCD_Reg[2] |= BIT(14);LCD_Reg[3] |= BIT(14);}
//    else if((Batt_SOC <= BATT_L3)&&(Batt_SOC > BATT_L2)){LCD_Reg[1] |= BIT(13);LCD_Reg[2] |= BIT(13);LCD_Reg[3] |= BIT(13);}
//    else if(Batt_SOC  <= BATT_L4){LCD_Reg[0] |= BIT(13);LCD_Reg[1]  |= BIT(13);LCD_Reg[2] |= BIT(13);LCD_Reg[3] |= BIT(13);}
//    else if(Batt_SOC  >  BATT_L4){LCD_Reg[0] |= BIT(13);LCD_Reg[1]  |= BIT(13)|BIT(12);LCD_Reg[2] |= BIT(13);LCD_Reg[3] |= BIT(13);}
//  }
//  else
//  {
//    /* CLEAR SEGMENTS */
//    CLEAR_BITS(LCD_Reg[0],28,13);
//    CLEAR_BITS(LCD_Reg[1],13,12);
//    CLEAR_BITS(LCD_Reg[2],13);
//    CLEAR_BITS(LCD_Reg[3],13);
//    /* turn on */
//    LCD_Reg[0] |= BIT(28);
//    LCD_Reg[0] ^= BIT(12);    
//  }
//}

void status_batt(uint8_t Batt_SOC)
{
  if(Batt_SOC >= BATT_LOW)
  {
    /* CLEAR SEGMENTS (low-batt symbol + all level bars) */
    CLEAR_BITS(LCD_Reg[0],14,18);
    CLEAR_BITS(LCD_Reg[1],14);
    CLEAR_BITS(LCD_Reg[2],14);
    CLEAR_BITS(LCD_Reg[3],14);

    /* Turn on battery outline */
   // LCD_Reg[0] |= BIT(12);

    if(Batt_SOC <= BATT_L1){LCD_Reg[3] |= BIT(14);} /* bottom bar only */
    else if(Batt_SOC <= BATT_L2)
    {
      LCD_Reg[2] |= BIT(14);
      LCD_Reg[3] |= BIT(14);
    }
    else if(Batt_SOC <= BATT_L3)
    {
      LCD_Reg[1] |= BIT(14);
      LCD_Reg[2] |= BIT(14);
      LCD_Reg[3] |= BIT(14);
    }
    else if(Batt_SOC <= BATT_L4)
    {
      LCD_Reg[0] |= BIT(14);
      LCD_Reg[1] |= BIT(14);
      LCD_Reg[2] |= BIT(14);
      LCD_Reg[3] |= BIT(14);
    }
    else  /* Batt_SOC > BATT_L4 : full */
    {
      LCD_Reg[0] |= BIT(18) | BIT(14);
      LCD_Reg[1] |= BIT(14);
      LCD_Reg[2] |= BIT(14);
      LCD_Reg[3] |= BIT(14);
    }
  }
  else
  {
    /* CLEAR level bars */
    CLEAR_BITS(LCD_Reg[0],14,18);
    CLEAR_BITS(LCD_Reg[1],14);
    CLEAR_BITS(LCD_Reg[2],14);
    CLEAR_BITS(LCD_Reg[3],14);

    /* low battery: symbol on, outline blinks */
  //  LCD_Reg[0] |= BIT(28);
   // LCD_Reg[0] ^= BIT(12);
  }
}

//void DISP_GEAR(uint8_t Gear)
//{
//  
//  
//  /* Clear Gear number */
//  CLEAR_BITS(LCD_Reg[0],0,2);
//  CLEAR_BITS(LCD_Reg[1],0);
//  CLEAR_BITS(LCD_Reg[2],0);
//  CLEAR_BITS(LCD_Reg[3],0);
//  
//  /* Turn on GEAR symbol*/
//  LCD_Reg[2] |= BIT(1);
//  
//  switch(Gear)
//  {
//  case 1:{LCD_Reg[3] |= BIT(0);}break;
//  case 2:{LCD_Reg[2] |= BIT(0);}break;
//  case 3:{LCD_Reg[1] |= BIT(0);}break;
//  case 4:{LCD_Reg[0] |= BIT(0);}break;
//  case 5:{LCD_Reg[0] |= BIT(2);}break;/*Reverse Gear */
//  }
//}

void DISP_GEAR(uint8_t Gear)
{
  
  
  /* Clear Gear number + Reverse + GEAR symbol */
  CLEAR_BITS(LCD_Reg[0],4);
  CLEAR_BITS(LCD_Reg[1],4);
  CLEAR_BITS(LCD_Reg[2],4,5);
  CLEAR_BITS(LCD_Reg[3],4,5);
  
  /* Turn on GEAR symbol*/
  LCD_Reg[3] |= BIT(5);
  
  switch(Gear)
  {
  case 1:{LCD_Reg[3] |= BIT(4);}break;
  case 2:{LCD_Reg[2] |= BIT(4);}break;
  case 3:{LCD_Reg[1] |= BIT(4);}break;
  case 4:{LCD_Reg[0] |= BIT(4);}break;
  case 5:{LCD_Reg[2] |= BIT(5);}break;/*Reverse Gear */
  }
}
void Set_Data(uint16_t Data_Val)
{
  uint16_t  SegMult = 0x01;
  uint8_t Seg_Counter =0 ;
  
  for(Seg_Counter = 0;Seg_Counter < 16; Seg_Counter++)                          // check 16 bits
  {
    if((SegMult & Data_Val)) DATA_595_SET;
    else    DATA_595_RESET;
    /**************** creating clock ************************/
    CLK_595_SET;
    for(int i = 0;i < 500;i++){__NOP();}
    //    mSec_Delay(1);
    CLK_595_RESET;
    for(int i = 0;i < 500;i++){__NOP();}
    //mSec_Delay(1);            // Set Clock Pulse
    /*******************************************************/  
    
    /*************** bit shifting ***********************/
    SegMult = SegMult << 1;				                        // Right Shift Multilplier
    /****************************************************/
  } 					                        	        // Delay to remove ghost Image
  LAT_595_SET;
  for(int i = 0;i < 100;i++){__NOP();}
  LAT_595_RESET;
  for(int i = 0;i < 100;i++){__NOP();}
}
//static inline void tiny_delay(void){ __NOP(); __NOP(); __NOP(); __NOP(); }
//
//void Set_Data(uint16_t Data_Val)
//{
//    LAT_595_RESET;
//    tiny_delay();
//
//    for(int i = 15; i >= 0; i--)
//    {
//        if(Data_Val & (1U << i))
//        {
//            DATA_595_SET;
//        }
//        else
//        {
//            DATA_595_RESET;
//        }
//
//        tiny_delay();
//
//        CLK_595_SET;
//        tiny_delay();
//
//        CLK_595_RESET;
//        tiny_delay();
//    }
//
//    LAT_595_SET;
//
//    for(volatile int k = 0; k < 50; k++)
//    {
//        __NOP();
//    }
//
//    LAT_595_RESET;
//    tiny_delay();
//}

//void DISP_PARKING(void)
//{
//  /*  Clear Speed Segment */
//  /* Digit 1 */
//  CLEAR_BITS(LCD_Reg[0],25);
//  CLEAR_BITS(LCD_Reg[1],25,26); 
//  CLEAR_BITS(LCD_Reg[2],25,26); 
//  CLEAR_BITS(LCD_Reg[3],25,26); 
//  
//  /* Digit 2 */
//  CLEAR_BITS(LCD_Reg[0],23);
//  CLEAR_BITS(LCD_Reg[1],23,24); 
//  CLEAR_BITS(LCD_Reg[2],23,24); 
//  CLEAR_BITS(LCD_Reg[3],23,24); 
//  
////  //  /* Clear Speed Bar */
////  CLEAR_BITS(LCD_Reg[0],6,5,4);
////  CLEAR_BITS(LCD_Reg[1],6,5,4); 
////  CLEAR_BITS(LCD_Reg[2],6,5,4,3); 
////  CLEAR_BITS(LCD_Reg[3],6,5,4,3); 
////  
////  /* Display Speed bar */
////  LCD_Reg[0]  |= BIT(1);
////  
////  /* Write P */
////  LCD_Reg[0] &= ~BIT(27);
////  LCD_Reg[1] |= BIT(28);
////  LCD_Reg[2] |= BIT(28)|BIT(27);
////  LCD_Reg[3] |= BIT(28)|BIT(27);
//}

void DISP_PARKING(void)
{
  /*  Clear Speed Segment (SEG15..SEG18) */
  /* Digit 1 : SEG17 / SEG18 */
  CLEAR_BITS(LCD_Reg[0],17);
  CLEAR_BITS(LCD_Reg[1],17,18); 
  CLEAR_BITS(LCD_Reg[2],17,18); 
  CLEAR_BITS(LCD_Reg[3],17,18); 
  
  /* Digit 2 : SEG15 / SEG16 */
  CLEAR_BITS(LCD_Reg[0],15);
  CLEAR_BITS(LCD_Reg[1],15,16); 
  CLEAR_BITS(LCD_Reg[2],15,16); 
  CLEAR_BITS(LCD_Reg[3],15,16); 
  
//  //  /* Clear Speed Bar */
//  CLEAR_BITS(LCD_Reg[0],6,5,4);
//  CLEAR_BITS(LCD_Reg[1],6,5,4); 
//  CLEAR_BITS(LCD_Reg[2],6,5,4,3); 
//  CLEAR_BITS(LCD_Reg[3],6,5,4,3); 
//  
//  /* Display Speed bar */
//  LCD_Reg[0]  |= BIT(1);
  
  /* Write P on digit 1 (segments a,b,e,f,g), digit 2 stays blank */
  LCD_Reg[1] |= BIT(17)|BIT(18);
  LCD_Reg[2] |= BIT(18);
  LCD_Reg[3] |= BIT(17)|BIT(18);
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




//void DISP_SPEEDBAR (float Vehicle_SPEED)
//{
//  if(Vehicle_SPEED > 0) 
//  { 
//    if(Vehicle_SPEED > 5.0)
//    {
//      LCD_Reg[0] |= BIT(6);
//    } 
//    
//    if(Vehicle_SPEED > 14.0)
//    {
//      LCD_Reg[0] |= BIT(6);
//      LCD_Reg[1] |= BIT(6);
//    } 
//    if(Vehicle_SPEED > 15.0)
//    {
//      LCD_Reg[0] |= BIT(6);
//      LCD_Reg[1] |= BIT(6);
//      LCD_Reg[2] |= BIT(6);
//    }
//    if(Vehicle_SPEED > 20.0)
//    {
//      LCD_Reg[0] |= BIT(6);
//      LCD_Reg[1] |= BIT(6);
//      LCD_Reg[2] |= BIT(6);
//      LCD_Reg[3] |= BIT(6);
//    }
//    if(Vehicle_SPEED > 29.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5);
//      LCD_Reg[1] |= BIT(6);
//      LCD_Reg[2] |= BIT(6);
//      LCD_Reg[3] |= BIT(6);
//      
//    } 
//    if(Vehicle_SPEED > 30.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5);
//      LCD_Reg[1] |= BIT(6)|BIT(5);
//      LCD_Reg[2] |= BIT(6);
//      LCD_Reg[3] |= BIT(6);
//    } 
//    if(Vehicle_SPEED > 40.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5);
//      LCD_Reg[1] |= BIT(6)|BIT(5);
//      LCD_Reg[2] |= BIT(6)|BIT(5);
//      LCD_Reg[3] |= BIT(6);
//    } 
//    if(Vehicle_SPEED > 44.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5);
//      LCD_Reg[1] |= BIT(6)|BIT(5);
//      LCD_Reg[2] |= BIT(6)|BIT(5);
//      LCD_Reg[3] |= BIT(6)|BIT(5);
//    }  
//    if(Vehicle_SPEED > 45.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5);
//      LCD_Reg[1] |= BIT(6)|BIT(5);
//      LCD_Reg[2] |= BIT(6)|BIT(5);
//      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
//    } 
//    if(Vehicle_SPEED > 50.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5);
//      LCD_Reg[1] |= BIT(6)|BIT(5);
//      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4);
//      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
//      
//    } 
//    
//    if(Vehicle_SPEED > 59.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5);
//      LCD_Reg[1] |= BIT(6)|BIT(5)|BIT(4);
//      LCD_Reg[2] |= BIT(6)|BIT(5);
//      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
//    } 
//    
//    if(Vehicle_SPEED > 60.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(4);
//      LCD_Reg[1] |= BIT(6)|BIT(5);
//      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4);
//      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
//    } 
//    
//    if(Vehicle_SPEED > 74.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(4);
//      LCD_Reg[1] |= BIT(6)|BIT(5);
//      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4)|BIT(3);
//      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4);
//      
//    } 
//    
//    if(Vehicle_SPEED > 89.0)
//    {
//      LCD_Reg[0] |= BIT(6)|BIT(5)|BIT(4);
//      LCD_Reg[1] |= BIT(6)|BIT(5)|BIT(4);
//      LCD_Reg[2] |= BIT(6)|BIT(5)|BIT(4)|BIT(3); 
//      LCD_Reg[3] |= BIT(6)|BIT(5)|BIT(4)|BIT(3);
//    }
//  }
//  
//}

void DISP_SPEEDBAR (float Vehicle_SPEED)
{
  /* Clear Speed Bar */
  CLEAR_BITS(LCD_Reg[0],3,2,1,0);
  CLEAR_BITS(LCD_Reg[1],3,2,1,0);
  CLEAR_BITS(LCD_Reg[2],3,2,1,0);
  CLEAR_BITS(LCD_Reg[3],3,2,1,0);

  if(Vehicle_SPEED > 0)
  {
    /* Bit 3 : 0 -> 20 */
    if(Vehicle_SPEED > 4.0) {LCD_Reg[0] |= BIT(3);}
    if(Vehicle_SPEED > 9.0) {LCD_Reg[1] |= BIT(3);}
    if(Vehicle_SPEED > 10.0){LCD_Reg[2] |= BIT(3);}
    if(Vehicle_SPEED > 13.0){LCD_Reg[3] |= BIT(3);}

    /* Bit 2 : 20 -> 30 */
    if(Vehicle_SPEED > 18.0){LCD_Reg[0] |= BIT(2);}
    if(Vehicle_SPEED > 22.0){LCD_Reg[1] |= BIT(2);}
    if(Vehicle_SPEED > 26.0){LCD_Reg[2] |= BIT(2);}
    if(Vehicle_SPEED > 29.0){LCD_Reg[3] |= BIT(2);}

    /* Bit 1 : 30 -> 40 (COM4, COM3) */
    if(Vehicle_SPEED > 32.0){LCD_Reg[3] |= BIT(1);}
    if(Vehicle_SPEED > 36.0){LCD_Reg[2] |= BIT(1);}
    if(Vehicle_SPEED > 39.0){LCD_Reg[1] |= BIT(1);}

    /* Bit 1 : 40 -> 50 (COM2, COM1) */   
    if(Vehicle_SPEED > 44.0){LCD_Reg[0] |= BIT(1);}
     if(Vehicle_SPEED > 47.0){LCD_Reg[0] |= BIT(0);}
    
    /* Bit 0 : 50 -> 60 (COM1, COM2) */   
    if(Vehicle_SPEED > 54.0){LCD_Reg[1] |= BIT(0);}
    if(Vehicle_SPEED > 59.0){LCD_Reg[2] |= BIT(0);}
    
    /* Bit 0 : 60 -> 70 (COM3, COM4) */   
    if(Vehicle_SPEED > 63.0){LCD_Reg[3] |= BIT(0);}
  }
}

void status_batt_soc(uint8_t Batt_SOC)
{
  /* Clear both SOC digits (SEG10..SEG13), COM1 bit 12 (battery outline) is left untouched */
  CLEAR_BITS(LCD_Reg[0],13,11);
  CLEAR_BITS(LCD_Reg[1],13,12,11,10);
  CLEAR_BITS(LCD_Reg[2],13,12,11,10);
  CLEAR_BITS(LCD_Reg[3],13,12,11,10);

  /* Display % icon */
  LCD_Reg[0] |= BIT(10);

  char BV_Buffer[3];
  if(Batt_SOC > 99){Batt_SOC = 99;}
  sprintf(BV_Buffer,"%2d",(char)Batt_SOC);

  /* Tens digit */
  switch(BV_Buffer[0])
  {
  case '0' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(13)|BIT(12); LCD_Reg[2] |= BIT(13);         LCD_Reg[3] |= BIT(13)|BIT(12); break;
  case '1' :                        LCD_Reg[1] |= BIT(12);                                        LCD_Reg[3] |= BIT(12);         break;
  case '2' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(12);         LCD_Reg[2] |= BIT(13)|BIT(12); LCD_Reg[3] |= BIT(13);         break;
  case '3' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(12);         LCD_Reg[2] |= BIT(12);         LCD_Reg[3] |= BIT(13)|BIT(12); break;
  case '4' :                        LCD_Reg[1] |= BIT(13)|BIT(12); LCD_Reg[2] |= BIT(12);         LCD_Reg[3] |= BIT(12);         break;
  case '5' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(13);         LCD_Reg[2] |= BIT(12);         LCD_Reg[3] |= BIT(13)|BIT(12); break;
  case '6' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(13);         LCD_Reg[2] |= BIT(13)|BIT(12); LCD_Reg[3] |= BIT(13)|BIT(12); break;
  case '7' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(12);                                        LCD_Reg[3] |= BIT(12);         break;
  case '8' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(13)|BIT(12); LCD_Reg[2] |= BIT(13)|BIT(12); LCD_Reg[3] |= BIT(13)|BIT(12); break;
  case '9' : LCD_Reg[0] |= BIT(13); LCD_Reg[1] |= BIT(13)|BIT(12); LCD_Reg[2] |= BIT(12);         LCD_Reg[3] |= BIT(13)|BIT(12); break;
  }

  /* Units digit */
  switch(BV_Buffer[1])
  {
  case '0' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(11)|BIT(10); LCD_Reg[2] |= BIT(11);         LCD_Reg[3] |= BIT(11)|BIT(10); break;
  case '1' :                        LCD_Reg[1] |= BIT(10);                                        LCD_Reg[3] |= BIT(10);         break;
  case '2' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(10);         LCD_Reg[2] |= BIT(11)|BIT(10); LCD_Reg[3] |= BIT(11);         break;
  case '3' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(10);         LCD_Reg[2] |= BIT(10);         LCD_Reg[3] |= BIT(11)|BIT(10); break;
  case '4' :                        LCD_Reg[1] |= BIT(11)|BIT(10); LCD_Reg[2] |= BIT(10);         LCD_Reg[3] |= BIT(10);         break;
  case '5' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(11);         LCD_Reg[2] |= BIT(10);         LCD_Reg[3] |= BIT(11)|BIT(10); break;
  case '6' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(11);         LCD_Reg[2] |= BIT(11)|BIT(10); LCD_Reg[3] |= BIT(11)|BIT(10); break;
  case '7' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(10);                                        LCD_Reg[3] |= BIT(10);         break;
  case '8' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(11)|BIT(10); LCD_Reg[2] |= BIT(11)|BIT(10); LCD_Reg[3] |= BIT(11)|BIT(10); break;
  case '9' : LCD_Reg[0] |= BIT(11); LCD_Reg[1] |= BIT(11)|BIT(10); LCD_Reg[2] |= BIT(10);         LCD_Reg[3] |= BIT(11)|BIT(10); break;
  }
}