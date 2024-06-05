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
#define DIR_RIGHT 12 //CABOS VERMELHOS
#define VEL_RIGHT 14 //CABOS CINZA
#define DIR_LEFT 27 //CABOS VERMELHOS
#define VEL_LEFT 26 //CABOS CINZA

// Bombas
#define BOMBA_1 19
#define BOMBA_2 18

// Botões
#define BOTAO_1 33
#define BOTAO_2 32

// NÃO USAR 21 E 22!
// SCL SDA

#define commonAnode true

// byte gammatable[256];

// Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

enum State {
  IDLE,
  WALKING,
  STOPPED,
  ROTATING,
  ALERT
};

enum Side {
  LEFT,
  RIGHT
};

// float calcular() {
//   long soma = 0;
//   int num_leituras = 32;
//   float peso_objeto;

//   // Aguarda a usuário digitar o peso do objeto conhecido
//   Serial.println("Por favor, insira o peso do objeto em quilogramas:");
//   while (!Serial.available()) {
//   }
//   peso_objeto = Serial.parseFloat();
//   Serial.print("Peso do objeto inserido: ");
//   Serial.print(peso_objeto, 3);
//   Serial.println(" kg");

//   // Realiza 32 leitura para calcular o valor da escala de calibração
//   Serial.println("Realizando 32 leituras para calcular a média:");
//   for (int i = 0; i < num_leituras; i++) {
//     soma += balanca_1.get_value(10);
//     delay(100);
//   }
// }

// Estados iniciais
State state = IDLE;
Side side = LEFT;

// Função de controle de velocidade
// void motor(int vel_left, int vel_right) {
//   if (vel_left == 0) {
//     digitalWrite(DIR_LEFT, 0);
//     digitalWrite(VEL_LEFT, 0);
//   }
//   if (vel_right == 0) {
//     digitalWrite(DIR_RIGHT, 0);
//     digitalWrite(VEL_RIGHT, 0);
//   }

//   if (vel_left > 0) {
//     analogWrite(VEL_LEFT, abs(vel_left));
//     analogWrite(DIR_LEFT, LOW);
//   } else {
//     analogWrite(VEL_LEFT, LOW);
//     analogWrite(DIR_LEFT, abs(vel_left));
//   }

//   if (vel_right > 0) {
//     analogWrite(VEL_RIGHT, abs(vel_right));
//     analogWrite(DIR_RIGHT, LOW);
//   } else {
//     analogWrite(VEL_RIGHT, LOW);
//     analogWrite(DIR_RIGHT, abs(vel_right));
//   }
// }

int sensor_l = 0;
int sensor_r = 0;
// int botao_1 = LOW;
// int botao_2 = LOW;
// float escala = 0;
// float peso_min = 0.1000;
// long soma = 0;
// float media = 0;
// int num_leituras = 32;

void setup() {
  // Sensores
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  // Motores
  pinMode(VEL_RIGHT, OUTPUT);
  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(VEL_LEFT, OUTPUT);
  pinMode(DIR_LEFT, OUTPUT);

  digitalWrite(DIR_LEFT,LOW);
  digitalWrite(DIR_RIGHT,LOW);

  // // Bombas
  // pinMode(BOMBA_1, OUTPUT);
  // pinMode(BOMBA_2, OUTPUT);

  // // Botões
  // pinMode(BOTAO_1, INPUT_PULLDOWN);
  // pinMode(BOTAO_2, INPUT_PULLDOWN);

  Serial.begin(9600);

  // Inicia sensor de cor
  // if (tcs.begin()) {
  //   Serial.println("Found sensor");
  // } else {
  //   Serial.println("No TCS34725 found ... check your connections");
  //   while (1) {}
  // }
}


int velocidade = 120;

void loop() {
  if(digitalRead(SENSOR_LEFT)== 0 && digitalRead(SENSOR_RIGHT)== 0){
    Serial.println("Frente");
    analogWrite(VEL_LEFT,velocidade);
    analogWrite(VEL_RIGHT,velocidade);
  }

  if (digitalRead(SENSOR_LEFT) == 1 && digitalRead(SENSOR_RIGHT)==0 ) {
    analogWrite(VEL_LEFT,velocidade);
    analogWrite(VEL_RIGHT,0);
    Serial.println("Esquerda");
  }
  if (digitalRead(SENSOR_RIGHT) == 1 && digitalRead(SENSOR_LEFT)==0) {
    analogWrite(VEL_LEFT,0);
    analogWrite(VEL_RIGHT,velocidade);
    Serial.println("Direita");
  }

  if(digitalRead(SENSOR_LEFT)== 1 && digitalRead(SENSOR_RIGHT)== 1){
    Serial.println("Perdido");
    analogWrite(VEL_LEFT,0);
    analogWrite(VEL_RIGHT,0);
  }
}
