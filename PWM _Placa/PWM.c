#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"
#include <stdio.h>

#define PIN_LED 12
#define PERIODO 20000 // Período em us
#define WRAP 39062  // Valor de "WRAP" com base na equação da frequência pwm, wrap = f_clk/(f_pwm * (di + df/16))
                    // f_clk = 125 * 10^6, f_pwm = 50, assumindo (di + df/16) = 64: wrap = 125000000 / (50 * 64) = 39062
static void set_pwm_duty(uint16_t duty_us); // Função para configurar o ciclo de trabalho (duty cycle) do PWM
static uint pwm_setup_irq(); // Configuração do PWM com interrupção
static void wrapHandler(); // Manipulador de interrupção para atualizar o ciclo de trabalho do LED

int main() {
    stdio_init_all();
    
    gpio_set_function(PIN_LED, GPIO_FUNC_PWM); // Configura o GPIO do LED como PWM
    pwm_set_clkdiv(pwm_gpio_to_slice_num(PIN_LED), 64.0); // Configura o divisor de clock para o PIN_LED
    
    // Define posições iniciais do LED
    printf("Ajustando o LED para ciclo de 2400\n");
    set_pwm_duty(2400); // LED com ciclo ativo de 2400
    sleep_ms(5000);
    
    // Altera a posição do LED
    printf("Ajustando o LED para ciclo de 1470\n");
    set_pwm_duty(1470); // LED com ciclo ativo de 1470
    sleep_ms(5000);
    
    // Altera a posição do LED
    printf("Ajustando o LED para ciclo de 500\n");
    set_pwm_duty(500); // LED com ciclo ativo de 500
    sleep_ms(5000);
    
    // Configura e ativa as interrupções do PWM
    pwm_setup_irq(PIN_LED);
    
    // Loop infinito para manter o programa em execução
    while (true) {
        sleep_ms(10);
    }
}
// Função para configurar o ciclo de trabalho (duty cycle) do PWM
static void set_pwm_duty(uint16_t duty_us) {
    uint slice = pwm_gpio_to_slice_num(PIN_LED); // Obtém o slice do PWM correspondente ao GPIO do LED
    pwm_set_clkdiv(slice, 64.0); // Define o valor do Clock Divider para o PWM
    pwm_set_wrap(slice, WRAP); // Define o valor de WRAP para o PWM
    pwm_set_gpio_level(PIN_LED, (WRAP * duty_us)/PERIODO); // Envia o valor do nível para o LED
    pwm_set_enabled(slice, true); // Habilita o PWM para o slice correspondente
}
// Configuração do PWM com interrupção
static uint pwm_setup_irq() {

   uint slice = pwm_gpio_to_slice_num(PIN_LED); // Obtém o canal PWM da GPIO
   gpio_set_function(PIN_LED, GPIO_FUNC_PWM); // Configura o GPIO como PWM
    
    pwm_clear_irq(slice); // Reseta qualquer interrupção para o slice
    pwm_set_irq_enabled(slice, true); // Habilita a interrupção do PWM para o slice
    irq_set_exclusive_handler(PWM_IRQ_WRAP, wrapHandler); // Define a rotina de interrupção
    irq_set_enabled(PWM_IRQ_WRAP, true); // Habilitando a interrupção
    
    pwm_config config = pwm_get_default_config(); // Obtém a configuração padrão do PWM
    pwm_config_set_clkdiv(&config, 64.0); // Define o divisor de clock para ajustar do PWM
    pwm_init(slice, &config, true); // Inicializa o PWM com as configurações definidas

    return slice;
}
// Manipulador de interrupção para atualizar o ciclo de trabalho do PIN_LED
void wrapHandler() {
    static uint16_t duty = 500; // Ciclo ativo inicial de 500 us
    static bool original_direction = true; // Define que o movimento deve ser feito da direção inicial
    pwm_clear_irq(pwm_gpio_to_slice_num(PIN_LED)); // Limpa a interrupção do PWM do LED
    // Alterna a direção do movimento do LED (ciclo de trabalho)
    if (original_direction) {
        duty += 5;
        if (duty >= 2400) {
            original_direction = false;
        }
    } 
    else {
        duty -= 5;
        if (duty <= 500) {
            original_direction = true;
        }
    }
    set_pwm_duty(duty); // Define o PWM do LED
}