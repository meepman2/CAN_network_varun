unsigned long Can_Init_Flags;
unsigned char Can_Send_Flags, Can_Rcv_Flags; // can flags
unsigned char Rx_Data_Len;                                   // received data length in bytes
char RxTx_Data[8] , send_data[8];                                          // can rx/tx data buffer
char Msg_Rcvd;                                               // reception flag
const long ID_1st = 1522, ID_2nd = 2 ,ID_22nd = 22, ID_3rd = 3;                       // node IDs
long Rx_ID;
unsigned int result;
char lo, hi ;
int i,j,k;

void main() {
  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_0);
  GPIOB_ODR.b0 = 0;

  Can_Init_Flags = 0;                                       //
  Can_Send_Flags = 0;                                       // clear flags
  Can_Rcv_Flags  = 0;                                       //

  Can_Send_Flags = _CAN_TX_STD_FRAME &                      //     with CANWrite
                   _CAN_TX_NO_RTR_FRAME;

  Can_Init_Flags = _CAN_CONFIG_AUTOMATIC_RETRANSMISSION &          // form value to be used
                   _CAN_CONFIG_RX_FIFO_NOT_LOCKED_ON_OVERRUN &     // with CANInit
                   _CAN_CONFIG_TIME_TRIGGERED_MODE_DISABLED &
                   _CAN_CONFIG_TX_FIFO_PRIORITY_BY_IDINTIFIER &
                   _CAN_CONFIG_WAKE_UP;

  CAN1InitializeAdvanced(1,2,8,3,8,Can_Init_Flags, &_GPIO_MODULE_CAN1_PB89);  // Initialize CAN module
  CAN1SetOperationMode(_CAN_OperatingMode_Initialization);
  CANSetFilterScale32(0, _CAN_FILTER_ENABLED & _CAN_FILTER_ID_MASK_MODE & _CAN_FILTER_STD_MSG, 1522, -1);
  CANSetFilterScale32(1, _CAN_FILTER_ENABLED & _CAN_FILTER_ID_MASK_MODE & _CAN_FILTER_STD_MSG, 1532, -1);

  CAN1SetOperationMode(_CAN_OperatingMode_Normal);               // set NORMAL mode

  while (1) {                                                                  // endless loop

    Msg_Rcvd = CAN1Read(0, &Rx_ID , RxTx_Data , &Rx_Data_Len, &Can_Rcv_Flags); // receive message
    if ((Rx_ID==1522) &&Msg_Rcvd)                                                 // if message received
    {
        for(i = 0; i < 8; i++){
        send_data[i] = 1;
        }

      CAN1Write(1523, send_data, 8, Can_Send_Flags);        // stm1 messages
      CAN1Write(1524, send_data, 8, Can_Send_Flags);
      CAN1Write(1525, send_data, 8, Can_Send_Flags);
      Delay_ms(5);

      GPIOB_ODR.b0 = 1;

      CAN1Write(1529, send_data, 8, Can_Send_Flags);
      CAN1Write(1530, send_data, 4, Can_Send_Flags);
      Delay_ms(5);

      CAN1Write(1533, send_data, 6, Can_Send_Flags);
      CAN1Write(1529, send_data, 8, Can_Send_Flags);
      CAN1Write(1530, send_data, 4, Can_Send_Flags);
      Delay_ms(5);

      CAN1Write(1529, send_data, 8, Can_Send_Flags);
      CAN1Write(1530, send_data, 4, Can_Send_Flags);
      GPIOB_ODR.b0 = 0;

    }
  }
}