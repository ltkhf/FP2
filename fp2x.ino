#include <Dynamixel2Arduino.h>

#define DXL_SERIAL   Serial
//#define DEBUG_SERIAL soft_serial
const int DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN/DATA
int pb0 =3;//buka pintu lt 1
int pb1 =4;// naik dalam lift
int pb2 =5;//turun dari 2 dalam lift
int pb3 =6;//buka pintu lt 2
bool ditekan =false;
bool p1last = HIGH;
bool p1now =LOW;

uint8_t tunggu;

const uint8_t DXL_ID0 = 1;//door
const uint8_t DXL_ID1 = 2;//updown
const uint8_t DXL_ID2 = 10;//lroll
const float DXL_PROTOCOL_VERSION = 1.0;

Dynamixel2Arduino dxl(DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;//actuator.cpp

//sensor
const byte sensor = 7;
int nilai;

//set pos
int buka=1023;//0~4095
int tutup=0;//0~4095
int high=16390;//-28,672 ~ 28,672
int low=3002;//-28,672 ~ 28,672

void door() {
  dxl.setGoalPosition(DXL_ID0, buka); //pintu di buka
  dxl.ledOn(DXL_ID0);

  //ditambahin sensor
//  nilai = digitalRead(sensor);
//  if (nilai == true ) { //sensor deteksi
//    delay (2000);
//    while (nilai == true) { //nunggu
//    }
//  }    else {//sensor ga deteksi
    dxl.setGoalPosition(DXL_ID0, 0);
    dxl.ledOff(DXL_ID0);
    delay (200);
  }

void setup() {
  Serial.begin (115200);
  pinMode (pb0 ,INPUT_PULLUP);
  pinMode (pb1, INPUT_PULLUP);
  pinMode (pb2, INPUT_PULLUP);
  pinMode (pb3, INPUT_PULLUP);
  pinMode (sensor, INPUT);

  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID0); dxl.ping(DXL_ID1); dxl.ping(DXL_ID2);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID0);
  dxl.torqueOff(DXL_ID1);
  dxl.torqueOff(DXL_ID2);
  dxl.setOperatingMode(DXL_ID0, OP_POSITION);
  dxl.setOperatingMode(DXL_ID1, OP_POSITION);
  dxl.setOperatingMode(DXL_ID2, OP_POSITION);
  dxl.torqueOn(DXL_ID0);
  dxl.torqueOn(DXL_ID1);
  dxl.torqueOn(DXL_ID2);


  //set moving spped and mode
  dxl.writeControlTableItem(MOVING_SPEED, DXL_ID0, 100 );
  dxl.writeControlTableItem(MOVING_SPEED, DXL_ID1, 100 );
  dxl.writeControlTableItem(MOVING_SPEED, DXL_ID2, 100 );
  dxl.writeControlTableItem(CW_ANGLE_LIMIT, DXL_ID0, 0);
  dxl.writeControlTableItem(CCW_ANGLE_LIMIT, DXL_ID0, 4095);//jointmode
  dxl.writeControlTableItem(CW_ANGLE_LIMIT, DXL_ID1, 4095);
  dxl.writeControlTableItem(CCW_ANGLE_LIMIT, DXL_ID1, 4095);//multi-turn
  dxl.writeControlTableItem(CW_ANGLE_LIMIT, DXL_ID2, 4095);
  dxl.writeControlTableItem(CCW_ANGLE_LIMIT, DXL_ID2, 4095);//multi-turn
  //https://emanual.robotis.com/docs/en/dxl/mx/mx-28/#cwccw-angle-limit6-8
  //https://emanual.robotis.com/docs/en/dxl/mx/mx-28/#multi-turn
  //https://emanual.robotis.com/docs/en/dxl/mx/mx-28/#moving-speed
}

