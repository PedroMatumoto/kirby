#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define redpin 3
#define greenpin 5
#define bluepin 6
#define sensor1 15
#define sensor2 14

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
State state = IDLE;
Side side = LEFT;
int forced = 0;

void setup()
{
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  Serial.begin(9600);

  if (tcs.begin())
  {
    Serial.println("Found sensor");
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    // while (1); // halt!
    while(1) {}
  }
}

void loop()
{
  float red, green, blue;

  tcs.setInterrupt(false); // turn on LED

  delay(60); // takes 50ms to read

  //  tcs.getRGB(&red, &green, &blue);

  //  Serial.print("R:\t"); Serial.print(int(red));
  //  Serial.print("\tG:\t"); Serial.print(int(green));
  //  Serial.print("\tB:\t"); Serial.print(int(blue));
  // Serial.print("\n");

  switch (state)
  {
  case IDLE:
    // Verifica motor
    // Estando OK, coloca no modo WALKING
    state = WALKING;
    // Caso NOK, fica stunado
    // while (1)
    // {
    // }
    break;
  case WALKING:
    tcs.getRGB(&red, &green, &blue);
    // Serial.print("R:\t"); Serial.print(int(red));
    // Serial.print("\tG:\t"); Serial.print(int(green));
    // Serial.print("\tB:\t"); Serial.print(int(blue));
    // Serial.print("\n");

    // Serial.println("ESTOU ANDANDO");

    if (green > 130)
    {
      state = STOPPED;
    }
    // MOTOR!
    Serial.print("Sensor 1:\t");
    Serial.print(digitalRead(sensor1));
    Serial.print("\tSensor 2:\t");
    Serial.print(digitalRead(sensor2));
    Serial.print("\n");

    // if (sensor1 > 200)
    // {
    //   state = ROTATING;
    //   side = LEFT;
    // }
    // else if (sensor2 > 200)
    // {
    //   state = ROTATING;
    //   side = RIGHT;
    // }

    break;
  case STOPPED:
    // Para para servir
    // Depois que servir, volta a andar
    // Verifica tanque, se está em estado crítico
    // Se sim
    Serial.println("ESTOU PARADO");
    // state = ALERT;
    // Se não
    state = WALKING;
    break;
  case ROTATING:
    // Para de andar
    // Gira
    if (side == LEFT)
    {
    }
    else if (side == RIGHT)
    {
    }
    // Anda e depois seta estado
    if (sensor1 ==0 && sensor2==0){
      state = WALKING;
    }
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