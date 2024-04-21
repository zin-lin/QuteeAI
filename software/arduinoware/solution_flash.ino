// author: Zin Lin Htun
// reviewed by: Team 88 members
// Walk Simple code Arduino
// Using Arduino ARDUINO_AVR_UNO


#include <DynamixelShield.h>
#include <Adafruit_BNO055.h>

// #if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
//   #include <SoftwareSerial.h>
//   SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
//   #define DXL soft_serial
// #elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAM_ZERO)
//   #define DEBUG_SERIAL SerialUSB
// #else
//   #define DEBUG_SERIAL Serial
// #endif

#define SERVO_1_ID 1
#define SERVO_2_ID 2

#include <SoftwareSerial.h>
SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DXL soft_serial

const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;
int x = 1;
DynamixelShield dxl;

//This namespace is required to use Control table item names
using namespace ControlTableItem;
// dynamixel ids
int ids[] = {11, 12, 13, 21, 22, 23, 31, 32, 33, 41, 42, 43};

void neutral_pose(){
  for (uint8_t i = 1; i < 12; i+=3){

    dxl.setGoalPosition(ids[i], 1748);
    //delay(100);
  }

  for (uint8_t i = 0; i < 12; i+=3){

    dxl.setGoalPosition(ids[i], 2048);
    //delay(100);
  }


  for (uint8_t i = 2; i < 12; i+=3){

    dxl.setGoalPosition(ids[i], 1748);
    //delay(100);
  }
}

void setup() {
  // put your setup code here, to run once:

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_POSITION);
  dxl.torqueOn(DXL_ID);
  for (uint8_t i = 0; i < 12; i++){

    dxl.torqueOff(ids[i]);
    dxl.setOperatingMode(ids[i], OP_POSITION);
    dxl.torqueOn(ids[i]);
    //delay(1000);
  }

  for (uint8_t i = 0; i < 12; i++){

    dxl.setGoalPosition(ids[i], 2048);
    delay(100);
  }

  neutral_pose();

}

// walk functionality
void walk (bool f2e = true){
  // first 2 leggers
  int idsX0 [] = {13, 23};
  for (uint8_t i = 0; i < 2; i++) {

      dxl.setGoalPosition(idsX0[i], 2248);


  }

  delay(200);

  int idsX1 [] = {12, 22};
  for (uint8_t i = 0; i < 2; i++) {

      dxl.setGoalPosition(idsX1[i], 1248);



  }
  delay(200);

  // second 2 leggers
  for (uint8_t i = 6; i < 12; i++) {
    if (i != 6 && i != 9 ){
      dxl.setGoalPosition(ids[i], 1248);
      // delay(100);
    }
  }
  delay(270);

  // for all leggers to return to 0
  neutral_pose();

}

void easy_walk() {
  dxl.setGoalPosition(12, 2048 );
  delay(40);
  dxl.setGoalPosition(13, 2348 );

  delay(400);

  dxl.setGoalPosition(22, 2048 );
  delay(40);
  dxl.setGoalPosition(23, 2348 );

  delay(400);

  dxl.setGoalPosition(33, 2048 );
  delay(40);
  dxl.setGoalPosition(43, 2048 );

  delay(400);

  dxl.setGoalPosition(31, 1748 );
  delay(40);
  dxl.setGoalPosition(41, 2348 );

  delay(400);

  neutral_pose();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  // Please refer to e-Manual(http://emanual.robotis.com/docs/en/parts/interface/dynamixel_shield/) for available range of value.
  // DEBUG_SERIAL.println(dxl.ping(1));
  // Set Goal Position in RAW value
  easy_walk();


}