 

/* Prototype */
void Disp_BattVolt(float Batt_Voltage);
 void Disp_OdoTrip_Meter(float Vehicle_ODO_Meter);
void Disp_Speed(float Vehicle_SPEED);
void Disp_BattCap(int Vehicle_capacity);
void Dis_SpeedBar(void);
void status_batt(uint8_t Batt_SOC);
void Set_Data(uint16_t Data_Val);
void Batt_Temperature(float Batt_Temp);
void DISP_GEAR(uint8_t Gear);
void DISP_PARKING(void);
void DISP_SPEEDBAR (float Vehicle_SPEED);
void DISP_REVERSE(void);
/*------------------------------- MACROS--------------------------------------*/
#define BIT(n)             (1UL << (n))                  /* Create bit mask */
 
/* Combine multiple BITs */
#define BIT_MASK1(a)                (BIT(a))
#define BIT_MASK2(a,b)              (BIT(a) | BIT(b))
#define BIT_MASK3(a,b,c)            (BIT(a) | BIT(b) | BIT(c))
#define BIT_MASK4(a,b,c,d)          (BIT(a) | BIT(b) | BIT(c) | BIT(d))
#define BIT_MASK5(a,b,c,d,e)        (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e))
#define BIT_MASK6(a,b,c,d,e,f)      (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e) | BIT(f))
#define BIT_MASK7(a,b,c,d,e,f,g)    (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e) | BIT(f) | BIT(g))
#define BIT_MASK8(a,b,c,d,e,f,g,h)  (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e) | BIT(f) | BIT(g) | BIT(h))

/* Macro overloading trick */
#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,NAME,...) NAME

/* Main CLEAR_BITS macro (up to 8 bit positions) */
#define CLEAR_BITS(REG, ...)  ((REG) &= ~GET_MACRO(__VA_ARGS__, BIT_MASK8, BIT_MASK7, BIT_MASK6, BIT_MASK5, BIT_MASK4, BIT_MASK3, BIT_MASK2, BIT_MASK1)(__VA_ARGS__))

#define DIS_RW        LCD_Reg[3] |=  BIT(12);
#define CLR_RW        LCD_Reg[3] &= ~BIT(12);  
#define TGL_RW        LCD_Reg[3] ^=  BIT(12);

#define DIS_LW        LCD_Reg[3] |=  BIT(11);
#define CLR_LW        LCD_Reg[3] &= ~BIT(11);  
#define TGL_LW        LCD_Reg[3] ^=  BIT(11);

#define DIS_HL        LCD_Reg[2] |=  BIT(11);
#define CLR_HL        LCD_Reg[2] &= ~BIT(11);  
#define TGL_HL        LCD_Reg[2] ^=  BIT(11);

#define DIS_STAND     LCD_Reg[0] |=  BIT(14);// throttle fail
#define CLR_STAND     LCD_Reg[0] &= ~BIT(14);  
#define TGL_STAND     LCD_Reg[0] ^=  BIT(14);

#define DIS_THROTTLE  LCD_Reg[1] |=  BIT(14);// ecu fail
#define CLR_THROTTLE  LCD_Reg[1] &= ~BIT(14);  
#define TGL_THROTTLE  LCD_Reg[1] ^=  BIT(14);

#define DIS_ECU       LCD_Reg[2] |=  BIT(14);
#define CLR_ECU       LCD_Reg[2] &= ~BIT(14);  
#define TGL_ECU       LCD_Reg[2] ^=  BIT(14);
                      
#define DIS_MOTOR     LCD_Reg[3] |=  BIT(14);//break
#define CLR_MOTOR     LCD_Reg[3] &= ~BIT(14);  
#define TGL_MOTOR     LCD_Reg[3] ^=  BIT(14);
                      
#define DIS_CHG       LCD_Reg[1] |=  BIT(11);
#define CLR_CHG       LCD_Reg[1] &= ~BIT(11);  
#define TGL_CHG       LCD_Reg[1] ^=  BIT(11);

#define DIS_RDY      LCD_Reg[0] |=   BIT(3);
#define CLR_RDY      LCD_Reg[0] &=  ~BIT(3);
#define TGL_RDY      LCD_Reg[0] ^=   BIT(3);

