#include <dht.h>

/**
 * Define IO Pins
 */
// Movement
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
//Sensors
#define temp A0
#define flame A1
#define buzzer A2
#define light A3

/*
 * Define Variables
 */
#define CAR_SPEED 150
#define HOT 37
typedef unsigned char u8;
unsigned char door;
dht DHT;
const int sensorMin = 0;
const int sensorMax = 1024;
unsigned char current_temp;
int current_flame;
int door1 = 0;
int door2 = 0;
int door3 = 0;

/**
 * Sensor Methods
 */
 //Temperature
 int scanTemp() {
  DHT.read11(temp);
  return DHT.temperature;
 }

 //Flame
 int scanFire() {
  int sensorReading = analogRead(flame);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  return range;
 }

//Buzzer
void buzz(){
  tone(buzzer, 1000);
}

//Light
void flash(){
  digitalWrite(light,HIGH);
}

//Door Read/Selection
void doorSelect() {
  //From Bluetooth sensors not built
  door1 = Serial.read();
  door2 = Serial.read();
  door3 = Serial.read();
  if (door1 > HOT) {
    if (door2 > HOT) {
      if (door3 > HOT) {
        return 4;
      }
      else return 3;
    }
    else return 2;
  }
  else return 1;
}

/**
 * Movement Methods
 */
void forward(int iSpeed) {
  analogWrite(ENA, iSpeed);
  analogWrite(ENB, iSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void back(int iSpeed) {
  analogWrite(ENA, iSpeed);
  analogWrite(ENB, iSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left(int iSpeed) {
  analogWrite(ENA, iSpeed);
  analogWrite(ENB, iSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right(int iSpeed) {
  analogWrite(ENA, iSpeed);
  analogWrite(ENB, iSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void car_stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

/**
 * Path Methods
 */

void door1_path() {
  right(CAR_SPEED);
  delay(750);
  forward(CAR_SPEED);
  delay(6000);
  right(CAR_SPEED);
  delay(750);
  forward(CAR_SPEED);
  delay(13000);
  left(CAR_SPEED);
  delay(750);
  forward(CAR_SPEED);
  delay(18000);
  car_stop();
}

void door2_path(){
  forward(CAR_SPEED);
  delay(7000);
  left(CAR_SPEED);
  delay(750);
  forward(CAR_SPEED);
  delay(16000);
  car_stop();
}

void door3_path(){
  forward(CAR_SPEED);
  delay(1500);
  left(CAR_SPEED);
  delay(750);
  forward(CAR_SPEED);
  delay(4000);
  right(CAR_SPEED);
  delay(750);
  forward(CAR_SPEED); 
  delay(3500);
  left(CAR_SPEED);
  delay(450);
  forward(CAR_SPEED);
  delay(4500);
  car_stop();
}

void door4_path(){
  forward(CAR_SPEED);
  delay(1000);
  right(CAR_SPEED);
  delay(600);
  forward(CAR_SPEED); 
  delay(15000);
  left(CAR_SPEED);
  delay(750);
  forward(CAR_SPEED);
  delay(9000);
  car_stop();
}

/**
 * Setup
 */

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(light,OUTPUT);
  Serial.begin(9600);
}

/**
 * Main
 */

void loop() {
  current_temp = scanTemp();
  current_flame = scanFire();
  if(current_temp > HOT || current_flame < 2) {
    buzz();
    flash();
    door = doorSelect();
    switch(door){
      case 1: door1_path(); break;
      case 2: door2_path(); break;
      case 3: door3_path(); break;
      case 4: door4_path(); break;
      default: break;
    }    
  delay(5000); 
  }      
}