void Up() {
  //5loop   4loop   3loop   2loop
  //21000   16390   1225
  int naik = 0;
  naik = dxl.getPresentPosition(DXL_ID1);
  if ((naik = 0)) {
    dxl.setGoalPosition (DXL_ID1, high);
    dxl.writeControlTableItem(LED_GREEN, DXL_ID1, true);
    delay (500);
    int naik2 = 0;
    naik2 = dxl.getPresentPosition(DXL_ID2);
    if ((naik2 = 0)) {
      dxl.setGoalPosition (DXL_ID1, high);
      dxl.writeControlTableItem(LED_GREEN, DXL_ID2, true);
      delay (500);
      door();
    }
  }
}

void down() {
  int turun = 0;
  turun = dxl.getPresentPosition(DXL_ID1);
  if ((turun = 0)) {
    dxl.setGoalPosition (DXL_ID1,low);
    dxl.writeControlTableItem(LED_RED, DXL_ID1, true);
    delay (500);
    int turun2 = 0;
    turun2 = dxl.getPresentPosition(DXL_ID2);
    if (turun2 = 0) {
      dxl.setGoalPosition (DXL_ID2, low);
      dxl.writeControlTableItem(LED_RED, DXL_ID2, true);
      delay (500);
      door();
    }
  }
}

void loop() {
  //pintu indikatornya
  bool p1,p2,lt1,lt2;
  p1=digitalRead(pb0);
  p2=digitalRead(pb3);
  lt1=digitalRead(pb1);
  lt2=digitalRead(pb2);
  
 if (p1 == ditekan) { // lt1
   dxl.setGoalPosition(DXL_ID0, buka);
       dxl.ledOn(DXL_ID0);
      door();
}
  else { //buka pintu DARI LANTAI 1
dxl.setGoalPosition(DXL_ID0, tutup);
    dxl.ledOff(DXL_ID0);
  
    }

//penumpang masuk udah di lift
if (lt1 == ditekan) { //naik dari lift ke lt2
  delay(1000);
  dxl.setGoalPosition(DXL_ID0, tutup);//pintu ditutup dulu baru bisa naik
  dxl.writeControlTableItem(LED_BLUE, DXL_ID2, false);
  Up();
  delay (500);
  while (lt1 == ditekan) { //ke lt2
         delay (500);
      } p1last = p1now;
    }
  
 //lampu indikator sampai ke lt 2
else{
  dxl.setGoalPosition(DXL_ID1, low);
  dxl.setGoalPosition (DXL_ID0,tutup);
  delay (200);
}

if (lt2 == ditekan) {
  delay(1000);
  dxl.setGoalPosition(DXL_ID0, low);//pintu ditutup dulu baru bisa turun
  down ();//ke pos goal
  while (lt2 == ditekan) { //NAIK DARI LANTAI 1
    p1now = digitalRead(p1);
    if (p1now != p1last) {
      if (p1now == false) {
      } p1last = p1now;
    }
  }
}
else {

}

tunggu = dxl.getPresentPosition(DXL_ID2);
// dxl.setGoalPosition(DXL_ID1,);//naik
// dxl.setGoalPosition(DXL_ID1,);//turun

if((p2 == ditekan) && (p1 == true)) { //mau ada turun dari lt2, inget pullup
  while ((tunggu <= high) && (tunggu > low)) { //tapi posisi mau naik lift nya, masih di antara lt1 dan lt2
     down();
  }
  while ((tunggu > low) && (tunggu <high)) { //tapi posisi mau turun lift nya, masih di antara lt1 dan lt2
    Up();
    down();
    while ((tunggu = 0) &&(tunggu = high)){
      door();
    }     
  }
}


if ((p2 == true) && (p1 == ditekan)) { //mau turun, inget pullup
     while ((tunggu <= high) && (tunggu > low)) {
      down();
      delay (200);
      Up();}
      while (tunggu = 0) {
        Up();
      }}
    
if ((p2 == ditekan) && (p1 == ditekan)) { //mau ada naik dari lt1 dan turun lt2
  while ((tunggu <high) && (tunggu > low)) { //tapi posisi mau naik lift nya, masih di antara lt1 dan lt2
    down();
    Up();  }
  while ((tunggu <high) && (tunggu <= low)) {
    Up();
    down();}
    while ((tunggu = high)&& (tunggu = low)) {
      door();
    }
  }
}
