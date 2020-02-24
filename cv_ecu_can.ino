#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;

MCP2515 mcp2515(10);


void setup() {

  canMsg1.can_id  = 1520;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0;
  canMsg1.data[1] = 0;
  canMsg1.data[2] = 0;
  canMsg1.data[3] = 0;
  canMsg1.data[4] = 0;
  canMsg1.data[5] = 0;
  canMsg1.data[6] = 0;
  canMsg1.data[7] = 0;

  canMsg2.can_id  = 1521;
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 1;
  canMsg2.data[1] = 1;
  canMsg2.data[2] = 1;
  canMsg2.data[3] = 1;
  canMsg2.data[4] = 1;
  canMsg2.data[5] = 1;
  canMsg2.data[6] = 1;
  canMsg2.data[7] = 1;
  
  canMsg3.can_id  = 1522;
  canMsg3.can_dlc = 8;
  canMsg3.data[0] = 2;
  canMsg3.data[1] = 2;
  canMsg3.data[2] = 2;
  canMsg3.data[3] = 2;
  canMsg3.data[4] = 2;
  canMsg3.data[5] = 2;
  canMsg3.data[6] = 2;
  canMsg3.data[7] = 2;


  
  while (!Serial);
  Serial.begin(9600);
  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {
  
 mcp2515.sendMessage(&canMsg1);

 mcp2515.sendMessage(&canMsg2);

 mcp2515.sendMessage(&canMsg3);
  
  Serial.println("Messages sent");
  delay(20);


  
}
