#include "main.h"

// 
FDCAN_RxHeaderTypeDef   RxHeader;
FDCAN_TxHeaderTypeDef   TxHeader;

/*******************************************************************************
* Function Name  : RxCAN
* Description    : RECEIVE CAN FRAMES
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void RxCAN(void)
{
  switch(RxHeader.Identifier)
  { 
  case 0x02F4: 
    Vehicle.BV=(RxData[1]<<8|RxData[0])/10;                                     //(voltage msb Hex_1 | voltage_lsb Hex_0)*0.1=xxxx volt
    Vehicle.SOC=RxData[4];
    break;
    
  case 0x05F4:
    Vehicle.MaxTemp=(RxData[0])-50;                                             //-50 offset_  Battery temp Hex_1-50= xx *c
    break;
    
  case 0x18F128F4: 
    Vehicle.BC=(RxData[3]<<8|RxData[2])/10;                                     //Total Battery Capacity (AH)_Full Charge 
    break;
    
  case 0x100:
    Vehicle.BV = (RxData[0]<<8|RxData[1])/100; 
    
    break;
  case 0x101:
    
    Vehicle.SOC = (RxData[4]<<8|RxData[5]); 
    Vehicle.BC = (RxData[0]<<8|RxData[1])/100; 
    break;
  case 0x105:
    Vehicle.MaxTemp = (RxData[0]<<8|RxData[1])/100; 
    break;
    
    
  case 0x18FF33D0:  
    Vehicle.BC = (RxData[0]<<8|RxData[1]) /100;
    break;
    
  case 0x18FF2FD2:
    Vehicle.SOC = RxData[2];
    break;
   
  case 0x18FF01D0:
    Vehicle.BV  = (RxData[1] << 24|RxData[2] << 16|RxData[3] << 8|RxData[4])/100;
    break;
    
  case 0x18FF31D0:
    Vehicle.Batt_TEMP = RxData[0];
    break;
    
    
    
  default: break;
  }
}


/*******************************************************************************
* Function Name  : SOKUDO_BATT_REMOTE_FRAME
* Description    : This function is used to send Remote Frame 
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void SOKUDO_BATT_REMOTE_FRAME(void)
{
  if(remote_count)remote_count--;
  else remote_count = 3;
  switch(remote_count)
  {
  case 3:TxHeader.Identifier = 0x100;break;
  case 2:TxHeader.Identifier = 0x101;break;
  case 1:TxHeader.Identifier= 0x105;break;
  }
  TxHeader.IdType              = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType         = FDCAN_REMOTE_FRAME;
  TxHeader.DataLength          = FDCAN_DLC_BYTES_0;
  
  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData); 
}



/*******************************************************************************
* Function Name  : CAN_Management
* Description    : This function is used to send Remote Frame and receive data
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_Management(void)
{
  /* Transmit Remote Frame (Null data )*/
  if(Flag_Send_Frame){Flag_Send_Frame = RESET;SOKUDO_BATT_REMOTE_FRAME();}
  
  /*receive CAN Data and Store in Variable*/
  if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK){RxCAN();}
}




/*******************************************************************************
* Function Name  : FDCAN_Config
* Description    : Receive CAN Frames from Battery 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FDCAN_Config(void)
{
  FDCAN_FilterTypeDef sFilterConfig;

    /* Prepare Tx Header */
  TxHeader.Identifier          = 0x1ED;                                         //default Unique Id. as user wish
  TxHeader.IdType              = FDCAN_EXTENDED_ID;
  TxHeader.TxFrameType         = FDCAN_DATA_FRAME;
  TxHeader.DataLength          = FDCAN_DLC_BYTES_8;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;
  TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;
  TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker       = 0;
  
  
  /* Configure Rx filter */
  sFilterConfig.IdType       = FDCAN_EXTENDED_ID;                               //Standard_small size,EXTENDED_ID_ custom size  as per Protocol decided
  sFilterConfig.FilterIndex  = 1;
  sFilterConfig.FilterType   =  FDCAN_FILTER_RANGE_NO_EIDM;//
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1    = 0x00;
  sFilterConfig.FilterID2    = 0x1FFFFFFF;
  
  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)               //Config the FDCAN Configuration
  {
    Error_Handler();
  }

  /* Start the FDCAN module */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)  //FDCAN Alert Enable
  {
    Error_Handler();
  }
  SOKUDO_BATT_REMOTE_FRAME();
}



/*******************************************************************************
* Function Name  : CANDBC_Data_Extract
* Description    : Extracts and decodes a specific data value from a CAN message 
*                  frame based on bit position, byte order, and value type.
* Input         : uint8_t *can_data - Pointer to the CAN data frame (max 8 bytes)
*                 uint16_t Data_startBit - Bit position where extraction starts
*                 uint8_t Data_Length - Number of bits representing the data value
*                 uint8_t Byte_Order - Defines byte order (0: little-endian, 1: big-endian)
*                 uint8_t Value_Type - Specifies value type (0: unsigned, 1: signed)
*                 float Data_Factor - Scaling factor applied to extracted value
* Output        : None
* Return        : float - Decoded and scaled value
*******************************************************************************/
//float CANDBC_Data_Extract(const uint8_t *data,uint16_t start_bit, uint8_t bit_len,uint8_t byte_order, uint8_t value_type,float factor)
// {
//   
//   
//   
//   
//   
//   
//   
//   
// }




































