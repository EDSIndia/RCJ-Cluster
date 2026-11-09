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
  /* CLEAR voltage digits only (digit 3,2,1) + V icon + '.' */
  CLEAR_BITS(LCD_Reg[0],24,27,29,25,28,30);
  CLEAR_BITS(LCD_Reg[1],23,24,27,28,29,30);
  CLEAR_BITS(LCD_Reg[2],23,24,27,28,29,30);
  CLEAR_BITS(LCD_Reg[3],23,24,27,28,29,30);

  /* Display Voltage icon */
  LCD_Reg[0] |= BIT(25);

  if(Batt_Voltage < 0){Batt_Voltage = 0;}
  if(Batt_Voltage > 999){Batt_Voltage = 999;}

  /* Convert battery voltage to string format (e.g., "12.3") */
  char BV_Buffer[5];
  if(Batt_Voltage >= 100){sprintf(BV_Buffer, "%3d",(int)Batt_Voltage);}   /* 100 and above : no decimal */
  else
  {
    LCD_Reg[0] |= BIT(28);                                                /* Display '.' */
    sprintf(BV_Buffer, "%3d", (int)(Batt_Voltage * 10));
    if(BV_Buffer[1] == ' ')BV_Buffer[1] = '0';                            /* 0.5 -> " 0.5" */
  }

  /* Tens digit : digit 3 (SEG23 / SEG24) */
  switch(BV_Buffer[0])
  {
  case '0' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '1' :                        LCD_Reg[1] |= BIT(23);                                       LCD_Reg[3] |= BIT(23);         break;
  case '2' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(24)|BIT(23); LCD_Reg[3] |= BIT(24);         break;
  case '3' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '4' :                        LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(23);         break;
  case '5' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '6' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(24)|BIT(23); LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '7' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(23);                                       LCD_Reg[3] |= BIT(23);         break;
  case '8' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24)|BIT(23); LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '9' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  default:break;
  }

  /* Units digit : digit 2 (SEG27 / SEG28) */
  switch(BV_Buffer[1])
  {
  case '0' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(27);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '1' :                        LCD_Reg[1] |= BIT(28);                                       LCD_Reg[3] |= BIT(28);         break;
  case '2' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(28);        LCD_Reg[2] |= BIT(27)|BIT(28); LCD_Reg[3] |= BIT(27);         break;
  case '3' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(28);        LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '4' :                        LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(28);         break;
  case '5' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '6' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(27)|BIT(28); LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '7' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(28);                                       LCD_Reg[3] |= BIT(28);         break;
  case '8' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(27)|BIT(28); LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '9' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  default:break;
  }

  /* Tenths digit : digit 1 (SEG29 / SEG30) */
  switch(BV_Buffer[2])
  {
  case '0' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(29);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '1' :                        LCD_Reg[1] |= BIT(30);                                       LCD_Reg[3] |= BIT(30);         break;
  case '2' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(30);        LCD_Reg[2] |= BIT(29)|BIT(30); LCD_Reg[3] |= BIT(29);         break;
  case '3' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(30);        LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '4' :                        LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(30);         break;
  case '5' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29);        LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '6' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29);        LCD_Reg[2] |= BIT(29)|BIT(30); LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '7' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(30);                                       LCD_Reg[3] |= BIT(30);         break;
  case '8' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(29)|BIT(30); LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '9' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
  default:break;
  }
}
/*******************************************************************************
* Function Name  : Batt_Temperature
* Description    : Display The Battery Temperature (Celsius, 0.1 resolution)
* Input          : (float )Batt_Temp
* Output         : None
* Return         : None
*******************************************************************************/
void Batt_Temperature(float Batt_Temp)
{
  /* CLEAR temperature digits only (digit 6,5,4) + degC icon + '.' */
  CLEAR_BITS(LCD_Reg[0],22,20,26,21,19);
  CLEAR_BITS(LCD_Reg[1],19,20,21,22,25,26);
  CLEAR_BITS(LCD_Reg[2],19,20,21,22,25,26);
  CLEAR_BITS(LCD_Reg[3],19,20,21,22,25,26);

  /* Display Celsius icon */
  LCD_Reg[0] |= BIT(21);

  /* Display '.' (between digit 5 and 4) */
  LCD_Reg[0] |= BIT(19);

  if(Batt_Temp < 0){Batt_Temp = 0;}          /* no minus segment in bitmap */
  if(Batt_Temp > 99.9){Batt_Temp = 99.9;}

  char Temp_Buffer[5];
  sprintf(Temp_Buffer, "%3d", (int)(Batt_Temp * 10));
  if(Temp_Buffer[1] == ' ')Temp_Buffer[1] = '0';     /* 0.5 -> " 0.5" */

  /* Tens digit : digit 6 (SEG21 / SEG22) */
  switch(Temp_Buffer[0])
  {
  case '0' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22);         LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '1' :                        LCD_Reg[1] |= BIT(21);                                       LCD_Reg[3] |= BIT(21);         break;
  case '2' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(22)|BIT(21); LCD_Reg[3] |= BIT(22);         break;
  case '3' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '4' :                        LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(21);         break;
  case '5' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '6' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(22)|BIT(21); LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '7' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(21);                                       LCD_Reg[3] |= BIT(21);         break;
  case '8' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22)|BIT(21); LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '9' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(22)|BIT(21); break;
  default:break;
  }

  /* Units digit : digit 5 (SEG19 / SEG20) */
  switch(Temp_Buffer[1])
  {
  case '0' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(20);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '1' :                        LCD_Reg[1] |= BIT(19);                                       LCD_Reg[3] |= BIT(19);         break;
  case '2' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= BIT(20)|BIT(19); LCD_Reg[3] |= BIT(20);         break;
  case '3' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '4' :                        LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(19);         break;
  case '5' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20);        LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '6' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20);        LCD_Reg[2] |= BIT(20)|BIT(19); LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '7' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(19);                                       LCD_Reg[3] |= BIT(19);         break;
  case '8' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(20)|BIT(19); LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '9' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  default:break;
  }

  /* Tenths digit : digit 4 (SEG25 / SEG26) */
  switch(Temp_Buffer[2])
  {
  case '0' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '1' :                        LCD_Reg[1] |= BIT(25);                                       LCD_Reg[3] |= BIT(25);         break;
  case '2' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(26)|BIT(25); LCD_Reg[3] |= BIT(26);         break;
  case '3' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '4' :                        LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(25);         break;
  case '5' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '6' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(26)|BIT(25); LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '7' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(25);                                       LCD_Reg[3] |= BIT(25);         break;
  case '8' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26)|BIT(25); LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '9' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
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
  /* Clear both speed digits */
  CLEAR_BITS(LCD_Reg[0],17,15);
  CLEAR_BITS(LCD_Reg[1],18,17,16,15);
  CLEAR_BITS(LCD_Reg[2],18,17,16,15);
  CLEAR_BITS(LCD_Reg[3],18,17,16,15);
 
  /* Display (mph km/h mile km) icon */
  LCD_Reg[0] |= BIT(23);   
 
  /* Display Speed bar */
  LCD_Reg[0] |= BIT(5);
 
  DISP_SPEEDBAR(Vehicle_SPEED);
  if(Vehicle_SPEED > 99){ Vehicle_SPEED =99;}
  
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
  CLEAR_BITS(LCD_Reg[0],22,20,26,24,27,29);
  CLEAR_BITS(LCD_Reg[0],21,25,28);
  CLEAR_BITS(LCD_Reg[1],19,20,21,22,23,24);
  CLEAR_BITS(LCD_Reg[1],25,26,27,28,29,30);
  CLEAR_BITS(LCD_Reg[2],19,20,21,22,23,24);
  CLEAR_BITS(LCD_Reg[2],25,26,27,28,29,30);
  CLEAR_BITS(LCD_Reg[3],19,20,21,22,23,24);
  CLEAR_BITS(LCD_Reg[3],25,26,27,28,29,30);
  
  
  char ODO_Buffer[7];
  int ODO_MTR = 0;
  
  /* Max 99999.9, after that wrap to 0 */
  if(Vehicle_ODO_TRIP_Meter >= 99999.9){Vehicle_ODO_TRIP_Meter = 0;}
  
  /* Always 0.1 resolution, decimal point always ON */
  ODO_MTR = (uint32_t)(Vehicle_ODO_TRIP_Meter *10);
  LCD_Reg[0] |= BIT(28);
  LCD_Reg[0] |= BIT(30);
  
  sprintf(ODO_Buffer,"%06d",(uint32_t)ODO_MTR);
  if(ODO_Buffer[3] == ' ')ODO_Buffer[3] = '0';
  
  /* Digit 6 : SEG21 / SEG22 */
  switch(ODO_Buffer[0])
  {
  case '0' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21); LCD_Reg[2] |= BIT(22);        LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '1' :                        LCD_Reg[1] |= BIT(21);                                       LCD_Reg[3] |= BIT(21);         break;
  case '2' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(22)|BIT(21); LCD_Reg[3] |= BIT(22);         break;
  case '3' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(21);        LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '4' :                        LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(21);         break;
  case '5' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '6' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22);        LCD_Reg[2] |= BIT(22)|BIT(21); LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '7' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(21);                                       LCD_Reg[3] |= BIT(21);         break;
  case '8' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(22)|BIT(21); LCD_Reg[3] |= BIT(22)|BIT(21); break;
  case '9' : LCD_Reg[0] |= BIT(22); LCD_Reg[1] |= BIT(22)|BIT(21);LCD_Reg[2] |= BIT(21);         LCD_Reg[3] |= BIT(22)|BIT(21); break;
  default:break;
  }
  
  /* Digit 5 : SEG19 / SEG20 */
  switch(ODO_Buffer[1])
  {
  case '0' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(20);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '1' :                        LCD_Reg[1] |= BIT(19);                                       LCD_Reg[3] |= BIT(19);         break;
  case '2' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= BIT(20)|BIT(19); LCD_Reg[3] |= BIT(20);         break;
  case '3' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(19);        LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '4' :                        LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(19);         break;
  case '5' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20);        LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '6' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20);        LCD_Reg[2] |= BIT(20)|BIT(19); LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '7' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(19);                                       LCD_Reg[3] |= BIT(19);         break;
  case '8' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(20)|BIT(19); LCD_Reg[3] |= BIT(20)|BIT(19); break;
  case '9' : LCD_Reg[0] |= BIT(20); LCD_Reg[1] |= BIT(20)|BIT(19);LCD_Reg[2] |= BIT(19);         LCD_Reg[3] |= BIT(20)|BIT(19); break;
  default:break;
  }
  
  /* Digit 4 : SEG25 / SEG26 */
  switch(ODO_Buffer[2])
  { 
  case '0' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '1' :                        LCD_Reg[1] |= BIT(25);                                       LCD_Reg[3] |= BIT(25);         break;
  case '2' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(26)|BIT(25); LCD_Reg[3] |= BIT(26);         break;
  case '3' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(25);        LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '4' :                        LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(25);         break;
  case '5' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '6' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26);        LCD_Reg[2] |= BIT(26)|BIT(25); LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '7' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(25);                                       LCD_Reg[3] |= BIT(25);         break;
  case '8' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(26)|BIT(25); LCD_Reg[3] |= BIT(26)|BIT(25); break;
  case '9' : LCD_Reg[0] |= BIT(26); LCD_Reg[1] |= BIT(26)|BIT(25);LCD_Reg[2] |= BIT(25);         LCD_Reg[3] |= BIT(26)|BIT(25); break;
  default:break;
  }
  
  /* Digit 3 : SEG23 / SEG24 */
  switch(ODO_Buffer[3])
  {
  case '0' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '1' :                        LCD_Reg[1] |= BIT(23);                                       LCD_Reg[3] |= BIT(23);         break;
  case '2' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(24)|BIT(23); LCD_Reg[3] |= BIT(24);         break;
  case '3' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(23);        LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '4' :                        LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(23);         break;
  case '5' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '6' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24);        LCD_Reg[2] |= BIT(24)|BIT(23); LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '7' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(23);                                       LCD_Reg[3] |= BIT(23);         break;
  case '8' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(24)|BIT(23); LCD_Reg[3] |= BIT(24)|BIT(23); break;
  case '9' : LCD_Reg[0] |= BIT(24); LCD_Reg[1] |= BIT(24)|BIT(23);LCD_Reg[2] |= BIT(23);         LCD_Reg[3] |= BIT(24)|BIT(23); break;
  default:break;
  }
  
  /* Digit 2 : SEG27 / SEG28 (right segments use bit 28, left use bit 27) */
  switch(ODO_Buffer[4])
  {
  case '0' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(27);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '1' :                        LCD_Reg[1] |= BIT(28);                                       LCD_Reg[3] |= BIT(28);         break;
  case '2' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(28);        LCD_Reg[2] |= BIT(27)|BIT(28); LCD_Reg[3] |= BIT(27);         break;
  case '3' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(28);        LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '4' :                        LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(28);         break;
  case '5' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '6' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27);        LCD_Reg[2] |= BIT(27)|BIT(28); LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '7' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(28);                                       LCD_Reg[3] |= BIT(28);         break;
  case '8' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(27)|BIT(28); LCD_Reg[3] |= BIT(27)|BIT(28); break;
  case '9' : LCD_Reg[0] |= BIT(27); LCD_Reg[1] |= BIT(27)|BIT(28);LCD_Reg[2] |= BIT(28);         LCD_Reg[3] |= BIT(27)|BIT(28); break;
  default:break;
  } 
  
  /* Digit 1 : SEG29 / SEG30 (right segments use bit 30, left use bit 29) */
  switch(ODO_Buffer[5])
  {
  case '0' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(29);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '1' :                        LCD_Reg[1] |= BIT(30);                                       LCD_Reg[3] |= BIT(30);         break;
  case '2' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(30);        LCD_Reg[2] |= BIT(29)|BIT(30); LCD_Reg[3] |= BIT(29);         break;
  case '3' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(30);        LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '4' :                        LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(30);         break;
  case '5' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29);        LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '6' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29);        LCD_Reg[2] |= BIT(29)|BIT(30); LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '7' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(30);                                       LCD_Reg[3] |= BIT(30);         break;
  case '8' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(29)|BIT(30); LCD_Reg[3] |= BIT(29)|BIT(30); break;
  case '9' : LCD_Reg[0] |= BIT(29); LCD_Reg[1] |= BIT(29)|BIT(30);LCD_Reg[2] |= BIT(30);         LCD_Reg[3] |= BIT(29)|BIT(30); break;
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
//  for(int i = 0;i < 1000; i++);
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
    /* CLEAR SEGMENTS (low-batt symbol + all level bars) */
    CLEAR_BITS(LCD_Reg[0],14,18);
    CLEAR_BITS(LCD_Reg[1],14);
    CLEAR_BITS(LCD_Reg[2],14);
    CLEAR_BITS(LCD_Reg[3],14);

    /* Turn on battery outline */
    LCD_Reg[0] |= BIT(16);
    LCD_Reg[0] |= BIT(12);

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
    
    if(Flag_TGL) 
    {
      LCD_Reg[0] |= BIT(16);    
      LCD_Reg[3] |= BIT(14);
    }
    else
    {
      LCD_Reg[0] &= ~BIT(16);
      LCD_Reg[3] &= ~BIT(14);
    }
  }
}
/*******************************************************************************
* Function Name  : DISP_GEAR
* Description    : Display The Gear Symbol and Gear Number (1 - 4, 5 = Reverse)
* Input          : (uint8_t )Gear
* Output         : None
* Return         : None
*******************************************************************************/
void DISP_GEAR(uint8_t Gear)
{ 
  /* Clear Gear number  */
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

/*******************************************************************************
* Function Name  : Set_Data
* Description    : Shift 16 bit segment data out to the 595 IC (LSB first) and latch
* Input          : (uint16_t )Data_Val
* Output         : None
* Return         : None
*******************************************************************************/
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
    for(int i = 0;i < 100;i++){__NOP();}
    //    mSec_Delay(1);
    CLK_595_RESET;
    for(int i = 0;i < 100;i++){__NOP();}
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

/*******************************************************************************
* Function Name  : DISP_PARKING
* Description    : Display "P" (Parking) on the speed digits
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DISP_PARKING(void)
{
  /*  Clear Speed Segment */
  /* Digit 1 : */
  CLEAR_BITS(LCD_Reg[0],17);
  CLEAR_BITS(LCD_Reg[1],17,18); 
  CLEAR_BITS(LCD_Reg[2],17,18); 
  CLEAR_BITS(LCD_Reg[3],17,18); 
  
  /* Digit 2 :  */
  CLEAR_BITS(LCD_Reg[0],15);
  CLEAR_BITS(LCD_Reg[1],15,16); 
  CLEAR_BITS(LCD_Reg[2],15,16); 
  CLEAR_BITS(LCD_Reg[3],15,16); 
  
   /* Clear Speed Bar */
  CLEAR_BITS(LCD_Reg[0],3,2,1,0);
  CLEAR_BITS(LCD_Reg[1],3,2,1,0);
  CLEAR_BITS(LCD_Reg[2],3,2,1,0);
  CLEAR_BITS(LCD_Reg[3],3,2,1,0);

  /* Write P on digit 1 , digit 2 blank */
  LCD_Reg[1] |= BIT(18);
  LCD_Reg[2] |= BIT(17)|BIT(18);
  LCD_Reg[3] |= BIT(17)|BIT(18);
}
/*******************************************************************************
* Function Name  : DISP_REVERSE
* Description    : Display "R" (Reverse) when vehicle speed is 0, otherwise display speed
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
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
/*******************************************************************************
* Function Name  : DISP_SPEEDBAR
* Description    : Display Speed Bar (arc segments 0 - 70 Km/h)
* Input          : (float )Vehicle_SPEED
* Output         : None
* Return         : None
*******************************************************************************/
void DISP_SPEEDBAR (float Vehicle_SPEED)
{
  /* Clear Speed Bar */
  CLEAR_BITS(LCD_Reg[0],3,2,1,0);
  CLEAR_BITS(LCD_Reg[1],3,2,1,0);
  CLEAR_BITS(LCD_Reg[2],3,2,1,0);
  CLEAR_BITS(LCD_Reg[3],3,2,1,0);

  if(Vehicle_SPEED > 0)
  {
    /*  0 -> 20 */
    if(Vehicle_SPEED > 4.0) {LCD_Reg[0] |= BIT(3);}
    if(Vehicle_SPEED > 9.0) {LCD_Reg[1] |= BIT(3);}
    if(Vehicle_SPEED > 10.0){LCD_Reg[2] |= BIT(3);}
    if(Vehicle_SPEED > 13.0){LCD_Reg[3] |= BIT(3);}

    /*  20 -> 30 */
    if(Vehicle_SPEED > 18.0){LCD_Reg[0] |= BIT(2);}
    if(Vehicle_SPEED > 22.0){LCD_Reg[1] |= BIT(2);}
    if(Vehicle_SPEED > 26.0){LCD_Reg[2] |= BIT(2);}
    if(Vehicle_SPEED > 29.0){LCD_Reg[3] |= BIT(2);}

    /*  30 -> 40 (COM4, COM3) */
    if(Vehicle_SPEED > 32.0){LCD_Reg[3] |= BIT(1);}
    if(Vehicle_SPEED > 36.0){LCD_Reg[2] |= BIT(1);}
    if(Vehicle_SPEED > 39.0){LCD_Reg[1] |= BIT(1);}

    /*  40 -> 50 (COM2, COM1) */   
    if(Vehicle_SPEED > 44.0){LCD_Reg[0] |= BIT(1);}
     if(Vehicle_SPEED > 47.0){LCD_Reg[0] |= BIT(0);}
    
    /*  50 -> 60 (COM1, COM2) */   
    if(Vehicle_SPEED > 54.0){LCD_Reg[1] |= BIT(0);}
    if(Vehicle_SPEED > 59.0){LCD_Reg[2] |= BIT(0);}
    
    /*  60 -> 70 (COM3, COM4) */   
    if(Vehicle_SPEED > 63.0){LCD_Reg[3] |= BIT(0);}
  }
}
/*******************************************************************************
* Function Name  : status_batt_soc
* Description    : Display Battery SOC in percentage (2 digits + % icon, max 99)
* Input          : (uint8_t )Batt_SOC
* Output         : None
* Return         : None
*******************************************************************************/
void status_batt_soc(uint8_t Batt_SOC)
{
  /* Clear both SOC digits */
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