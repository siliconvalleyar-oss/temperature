/**
 * @file Device_t.cpp
 * @brief Implementación de la clase Device_t - Branch Raspberry
 * 
 * Implementa la lógica principal del dispositivo para Raspberry Pi,
 * incluyendo:
 * - Obtención de temperatura desde OpenWeatherMap API
 * - Display en pantalla OLED SSD1306
 * - Salida por consola
 * 
 * @author Proyecto RaspberryPi
 * @version 0.1.0
 * @date 2026
 * 
 * @note Esta rama está diseñada exclusivamente para Raspberry Pi
 */

#include "Device_t.hpp"
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include "oled/SSD1306_OLED.hpp"
#include <bcm2835.h>

// Using para simplificar el uso de nlohmann::json
using json = nlohmann::json;

/**
 * @brief Callback para escribir la respuesta de curl en un std::string
 * @param contents Puntero a los datos recibidos
 * @param size Tamaño de cada elemento
 * @param nmemb Número de elementos
 * @param s Puntero al string donde almacenar los datos
 * @return Número total de bytes procesados
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    s->append((char*)contents, newLength);
    return newLength;
}

namespace Device {

/**
 * @brief Constructor de Device_t
 * 
 * Inicializa valores por defecto y configura parámetros del dispositivo.
 */
Device_t::Device_t()
    : m_city("Buenos Aires")
    , m_country("AR")
    , m_apiKey("25647dfe3f88a205e55209d37e970b4d")  // API key de ejemplo
    , m_hardwareInitialized(false)
    , m_updateIntervalMs(60000)  // 60 segundos por defecto
{
    std::cout << "Device_t: Constructor llamado" << std::endl;
    std::cout << "Device_t: Ciudad configurada: " << m_city << ", " << m_country << std::endl;
}

/**
 * @brief Destructor de Device_t
 * 
 * Limpia recursos del dispositivo.
 */
Device_t::~Device_t() {
    std::cout << "Device_t: Destructor llamado" << std::endl;
    if (m_hardwareInitialized) {
        bcm2835_close();
    }
}

/**
 * @brief Inicializa el hardware del dispositivo
 * 
 * Inicializa bcm2835 para acceso a GPIO/SPI/I2C.
 * Requiere ejecutarse como root en Raspberry Pi.
 * 
 * @return true si la inicialización fue exitosa
 */
bool Device_t::initializeHardware() {
    std::cout << "Device_t: Inicializando hardware..." << std::endl;
    
    if (!bcm2835_init()) {
        std::cerr << "Device_t: Error al inicializar bcm2835" << std::endl;
        std::cerr << "Device_t: Verifique que está ejecutando como root" << std::endl;
        m_hardwareInitialized = false;
        return false;
    }
    
    m_hardwareInitialized = true;
    std::cout << "Device_t: bcm2835 inicializado correctamente" << std::endl;
    
    return true;
}

/**
 * @brief Obtiene la temperatura de la API de OpenWeatherMap
 * 
 * Realiza una solicitud HTTP GET a la API y parsea la respuesta
 * para extraer la temperatura actual.
 * 
 * @param city Nombre de la ciudad
 * @param country Código del país
 * @return Temperatura en grados Celsius, o -999.0 en caso de error
 */
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

/**
 * @brief Realiza una solicitud HTTP GET
 * 
 * Utiliza libcurl para realizar la solicitud HTTP.
 * 
 * @param url URL completa de la solicitud
 * @return Respuesta como string, o string vacío en caso de error
 */
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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);  // Timeout de 10 segundos
        
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
 * @brief Parsea la respuesta JSON y extrae la temperatura
 * 
 * @param jsonResponse Respuesta JSON de la API
 * @return Temperatura en grados Celsius
 */
double Device_t::parseTemperature(const std::string& jsonResponse) {
    try {
        auto jsonData = json::parse(jsonResponse);
        
        if (jsonData.contains("main") && jsonData["main"].contains("temp")) {
            double temperature = jsonData["main"]["temp"];
            return temperature;
        } else {
            std::cerr << "Device_t: No se encontró 'temp' en la respuesta JSON" << std::endl;
            return -999.0;
        }
    } catch (json::parse_error& e) {
        std::cerr << "Device_t: Error al parsear JSON: " << e.what() << std::endl;
        return -999.0;
    }
}

/**
 * @brief Muestra la temperatura en la pantalla OLED
 * 
 * Inicializa el display SSD1306 y muestra la temperatura formateada.
 * 
 * @param temperature Temperatura a mostrar
 * @param city Nombre de la ciudad
 */
void Device_t::displayOnOLED(double temperature, const std::string& city) {
    if (!m_hardwareInitialized) {
        std::cerr << "Device_t: Hardware no inicializado, no se puede usar OLED" << std::endl;
        return;
    }
    
    try {
        SSD1306 oled(128, 64);
        oled.OLEDbegin();
        oled.OLEDclearBuffer();
        
        // Configurar texto
        oled.setTextSize(1);
        oled.setTextColor(WHITE);
        oled.setCursor(0, 0);
        oled.print("Ciudad: ");
        oled.println(city.c_str());
        
        oled.setCursor(0, 16);
        oled.print("Temp: ");
        oled.print(temperature);
        oled.println(" C");
        
        oled.OLEDupdate();
    } catch (...) {
        std::cerr << "Device_t: Error al inicializar OLED" << std::endl;
    }
}

/**
 * @brief Muestra la temperatura por consola
 * 
 * Imprime la temperatura formateada en la salida estándar.
 * 
 * @param temperature Temperatura a mostrar
 * @param city Nombre de la ciudad
 */
void Device_t::displayOnConsole(double temperature, const std::string& city) {
    std::cout << "========================================" << std::endl;
    std::cout << "  Temperatura Actual" << std::endl;
    std::cout << "  Ciudad: " << city << std::endl;
    std::cout << "  Temperatura: " << temperature << " °C" << std::endl;
    std::cout << "========================================" << std::endl;
}

/**
 * @brief Método principal de ejecución del dispositivo
 * 
 * Este es el punto de entrada principal que ejecuta la lógica del dispositivo:
 * 1. Inicializa el hardware
 * 2. Obtiene la temperatura de la API
 * 3. Muestra la temperatura en OLED y consola
 * 4. Espera el intervalo de actualización y repite
 */
void Device_t::run() {
    std::cout << "Device_t: Iniciando ejecución..." << std::endl;
    
    // Inicializar hardware
    if (!initializeHardware()) {
        std::cerr << "Device_t: Error al inicializar hardware" << std::endl;
        std::cerr << "Device_t: La aplicación requiere Raspberry Pi con bcm2835" << std::endl;
        return;
    }
    
    // Bucle principal
    while (true) {
        // Obtener temperatura
        double temperature = fetchTemperature(m_city, m_country);
        
        if (temperature > -900.0) {
            // Mostrar en OLED
            displayOnOLED(temperature, m_city);
            
            // Mostrar por consola
            displayOnConsole(temperature, m_city);
        } else {
            std::cerr << "Device_t: No se pudo obtener la temperatura" << std::endl;
        }
        
        // Esperar antes de la próxima actualización
        std::cout << "Device_t: Esperando " << m_updateIntervalMs / 1000 
                  << " segundos para la próxima actualización..." << std::endl;
        bcm2835_delay(m_updateIntervalMs);
    }
}

} // namespace Device
