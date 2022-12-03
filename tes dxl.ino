/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/
  
  #include <Dynamixel2Arduino.h>
    const int DXL_DIR_PIN = 2;
  
const uint8_t DXL_ID0 = 10;//door
//const uint8_t DXL_ID1 = 2;//updown
//const uint8_t DXL_ID2 = 10;//lroll
const float DXL_PROTOCOL_VERSION = 1.0;

Dynamixel2Arduino dxl(DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup() {
 
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(115200);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
dxl.ping(DXL_ID0);
//dxl.ping(DXL_ID1);
//dxl.ping(DXL_ID2);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID0);
//  dxl.torqueOff(DXL_ID1);
//  dxl.torqueOff(DXL_ID2);
  dxl.setOperatingMode(DXL_ID0, OP_POSITION);
//  dxl.setOperatingMode(DXL_ID1, OP_POSITION);
//  dxl.setOperatingMode(DXL_ID2, OP_POSITION);
  dxl.torqueOn(DXL_ID0);
//  dxl.torqueOn(DXL_ID1);
//  dxl.torqueOn(DXL_ID2);
  // Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
  dxl.writeControlTableItem(MOVING_SPEED, DXL_ID0,100);}
//  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID1, 30);}
//      dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID2, 30);
//}

void loop() {
  // put your main code here, to run repeatedly:
//  dxl.ledOn(DXL_ID0);
//  dxl.ledOn(DXL_ID1);
//  dxl.ledOn(DXL_ID2);
  // Please refer to e-Manual(http://emanual.robotis.com/docs/en/parts/interface/dynamixel_shield/) for available range of value. 
  // Set Goal Position in RAW value
  dxl.setGoalPosition(DXL_ID0, 180);
 // dxl.setGoalPosition(DXL_ID1, 90);
//  dxl.setGoalPosition(DXL_ID2, 1000);
  delay (2000);
  dxl.setGoalPosition(DXL_ID0, 0);
//  dxl.setGoalPosition(DXL_ID1, 0);
//  dxl.setGoalPosition(DXL_ID2, 0);
 delay(1000);
  // dxl.ledOff(DXL_ID0);
 // dxl.ledOff(DXL_ID1);
//  dxl.ledOff(DXL_ID2);
}
