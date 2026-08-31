/**
 * @file Device_t.cpp
 * @brief Implementación de la clase Device_t
 * 
 * Usa Open-Meteo API (gratis, sin API key) para obtener clima.
 * 
 * @author Proyecto RaspberryPi
 * @version 0.2.0
 * @date 2026
 */

#include "Device_t.hpp"
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include "nlohmann/json.hpp"

#ifdef HAS_BCM2835
    #include <bcm2835.h>
    
    #define OLED_ADDR 0x3C
    #define OLED_CMD 0x00
    #define OLED_DATA 0x40
#endif

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

namespace Device {

#ifdef HAS_BCM2835
bool oled_write_byte(uint8_t reg, uint8_t data) {
    char buf[2] = {static_cast<char>(reg), static_cast<char>(data)};
    bcm2835_i2c_setSlaveAddress(OLED_ADDR);
    return bcm2835_i2c_write(buf, 2) == BCM2835_I2C_REASON_OK;
}

void oled_command(uint8_t cmd) {
    oled_write_byte(OLED_CMD, cmd);
}

void oled_data(uint8_t data) {
    oled_write_byte(OLED_DATA, data);
}

bool oled_init() {
    if (!bcm2835_i2c_begin()) {
        return false;
    }
    
    bcm2835_i2c_setSlaveAddress(OLED_ADDR);
    bcm2835_i2c_set_baudrate(100000);
    
    oled_command(0xAE);
    oled_command(0xD5); oled_command(0x80);
    oled_command(0xA8); oled_command(0x3F);
    oled_command(0xD3); oled_command(0x00);
    oled_command(0x40);
    oled_command(0x8D); oled_command(0x14);
    oled_command(0x20); oled_command(0x00);
    oled_command(0xA1);
    oled_command(0xC8);
    oled_command(0xDA); oled_command(0x12);
    oled_command(0x81); oled_command(0xCF);
    oled_command(0xD9); oled_command(0xF1);
    oled_command(0xDB); oled_command(0x40);
    oled_command(0xA4);
    oled_command(0xA6);
    oled_command(0xAF);
    
    bcm2835_delay(100);
    return true;
}

void oled_clear() {
    oled_command(0x21); oled_command(0); oled_command(127);
    oled_command(0x22); oled_command(0); oled_command(7);
    for (int i = 0; i < 1024; i++) oled_data(0x00);
}

void oled_text(int x, int y, const char* text) {
    // Fuente ASCII 5x8 estándar (0x20-0x7E)
    static const uint8_t font5x8[][5] = {
        {0x00,0x00,0x00,0x00,0x00}, // 32: Space
        {0x00,0x00,0x5F,0x00,0x00}, // 33: !
        {0x00,0x07,0x00,0x07,0x00}, // 34: "
        {0x14,0x7F,0x14,0x7F,0x14}, // 35: #
        {0x24,0x2A,0x7F,0x2A,0x12}, // 36: $
        {0x23,0x13,0x08,0x64,0x62}, // 37: %
        {0x36,0x49,0x55,0x22,0x50}, // 38: &
        {0x00,0x05,0x03,0x00,0x00}, // 39: '
        {0x00,0x1C,0x22,0x41,0x00}, // 40: (
        {0x00,0x41,0x22,0x1C,0x00}, // 41: )
        {0x14,0x08,0x3E,0x08,0x14}, // 42: *
        {0x08,0x08,0x3E,0x08,0x08}, // 43: +
        {0x00,0x50,0x30,0x00,0x00}, // 44: ,
        {0x08,0x08,0x08,0x08,0x08}, // 45: -
        {0x00,0x60,0x60,0x00,0x00}, // 46: .
        {0x20,0x10,0x08,0x04,0x02}, // 47: /
        {0x3E,0x51,0x49,0x45,0x3E}, // 48: 0
        {0x00,0x42,0x7F,0x40,0x00}, // 49: 1
        {0x42,0x61,0x51,0x49,0x46}, // 50: 2
        {0x21,0x41,0x45,0x4B,0x31}, // 51: 3
        {0x18,0x14,0x12,0x7F,0x10}, // 52: 4
        {0x27,0x45,0x45,0x45,0x39}, // 53: 5
        {0x3C,0x4A,0x49,0x49,0x30}, // 54: 6
        {0x01,0x71,0x09,0x05,0x03}, // 55: 7
        {0x36,0x49,0x49,0x49,0x36}, // 56: 8
        {0x06,0x49,0x49,0x29,0x1E}, // 57: 9
        {0x00,0x36,0x36,0x00,0x00}, // 58: :
        {0x00,0x56,0x36,0x00,0x00}, // 59: ;
        {0x08,0x14,0x22,0x41,0x00}, // 60: <
        {0x14,0x14,0x14,0x14,0x14}, // 61: =
        {0x00,0x41,0x22,0x14,0x08}, // 62: >
        {0x02,0x01,0x51,0x09,0x06}, // 63: ?
        {0x32,0x49,0x79,0x41,0x3E}, // 64: @
        {0x7E,0x11,0x11,0x11,0x7E}, // 65: A
        {0x7F,0x49,0x49,0x49,0x36}, // 66: B
        {0x3E,0x41,0x41,0x41,0x22}, // 67: C
        {0x7F,0x41,0x41,0x22,0x1C}, // 68: D
        {0x7F,0x49,0x49,0x49,0x41}, // 69: E
        {0x7F,0x09,0x09,0x09,0x01}, // 70: F
        {0x3E,0x41,0x49,0x49,0x7A}, // 71: G
        {0x7F,0x08,0x08,0x08,0x7F}, // 72: H
        {0x00,0x41,0x7F,0x41,0x00}, // 73: I
        {0x20,0x40,0x41,0x3F,0x01}, // 74: J
        {0x7F,0x08,0x14,0x22,0x41}, // 75: K
        {0x7F,0x40,0x40,0x40,0x40}, // 76: L
        {0x7F,0x02,0x0C,0x02,0x7F}, // 77: M
        {0x7F,0x04,0x08,0x10,0x7F}, // 78: N
        {0x3E,0x41,0x41,0x41,0x3E}, // 79: O
        {0x7F,0x09,0x09,0x09,0x06}, // 80: P
        {0x3E,0x41,0x51,0x21,0x5E}, // 81: Q
        {0x7F,0x09,0x19,0x29,0x46}, // 82: R
        {0x46,0x49,0x49,0x49,0x31}, // 83: S
        {0x01,0x01,0x7F,0x01,0x01}, // 84: T
        {0x3F,0x40,0x40,0x40,0x3F}, // 85: U
        {0x1F,0x20,0x40,0x20,0x1F}, // 86: V
        {0x3F,0x40,0x38,0x40,0x3F}, // 87: W
        {0x63,0x14,0x08,0x14,0x63}, // 88: X
        {0x07,0x08,0x70,0x08,0x07}, // 89: Y
        {0x61,0x51,0x49,0x45,0x43}, // 90: Z
    };
    
    int page = y / 8;
    int col = x;
    
    oled_command(0x21); oled_command(col); oled_command(127);
    oled_command(0x22); oled_command(page); oled_command(page);
    
    for (int i = 0; text[i] != '\0' && col < 123; i++) {
        char c = text[i];
        int idx = -1;
        
        // Convertir minúsculas a mayúsculas
        if (c >= 'a' && c <= 'z') {
            c = c - 32; // Convertir a mayúscula
        }
        
        if (c >= ' ' && c <= 'Z') {
            idx = c - ' ';
        }
        
        if (idx >= 0 && idx < 59) {
            for (int j = 0; j < 5; j++) {
                oled_data(font5x8[idx][j]);
            }
            oled_data(0x00); // Espacio entre letras
        } else {
            // Para caracteres no mapeados, enviar espacios
            for (int j = 0; j < 6; j++) oled_data(0x00);
        }
        col += 6;
    }
}
#endif

Device_t::Device_t()
    : m_city("Buenos Aires")
    , m_country("AR")
    , m_apiKey("")
    , m_hardwareInitialized(false)
    , m_updateIntervalMs(60000)
{
    std::cout << "Device_t: Constructor llamado" << std::endl;
}

Device_t::~Device_t() {
    std::cout << "Device_t: Destructor llamado" << std::endl;
    #ifdef HAS_BCM2835
        if (m_hardwareInitialized) {
            bcm2835_i2c_end();
            bcm2835_close();
        }
    #endif
}

bool Device_t::initializeHardware() {
    std::cout << "Device_t: Inicializando hardware..." << std::endl;
    
    #ifdef HAS_BCM2835
        if (!bcm2835_init()) {
            std::cerr << "Device_t: Error al inicializar bcm2835" << std::endl;
            return false;
        }
        
        if (!oled_init()) {
            std::cerr << "Device_t: Error al inicializar OLED" << std::endl;
            return false;
        }
        
        m_hardwareInitialized = true;
        std::cout << "Device_t: Hardware inicializado correctamente" << std::endl;
        return true;
    #else
        std::cout << "Device_t: Modo PC (sin hardware)" << std::endl;
        return false;
    #endif
}

/**
 * @brief Obtiene coordenadas de una ciudad usando Geocoding API de Open-Meteo
 * @return "lat,lon" o string vacío en caso de error
 */
std::string Device_t::getCoordinates(const std::string& city) {
    // Reemplazar espacios con + para URL encoding
    std::string encodedCity = city;
    for (char& c : encodedCity) {
        if (c == ' ') c = '+';
    }
    
    std::string url = "https://geocoding-api.open-meteo.com/v1/search?name=" 
                     + encodedCity + "&count=1&language=es";
    
    std::string response = httpGet(url);
    
    if (response.empty()) {
        return "";
    }
    
    try {
        auto data = json::parse(response);
        
        if (data.contains("results") && data["results"].size() > 0) {
            double lat = data["results"][0]["latitude"];
            double lon = data["results"][0]["longitude"];
            
            std::ostringstream ss;
            ss << lat << "," << lon;
            return ss.str();
        }
    } catch (json::parse_error& e) {
        std::cerr << "Device_t: Error al parsear geocoding: " << e.what() << std::endl;
    }
    
    return "";
}

/**
 * @brief Obtiene clima de Open-Meteo API (gratis, sin API key)
 */
double Device_t::fetchTemperature(const std::string& city, const std::string& country) {
    // Primero obtener coordenadas
    std::string coords = getCoordinates(city);
    
    if (coords.empty()) {
        std::cerr << "Device_t: No se pudieron obtener coordenadas de " << city << std::endl;
        return -999.0;
    }
    
    // Separar lat y lon
    size_t commaPos = coords.find(',');
    if (commaPos == std::string::npos) {
        return -999.0;
    }
    
    std::string lat = coords.substr(0, commaPos);
    std::string lon = coords.substr(commaPos + 1);
    
    // Construir URL de Open-Meteo
    std::string url = "https://api.open-meteo.com/v1/forecast?latitude=" + lat 
                    + "&longitude=" + lon 
                    + "&current_weather=true";
    
    std::cout << "Device_t: Obteniendo clima de " << city << "..." << std::endl;
    
    std::string response = httpGet(url);
    
    if (response.empty()) {
        std::cerr << "Device_t: Respuesta vacía de Open-Meteo" << std::endl;
        return -999.0;
    }
    
    return parseTemperature(response);
}

std::string Device_t::httpGet(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    long http_code = 0;
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_cleanup(curl);
        
        if (res != CURLE_OK) {
            std::cerr << "Device_t: Error en curl: " << curl_easy_strerror(res) << std::endl;
            return "";
        }
        
        if (http_code != 200) {
            std::cerr << "Device_t: HTTP error, código: " << http_code << std::endl;
            return "";
        }
    } else {
        std::cerr << "Device_t: Error al inicializar curl" << std::endl;
        return "";
    }
    
