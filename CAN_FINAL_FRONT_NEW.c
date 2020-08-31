unsigned long Can_Init_Flags;
unsigned char Can_Send_Flags, Can_Rcv_Flags;           // can flags
unsigned char Rx_Data_Len;                                   // received data length in bytes
char RxTx_Data[8], send_data1[8], send_data2[8], send_data3[8],
                   send_data4[8], send_data5[8], send_data6[8],
                   send_data7[8], send_data8[8], send_data9[8],
                   send_data10[8], send_data11[8], send_data12[8]; // can rx/tx data buffer
char Msg_Rcvd;                                               // reception flag
long Rx_ID;
int i,j,k;
int channels = 0;
short read_array[12];
char high_byte[12], low_byte[12];
int len1,len2,len3;                                        //lenghts of messages

// read function
int read(int read_array[12], int channels)
{
int p =0,q =0;
for(p = 0; p < channels; p++){
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

// formating functions
int format(int array1[8],int array2[8],int array3[8])
{
int p,q;
for (p = 0,q = 0;p < 8;p++,q++){
array1[p] = low_byte[q];
array1[p+1] = high_byte[q];
p++;
}
for (p = 0,q = 0;p < 8;p++,q++){
array2[p] = low_byte[q+4];
array2[p+1] = high_byte[q+4];
p++;
}
for (p = 0,q = 0;p < 8;p++,q++){
array3[p] = low_byte[q+8];
array3[p+1] = high_byte[q+8];
p++;
}
}

//write function
Write(int ID,int send_data,int len ,unsigned long Can_Send_Flags)
{
while(1){
      if( CAN1Write(ID, send_data, len, Can_Send_Flags) != 0xff)
        { break;
        }
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

  CAN1InitializeAdvanced(2,2,7,4,8,Can_Init_Flags, &_GPIO_MODULE_CAN1_PB89);  // Initialize CAN module
  CAN1SetOperationMode(_CAN_OperatingMode_Initialization);
  CANSetFilterScale32(0, _CAN_FILTER_ENABLED & _CAN_FILTER_ID_MASK_MODE & _CAN_FILTER_STD_MSG, 1522, -1);
  CANSetFilterScale32(1, _CAN_FILTER_ENABLED & _CAN_FILTER_ID_MASK_MODE & _CAN_FILTER_STD_MSG, 1532, -1);

  CAN1SetOperationMode(_CAN_OperatingMode_Normal);               // set NORMAL mode

  while (1) {                                                                  // endless loop

    Msg_Rcvd = CAN1Read(0, &Rx_ID , RxTx_Data , &Rx_Data_Len, &Can_Rcv_Flags); // receive message
    if ((Rx_ID==1522) &&Msg_Rcvd)                                                 // if message received
    {
      Delay_us(250);
      
      CAN1Write(1523, send_data1, 8, Can_Send_Flags);     // send message set1
      CAN1Write(1524, send_data2, 8, Can_Send_Flags);
      CAN1Write(1525, send_data3, 8, Can_Send_Flags);
      //Delay_us(300);
      

      Write(1526, send_data4, 8, Can_Send_Flags);     // send message set2
      Write(1527, send_data5, 8, Can_Send_Flags);
      Write(1528, send_data6, 8, Can_Send_Flags);
      //Delay_us(300);
      

      Write(1529, send_data7, 8, Can_Send_Flags);     // send message set3
      Write(1530, send_data8, 8, Can_Send_Flags);
      Write(1531, send_data9, 8, Can_Send_Flags);
      //Delay_us(300);
      

      Write(1532, send_data10, 8, Can_Send_Flags);     // send message set4
      Write(1533, send_data11, 8, Can_Send_Flags);
      Write(1534, send_data12, 8, Can_Send_Flags);
      //Delay_us(300);

      //read for data
      channels = 12;
       read(read_array, channels);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      format(send_data1,send_data2,send_data3);
      
      Delay_ms(5);
      
      //read for data
      channels = 6;
       read(read_array,channels);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      format(send_data4,send_data5,send_data6);

      Delay_ms(5);
      
      //read for data
      channels = 9;
       read(read_array,channels);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      format(send_data7,send_data8,send_data9);

      Delay_ms(5);
      
      //read for data
      channels = 6;
       read(read_array,channels);
      //high and low
      split(read_array, high_byte, low_byte);
      //message formating
      format(send_data10,send_data11,send_data12);

    }
  }

}