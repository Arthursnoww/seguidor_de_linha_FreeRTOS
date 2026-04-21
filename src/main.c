#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "hardware/pwm.h"
#include "../inc/FreeRTOSConfig.h"

#define SENSOR_COUNT 5
const uint SENSOR_PINS[SENSOR_COUNT] = {5, 6, 7, 8, 9}; // S1 a S5

const int pesos[SENSOR_COUNT] = {-2, -1, 0, 1, 2};


#define IN1 10
#define IN2 11
#define IN3 12
#define IN4 13
#define ENA 14 // PWM
#define ENB 15 // PWM

// === RTOS handles ===
QueueHandle_t xQueueSensor;
SemaphoreHandle_t xSensorMutex;

// === PWM ===
void pwm_init_motor(uint pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, 255);
    pwm_set_enabled(slice, true);
}

void pwm_set_speed(uint pin, uint8_t duty)
{
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(pin), duty);
}

void motor_forward(uint8_t speed_left, uint8_t speed_right)
{
    gpio_put(IN1, 0);
    gpio_put(IN2, 1); // Motor esquerdo frente
    gpio_put(IN3, 1);
    gpio_put(IN4, 0);                // Motor direito frente
    pwm_set_speed(ENA, speed_left);  // PWM no motor esquerdob
    pwm_set_speed(ENB, speed_right); // PWM no motor direito
    printf("Frente - Velocidade: Esquerdo = %d, Direito = %d\n", speed_left, speed_right);
}

void motor_left(uint8_t speed)
{
    gpio_put(IN1, 0);
    gpio_put(IN2, 0); // esquerdo parado
    gpio_put(IN3, 0);
    gpio_put(IN4, 1); // direito frente
    pwm_set_speed(ENA, 100);
    pwm_set_speed(ENB, speed);
    printf("Esquerda\n");
}



// === Task: leitura dos sensores ===
void sensor_task(void *pvParameters)
{
    int leitura[SENSOR_COUNT];

    while (1)
    {
        for (int i = 0; i < SENSOR_COUNT; i++)
        {
            leitura[i] = gpio_get(SENSOR_PINS[i]);
        }

        xSemaphoreTake(xSensorMutex, portMAX_DELAY);
        xQueueSend(xQueueSensor, &leitura, 0);
        xSemaphoreGive(xSensorMutex);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void motor_task(void *pvParameters) {
    int sensores[SENSOR_COUNT];
    int erro = 0, erro_anterior = 0;
    float Kp = 18.0;
    float Kd = 55.0;

    const int velocidade_base = 50;

    while (1) {
        if (xQueueReceive(xQueueSensor, &sensores, portMAX_DELAY) == pdTRUE) {
            // Cálculo do erro: soma dos pesos dos sensores ativados
            int soma_erro = 0;
            int total_ativos = 0;

            for (int i = 0; i < SENSOR_COUNT; i++) {
                if (sensores[i] == 1) {
                    soma_erro += pesos[i];
                    total_ativos++;
                }
            }

            if (total_ativos > 0) {
                erro = soma_erro / total_ativos;
            } else {
                // Linha perdida: mantém erro anterior
                erro = erro_anterior > 0 ? 3 : -3;
            }

            int derivada = erro - erro_anterior;
            erro_anterior = erro;

            float correcao = Kp * erro + Kd * derivada;

            // Define velocidades dos motores com base na correção
            int vel_esq = velocidade_base + (int)correcao;
            int vel_dir = velocidade_base - (int)correcao;

            // Saturação para não ultrapassar o PWM máximo
            if (vel_esq > 65) vel_esq = 65;
            if (vel_esq < 0) vel_esq = 0;
            if (vel_dir > 65) vel_dir = 65;
            if (vel_dir < 0) vel_dir = 0;

            // Aplica controle de velocidade com os dois motores pra frente
            motor_forward(vel_esq, vel_dir);
        }
    }
}

// === Main ===
int main()
{
    stdio_init_all();

    // Sensores
    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        gpio_init(SENSOR_PINS[i]);
        gpio_set_dir(SENSOR_PINS[i], GPIO_IN);
        gpio_pull_up(SENSOR_PINS[i]);
    }

    // Ponte H - direção
    gpio_init(IN1);
    gpio_set_dir(IN1, GPIO_OUT);
    gpio_init(IN2);
    gpio_set_dir(IN2, GPIO_OUT);
    gpio_init(IN3);
    gpio_set_dir(IN3, GPIO_OUT);
    gpio_init(IN4);
    gpio_set_dir(IN4, GPIO_OUT);

    // PWM nos pinos ENA/ENB
    pwm_init_motor(ENA);
    pwm_init_motor(ENB);

    // RTOS
    xQueueSensor = xQueueCreate(1, sizeof(int[SENSOR_COUNT]));
    xSensorMutex = xSemaphoreCreateMutex();

    xTaskCreate(sensor_task, "SensorTask", 256, NULL, 1, NULL);
    xTaskCreate(motor_task, "MotorTask", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
    return 0;
}