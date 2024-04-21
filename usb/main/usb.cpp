#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "driver/spi_master.h"
#include "driver/spi_common.h"
#include <unistd.h>


#define UART_PORT           UART_NUM_1
#define TX_PIN              GPIO_NUM_17   // Replace with your actual GPIO pin number
#define RX_PIN              GPIO_NUM_18  // Replace with your actual GPIO pin number

#define BAUD_RATE           115200
#define TAG "USB"

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



void send_uart_command(uint8_t *command, size_t len) {
    uart_write_bytes(UART_PORT, (const char *)command, len);
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

void listen_uart_response(uint8_t *buffer, size_t len) {
    uart_read_bytes(UART_PORT, buffer, len, 1000); // Wait indefinitely for response
}

// C for app main because ESP-IDF has a thing for C and not C++, C++: Ouch! Dude Uncool.
extern "C" void app_main() {
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
            0x01,                    // Packet ID (replace with actual motor ID)
            0x09, 0x00,              // Length (excluding headers)
            0x03,                    // Instruction (Write)
            0x74, 0x00,
            0x00, 0x03,              // value: Goal position: 0 (Little Endian)
            0x00, 0x00  , // CRC high
            0xCA,	0x89 // CRC low
            // CRC
    };

    uint8_t ping[] = {
            0xFF, 0xFF, 0xFD, 0x00, // Headers and Reserved
            0x01,                    // Packet ID (replace with actual motor ID)
            0x03, 0x00,              // Length (excluding headers)
            0x01,                    // Instruction (Ping)
            0x19,	0x4E // CRC low
    };

    uint8_t status[] = {
            0x00, 0x00, 0x00, 0x00, // Headers and Reserved,
            0x00,
            0x00,0x00, 0x00,0x00,0x00,
            0x00,0x00,0x00,0x00
    };

    size_t command_len = sizeof(cmd) / sizeof(cmd[0]); // All commands have the same length
    size_t tor_len = sizeof(tor) / sizeof(tor[0]); // All commands have the same length
    size_t pin_len = sizeof(ping) / sizeof(ping[0]); // All commands have the same length

    esp_log_level_set("*", ESP_LOG_INFO); // Set log level to INFO for all components

    // prepare torque
    uint16_t crc = calculate_crc(tor, tor_len - 2);
    uint8_t crc_msb = (crc >> 8) & 0xFF; // Shift right by 8 bits and mask with 0xFF to get the upper byte
    uint8_t crc_lsb = crc & 0xFF;         // Mask with 0xFF to get the lower byte
    tor[tor_len - 2] = crc_lsb;
    tor[tor_len - 1] = crc_msb;

    uint16_t crc_c = calculate_crc(cmd, command_len - 2);
    uint8_t crcc_msb = (crc_c >> 8) & 0xFF; // Shift right by 8 bits and mask with 0xFF to get the upper byte
    uint8_t crcc_lsb = crc_c & 0xFF;         // Mask with 0xFF to get the lower byte
    cmd[command_len - 2] = crcc_lsb;
    cmd[command_len- 1] = crcc_msb;

    vTaskDelay(1000);

    for (int x = 0; x < 10; x++){
        send_uart_command(ping, pin_len); // motor position to 0
        ESP_LOGI(TAG, "Transmitting");

        // Listen for response
        //uint8_t response[14]; // Adjust size according to your expected response
        listen_uart_response(status, sizeof(status));

        // Process response
        // You can add code here to process or log the received response
        ESP_LOG_BUFFER_HEXDUMP(TAG, status, sizeof(status), ESP_LOG_INFO);

        vTaskDelay(200);
    }
}
