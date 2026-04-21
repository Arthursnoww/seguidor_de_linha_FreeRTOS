# 🚗 Seguidor de Linha com FreeRTOS no Raspberry Pi Pico

Projeto de um **robô seguidor de linha** utilizando **Raspberry Pi Pico**, sensores infravermelhos e **FreeRTOS** para controle multitarefa em tempo real.

---

## 📸 Imagem do Carrinho

> Adicione aqui a foto do projeto:

![Carrinho Seguidor de Linha](images/carrinho.jpg)

---

## 🎯 Objetivo

Desenvolver um carrinho autônomo capaz de seguir uma linha no chão utilizando:

- 5 sensores infravermelhos
- Controle de dois motores DC
- Ponte H
- PWM para velocidade
- Controle proporcional-derivativo (PD)
- FreeRTOS para multitarefas

---

## ⚙️ Tecnologias Utilizadas

- **C/C++**
- **Raspberry Pi Pico SDK**
- **FreeRTOS**
- **PWM Hardware**
- **VS Code + CMake**

---

## 🧠 Funcionamento

O sistema possui duas tarefas principais:

### 🔹 `sensor_task`

Responsável por:

- Ler os 5 sensores IR
- Enviar valores para fila (`Queue`)

### 🔹 `motor_task`

Responsável por:

- Receber leitura dos sensores
- Calcular erro da posição da linha
- Aplicar controle **PD**
- Ajustar velocidade dos motores via PWM

---

## 📌 Mapeamento dos Sensores

| Sensor | GPIO |
|-------|------|
| S1 | 5 |
| S2 | 6 |
| S3 | 7 |
| S4 | 8 |
| S5 | 9 |

---

## 🔌 Controle dos Motores

| Função | GPIO |
|--------|------|
| IN1 | 10 |
| IN2 | 11 |
| IN3 | 12 |
| IN4 | 13 |
| ENA PWM | 14 |
| ENB PWM | 15 |

---

## 📈 Controle PD

```text
Correção = Kp * erro + Kd * derivada