    return readBuffer;
}

/**
 * @brief Parsea la respuesta de Open-Meteo
 * 
 * Formato: { "current_weather": { "temperature": 22.5, ... } }
 */
double Device_t::parseTemperature(const std::string& jsonResponse) {
    try {
        auto data = json::parse(jsonResponse);
        
        if (data.contains("current_weather") && 
            data["current_weather"].contains("temperature")) {
            return data["current_weather"]["temperature"];
        } else {
            std::cerr << "Device_t: No se encontró 'current_weather.temperature'" << std::endl;
            return -999.0;
        }
    } catch (json::parse_error& e) {
        std::cerr << "Device_t: Error al parsear JSON: " << e.what() << std::endl;
        return -999.0;
    }
}

void Device_t::displayOnOLED(double temperature, const std::string& city) {
    #ifdef HAS_BCM2835
        if (!m_hardwareInitialized) {
            return;
        }
        
        oled_clear();
        oled_text(0, 0, "CLIMA");
        oled_text(0, 16, city.c_str());
        
        char tempStr[20];
        snprintf(tempStr, sizeof(tempStr), "%.1f C", temperature);
        oled_text(0, 32, tempStr);
        
        std::cout << "Device_t: Temperatura mostrada en OLED" << std::endl;
    #endif
}

