#include <Arduino.h>
#include <Ultrasonic.h>
#include <Servo.h>
#include "DcMotor.hpp"

//Pino Servo
#define PIN_SERVO       10

//Pinos SR04
#define PIN_ECHO        9
#define PIN_TRIGGER     8 

//Pinos Motor A
#define PINA1          2
#define PINA2          4
#define PIN_SPEED_A     3

//Pinos Motor B
#define PIN_B1          6 
#define PIN_B2          7
#define PIN_SPEED_B     5 

//Pinos leds
#define PIN_LED_PLAY    11
#define PIN_LED_STANDBY 12
#define PIN_LED_FOLLOW  13

#define PLAY            1
#define STANDBY         2
#define FOLLOW          3

#define SPEED_SEARCH    150  //Velocidade para procurar objeto
#define MAX_DISTANCE    90  //Distancia maxima onde o objeto pode estar
#define VAR_DISTANCE    4    //Variância padrão de distancia do objeto em cm
#define TURN_SPEED      150  //Velocidade para fazer a volta   

void go(short speed);
void back(short speed);
void left();
void right();
void stop();
void search();
void enjoy();
void changeLeds(short led);

short distanceDefault = 40; //Distância padrão em cm

Servo servo;
short position;

Ultrasonic ultrasonic(PIN_TRIGGER, PIN_ECHO);

short speedDefault = 255;

void setup(){
  pinMode(PINA1,OUTPUT);
  pinMode(PINA2,OUTPUT);

  pinMode(PIN_B1,OUTPUT);
  pinMode(PIN_B2,OUTPUT);

  pinMode(PIN_SPEED_A,OUTPUT);
  pinMode(PIN_SPEED_B,OUTPUT);

  pinMode(PIN_LED_PLAY,OUTPUT);
  pinMode(PIN_LED_STANDBY,OUTPUT);
  pinMode(PIN_LED_FOLLOW,OUTPUT);

  servo.attach(PIN_SERVO);
  servo.write(0);

  Serial.begin(9600);
}

DcMotor motorA(PINA1, PINA2, PIN_SPEED_A, speedDefault);
DcMotor motorB(PIN_B1, PIN_B2, PIN_SPEED_B, speedDefault);

char inBluetooth = 'x';
bool ini = true;

void loop(){
  if(Serial.available() || ini == true){ //Quando houver dados no bluetooth
    
    inBluetooth = Serial.read();
   
    if(ini == true){
      inBluetooth = 'P';
      ini = false;
    }

    if(inBluetooth == 'D'){ //Alterar distancia que o robo deve manter do objeto
      
      while(!Serial.available()); //Aguardando uma distancia entre 1cm e a distancia maxima configurada
      short temp = int(Serial.read());
      if(temp > 1 && temp <= MAX_DISTANCE){
        Serial.print("Mudou para: ");
        Serial.println(temp);
        distanceDefault = temp;
      }

    }else if(inBluetooth == 'E'){ //Se quiser brincar, digite E
      changeLeds(PLAY);
      enjoy();
    }else if(inBluetooth == 'P'){
      changeLeds(STANDBY);
      stop();
      while(!Serial.available() && Serial.read() != 'f');
    }
  }

  changeLeds(FOLLOW);
  
  short currentDistance = ultrasonic.read();
  
  if(currentDistance > (distanceDefault + VAR_DISTANCE) && currentDistance < MAX_DISTANCE)
    go(speedDefault);
  else if(currentDistance < (distanceDefault - VAR_DISTANCE))
    back(speedDefault);
  else if(currentDistance > MAX_DISTANCE)
    search();
  else{
    Serial.println("Achou");
    stop();
  }
    delay(100);
}

void go(short speed){
  motorA.goSpeed(speed);
  motorB.goSpeed(speed);
  Serial.println("GO");
}

void back(short speed){
  motorA.backSpeed(speed);
  motorB.backSpeed(speed);
  Serial.println("BACK");
}

void left(){
  motorA.goSpeed(TURN_SPEED);
  motorB.backSpeed(TURN_SPEED);
  Serial.println("LEFT");
}

void right(){
  motorA.backSpeed(TURN_SPEED);
  motorB.goSpeed(TURN_SPEED);
  Serial.println("RITHG");
}

void search(){
  short currentDistance = 160;
  stop();
  while(currentDistance > MAX_DISTANCE && inBluetooth != 'P'){
    inBluetooth = Serial.read();
    Serial.println("SEARCH");
    currentDistance = ultrasonic.read();
    motorA.backSpeed(SPEED_SEARCH);
    motorB.goSpeed(SPEED_SEARCH);
    delay(100);
  }
}

void stop(){
  
  motorA.stop();
  motorB.stop();

  delay(50);
}

void enjoy(){
  stop();
  while(inBluetooth != 'e'){ //Se quiser parar de brincar, digite e
    while(!Serial.available());
    inBluetooth = Serial.read();
    
    if(inBluetooth == 'g') //Ir pra frente
      go(speedDefault);
    else if(inBluetooth == 'b') //Voltar
      back(speedDefault);
    else if(inBluetooth == 'r'){ //Direta
      stop();
      right();
    }
    else if(inBluetooth == 'l'){ //Esquerda
      stop();
      left();
    }
    else if(inBluetooth == 's') //Parar
      stop();
  }
  stop();
}

void changeLeds(short led){
  switch(led){
    case 1:
      digitalWrite(PIN_LED_PLAY, 1);
      digitalWrite(PIN_LED_FOLLOW, 0);
      digitalWrite(PIN_LED_STANDBY, 0);
      break;
    case 2:
      digitalWrite(PIN_LED_PLAY, 0);
      digitalWrite(PIN_LED_FOLLOW, 1);
      digitalWrite(PIN_LED_STANDBY, 0);
      break;
    case 3:
      digitalWrite(PIN_LED_PLAY, 0);
      digitalWrite(PIN_LED_FOLLOW, 0);
      digitalWrite(PIN_LED_STANDBY, 1);
      break;
  }
}