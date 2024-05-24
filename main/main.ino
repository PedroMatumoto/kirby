#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "HX711.h"

// 21 - SDA
// 22 - SCL

// Sensores peso
#define DT1 35
#define SCK1 34
#define DT2 39
#define SCK2 36

HX711 balanca_1;
HX711 balanca_2;

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

float calcular(){
  long soma = 0;
  int num_leituras = 32;
  float peso_objeto;
 
  // Aguarda a usuário digitar o peso do objeto conhecido
  Serial.println("Por favor, insira o peso do objeto em quilogramas:");
  while (!Serial.available()) {
  }
  peso_objeto = Serial.parseFloat();
  Serial.print("Peso do objeto inserido: ");
  Serial.print(peso_objeto, 3);
  Serial.println(" kg");
 
  // Realiza 32 leitura para calcular o valor da escala de calibração
  Serial.println("Realizando 32 leituras para calcular a média:");
  for (int i = 0; i < num_leituras; i++) {
    soma += balanca_1.get_value(10); 
    delay(100);
  }
}

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
float escala = 0;
float peso_min = 0.1000;
long soma = 0;
float media = 0;
int num_leituras = 32;

void setup()
{
  int num_leituras = 32;

  balanca_1.begin(DT1,SCK1);
  balanca_2.begin(DT2,SCK2);
  
  balanca_1.set_scale();
  balanca_1.tare(20);
  delay(2000);
  escala = calcular();


  
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

  Serial.println(digitalRead(SENSOR_LEFT));

   tcs.getRGB(&red, &green, &blue);

  switch (state)
  {
  case IDLE:
    soma = 0;
    media = 0;
    for (int i = 0; i < num_leituras; i++) {
    soma += balanca_1.get_value(10); 
    delay(100);
    }
    media = soma / (float)num_leituras;
    media=media/escala;
    if (media < peso_min){
      state = IDLE;
    }
    else{
      soma = 0;
      media = 0;
      for (int i = 0; i < num_leituras; i++) {
      soma += balanca_2.get_value(10); 
      delay(100);
      }
      media = soma / (float)num_leituras;
      media=media/escala;
      if (media < peso_min){
        state = IDLE;
      }
      else{
        state = WALKING;
      }
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

    while (botao_1 == LOW) {
      botao_1 = digitalRead(BOTAO_1);
    }

    delay(1000);
    digitalWrite(BOMBA_2, HIGH);
    delay(5000);
    digitalWrite(BOMBA_2, LOW);
    delay(1000);    

        soma = 0;
    media = 0;
    for (int i = 0; i < num_leituras; i++) {
    soma += balanca_1.get_value(10); 
    delay(100);
    }
    media = soma / (float)num_leituras;
    media=media/escala;
    if (media < peso_min){
      state = ALERT;
    }
    else{
      soma = 0;
      media = 0;
      for (int i = 0; i < num_leituras; i++) {
      soma += balanca_2.get_value(10); 
      delay(100);
      }
      media = soma / (float)num_leituras;
      media=media/escala;
      if (media < peso_min){
        state = ALERT;
      }
      else{
        state = WALKING;
      }
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
