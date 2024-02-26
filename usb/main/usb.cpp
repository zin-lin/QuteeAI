#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "driver/spi_master.h"
#include "driver/spi_common.h"



#define UART_PORT           UART_NUM_0
#define TX_PIN              GPIO_NUM_17   // Replace with your actual GPIO pin number
#define RX_PIN              GPIO_NUM_18   // Replace with your actual GPIO pin number

#define BAUD_RATE           57600

const int DXL_IDs[] = {1};
void init_uart() {

    uart_config_t uart_config = {
            .baud_rate = BAUD_RATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    uart_driver_install(UART_PORT, 1024, 0, 0, NULL, 0);
    uart_set_mode(UART_NUM_1, UART_MODE_RS485_APP_CTRL);
    UART1.rs485_conf.rs485rxby_tx_en = 0;  // don't send while receiving => collision avoidance
    UART1.rs485_conf.rs485tx_rx_en = 1;  // loopback (1), so collision detection works



}

// Function to perform synchronous write operation
/// Non Functional
/// TO-DO fix this to sync write
void sync_write(uint8_t id, uint16_t address ,uint8_t length) {
    // Construct the packet to send
    uint8_t packet[128]; // Adjust the size as per your requirement
    packet[0] = 0xFF;
    packet[1] = 0xFF;
    packet[2] = id;
    packet[3] = length + 4;
    packet[4] = 0x83;
    packet[5] = address & 0xFF;
    packet[6] = address >> 8;
    packet[7] = length;
    for (int i = 0; i < length; i++) {
        packet[8 + i] = 0x00;
    }
    uint8_t checksum = ~(id + length + 4 + 0x83 + (address & 0xFF) + (address >> 8) + length);
    for (int i = 0; i < length; i++) {
        checksum -= 0x00;
    }
    packet[8 + length] = checksum;

    // Send the packet over UART
    uart_write_bytes(UART_PORT, (const char *)packet, length + 9);
}

void send_uart_command(uint8_t *command, size_t len) {
    uart_write_bytes(UART_PORT, (const char *)command, len);
}

void uart_print(const char *message) {
    uart_write_bytes(UART_PORT, message, strlen(message));
}

void init_motors() {

    // init sync write
    for (int i = 0; i < 12; i ++){
        sync_write(DXL_IDs[i],116,4 );
    }

}


// calculate crc
uint16_t calculate_crc(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF; // Initialize CRC with 0xFFFF
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8; // XOR with next byte
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; // Polynomial division
            } else {
                crc <<= 1; // Shift left
            }
        }
    }
    return crc;
}

// C for app main because ESP-IDF has a thing for C and not C++, C++: Ouch! Dude Uncool.
extern "C" void app_main() {
    init_uart();
    init_motors();
//
//    // Set goal position (clockwise) command packets for two motors
    uint8_t motor1_id = 1;
    uint16_t motor1_goal_position = 1500; // Example desired goal position for motor 1
    uint8_t motor1_goal_position_LSB = motor1_goal_position & 0xFF;
    uint8_t motor1_goal_position_MSB = (motor1_goal_position >> 8) & 0xFF;
    uint8_t motor1_command[] = {motor1_id, 0x03, motor1_goal_position_LSB, motor1_goal_position_MSB}; // Example command for motor 1
//
////    uint8_t motor2_id = 12;
////    uint16_t motor2_goal_position = 2000; // Example desired goal position for motor 2
////    uint8_t motor2_goal_position_LSB = motor2_goal_position & 0xFF;
////    uint8_t motor2_goal_position_MSB = (motor2_goal_position >> 8) & 0xFF;
////    uint8_t motor2_command[] = {motor2_id, 0x07, motor2_goal_position_LSB, motor2_goal_position_MSB}; // Example command for motor 2

// torque cmd
    uint8_t tor[] = {
            0xFF, 0xFF, 0xFD, 0x00, // Headers and Reserved
            0x01,                    // Packet ID (replace with actual ID)
            0x07, 0x00,              // Length (excluding headers)
            0x03,                    // Instruction (Write)
            0x40, 0x00,         // add: 64 (Little Endian)
            0x01, 0x00,        // value: 1
            0xCA,	0x89        // crc
            // CRC
    };

// position cmd
    uint8_t cmd[] = {
            0xFF, 0xFF, 0xFD, 0x00, // Headers and Reserved
            0x01,                    // Packet ID (replace with actual ID)
            0x09, 0x00,              // Length (excluding headers)
            0x03,                    // Instruction (Write)
            0x74, 0x00,
            motor1_goal_position_LSB, motor1_goal_position_MSB,              // value: Goal position: 1500 (Little Endian)
            0x00, 0x00  , // CRC high
            0xCA,	0x89 // CRC low
            // CRC
    };


    size_t command_len = sizeof(cmd) / sizeof(cmd[0]); // All commands have the same length
    size_t tor_len = sizeof(tor) / sizeof(tor[0]); // All commands have the same length
    //
//
//    // Send commands for both motors


    // Delay or perform other actions as needed
    // vTaskDelay(1000 / portTICK_PERIOD_MS); // Example: sending commands every second

    esp_log_level_set("*", ESP_LOG_INFO); // Set log level to INFO for all components

    while (1){
        uint16_t crc = calculate_crc(tor, tor_len - 2);
        uint8_t crc_msb = (crc >> 8) & 0xFF; // Shift right by 8 bits and mask with 0xFF to get the upper byte
        uint8_t crc_lsb = crc & 0xFF;         // Mask with 0xFF to get the lower byte
        tor[tor_len - 2] = crc_lsb;
        tor[tor_len - 1] = crc_msb;

        send_uart_command(tor, tor_len);
        send_uart_command(cmd, command_len);

        ESP_LOGI("main", "Hello, ESP32-S3!");
        ESP_LOGI("main", "This is a logging test.");
    }
}
