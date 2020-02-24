unsigned long Can_Init_Flags;
unsigned char Can_Send_Flags, Can_Rcv_Flags;           // can flags
unsigned char Rx_Data_Len;                                   // received data length in bytes
char RxTx_Data[8], send_data1[8], send_data2[8], send_data3[8]; // can rx/tx data buffer
char Msg_Rcvd;                                               // reception flag
long Rx_ID;
int i,j,k;
short read_array[12];
char high_byte[12], low_byte[12];
int len1,len2,len3;                                        //lenghts of messages

// read function
int read(int read_array[12])
{
int p =0,q =0;
for(p = 0; p < 11; p++){
read_array[p] =  ADC1_Get_Sample(p);
}
}

//split into high and low byte function
int split(int read_array[12],int high_byte[12],int low_byte[12])
{
int p,q;
for (p = 0,q = 0;p < 11,q < 11;p++,q++)
{
low_byte[q]  = (read_array[p]);
high_byte[q] = read_array[p]>>8;
}
}

// formating function
int format(int l1,int l2,int l3)
{
int p,q;
for (p = 0,q = 0;p < l1;p++,q++){
send_data1[p] = low_byte[q];
send_data1[p+1] = high_byte[q];
p++;
}
for (p = 0,q = 0;p < l2;p++,q++){
send_data2[p] = low_byte[q+4];
send_data2[p+1] = high_byte[q+4];
p++;
}
for (p = 0,q = 0;p < l3;p++,q++){
send_data3[p] = low_byte[q+8];
send_data3[p+1] = high_byte[q+8];
p++;
}
}

void main() {

ADC_Set_Input_Channel(_ADC_CHANNEL_0 | _ADC_CHANNEL_1 | _ADC_CHANNEL_2 | _ADC_CHANNEL_3 | _ADC_CHANNEL_4 );
ADC_Set_Input_Channel(_ADC_CHANNEL_5 | _ADC_CHANNEL_6 | _ADC_CHANNEL_7 | _ADC_CHANNEL_8 | _ADC_CHANNEL_9 );
ADC_Set_Input_Channel(_ADC_CHANNEL_10 | _ADC_CHANNEL_11 | _ADC_CHANNEL_12 | _ADC_CHANNEL_13 | _ADC_CHANNEL_14 | _ADC_CHANNEL_15 );

  ADC1_Init();
  ADC2_Init();
  ADC3_Init();
  
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
      //read for data
       read(read_array);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      len1 = 8;
      len2 = 8;
      len3 = 8;
      format(len1,len2,len3);
      
      CAN1Write(1523, send_data1, 8, Can_Send_Flags);     // send message set1
      CAN1Write(1524, send_data2, 8, Can_Send_Flags);
      CAN1Write(1525, send_data3, 8, Can_Send_Flags);
      Delay_ms(5);

      //read for data
      read(read_array);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      len1 = 8;
      len2 = 4;
      len3 = 0;
      format(len1,len2,len3);
      
      CAN1Write(1529, send_data1, 8, Can_Send_Flags);    // send message set2
      CAN1Write(1530, send_data2, 4, Can_Send_Flags);
      Delay_ms(5);

      //read for data
      read(read_array);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      len1 = 8;
      len2 = 4;
      len3 = 6;
      format(len1,len2,len3);
      
      CAN1Write(1533, send_data3, 6, Can_Send_Flags);     //send message set3
      CAN1Write(1529, send_data1, 8, Can_Send_Flags);
      CAN1Write(1530, send_data2, 4, Can_Send_Flags);
      Delay_ms(5);

      //read for data
      read(read_array);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      len1 = 8;
      len2 = 4;
      len3 = 0;
      format(len1,len2,len3);
      
      CAN1Write(1529, send_data1, 8, Can_Send_Flags);     //send message set4
      CAN1Write(1530, send_data2, 4, Can_Send_Flags);

    }
  }
  
}