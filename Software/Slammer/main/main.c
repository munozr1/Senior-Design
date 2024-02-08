#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"

#define PIN_19      19
#define SERVO_MIN_PULSEWIDTH 500  // Minimum pulse width in microseconds
#define SERVO_MAX_PULSEWIDTH 2400 // Maximum pulse width in microseconds
#define SERVO_MAX_DEGREE 180      // Maximum angle in degrees

void init_pwm() {
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PIN_19);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    // Set frequency to 50Hz 
    pwm_config.cmpr_a = 0;       // Initial duty cycle

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
}

void move_servo(int angle) {
    uint32_t duty = (SERVO_MIN_PULSEWIDTH + (angle * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / SERVO_MAX_DEGREE));
    ledc_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty); // idk if this line updates the duty cycle or not. This lines should be responsible
    // for moving the servo but it does not move at all.

}

void app_main(void) {
    init_pwm();

    while (1) {
        // Move the servo to the right (0 degrees)
        move_servo(0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Move the servo to the left (180 degrees)
        move_servo(180);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
