#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"

#include <string.h>
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "esp_event.h"
#include "argtable3/argtable3.h"
#include "ping/ping_sock.h"
#include "esp_system.h"
#include "esp_netif.h"

#define SSID "Moto G"
#define PASS "83f17b859413"
#define TAG "WIFI"

static void cmd_ping_on_ping_success(esp_ping_handle_t hdl, void *args)
{
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    esp_ping_get_profile(hdl, ESP_PING_PROF_SEQNO, &seqno, sizeof(seqno));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TTL, &ttl, sizeof(ttl));
    esp_ping_get_profile(hdl, ESP_PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    esp_ping_get_profile(hdl, ESP_PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    esp_ping_get_profile(hdl, ESP_PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    printf("%" PRIu32 " bytes from %s icmp_seq=%" PRIu16 " ttl=%" PRIu16 " time=%" PRIu32 " ms\n",
            recv_len, ipaddr_ntoa((ip_addr_t *)&target_addr), seqno, ttl, elapsed_time);
}

static int do_ping_cmd(void)
{
    // Ping configuration
    esp_ping_config_t config = ESP_PING_DEFAULT_CONFIG();

    // IP address
    config.target_addr.type = IPADDR_TYPE_V4;
    ipaddr_aton("8.8.8.8", &config.target_addr);
    const char *ip_string = ipaddr_ntoa(&config.target_addr);
    printf("IP Address for config.target_addr: %s\n", ip_string);

    // Set callback function
    esp_ping_callbacks_t cbs = {
            .cb_args = NULL,
            .on_ping_success = cmd_ping_on_ping_success,
    };

    // Ping session parameters: (config - ping configuration, cbs - callback functions, hdl_out  - handle of ping session)
    esp_ping_handle_t ping;
    esp_ping_new_session(&config, &cbs, &ping);
    esp_ping_start(ping);

    return 0;
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
        case WIFI_EVENT_STA_START:
            printf("WiFi connecting WIFI_EVENT_STA_START ... \n");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            printf("WiFi connected WIFI_EVENT_STA_CONNECTED ... \n");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            printf("WiFi lost connection WIFI_EVENT_STA_DISCONNECTED ... \n");
            break;
        case IP_EVENT_STA_GOT_IP:
        {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "WiFi got IP: %s\n\n", ip4addr_ntoa((const ip4_addr_t *)&event->ip_info.ip));

        }
            break;
        default:
            break;
    }
}


void wifi_connection()
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
            .sta = {
                    .ssid = SSID,
                    .password = PASS}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_connect();
}

static esp_err_t post_handler(httpd_req_t *req)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    httpd_resp_send(req, "URI POST Response ... from ESP32", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void server_initiation()
{
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server_handle = NULL;
    httpd_start(&server_handle, &server_config);
    httpd_uri_t uri_post = {
            .uri = "/",
            .method = HTTP_POST,
            .handler = post_handler,
            .user_ctx = NULL};
    httpd_register_uri_handler(server_handle, &uri_post);
}

void app_main(void)
{
    vTaskDelay(10000);
    wifi_connection();
    server_initiation();

    vTaskDelay(30000 / portTICK_PERIOD_MS);
    do_ping_cmd();
}