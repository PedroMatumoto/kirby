// Programa: Balança Inteligente com Arduino Nano ESP32 - Calibração
// Autor: Rosana Guse
 
// Adiciona as bibliotecas utilizadas no código
#include "HX711.h"
 
// Define os pinos de ligação com o Arduino
#define DT 25
#define SCK 18
 
// Cria um objeto chamado balança
HX711 balanca;
float escala;
void setup() {
   
  // Inicia o módulo HX711
  balanca.begin(DT, SCK);
 
  // Inicia a Comunicação Serial
  Serial.begin(9600);
  delay(5000);
   
  // Faz a verificação e registro da tara
  Serial.print("Leitura da tara:  ");
  Serial.println(balanca.read());
  Serial.println("Registrando a tara...");
  balanca.set_scale();
  balanca.tare(20);
  delay(2000);
  escala=calcular();
}
 
void loop() {
   
  // Cria variáveis temporárias
  long soma = 0;
  int num_leituras = 32;
 
  // Aguarda a usuário digitar o peso do objeto conhecido
  Serial.println("Insira o objeto:");
  while (!Serial.available()) {
  }
 
  // Realiza 32 leitura para calcular o valor da escala de calibração
  Serial.println("Realizando 32 leituras para calcular a média:");
  for (int i = 0; i < num_leituras; i++) {
    soma += balanca.get_value(10); 
    delay(100);
  }
  float media = soma / (float)num_leituras;
  media=media/escala;
  Serial.print("Peso calculado: ");
  Serial.println(media, 2);
  // Limpa o buffer serial para ser feita uma nova leitura
  while (Serial.available()) {
    Serial.read();
  }
}
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
    soma += balanca.get_value(10); 
    delay(100);
  }
  float media = soma / (float)num_leituras;
  float escala_calculada = media / peso_objeto;
  Serial.print("Média das 32 leituras: ");
  Serial.println(media, 2);
  Serial.print("Escala calculada: ");
  Serial.println(escala_calculada, 2); 
  // Limpa o buffer serial para ser feita uma nova leitura
  while (Serial.available()) {
    Serial.read();
   return escala_calculada;
 }
  // Aguarda 5 segundos antes de realizar a próxima leitura
  delay(5000);  
}


