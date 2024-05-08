#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Sensores
#define SENSOR_LEFT 15
#define SENSOR_RIGHT 4

// Motor
#define VEL_RIGHT 12
#define DIR_RIGHT 14
#define VEL_LEFT 27
#define DIR_LEFT 26

// Bombas
#define BOMBA_1 35
#define BOMBA_2 34

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

void setup()
{
  
  // Sensores
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  //LED 
  pinMode(2,INPUT);

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

  delay(60); // takes 50ms to read

  Serial.println(digitalRead(SENSOR_LEFT));

   tcs.getRGB(&red, &green, &blue);

  switch (state)
  {
  case IDLE:
    // Verifica tanque
    // Estando OK, coloca no modo WALKING
    state = WALKING;
    Serial.println("OIEEEE");
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
    motor(150, 150);

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
    Serial.println("ESTOU PARADO");
    delay(5000);
    state = WALKING;
    // state = ALERT;
    // Se não
    // state = WALKING;
    break;
  case ROTATING:
    digitalWrite(2,HIGH);
    // Para de andar
    // Gira
    motor(0, 0);
    delay(1000);

    if (side == LEFT)
    {
      motor(150, -150);
      delay(5000);
      state = WALKING;
    }
    else if (side == RIGHT)
    {
      motor(-150, 150);
      delay(5000);
      state = WALKING;
    }
    // Anda e depois seta estado
    // if (sensor1 ==0 && sensor2==0){
    //   state = WALKING;
    // }
    break;
  case ALERT:
    tcs.getRGB(&red, &green, &blue);
    // MOTOR
    // Verifica curvas
    // Verifica se chegou no azul
    if (blue > 130)
    {
      state = IDLE;
    }
    break;
  }
}