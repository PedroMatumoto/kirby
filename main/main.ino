#include <Wire.h>
#include "Adafruit_TCS34725.h"

// 21 - SDA
// 22 - SCL


// Sensores
#define SENSOR_LEFT 15
#define SENSOR_RIGHT 4

// Motor
#define DIR_RIGHT 12  //CABOS VERMELHOS
#define VEL_RIGHT 14  //CABOS CINZA
#define DIR_LEFT 27   //CABOS VERMELHOS
#define VEL_LEFT 26   //CABOS CINZA

// Bombas
#define BOMBA_1 19
#define BOMBA_2 18

// Botões
#define BOTAO_1 35
#define BOTAO_2 34

// NÃO USAR 21 E 22!
// SCL SDA

#define commonAnode true

byte gammatable[256];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X);

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

// Estados iniciais
State state = IDLE;
Side side = LEFT;

// int botao_1 = LOW;
// int botao_2 = LOW;

void setup() {
  // Sensores
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  // Motores
  pinMode(VEL_RIGHT, OUTPUT);
  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(VEL_LEFT, OUTPUT);
  pinMode(DIR_LEFT, OUTPUT);

  digitalWrite(DIR_LEFT, LOW);
  digitalWrite(DIR_RIGHT, LOW);

  // // Bombas
  // pinMode(BOMBA_1, OUTPUT);
  // pinMode(BOMBA_2, OUTPUT);

  // Botões
  pinMode(BOTAO_1, INPUT_PULLDOWN);
  pinMode(BOTAO_2, INPUT_PULLDOWN);

  Serial.begin(9600);

  //Inicia sensor de cor
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1) {
      Serial.println("Sensor nao iniciado");
    }
  }
}


int velocidade = 130;

void loop() {
  float red, green, blue;

  tcs.setInterrupt(true);  // turn on LED

  tcs.getRGB(&red, &green, &blue);


  if (green > 120) {
    Serial.println("Parado");
    analogWrite(VEL_LEFT, 0);
    analogWrite(VEL_RIGHT, 0);
    while (digitalRead(BOTAO_1) == LOW && digitalRead(BOTAO_2) == LOW) {
    }
  } else {
    if (digitalRead(SENSOR_LEFT) == 0 && digitalRead(SENSOR_RIGHT) == 0) {
      Serial.println("Frente");
      analogWrite(VEL_LEFT, velocidade);
      analogWrite(VEL_RIGHT, velocidade);Q   -
    }
    if (digitalRead(SENSOR_LEFT) == 1 && digitalRead(SENSOR_RIGHT) == 0) {
      analogWrite(VEL_LEFT, velocidade);
      analogWrite(VEL_RIGHT, 0);
      Serial.println("Esquerda");
    }
    if (digitalRead(SENSOR_RIGHT) == 1 && digitalRead(SENSOR_LEFT) == 0) {
      analogWrite(VEL_LEFT, 0);
      analogWrite(VEL_RIGHT, velocidade);
      Serial.println("Direita");
    }
    if (digitalRead(SENSOR_LEFT) == 1 && digitalRead(SENSOR_RIGHT) == 1) {
      Serial.println("Perdido");
      analogWrite(VEL_LEFT, 0);
      analogWrite(VEL_RIGHT, 0);
    }
  }
}
