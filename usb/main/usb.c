#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define UART_PORT           UART_NUM_0
#define TX_PIN              GPIO_NUM_1   // Replace with your actual GPIO pin number
#define RX_PIN              GPIO_NUM_3   // Replace with your actual GPIO pin number

#define BAUD_RATE           57600

void init_uart() {

    uart_config_t uart_config = {
            .baud_rate = BAUD_RATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, 16, 17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, 1024, 0, 0, NULL, 0);
}

void send_uart_command(uint8_t *command, size_t len) {
    uart_write_bytes(UART_PORT, (const char *)command, len);
}

void app_main() {
    init_uart();

    // Set goal position (clockwise) command packets for two motors
    uint8_t motor1_id = 1;
    uint16_t motor1_goal_position = 1500; // Example desired goal position for motor 1
    uint8_t motor1_goal_position_LSB = motor1_goal_position & 0xFF;
    uint8_t motor1_goal_position_MSB = (motor1_goal_position >> 8) & 0xFF;
    uint8_t motor1_command[] = {motor1_id, 0x07, motor1_goal_position_LSB, motor1_goal_position_MSB}; // Example command for motor 1

    uint8_t motor2_id = 2;
    uint16_t motor2_goal_position = 2000; // Example desired goal position for motor 2
    uint8_t motor2_goal_position_LSB = motor2_goal_position & 0xFF;
    uint8_t motor2_goal_position_MSB = (motor2_goal_position >> 8) & 0xFF;
    uint8_t motor2_command[] = {motor2_id, 0x07, motor2_goal_position_LSB, motor2_goal_position_MSB}; // Example command for motor 2

    size_t command_len = sizeof(motor1_command) / sizeof(motor1_command[0]); // All commands have the same length

    while (1) {
        // Send commands for both motors
        send_uart_command(motor1_command, command_len);
        send_uart_command(motor2_command, command_len);

        // Delay or perform other actions as needed
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Example: sending commands every second
    }
}
