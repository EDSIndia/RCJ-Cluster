/******************** (C) COPYRIGHT 2022 EDS-INDIA *****************************
* File Name     : App_RAM.c					       	       *
* Author        : SANTANU ROY                                                  *
* Date          : 26/02/2022                                                   *
* Description   : This file Define all Variables used                          *
* Revision	: Rev0                                                         *
*******************************************************************************/

#include "Main.h"

Device_InitTypeDef      Vehicle;
 

TAMP_TypeDef hrtc;

/*----- FLAGS ----------------------------------------------------------------*/
FlagStatus
Flag_Running,                                                                   // Vehicle Running
Flag_Init,                                                                      // Initialization Flag
Flag_Winker,                                                                    // Blink Winker
Flag_DriveLCD,                                                                  // Drive LCD
Flag_Second,                                                                    // Second Flag
Flag_mSec,                                                                      // Mili Second Flag
//Flag_startbit,                                                                  // Serial data Start-Bit Flag
Flag_revgear,                                                                   // Rev_Gear Flag
Flag_parking,                                                                   // Auto Parking Flag
Flag_odorst,                                                                    // ODOMETER Reset Flag
Flag_revspeed,                                                                  // Rev_Gear Speed Flag
Flag_loop,                                                                      // Loop Flag
Flag_timer,                                                                     // Timer_Systick Flag
Flag_5Second,                                                                   // 5 Second Flag,Using for toggle, ODOMETER Value and Range(KM)
Flag_OdoWR,                                                                     //Flag Odometer Write
Flag_volt,                                                                      //Flag Battery Voltage in Volt
Flag_BatteryLow,                                                                //Flag Battery LOw    
Flag_SideStand,                                                                 //Flag Sidestand Sensor Activated
Flag_Throttle_Fail,
Flag_ECU_Fail,
Flag_motor_Fail,
Flag_Display_Update,
Flag_odomtr;                                                                    //Flag Odometer 

char 
Batt_Volt,                                                                      //Battery Voltage
DataBufer[1024];                                                                // data buffer 1024 byte

unsigned char

Init_Time = 0,                                                                      // Initialize TIme
Speed_Cnt = 0,                                                                      // Speed MEasurement Counter
CharCnt = 0,                                                                        // Charecter Counter
KM_Travel = 0,                                                                      // 1 KM travel Write Odo
Time_Init = 0,                                                                      // Initialization Time
LCD_CT = 0;                                                                  // LCD Duty Cycle = 1/5

uint16_t CAPACITY;
 unsigned short
Ticks;
float

BV_ADC,                                                                         // Total sum of 0-20 number's Battery_Voltage (by ADC)
BV,                                                                             // Average Value of 20 number Battery_Volt
Old_ODO,                                                                        // Last Odo meter written
Old_Speed = 0,                                                                      // Old Speed Displayed
Dist_Meters,                                                                    // Distance travlled in meters
//refClock=0,                                                                     // Time Period
avg_speed = 0,                                                                  // Average Vehicle_Speed 
distance = 0,                                                                   // Distance in Meter
speeds[5] = {0};                                                                // Vehicle_Speed Buffer

int 
first_time=0,
val_batt,
R_Speed=0,
speed=1,                                                                        // Vehicle_Speed in Km/h
//count=0,                                                                        // Counter Veriable
//State_val,                                                                      // Pulse Level_Status-High/Low
//Off_time=0,                                                                     // Off Duty Cycle in %                                        
//On_time=0,                                                                      // On Duty Cycle in %   
//data_index = 0,                                                                 // Index 0-11 for 12 byte data buffer
CAP_MTR = 0;


int32_t 
bckup_data=0;                                                                   //Backup Data

uint64_t 
Temporary;                                                                      //Temp data Variable

FlagStatus
Flag_TGL,
Flag_Send_Frame;
uint32_t
read_data=0,                                                                    //Read Data
Timer,                                                                          // Variable for Systick-1 Min/60 Sec
//LCD_Reg[5] = {0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF},               // LCD Segment registers
//IC_Val1 = 0,                                                                    // First Input Capture_Val at first Falling Edge
//IC_Val2 = 0,                                                                    // Second Input Capture_Val at first Rasing Edge
//IC_Val3 = 0,                                                                    // Third Input Capture_Val at Second Falling Edge
//Difference = 0,                                                                 // Time Differnce b/w first two input Capture Value
//Difference_2=0,                                                                 // Time Differnce b/w Second and third input Capture Value
//Difference_3=0,                                                                 // Time Differnce b/w First and third input Capture Value
speed_index = 0;                                                                // Index 0-4 for 5 byte speed buffer

uint16_t 
//frequency=0,                                                                    // To Calculate Time Period of a Pulse
speed_rpm=0;                                                                    // Vehicle_Speed in RPM(Rev./ Min.)

uint8_t 
Old_Gear,
wait_Time = 0,
remote_count = 0,
TxData[8] = "08122023",                                                                      //Temp Buffer for FDCAN Data TX
RxData[8],                                                                      //Temp Buffer for FDCAN Data Receiving
data[15],                                                                       // 12 byte data buffer
//bit_index = 0,                                                                  // Index 0-7 for One Byte
//current_byte = 0,                                                               // Current data byte
//checksum,                                                                       // Checksum of all Received data
//sum,                                                                            // Sum(EXOR_Val) of all Received data 
speed_high=0,                                                                   // MSB of Speed data 
speed_low=0,                                                                    // LSB of Speed data  
Reg_0=0,Reg_1=0,Reg_2=0,Reg_3=0,Reg_4=0,Reg_5=0,Reg_6=0,Reg_7=0,Reg_8=0,Reg_9=0,Reg_10=0;  //11*1 Byte Reg. after verify Checksum
  

__IO uint32_t 
ADC_ConvertedData,                                                              //Read ADC Value
uwMeasuredFrequency = 0;                                                        // To Calculate Time Period of a Pulse

/* Variables for ADC conversion data */
uint32_t    ADC_Read_mVolt  = 0;                                                           /* Value of voltage calculated from ADC conversion data (unit: mV) */


uint8_t  
Button_Pressed = 0;