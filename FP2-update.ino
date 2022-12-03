#include <Dynamixel2Arduino.h>

#define DXL_SERIAL   Serial
//#define DEBUG_SERIAL soft_serial
const int DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN/DATA

#define Pb 3//buka pintu lt 1
#define Pb2 4// naik dalam lift
#define Pb3 5//turun dari 2 dalam lift
#define Pb4 6//buka pintu lt 2
bool Pbnow = HIGH;
bool Pblast = LOW;

uint8_t tunggu;

const uint8_t DXL_ID0 = 1;//door
const uint8_t DXL_ID1 = 2;//updown
const uint8_t DXL_ID2 = 10;//lroll
const float DXL_PROTOCOL_VERSION = 1.0;

Dynamixel2Arduino dxl(DXL_DIR_PIN);

//This namespace is required to use Control table item names
using namespace ControlTableItem;//actuator.cpp

//sensor
const byte sensor = 6;
int nilai;

void door() {
  dxl.setGoalPosition(DXL_ID0, 1023); //pintu di buka
  dxl.ledOn(DXL_ID0);

  //ditambahin sensor
  nilai = digitalRead(sensor);
  if (nilai == true ) { //sensor deteksi
    delay (2000);
    while (nilai == true) { //nunggu
    }
  }    else {//sensor ga deteksi
    dxl.setGoalPosition(DXL_ID0, 0);
    dxl.ledOff(DXL_ID0);
    delay (200);
  }
}

void setup() {
  Serial.begin (115200);
  pinMode (Pb, INPUT);
  pinMode (Pb2, INPUT);
  pinMode (Pb3, INPUT);
  pinMode (Pb4, INPUT);
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
    dxl.setGoalPosition (DXL_ID1, 16390);
    dxl.writeControlTableItem(LED_GREEN, DXL_ID1, true);
    delay (500);
    int naik2 = 0;
    naik2 = dxl.getPresentPosition(DXL_ID2);
    if ((naik2 = 0)) {
      dxl.setGoalPosition (DXL_ID1, 16390);
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
    dxl.setGoalPosition (DXL_ID1, 0);
    dxl.writeControlTableItem(LED_RED, DXL_ID1, true);
    delay (500);
    int turun2 = 0;
    turun2 = dxl.getPresentPosition(DXL_ID2);
    if (turun2 = 0) {
      dxl.setGoalPosition (DXL_ID2, 16390);
      dxl.writeControlTableItem(LED_RED, DXL_ID2, true);
      delay (500);
      door();
    }
  }
}

void loop() {
  //pintu indikatornya
  while (Pb == LOW) { // lt1
    dxl.setGoalPosition(DXL_ID0, 0);
  }
  while (Pb == HIGH) { //buka pintu DARI LANTAI 1
    dxl.setGoalPosition(DXL_ID0, 3060);
  }
  Pbnow = digitalRead(Pb);
  if (Pbnow != Pblast) {
    while (Pbnow == HIGH) {
      dxl.ledOn(DXL_ID0);
      door();
      Serial.println("selamat datang di lantai satuu!");
    }
  }
  else {
    dxl.setGoalPosition(DXL_ID0, 0);
    Serial.println ("pintunya nutup lagi deh");
  } dxl.ledOff(DXL_ID0);


//penumpang masuk udah di lift
if (Pb2 == HIGH) { //naik dari lift ke lt2
  delay(1000);
  dxl.setGoalPosition(DXL_ID0, 0);//pintu ditutup dulu baru bisa naik
  dxl.writeControlTableItem(LED_BLUE, DXL_ID2, false);
  Up();
  delay (500);
  while (Pb2 == HIGH) { //ke lt2
    Pbnow = digitalRead(Pb);
    if (Pbnow != Pblast) {
      if (Pbnow == LOW) {
        Serial.println("Kita naik ya ke lt2");
        delay (500);
      } Pblast = Pbnow;
    }
  }
  door();//lampu indikator sampai ke lt 2
}
else {
  dxl.setGoalPosition(DXL_ID1, 0);
  delay (200);
}


if (Pb3 == HIGH) {
  delay(1000);
  dxl.setGoalPosition(DXL_ID0, 0);//pintu ditutup dulu baru bisa turun
  down ();//ke pos goal
  while (Pb3 == HIGH) { //NAIK DARI LANTAI 1
    Pbnow = digitalRead(Pb);
    if (Pbnow != Pblast) {
      if (Pbnow == LOW) {
        Serial.println("Kita turun ke lt.1");
      } Pblast = Pbnow;
    }
  }

}
else {

}

tunggu = dxl.getPresentPosition(DXL_ID2);
// dxl.setGoalPosition(DXL_ID1,);//naik
// dxl.setGoalPosition(DXL_ID1,);//turun

if ((Pb4 == HIGH) && (Pb == LOW)) { //mau ada turun dari lt2
  while ((tunggu < 163090) && (tunggu >= 0)) { //tapi posisi mau naik lift nya, masih di antara lt1 dan lt2
    down();
    Serial.println("liftnya lagi naik ke lt.2 sabar ya");
    Serial.println("Iya ini pintunya udah di buka di lt.2");
    down();
  }
  while ((tunggu > 16390) && (tunggu >0)) { //tapi posisi mau turun lift nya, masih di antara lt1 dan lt2
    Up();
    Serial.println("liftnya turun dulu ke lantai 1");
    Serial.println("bentarr");
    down();
    Serial.println("Iya ini lagi naik lagi");

    Serial.println("Iya ini pintunya udah di buka di lt.2");
    while (tunggu = 0) {

    } while (tunggu = 16390) {
      Up();
      Serial.println("Iya ini pintunya udah di buka di lt.2");
    }
  }
}
else{}

if ((Pb4 == LOW) && (Pb == HIGH)) { //mau ada naik dari lt1
     while ((tunggu <= 16390) && (tunggu > 0)) {
      down();
      delay (200);
      Up();
      while (tunggu = 0) {
        Up();
      }}}
      else{}
  
if ((Pb4 == HIGH) && (Pb == HIGH)) { //mau ada naik dari lt1 dan turun lt2
  while ((tunggu <16390) && (tunggu > 0)) { //tapi posisi mau naik lift nya, masih di antara lt1 dan lt2
    down();
    Up();  }
  while (tunggu = 0) {
    door();
  }
  while ((tunggu < 1536) && (tunggu <= 1024)) {
Up();
down();
    while (tunggu = 16390) {
      door();
    }
  }
}}
