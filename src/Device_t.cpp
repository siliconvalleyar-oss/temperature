/**
 * @file Device_t.cpp
 * @brief Implementación de la clase Device_t - Branch All
 * 
 * Implementa la lógica principal del dispositivo, incluyendo:
 * - Obtención de temperatura desde OpenWeatherMap API
 * - Display en pantalla OLED SSD1306 (I2C directo)
 * - Salida por consola
 * 
 * @author Proyecto RaspberryPi
 * @version 0.1.0
 * @date 2026
 */

#include "Device_t.hpp"
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include "nlohmann/json.hpp"

#ifdef HAS_BCM2835
    #include <bcm2835.h>
    
    // Dirección I2C del OLED
    #define OLED_ADDR 0x3C
    #define OLED_CMD 0x00
    #define OLED_DATA 0x40
#endif

using json = nlohmann::json;

/**
 * @brief Callback para curl
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

namespace Device {

#ifdef HAS_BCM2835
/**
 * @brief Escribe un byte al OLED via I2C
 */
bool oled_write_byte(uint8_t reg, uint8_t data) {
    char buf[2] = {static_cast<char>(reg), static_cast<char>(data)};
    bcm2835_i2c_setSlaveAddress(OLED_ADDR);
    return bcm2835_i2c_write(buf, 2) == BCM2835_I2C_REASON_OK;
}

/**
 * @brief Envía un comando al OLED
 */
void oled_command(uint8_t cmd) {
    oled_write_byte(OLED_CMD, cmd);
}

/**
 * @brief Envía datos al OLED
 */
void oled_data(uint8_t data) {
    oled_write_byte(OLED_DATA, data);
}

/**
 * @brief Inicializa el OLED SSD1306
 */
bool oled_init() {
    if (!bcm2835_i2c_begin()) {
        return false;
    }
    
    bcm2835_i2c_setSlaveAddress(OLED_ADDR);
    bcm2835_i2c_set_baudrate(100000);
    
    // Secuencia de inicialización SSD1306
    oled_command(0xAE); // Display OFF
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
    oled_command(0xAF); // Display ON
    
    bcm2835_delay(100);
    
    return true;
}

/**
 * @brief Limpia el buffer del OLED
 */
void oled_clear() {
    oled_command(0x21); // Columna
    oled_command(0);
    oled_command(127);
    oled_command(0x22); // Página
    oled_command(0);
    oled_command(7);
    
    for (int i = 0; i < 1024; i++) {
        oled_data(0x00);
    }
}

/**
 * @brief Muestra texto en el OLED (fuente básica 5x8)
 */
void oled_text(int x, int y, const char* text) {
    // Fuente básica 5x8 - solo mayúsculas y algunos caracteres
    static const uint8_t font[][5] = {
        {0x00,0x00,0x00,0x00,0x00}, // Space
        {0x7F,0x08,0x08,0x08,0x7F}, // H
        {0x3E,0x41,0x41,0x41,0x3E}, // O
        {0x7F,0x40,0x40,0x40,0x40}, // L
        {0x7F,0x09,0x19,0x29,0x46}, // R
        {0x3E,0x41,0x41,0x41,0x3E}, // O (same as O)
        {0x7F,0x01,0x01,0x01,0x01}, // I
        {0x3F,0x40,0x40,0x40,0x3F}, // U
        {0x7F,0x09,0x19,0x29,0x46}, // R
        {0x01,0x01,0x7F,0x01,0x01}, // T
        {0x3E,0x41,0x41,0x51,0x73}, // E (approx)
        {0x07,0x08,0x70,0x08,0x07}, // X (approx)
        {0x7F,0x41,0x41,0x41,0x3E}, // D (approx)
        {0x7F,0x49,0x49,0x49,0x41}, // M (approx)
        {0x3E,0x41,0x41,0x41,0x3E}, // P (approx)
        {0x7F,0x09,0x19,0x29,0x46}, // R
        {0x7F,0x49,0x49,0x49,0x36}, // S (approx)
    };
    
    // Posición en páginas
    int page = y / 8;
    int col = x;
    
    oled_command(0x21); // Columna
    oled_command(col);
    oled_command(127);
    oled_command(0x22); // Página
    oled_command(page);
    oled_command(page);
    
    for (int i = 0; text[i] != '\0' && col < 128; i++) {
        char c = text[i];
        int idx = 0;
        
        if (c >= 'A' && c <= 'Z') idx = c - 'A' + 1;
        else if (c == ' ') idx = 0;
        else if (c == ':') idx = 5;
        else if (c == '.') idx = 11;
        else idx = 0;
        
        if (idx >= 0 && idx < 17) {
            for (int j = 0; j < 5; j++) {
                oled_data(font[idx][j]);
            }
            oled_data(0x00); // Espacio entre letras
        }
        col += 6;
    }
}
#endif

Device_t::Device_t()
    : m_city("Buenos Aires")
    , m_country("AR")
    , m_apiKey("25647dfe3f88a205e55209d37e970b4d")
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

double Device_t::fetchTemperature(const std::string& city, const std::string& country) {
    std::string api_url = "http://api.openweathermap.org/data/2.5/weather?q=" 
                         + city + "," + country 
                         + "&appid=" + m_apiKey 
                         + "&units=metric";
    
    std::cout << "Device_t: Obteniendo temperatura de " << city << "..." << std::endl;
    
    std::string response = httpGet(api_url);
    
    if (response.empty()) {
        std::cerr << "Device_t: Respuesta vacía de la API" << std::endl;
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

double Device_t::parseTemperature(const std::string& jsonResponse) {
    try {
        auto jsonData = json::parse(jsonResponse);
        
        if (jsonData.contains("main") && jsonData["main"].contains("temp")) {
            return jsonData["main"]["temp"];
        } else {
            std::cerr << "Device_t: No se encontró 'temp' en la respuesta JSON" << std::endl;
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
        oled_text(0, 0, "TEMPERATURA");
        oled_text(0, 16, city.c_str());
        
        // Convertir temperatura a string
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
    std::cout << "========================================" << std::endl;
}

void Device_t::run() {
    std::cout << "Device_t: Iniciando ejecución..." << std::endl;
    
    if (!initializeHardware()) {
        std::cerr << "Device_t: Hardware no disponible" << std::endl;
    }
    
    // Mostrar mensaje inicial en OLED
    #ifdef HAS_BCM2835
        if (m_hardwareInitialized) {
            oled_clear();
            oled_text(0, 0, "TEMPERATURA");
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