#define CLR_DISP {\
    CLEAR_BITS(LCD_Reg[0],29);\
    CLEAR_BITS(LCD_Reg[1],29,2);\
    CLEAR_BITS(LCD_Reg[2],29,2);\
    CLEAR_BITS(LCD_Reg[3],29,2);\
    CLEAR_BITS(LCD_Reg[0],27);\
    CLEAR_BITS(LCD_Reg[1],28,27);\
    CLEAR_BITS(LCD_Reg[2],28,27);\
    CLEAR_BITS(LCD_Reg[3],28,27);\
    CLEAR_BITS(LCD_Reg[0],6,5,4);\
    CLEAR_BITS(LCD_Reg[1],6,5,4);\
    CLEAR_BITS(LCD_Reg[2],6,5,4,3);\
    CLEAR_BITS(LCD_Reg[3],6,5,4,3)}

#define DISP_R {\
    LCD_Reg[0] |= 0x00;\
    LCD_Reg[1] |= BIT(28) | BIT(27);\
    LCD_Reg[2] |= BIT(28) | BIT(27);\
    LCD_Reg[3] |= BIT(28) | BIT(27);}



#define ALL_SEGS_ON    0xffffffff
#define ALL_SEGS_OFF   0x00000000
 
      
      








// Batt 72V
#define BATT_LOW      10 
#define BATT_L1       30 
#define BATT_L2       40  
#define BATT_L3       50
#define BATT_L4       70

extern 
uint32_t LCD_Reg[5];
 
/*-------------------------ICONS ---------------------------------------*/
/* Break */


// S9 TAK KR DIYE H AAGE KE KRNE H 
/*------------------------------------------------------------------------------ LCD MAPPING ---------------------------------------------------------------------------------
     S1     S2      S3      S4      S5      S6      S7      S8      S9      S10     S11     S12     S13     S14     S15     S16     S17     S18     S19     S20     S21
C1 - 1D      Q5      2D      8A      K1      7A      P2      6A      M4      S50     S60     S70     RW      M3      M2      V       12AFCD  12G     12BE    BVDP    BATT
C2 - 1C      2E      2C      8F      8B      7F      7B      6F      6B      8A      9F      9A      3A      3B      4A      4B      10F     10A     11F     11A     BV100
C3 - 1B      2G      2B      8E      8G      7E      7G      6E      6G      8B      9G      9B      3F      3G      4F      4G      10G     10B     11G     11B     BV75
C4 - 1A      2F      2A      8D      8C      7D      7C      7D      6C      8C      9E      9C      3E      3C      4E      4C      10E     10C     11E     11C     BV50
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

 
  /** Data Interface **/
#define DI_Pin                LL_GPIO_PIN_5
#define DI_GPIO_Port          GPIOA

#define LAT_Pin               LL_GPIO_PIN_6
#define LAT_GPIO_Port         GPIOA

#define CLK_Pin               LL_GPIO_PIN_7
#define CLK_GPIO_Port         GPIOA

/** LCD Communication **/
#define LCD_COM1_PORT         GPIOB
#define LCD_COM1_PIN          LL_GPIO_PIN_5

#define LCD_COM2_PORT         GPIOA
#define LCD_COM2_PIN          LL_GPIO_PIN_4

#define LCD_COM3_PORT         GPIOB
#define LCD_COM3_PIN          LL_GPIO_PIN_1

#define LCD_COM4_PORT         GPIOB
#define LCD_COM4_PIN          LL_GPIO_PIN_10

/** LCD Communication Pull Down **/
#define LCD_COM1N_PORT        GPIOA
#define LCD_COM1N_PIN         LL_GPIO_PIN_3

#define LCD_COM2N_PORT        GPIOB
#define LCD_COM2N_PIN         LL_GPIO_PIN_0

#define LCD_COM3N_PORT        GPIOB
#define LCD_COM3N_PIN         LL_GPIO_PIN_2

#define LCD_COM4N_PORT        GPIOB
#define LCD_COM4N_PIN         LL_GPIO_PIN_11

/** LCD Segment Pins **/
#define LCD_SEG1_PORT         GPIOA
#define LCD_SEG1_PIN          LL_GPIO_PIN_12

#define LCD_SEG2_PORT         GPIOA
#define LCD_SEG2_PIN          LL_GPIO_PIN_11

#define LCD_SEG3_PORT         GPIOA
#define LCD_SEG3_PIN          LL_GPIO_PIN_10

#define LCD_SEG4_PORT         GPIOC
#define LCD_SEG4_PIN          LL_GPIO_PIN_7

#define LCD_SEG5_PORT         GPIOC
#define LCD_SEG5_PIN          LL_GPIO_PIN_6

#define LCD_SEG6_PORT         GPIOA
#define LCD_SEG6_PIN          LL_GPIO_PIN_9

#define LCD_SEG7_PORT         GPIOA
#define LCD_SEG7_PIN          LL_GPIO_PIN_8

#define LCD_SEG8_PORT         GPIOB
#define LCD_SEG8_PIN          LL_GPIO_PIN_15

