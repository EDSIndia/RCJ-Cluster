

/* MOTOR CONTROLLER TYPE (uncomment it to use ) */
//#define CHINESE     
#define WABOUR
#define TIMER_PIN   LL_GPIO_PIN_7
#define TIMER_PORT  GPIOB
//#define EMF  
//#define ANAND_MINDO
 

#define ARR        0xFFFFF
#define TIMCLOCK   48000000
#define PRESCALAR  48
 

/*--------------------------  SIGNRATURE ID ----------------------------------*/
#define DEVICE_SIGN1           0x08
#define DEVICE_SIGN2           0x61
#define REVERSE_ID             0x04
#define PARKING_ID1            0x40
#define PARKING_ID2            0x44
#define PARKING_ID3            0X02



#define PARKING_MODE           ((ONE_WIRE.Reg[5]  == PARKING_ID1) || (ONE_WIRE.Reg[5] == PARKING_ID2) || (ONE_WIRE.Reg[2] == PARKING_ID3))
#define SIDE_STAND             ((ONE_WIRE.Reg[4]  == 0x21)        || (ONE_WIRE.Reg[4] == 0x22) || (ONE_WIRE.Reg[4] == 0x23))
#define MOTOR_FAIL             ((ONE_WIRE.Reg[3]  == 0x01)        || (ONE_WIRE.Reg[3] == 0x40) || (ONE_WIRE.Reg[3] == 0x41) || (ONE_WIRE.Reg[3] == 0x60)) && (speed_rpm == 0)
#define THROTTLE_FAIL          (((ONE_WIRE.Reg[3] == 0x20)        || (ONE_WIRE.Reg[3] == 0x60)) && (speed_rpm == 0)) 
 
void ONE_WIRE_COMM(void);
void ONE_WIRE_ISR(TIM_TypeDef *TIMx , uint32_t Channel);
  
typedef struct
{
//  FlagStatus
//    Flag_SideStand,                                                             //Flag Sidestand Sensor Activated
//Flag_parking,                                                                   // Auto Parking Flag
//Flag_revspeed;                                                                  // Rev_Gear Speed Flag
//
//    
//    
    
  uint8_t Reg[15],
  data[12];                                                                     // 12 byte data buffer
} typedef_1WIRE_COMM ; 
extern typedef_1WIRE_COMM ONE_WIRE;


extern int
count,
State_val; 

extern uint32_t 
IC_Val1,                                                                    // First Input Capture_Val at first Falling Edge
IC_Val2,                                                                    // Second Input Capture_Val at first Rasing Edge
IC_Val3,                                                                    // Third Input Capture_Val at Second Falling Edge
Difference  ,                                                                 // Time Differnce b/w first two input Capture Value
Difference_2,                                                                 // Time Differnce b/w Second and third input Capture Value
Difference_3;                                                                // Index 0-4 for 5 byte speed buffer

extern FlagStatus
Flag_startbit;
extern float refClock;
extern uint16_t frequency;

extern uint8_t 
bit_index ,
data_index,
sum  ,
current_byte  ,
checksum  ;

extern int
On_time,
Off_time;

  /*initialize variable */
 extern  uint8_t first_high;
 extern  uint8_t StartBit_Frequency;
  


