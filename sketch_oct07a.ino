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
#define CAR_SPEED 50
#define HOT 100
typedef unsigned char u8;
unsigned char door;
dht DHT;
const int sensorMin = 0;
const int sensorMax = 1024;
unsigned char current_temp;
int current_flame;

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

//Door read/selection (prioritize after presentation)
/*void doorSelect() {
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
}*/

//Buzzer
void buzz(){
  tone(buzzer,1000);
  delay(1000);
  noTone(buzzer);
  delay(1000); 
}

//Light
void flash(){
  digitalWrite(light,HIGH);
}

/**
 * Movement Methods
 */
void forward(u8 car_speed) {
  analogWrite(ENA, car_speed);
  analogWrite(ENB, car_speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void back(u8 car_speed) {
  analogWrite(ENA, car_speed);
  analogWrite(ENB, car_speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left(u8 car_speed) {
  analogWrite(ENA, 250);
  analogWrite(ENB, 250);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right(u8 car_speed) {
  analogWrite(ENA, 250);
  analogWrite(ENB, 250);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

/**
 * Path Methods
 */

void door1_path() {
  forward(CAR_SPEED);
  delay(1500);
  left(CAR_SPEED);
  delay(1000);
  forward(CAR_SPEED);
  delay(2000);
  right(CAR_SPEED);
  delay(1000);
  forward(CAR_SPEED); 
  delay(1500);
  left(CAR_SPEED);
  delay(1000);
  forward(CAR_SPEED);
  delay(1500);
  stop();
}

void door2_path(){
  forward(CAR_SPEED);
  delay(1500);
  stop();
}

void door3_path(){
  right(CAR_SPEED);
  delay(1000);
  stop();
}

void door4_path(){
  left(CAR_SPEED);
  delay(1000);
  stop();
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
    //door = doorSelect();
    door = 1;
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
