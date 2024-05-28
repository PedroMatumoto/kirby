#include <Wire.h>
#include "Adafruit_TCS34725.h"

// 21 - SDA
// 22 - SCL

// Sensores peso
#define NVL1 35
#define NVL2 34

// Sensores
#define SENSOR_LEFT 15
#define SENSOR_RIGHT 4

// Motor
#define VEL_RIGHT 12
#define DIR_RIGHT 14
#define VEL_LEFT 27
#define DIR_LEFT 26

// Bombas
#define BOMBA_1 19
#define BOMBA_2 18

// Botões
#define BOTAO_1 33
#define BOTAO_2 32

// NÃO USAR 21 E 22!
// SCL SDA

#define commonAnode true

byte gammatable[256];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

enum State
{
  IDLE,
  WALKING,
  STOPPED,
  ROTATING,
  ALERT
};

enum Side
{
  LEFT,
  RIGHT
};


// Estados iniciais
State state = IDLE;
Side side = LEFT;

// Função de controle de velocidade
void motor(int vel_left, int vel_right) {
  if (vel_left == 0){
    digitalWrite(DIR_LEFT, 0);
    digitalWrite(VEL_LEFT, 0);
  }
  if (vel_right == 0){
    digitalWrite(DIR_RIGHT,0);
    digitalWrite(VEL_RIGHT,0);
  }

  if (vel_left > 0) {
    analogWrite(VEL_LEFT, abs(vel_left));
    analogWrite(DIR_LEFT, LOW);
  } else {
    analogWrite(VEL_LEFT, LOW);
    analogWrite(DIR_LEFT, abs(vel_left));
  }

  if (vel_right > 0) {
    analogWrite(VEL_RIGHT, abs(vel_right));
    analogWrite(DIR_RIGHT, LOW);
  } else {
    analogWrite(VEL_RIGHT,LOW);
    analogWrite(DIR_RIGHT, abs(vel_right));
  }
}

int sensor_l = 0; 
int sensor_r = 0;
int botao_1 = LOW;
int botao_2 = LOW;
float nivel_atual_1 = 0;
float nivel_atual_2 = 0;
// voltar aqui
float nivel_minimo = 0;

void setup()
{
  // Sensores de nível  
  pinMode(NVL1, INPUT);
  pinMode(NVL2, INPUT);

  // Sensores
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  // Motores
  pinMode(VEL_RIGHT, OUTPUT);
  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(VEL_LEFT, OUTPUT);
  pinMode(DIR_LEFT, OUTPUT);

  // Bombas
  pinMode(BOMBA_1, OUTPUT);
  pinMode(BOMBA_2, OUTPUT);

  // Botões
  pinMode(BOTAO_1, INPUT_PULLDOWN);
  pinMode(BOTAO_2, INPUT_PULLDOWN);

  Serial.begin(9600);

  // Inicia sensor de cor
  if (tcs.begin())
  {
    Serial.println("Found sensor");
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    while(1) {}
  }
}



void loop()
{
  
  float red, green, blue;

  tcs.setInterrupt(false); // turn on LED

  delay(60); // takes 60ms to read

  tcs.getRGB(&red, &green, &blue);

  nivel_atual_1 = analogRead(NVL1);
  nivel_atual_2 = analogRead(NVL2);


  switch (state)
  {
  case IDLE:
    if (nivel_atual_1 < nivel_minimo || nivel_atual_2 <nivel_minimo){
      state = IDLE;
    }
    else{
      state = WALKING;
    }
    

    

    // Verifica tanque
    // Estando OK, coloca no modo WALKING
    
    // Caso NOK, fica stunado
    // while (1)
    // {
    // }
    break;
  case WALKING:
    tcs.getRGB(&red, &green, &blue);

    // Verifica se a cor é verde
    if (green > 120)
    {
      state = STOPPED;
      break;
    }

    // MOTOR!
    motor(90, 90);

    // Sensores infra
    if (digitalRead(SENSOR_LEFT) == 0)
    {
      state = ROTATING;
      side = LEFT;
    }
    else if (digitalRead(SENSOR_RIGHT) == 0)
    {
      state = ROTATING;
      side = RIGHT;
    }
    break;
  case STOPPED:
    // Para para servir
    // Depois que servir, volta a andar
    // Verifica tanque, se está em estado crítico

    // Se sim
    motor(0, 0);

    while (botao_1 == LOW && botao_2 == LOW) {
      botao_1 = digitalRead(BOTAO_1);
      botao_2 = digitalRead(BOTAO_2);
    }

    if (botao_1 == HIGH)
    {
      digitalWrite(BOMBA_1, HIGH);
      delay(5000);
      digitalWrite(BOMBA_1, LOW);
    }
    else if (botao_2 == HIGH)
    {
      digitalWrite(BOMBA_2, HIGH);
      delay(5000);
      digitalWrite(BOMBA_2, LOW);
    }

    botao_1 = LOW;
    botao_2 = LOW;

    nivel_atual_1 = analogRead(NVL1);
    nivel_atual_2 = analogRead(NVL2);

    if (nivel_atual_1 < nivel_minimo || nivel_atual_2 <nivel_minimo){
      state = ALERT;
    }
    else{
      state = WALKING;
    }
    break;
  case ROTATING:
    // Para de andar
    // Gira
    motor(0, 0);
    delay(2000);

    if (side == LEFT)
    {
      motor(90, -90);
      delay(250);
      state = WALKING;
    }
    else if (side == RIGHT)
    {
      motor(-90, 90);
      delay(250);
      state = WALKING;
    }

    motor(90, 90);
    delay(1000);
    break;
  case ALERT:
    tcs.getRGB(&red, &green, &blue);
    // MOTOR
    // Verifica curvas
    motor(90, 90);

    // Sensores infra
    
    // Verifica se chegou no azul
    if (blue > 130)
    {
      state = IDLE;
    }
    if (digitalRead(SENSOR_LEFT) == 0)
    {
      state = ROTATING;
      side = LEFT;
    }
    else if (digitalRead(SENSOR_RIGHT) == 0)
    {
      state = ROTATING;
      side = RIGHT;
    }
    break;
    
  }
}
