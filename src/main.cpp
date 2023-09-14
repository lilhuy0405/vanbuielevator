//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


#define TOTAL_FLOORS 3
#define FIRST_FLOOR 1
#define SECOND_FLOOR 2
#define THIRD_FLOOR 3

#define UP 1
#define DOWN 2
#define STOP 0

#define WHERE_ARE_YOU 'w'
#define WHAT_STATUS 's'
#define FIRST_FLOOR_PRESS '1'
#define SECOND_FLOOR_PRESS '2'
#define THIRD_FLOOR_PRESS '3'
#define TRIGGER_STATUS 't'


#define FORCE_STOP 'f'

#define ENGINE_UP_PIN 23
#define ENGINE_DOWN_PIN 22


#define TRIGGER_FIRST_FLOOR_DOWN 25
#define TRIGGER_SECOND_FLOOR_DOWN 26
#define TRIGGER_THIRD_FLOOR_DOWN 27



byte current_pos = SECOND_FLOOR;
byte current_status = STOP;
byte target_floor = 0;

void setup() {
  Serial.begin(115200);

  // setup pins mode for engine
  pinMode(ENGINE_UP_PIN, OUTPUT);
  pinMode(ENGINE_DOWN_PIN, OUTPUT);

  // setup pins mode for trigger
  pinMode(TRIGGER_FIRST_FLOOR_DOWN, INPUT);
  pinMode(TRIGGER_SECOND_FLOOR_DOWN, INPUT);
  pinMode(TRIGGER_THIRD_FLOOR_DOWN, INPUT);

  // setup bluetooth
  SerialBT.begin("ONGNOI"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
char to_char(byte data) {
  return data+48;
}

int forceStop(){
  if(current_status == STOP) {
    return 0;
  }
  digitalWrite(ENGINE_UP_PIN, LOW);
  digitalWrite(ENGINE_DOWN_PIN, LOW);
  current_status = STOP;
  return 0;
}

int moveToFloor(int floor) {
  // Only move if the elevator is stoped
  if (current_status != STOP) {
    return 0;
  }
  if(current_pos == floor) {
    return 0;
  }
  else if(current_pos < floor) {
    digitalWrite(ENGINE_DOWN_PIN, LOW);
    digitalWrite(ENGINE_UP_PIN, HIGH);
    current_status = UP;
    return 1;
  }
  else if(current_pos > floor) {
    digitalWrite(ENGINE_UP_PIN, LOW);
    digitalWrite(ENGINE_DOWN_PIN, HIGH);
    current_status = DOWN;
    return 2;
  }
  return 0;
}
int gotoSleep() {
  // move to second floor to sleep
  if (current_pos != SECOND_FLOOR) {
    return moveToFloor(SECOND_FLOOR);
    
  }
  return 0;
}
char digitalToString(byte data) {
  if (data == HIGH) {
    return 'H';
  }
  else {
    return 'L';
  }
}
void loop() {


  
  if (digitalRead(TRIGGER_FIRST_FLOOR_DOWN) == HIGH)
  {
    current_pos = FIRST_FLOOR;

    forceStop();
  }
  if (digitalRead(TRIGGER_SECOND_FLOOR_DOWN) == HIGH)
  {
    current_pos = SECOND_FLOOR;
    forceStop();
  }
    if (digitalRead(TRIGGER_THIRD_FLOOR_DOWN) == HIGH)
  {
    current_pos = THIRD_FLOOR;
    forceStop();
  }
    

  if (SerialBT.available()) {

    /* Logic for stop elevator when button trigged*/







    byte data = SerialBT.read();
    if (data == TRIGGER_STATUS) {
      SerialBT.write(digitalToString(digitalRead(TRIGGER_FIRST_FLOOR_DOWN)));
      SerialBT.write(digitalToString(digitalRead(TRIGGER_SECOND_FLOOR_DOWN)));
      SerialBT.write(digitalToString(digitalRead(TRIGGER_THIRD_FLOOR_DOWN)));
    }
    else if(data == WHERE_ARE_YOU) {
      Serial.write("WHERE_ARE_YOU CMD ");
      SerialBT.write(to_char(current_pos));
    }
    else if(data == WHAT_STATUS) {
      Serial.write("WHAT_STATUS CMD ");
      SerialBT.write(to_char(current_status));
    }
    else if(data == FIRST_FLOOR_PRESS) {
      Serial.write("FIRST_FLOOR_PRESS CMD ");
      moveToFloor(FIRST_FLOOR);
      
    }
    else if(data == FORCE_STOP) {
      Serial.write("FORCE_STOP CMD ");
      forceStop();
    }
    else if(data == SECOND_FLOOR_PRESS) {
      Serial.write("SECOND_FLOOR_PRESS CMD ");
      moveToFloor(SECOND_FLOOR);
    }
    else if(data == THIRD_FLOOR_PRESS) {
      Serial.write("THIRD_FLOOR_PRESS CMD ");
      moveToFloor(THIRD_FLOOR);
    }
    else{
      Serial.write(data);
    }
    
  }
  
  
}