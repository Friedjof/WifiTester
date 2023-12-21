#ifndef __CONFIG_CONFIG_MANAGER_H__
#define __CONFIG_CONFIG_MANAGER_H__

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define DEFAULT_CONFIG_FILE "/config.json"
#define JSON_BUFFER_SIZE 512
#define MAX_WIFI_SSID_LENGTH 32
#define MAX_WIFI_PASSWORD_LENGTH 64
#define MAX_MQTT_HOST_LENGTH 64
#define MAX_MQTT_USER_LENGTH 64
#define MAX_MQTT_PASSWORD_LENGTH 64
#define MAX_MQTT_TOPIC_LENGTH 64

// type definitions
typedef struct {
    unsigned int interval;
} sys_t;

typedef struct {
    char ssid[MAX_WIFI_SSID_LENGTH];
    char password[MAX_WIFI_PASSWORD_LENGTH];
} my_wifi_t;

typedef struct {
    char enabled;
    char host[MAX_MQTT_HOST_LENGTH];
    uint16_t port;
    char user[MAX_MQTT_USER_LENGTH];
    char password[MAX_MQTT_PASSWORD_LENGTH];
    char topic[MAX_MQTT_TOPIC_LENGTH];
} mqtt_config_t;

typedef struct {
    sys_t sys;
    my_wifi_t wifi;
    mqtt_config_t mqtt;
} config_t;


class ConfigManager {
    private:
        const char* filename;

    public:
        ConfigManager(const char* filename);
        ConfigManager();
        ~ConfigManager();

        config_t config;

        void load_config();

        const unsigned int get_interval();
        
        const char* get_wifi_ssid();
        const char* get_wifi_password();

        char get_mqtt_enabled();
        const char* get_mqtt_host();
        uint16_t get_mqtt_port();
        const char* get_mqtt_user();
        const char* get_mqtt_password();
        const char* get_mqtt_topic();
};

#endif