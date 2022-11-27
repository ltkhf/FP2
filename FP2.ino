
#include <Dynamixel2Arduino.h>
  #define DXL_SERIAL   Serial
  //#define DEBUG_SERIAL soft_serial
  const int DXL_DIR_PIN = 12; // DYNAMIXEL Shield DIR PIN

#define Pb 0//buka pintu lt 1
#define Pb2 1// naik dalam lift
#define Pb3 2//turun dari 2 dalam lift
#define Pb4 3//buka pintu lt 2
bool Pbnow = HIGH;
bool Pblast = LOW;
int ledUp = 4;//hold naik
int ledDown = 5;//hold turun

int naik,turun;
uint8_t tunggu;

const uint8_t DXL_ID0 = 3;
const uint8_t DXL_ID1 = 5;
const float DXL_PROTOCOL_VERSION = 1.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;//actuator.cpp

void door(){
dxl.setGoalPosition(DXL_ID0, 180);
delay(5000);
dxl.setGoalPosition(DXL_ID0, 0);
}

void setup() {
  // put your setup code here, to run once:
  pinMode (Pb,OUTPUT);
  pinMode (Pb2,OUTPUT);
  // Use UART port of DYNAMIXEL Shield to debug.
//  DEBUG_SERIAL.begin(115200);
//  while(!DEBUG_SERIAL);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID0);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID0);
  dxl.setOperatingMode(DXL_ID0, OP_POSITION);
  dxl.setOperatingMode(DXL_ID1, OP_POSITION);
  dxl.torqueOn(DXL_ID0);
    dxl.torqueOff(DXL_ID1);
  dxl.setOperatingMode(DXL_ID1, OP_POSITION);
  dxl.torqueOn(DXL_ID1);

  // Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID0,60 );
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID1,60 );
  dxl.writeControlTableItem(PRESENT_SPEED, DXL_ID1,0 );// set kecepatan sekarang jadi 0
  
}

void loop() {
 //pintu indikatornya
  while (Pb==LOW){// lt1
  dxl.setGoalPosition(DXL_ID0, 0);
  digitalWrite (ledUp,LOW);
  }
  while (Pb==HIGH){//buka pintu DARI LANTAI 1
  Pbnow = digitalRead(Pb);
  if (Pbnow !=Pblast){
    if (Pbnow == HIGH){
      ledUp = true;
      dxl.setGoalPosition(DXL_ID0, 180);//pintu kebuka
      delay(5000);
      }
      Pblast = Pbnow; 
      }
  dxl.setGoalPosition(DXL_ID0, 0);
  }
  
  //penumpang masuk
  if (Pb2==HIGH){//naik dari lift
    dxl.readControlTableItem(DXL_ID1, MOVING_SPEED, 512);//speed dari 0 ke 512
    while (Pb2==HIGH){//NAIK DARI LANTAI 1
  Pbnow = digitalRead(Pb);
  if (Pbnow !=Pblast){
    if (Pbnow == LOW){
      ledUp = true;
    }Pblast=Pbnow;
    }
  }
  door();//lampu indikator sampai ke lt 2
  }
  else {
    dxl.readControlTableItem(DXL_ID1, MOVING_SPEED, 0);
  }
  
tunggu = dxl.getPresentPosition(DXL_ID1);
// dxl.readControlTableItem(DXL_ID1, MOVING_SPEED, 512);//naik 
// dxl.readControlTableItem(DXL_ID1, MOVING_SPEED, -512);//turun

if ((Pb4 == HIGH) && (Pb2 ==LOW)){//turun dari lt2
while ((tunggu <512) && (tunggu>=0)){//tapi posisi mau naik lift nya, masih di antara lt1 dan lt2
dxl.readControlTableItem(DXL_ID1, MOVING_SPEED, 512);
door();
}
 while ((tunggu >-512) && (tunggu<=0)){
dxl.readControlTableItem(DXL_ID1, MOVING_SPEED, -512);//tapi posisi mau turun lift nya, masih di antara lt1 dan lt2
door();

door();
}
  }
}