void Device_t::displayOnConsole(double temperature, const std::string& city) {
    std::cout << "========================================" << std::endl;
    std::cout << "  Temperatura Actual" << std::endl;
    std::cout << "  Ciudad: " << city << std::endl;
    std::cout << "  Temperatura: " << temperature << " °C" << std::endl;
    std::cout << "  Fuente: Open-Meteo (gratis)" << std::endl;
    std::cout << "========================================" << std::endl;
}

void Device_t::run() {
    std::cout << "Device_t: Iniciando ejecución..." << std::endl;
    
    if (!initializeHardware()) {
        std::cerr << "Device_t: Hardware no disponible" << std::endl;
    }
    
    #ifdef HAS_BCM2835
        if (m_hardwareInitialized) {
            oled_clear();
            oled_text(0, 0, "CLIMA");
            oled_text(0, 16, "INICIANDO...");
            std::cout << "Device_t: Mensaje inicial enviado a OLED" << std::endl;
        }
    #endif
    
    while (true) {
        double temperature = fetchTemperature(m_city, m_country);
        
        if (temperature > -900.0) {
            displayOnOLED(temperature, m_city);
            displayOnConsole(temperature, m_city);
        } else {
            std::cerr << "Device_t: No se pudo obtener la temperatura" << std::endl;
            
            #ifdef HAS_BCM2835
                if (m_hardwareInitialized) {
                    oled_clear();
                    oled_text(0, 0, "ERROR");
                    oled_text(0, 16, "SIN DATOS");
                }
            #endif
        }
        
        std::cout << "Device_t: Esperando " << m_updateIntervalMs / 1000 
                  << " segundos..." << std::endl;
        
        #ifdef HAS_BCM2835
            bcm2835_delay(m_updateIntervalMs);
        #else
            std::this_thread::sleep_for(std::chrono::milliseconds(m_updateIntervalMs));
        #endif
    }
}

} // namespace Device