#define LCD_SEG9_PORT         GPIOB
#define LCD_SEG9_PIN          LL_GPIO_PIN_14

#define LCD_SEG10_PORT        GPIOB
#define LCD_SEG10_PIN         LL_GPIO_PIN_13

#define LCD_SEG11_PORT        GPIOB
#define LCD_SEG11_PIN         LL_GPIO_PIN_12
 

#define LCD_SEG20_PORT        GPIOB
#define LCD_SEG20_PIN         LL_GPIO_PIN_4

#define LCD_SEG21_PORT        GPIOB
#define LCD_SEG21_PIN         LL_GPIO_PIN_3

#define LCD_SEG22_PORT        GPIOD
#define LCD_SEG22_PIN         LL_GPIO_PIN_3

#define LCD_SEG23_PORT        GPIOD
#define LCD_SEG23_PIN         LL_GPIO_PIN_2

#define LCD_SEG24_PORT        GPIOD
#define LCD_SEG24_PIN         LL_GPIO_PIN_1

#define LCD_SEG25_PORT        GPIOD
#define LCD_SEG25_PIN         LL_GPIO_PIN_0

#define LCD_SEG26_PORT        GPIOA
#define LCD_SEG26_PIN         LL_GPIO_PIN_15

#define COM1_SET        { LCD_COM1N_PORT->BSRR = LCD_COM1N_PIN; LCD_COM1_PORT->BSRR = LCD_COM1_PIN; }
#define COM1_RST        { LCD_COM1_PORT->BRR = LCD_COM1_PIN; }
#define COM1_OFF        { LCD_COM1N_PORT->BRR = LCD_COM1N_PIN;  LCD_COM1_PORT->BSRR = LCD_COM1_PIN; }

#define COM2_SET        { LCD_COM2N_PORT->BSRR = LCD_COM2N_PIN; LCD_COM2_PORT->BSRR = LCD_COM2_PIN; }
#define COM2_RST        { LCD_COM2_PORT->BRR = LCD_COM2_PIN; }
#define COM2_OFF        { LCD_COM2N_PORT->BRR = LCD_COM2N_PIN;  LCD_COM2_PORT->BSRR = LCD_COM2_PIN; }

#define COM3_SET        { LCD_COM3N_PORT->BSRR = LCD_COM3N_PIN; LCD_COM3_PORT->BSRR = LCD_COM3_PIN; }
#define COM3_RST        { LCD_COM3_PORT->BRR = LCD_COM3_PIN; }
#define COM3_OFF        { LCD_COM3N_PORT->BRR = LCD_COM3N_PIN;  LCD_COM3_PORT->BSRR = LCD_COM3_PIN; }

#define COM4_SET        { LCD_COM4N_PORT->BSRR = LCD_COM4N_PIN; LCD_COM4_PORT->BSRR = LCD_COM4_PIN; }
#define COM4_RST        { LCD_COM4_PORT->BRR = LCD_COM4_PIN; }
#define COM4_OFF        { LCD_COM4N_PORT->BRR = LCD_COM4N_PIN;  LCD_COM4_PORT->BSRR = LCD_COM4_PIN; }

#define SEG1_SET        LCD_SEG1_PORT->BRR  = LCD_SEG1_PIN
#define SEG1_RST        LCD_SEG1_PORT->BSRR   = LCD_SEG1_PIN

#define SEG2_SET        LCD_SEG2_PORT->BRR  = LCD_SEG2_PIN
#define SEG2_RST        LCD_SEG2_PORT->BSRR   = LCD_SEG2_PIN

#define SEG3_SET        LCD_SEG3_PORT->BRR  = LCD_SEG3_PIN
#define SEG3_RST        LCD_SEG3_PORT->BSRR   = LCD_SEG3_PIN

#define SEG4_SET        LCD_SEG4_PORT->BRR  = LCD_SEG4_PIN
#define SEG4_RST        LCD_SEG4_PORT->BSRR   = LCD_SEG4_PIN

#define SEG5_SET        LCD_SEG5_PORT->BRR  = LCD_SEG5_PIN
#define SEG5_RST        LCD_SEG5_PORT->BSRR   = LCD_SEG5_PIN

#define SEG6_SET        LCD_SEG6_PORT->BRR  = LCD_SEG6_PIN
#define SEG6_RST        LCD_SEG6_PORT->BSRR   = LCD_SEG6_PIN

#define SEG7_SET        LCD_SEG7_PORT->BRR  = LCD_SEG7_PIN
#define SEG7_RST        LCD_SEG7_PORT->BSRR   = LCD_SEG7_PIN

