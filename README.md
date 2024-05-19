# Kirby

**Kirby** é o projeto semestral da disciplina **Microcontroladores e Sistemas Embarcados** (EEN251) do **Instituto Mauá de Tecnologia** (IMT) ministrada pelos professores [Sergio Ribeiro Augusto](https://www.linkedin.com/in/sergio-ribeiro-augusto-258a9ba0/?originalSubdomain=br) e [Rodrigo França](https://www.linkedin.com/in/rodrigo-fran%C3%A7a-847872b1/).<BR>

## Sumário

- [Requisitos](#requisitos)
- [Escopo](#escopo)
  - [Materiais](#materiais)
- [Funcionamento](#funcionamento)
  - [Máquina de estados](#máquina-de-estados)
    - [Estados](#estados)
- [Testes](#testes)
- [Autores](#autores)

# Requisitos

| id  | requisito                                                                   | tipo      |
| --- | --------------------------------------------------------------------------- | --------- |
| 1   | Utilizar sensores analógicos ou digitais                                    | Técnico   |
| 2   | Servir bebidas aos clientes                                                 | Funcional |
| 3   | Seguir um trajeto no chão                                                   | Funcional |
| 4   | Verificar o abastecimento dos tanques para evitar uso incorreto dos motores | Técnico   |
| 5   | Utilizar um microcontrolador                                                | Técnico   |
| 6   | Utilizar padrões de conexão (I2C, SPI) vistos em aula                       | Técnico   |
| 7   | Interagir com o usuário com botões                                          | Técnico   |

# Escopo

O projeto consiste em um robô autônomo que deve **seguir uma linha no chão**, **mudar de estado conforme a leitura de cores do sensor** e **interagir com o usuário através de botões**. O robô deve ser capaz de carregar dois tipos de líquidos em seus tanques e servir o usuário conforme a interação.

## Diagrama de blocos

<p align="center">
  <img width="910" alt="diagrama_blocos" src="assets/diagrama_blocos.png">
</p>

## Materiais

O robô é montado a partir:

- 1 microcontrolador **ESP32**
- 2 células de Carga de 5kg
- 2 conversores **HX711**
- 2 MOSFETs **IRLZ44N**
- 1 sensor de cor **TCS34725**
- 2 sensor de linha **QTR-8RC**
- 1 chassi de acrílico
- 1 ponte H L298N Mini
- 2 motores **3-6 V**
- 2 mini bombas
- 2 displays **OLED** I2C
- 3 botões

# Funcionamento

O funcionamento do robô é baseado em uma **máquina de estados** que controla o comportamento do robô conforme a leitura do **sensor infravermelho** e do **sensor de cor**.

Primeiramente, o robô deve ser calibrado para que o sensor de cor consiga identificar a linha no chão. Após a calibração, o robô deve seguir a linha até encontrar a cor verde, onde ele deve parar e interagir com o usuário.

Com a interação do usuário a partir dos botões, o robô deve servir o usuário com o líquido correspondente ao botão selecionado (dois líquidos até o momento). Caso o tanque atinja um **nível crítico**, o robô deve retornar ao ponto inicial (cor azul) e reiniciar o processo.

Os **displays OLED** são utilizados de forma estética para simular o olho do personagem Kirby.

## Máquina de estados

<p align="center">
  <img width="910" alt="maquina-estados" src="https://github.com/enzosakamoto/kirby/assets/98707474/b72dbbcb-96b6-489b-b1f6-7fdcb84cacf5">
</p>

### Estados

- **I**: Estado inicial
- **A**: Estado andando/seguindo
- **P**: Estado parado
- **C**: Estado crítico

# Testes

## Dia 15/05/2024

- **Teste de calibração do sensor de cor**
  - O sensor de cor foi calibrado para identificar a cor verde
- **Teste de calibração do sensor infravermelho**
  - O sensor infravermelho foi calibrado para identificar a linha preta no chão e iniciar o estado de girar
- **Teste de botão**
  - O botão foi testado para identificar a interação do usuário
- **Teste de bomba**
  - A bomba foi testada para identificar a interação do robô com o usuário e bombear o líquido

https://github.com/enzosakamoto/kirby/assets/98707474/40ef3852-a57a-4a14-8359-391d45e8bc85

# Autores

- [Antonio Ferrite](https://github.com/tom-ferrite)
- [Enzo Sakamoto](https://github.com/enzosakamoto)
- [Flavio Murata](https://github.com/flaviomurata)
- [Maria Fernanda Pinho](https://github.com/mafepinho)
- [Pedro Matumoto](https://github.com/pedromatumoto)
