/******************** (C) COPYRIGHT 2022 EDS-INDIA *****************************
* File Name     : App_RAM.h					       	       *
* Author        : SANTANU ROY                                                  *
* Date          : 26/05/2022                                                   *
* Description   : This file Define all Variables used                          *
* Revision	: Rev0                                                         *
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Application_RAM_H
#define __Application_RAM_H


typedef struct									// DATA STRUCTURE FOR VEHICLE 
 {
    uint16_t  SOC;                                                                  // State of Charge
    char  MODE;                                                                 // Vehicle Mode D/R/N/C
    char GearNo;
    float SPEED;                                                                // Vehicle Speed    
    float BV;                                                                   // Battery Voltage
    uint16_t BC;                                                                   // Battery Capacity
    float EBV;
    float Dist_Meters;                                                          // Distance travlled in meters
    float ODO;                                                                  // Vehicle Odo
    int Batt_TEMP;                                                              //Battery Temperature
    float Trip;								// Vehicle trip meter
    uint16_t MaxTemp;						                // Max temp among the cells
    FlagStatus Charging;
    FlagStatus Ignition;                                                        // Ignition STATUS 
    FlagStatus R_Winker;                                                        // Right Winker
    FlagStatus L_Winker;                                                        // Left Winker
  }Device_InitTypeDef;


extern FDCAN_RxHeaderTypeDef RxHeader;
extern FDCAN_TxHeaderTypeDef TxHeader;
extern Device_InitTypeDef    Vehicle;
extern FDCAN_HandleTypeDef hfdcan1;
extern TAMP_TypeDef hrtc;

extern FlagStatus
Flag_TGL,
Flag_Running,                                                                   // Vehicle Running
Flag_Init,                                                                      // Initialization Flag
Flag_Winker,                                                                    // Blink Winker
Flag_DriveLCD,                                                                  // Drive LCD
Flag_Second,                                                                    // Second Flag
Flag_mSec,                                                                      // Mili Second Flag    
Flag_startbit,                                                                  // Serial data Start-Bit Flag
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
Flag_odomtr,      
Flag_Throttle_Fail,
Flag_ECU_Fail,
Flag_motor_Fail, 
Flag_Display_Update,
Flag_Send_Frame;

extern char 
Batt_Volt,                                                                      //Battery Voltage
DataBufer[1024];                                                                // data buffer 1024 byte

extern unsigned char 

Init_Time,                                                                      // Initialize TIme
Speed_Cnt,                                                                      // Speed MEasurement Counter
CharCnt,                                                                        // Charecter Counter
KM_Travel,                                                                      // 1 KM travel Write Odo
Time_Init,                                                                      // Initialization Time
LCD_CT,                                                                         // Cycle Time
LCD_Duty;                                                                       // LCD Duty Cycle = 1/5

extern unsigned short
Ticks;

//GP_Counter,                                                                     // General Counter
//Dist_Pulse,                                                                     // To calculate the Distance
//Old_Pulse,                                                                      // Odometer Pulse
//Speed_Pulse,                                                                    // Pulse used to calculate speed of vehicle
//New_Speed[10],                                                                  // New Speed MEasured
//Calculate_Speed_Ticks,                                                          // Interval to calculate Speed
                                                                          // Sys Ticks
//Tick_5sec,
//Tick_odo,
//Tick_2msec,
//ODO_Tick;


extern float

BV_ADC,                                                                         // Total sum of 0-20 number's Battery_Voltage (by ADC)
BV,                                                                             // Average Value of 20 number Battery_Volt
Old_ODO,                                                                        // Last Odo meter written
Old_Speed,                                                                      // Old Speed Displayed
Dist_Meters,                                                                    // Distance travlled in meters
//refClock,                                                                       // Time Period
avg_speed,                                                                      // Average Vehicle_Speed
distance,                                                                       // Distance in Meter
speeds[5];                                                                      // Vehicle_Speed Buffer

extern int
first_time,
val_batt,
R_Speed,
speed,                                                                          // Vehicle_Speed in Km/h
//count,                                                                          // Counter Veriable
//State_val,                                                                      // Pulse Level_Status-High/Low
//Off_time,                                                                       // Off Duty Cycle in %
//On_time,                                                                        // On Duty Cycle in %   
//data_index,                                                                     // Index 0-11 for 12 byte data buffer
CAP_MTR;


extern int32_t 
bckup_data;


extern uint64_t 
Temporary;;                                                                     //Temp data Variable


extern int32_t 
bckup_data;


extern uint32_t
read_data,
Timer,                                                                          // Variable for Systick-1 Min/60 Sec
LCD_Reg[5],                                                                     // LCD Segment registers
//IC_Val1,                                                                        // First Input Capture_Val at first Falling Edge
//IC_Val2,                                                                        // Second Input Capture_Val at first Rasing Edge
//IC_Val3,                                                                        // Third Input Capture_Val at Second Falling Edge
//Difference,                                                                     // Time Differnce b/w first two input Capture Value
//Difference_2,                                                                   // Time Differnce b/w Second and third input Capture Value
//Difference_3,                                                                   // Time Differnce b/w First and third input Capture Value
speed_index;                                                                    // Index 0-4 for 5 byte speed buffer


extern uint16_t 
CAPACITY,
//frequency,                                                                      // To Calculate Time Period of a Pulse
speed_rpm;                                                                      // Vehicle_Speed in RPM(Rev./ Min.)

extern uint8_t 
Button_Pressed,
Old_Gear,
wait_Time,
remote_count,
TxData[8],
RxData[8],
data[15],                                                                       // 12 byte data buffer
bit_index,                                                                      // Index 0-7 for One Byte
//current_byte,                                                                   // Current data byte
//checksum,                                                                       // Checksum of all Received data
//sum,                                                                            // Sum(EXOR_Val) of all Received data 
speed_high,                                                                     // MSB of Speed data 
speed_low,                                                                      // LSB of Speed data  
Reg_0,Reg_1,Reg_2,Reg_3,Reg_4,Reg_5,Reg_6,Reg_7,Reg_8,Reg_9,Reg_10;             //11*1 Byte Reg. after verify Checksum

extern __IO uint32_t 
ADC_ConvertedData,
uwMeasuredFrequency;                                                            // To Calculate Time Period of a Pulse

          
                                                              /* ADC group regular conversion data */
extern uint32_t ADC_Read_mVolt;                                                                 /* Value of voltage calculated from ADC conversion data (unit: mV) */

/*----- FLASH PROGRAMMING FOR WRITTING CONFIG TABLE --------------------------*/
#define StartAddr           ((unsigned int)0x08000000)                          // MICRO FLASH START ADDRESS => 20kB
#define EndAddr             ((unsigned int)0x08020000)                          // MICRO FLASH END ADDRESS => 256 KB 

#endif