#define SEG8_SET        LCD_SEG8_PORT->BRR  = LCD_SEG8_PIN
#define SEG8_RST        LCD_SEG8_PORT->BSRR   = LCD_SEG8_PIN

#define SEG9_SET        LCD_SEG9_PORT->BRR  = LCD_SEG9_PIN
#define SEG9_RST        LCD_SEG9_PORT->BSRR   = LCD_SEG9_PIN

#define SEG10_SET       LCD_SEG10_PORT->BRR = LCD_SEG10_PIN
#define SEG10_RST       LCD_SEG10_PORT->BSRR  = LCD_SEG10_PIN

#define SEG11_SET       LCD_SEG11_PORT->BRR = LCD_SEG11_PIN
#define SEG11_RST       LCD_SEG11_PORT->BSRR  = LCD_SEG11_PIN

#define SEG12_SET       LCD_SEG12_PORT->BRR = LCD_SEG12_PIN
#define SEG12_RST       LCD_SEG12_PORT->BSRR  = LCD_SEG12_PIN

#define SEG13_SET       LCD_SEG13_PORT->BRR = LCD_SEG13_PIN
#define SEG13_RST       LCD_SEG13_PORT->BSRR  = LCD_SEG13_PIN

#define SEG14_SET       LCD_SEG14_PORT->BRR = LCD_SEG14_PIN
#define SEG14_RST       LCD_SEG14_PORT->BSRR  = LCD_SEG14_PIN

#define SEG15_SET       LCD_SEG15_PORT->BRR = LCD_SEG15_PIN
#define SEG15_RST       LCD_SEG15_PORT->BSRR  = LCD_SEG15_PIN

#define SEG16_SET       LCD_SEG16_PORT->BRR = LCD_SEG16_PIN
#define SEG16_RST       LCD_SEG16_PORT->BSRR  = LCD_SEG16_PIN

#define SEG17_SET       LCD_SEG17_PORT->BRR = LCD_SEG17_PIN
#define SEG17_RST       LCD_SEG17_PORT->BSRR  = LCD_SEG17_PIN

#define SEG18_SET       LCD_SEG18_PORT->BRR = LCD_SEG18_PIN
#define SEG18_RST       LCD_SEG18_PORT->BSRR  = LCD_SEG18_PIN

#define SEG19_SET       LCD_SEG19_PORT->BRR = LCD_SEG19_PIN
#define SEG19_RST       LCD_SEG19_PORT->BSRR  = LCD_SEG19_PIN

#define SEG20_SET       LCD_SEG20_PORT->BRR = LCD_SEG20_PIN
#define SEG20_RST       LCD_SEG20_PORT->BSRR  = LCD_SEG20_PIN

#define SEG21_SET       LCD_SEG21_PORT->BRR = LCD_SEG21_PIN
#define SEG21_RST       LCD_SEG21_PORT->BSRR  = LCD_SEG21_PIN

#define SEG22_SET       LCD_SEG22_PORT->BRR = LCD_SEG22_PIN
#define SEG22_RST       LCD_SEG22_PORT->BSRR  = LCD_SEG22_PIN

#define SEG23_SET       LCD_SEG23_PORT->BRR = LCD_SEG23_PIN
#define SEG23_RST       LCD_SEG23_PORT->BSRR  = LCD_SEG23_PIN

#define SEG24_SET       LCD_SEG24_PORT->BRR = LCD_SEG24_PIN
#define SEG24_RST       LCD_SEG24_PORT->BSRR  = LCD_SEG24_PIN

#define SEG25_SET       LCD_SEG25_PORT->BRR = LCD_SEG25_PIN
#define SEG25_RST       LCD_SEG25_PORT->BSRR  = LCD_SEG25_PIN

#define SEG26_SET       LCD_SEG26_PORT->BRR = LCD_SEG26_PIN
#define SEG26_RST       LCD_SEG26_PORT->BSRR  = LCD_SEG26_PIN





// LCD BKL
 

#define LAT_595_SET                    LAT_GPIO_Port-> BSRR = LAT_Pin            // SET DISPLAY
#define LAT_595_RESET                  LAT_GPIO_Port-> BRR = LAT_Pin             // RESET DISPLAY

#define CLK_595_SET                    CLK_GPIO_Port -> BSRR = CLK_Pin
#define CLK_595_RESET                  CLK_GPIO_Port -> BRR = CLK_Pin

#define DATA_595_SET                   DI_GPIO_Port -> BSRR = DI_Pin
#define DATA_595_RESET                 DI_GPIO_Port -> BRR = DI_Pin




 





