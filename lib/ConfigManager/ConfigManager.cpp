#include "ConfigManager.h"


// this section is for the implementation
ConfigManager::ConfigManager(const char* filename) {
    this->filename = filename;

    // start LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Could not initialize LittleFS");
        return;
    }
    
    // load config
    this->load_config();
}

ConfigManager::ConfigManager() {
    this->filename = DEFAULT_CONFIG_FILE;

    // start LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Could not initialize LittleFS");
        return;
    }

    // load config
    this->load_config();
}

ConfigManager::~ConfigManager() { }

// load config from LittleFS
void ConfigManager::load_config() {
    Serial.println("Loading config");
    Serial.print("Filename: ");
    Serial.println(this->filename);

    // Open file for reading
    File file = LittleFS.open(this->filename, "r");

    if (!file) {
        Serial.println("Failed to open config file");
        return;
    }
    Serial.println("File opened");

    // Allocate a buffer to store contents of the file
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.println("Failed to read file, using default configuration");
        return;
    }

    // Copy values from the JsonDocument to the Config
    this->config.sys.interval = doc["system"]["interval"] | 2000;

    strlcpy(this->config.wifi.ssid, doc["wifi"]["ssid"] | "", sizeof(this->config.wifi.ssid));
    strlcpy(this->config.wifi.password, doc["wifi"]["password"] | "", sizeof(this->config.wifi.password));

    this->config.mqtt.enabled = doc["mqtt"]["enabled"] | 0;
    strlcpy(this->config.mqtt.host, doc["mqtt"]["host"] | "", sizeof(this->config.mqtt.host));
    this->config.mqtt.port = doc["mqtt"]["port"] | 1883;
    strlcpy(this->config.mqtt.user, doc["mqtt"]["user"] | "", sizeof(this->config.mqtt.user));
    strlcpy(this->config.mqtt.password, doc["mqtt"]["password"] | "", sizeof(this->config.mqtt.password));
    strlcpy(this->config.mqtt.topic, doc["mqtt"]["topic"] | "", sizeof(this->config.mqtt.topic));

    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
}


// getters
const unsigned int ConfigManager::get_interval() {
    return this->config.sys.interval;
}

const char* ConfigManager::get_wifi_ssid() {
    return this->config.wifi.ssid;
}

const char* ConfigManager::get_wifi_password() {
    return this->config.wifi.password;
}

char ConfigManager::get_mqtt_enabled() {
    return this->config.mqtt.enabled;
}

const char* ConfigManager::get_mqtt_host() {
    return this->config.mqtt.host;
}

uint16_t ConfigManager::get_mqtt_port() {
    return this->config.mqtt.port;
}

const char* ConfigManager::get_mqtt_user() {
    return this->config.mqtt.user;
}

const char* ConfigManager::get_mqtt_password() {
    return this->config.mqtt.password;
}

const char* ConfigManager::get_mqtt_topic() {
    return this->config.mqtt.topic;
}
