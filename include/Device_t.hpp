/**
 * @file Device_t.hpp
 * @brief Clase principal del dispositivo Raspberry Pi
 * 
 * Define la clase Device_t que encapsula toda la lógica del dispositivo,
 * incluyendo la obtención de temperatura, display OLED y sensores.
 * 
 * @author Proyecto RaspberryPi
 * @version 0.1.0
 * @date 2026
 */

#ifndef DEVICE_T_HPP
#define DEVICE_T_HPP

#include <memory>
#include <string>
#include <functional>
#include <thread>
#include <chrono>

/**
 * @namespace Device
 * @brief Namespace que contiene las clases del dispositivo
 */
namespace Device {

/**
 * @class Device_t
 * @brief Clase principal que gestiona el dispositivo Raspberry Pi
 * 
 * Esta clase implementa la lógica principal del dispositivo, incluyendo:
 * - Obtención de temperatura desde OpenWeatherMap API
 * - Mostrar temperatura en pantalla OLED SSD1306 (solo RPi con OLED habilitado)
 * - Mostrar temperatura por consola
 * - Gestión de bcm2835 para GPIO/SPI/I2C (solo RPi con bcm2835 habilitado)
 */
class Device_t {
public:
    /**
     * @brief Constructor por defecto
     * 
     * Inicializa el dispositivo y sus componentes.
     * Configura bcm2835 si está disponible.
     */
    Device_t();

    /**
     * @brief Destructor
     * 
     * Libera recursos y limpia el dispositivo.
     */
    ~Device_t();

    /**
     * @brief Método principal de ejecución
     * 
     * Contiene el bucle principal del dispositivo.
     * Obtiene temperatura y la muestra en OLED y consola.
     */
    void run();

private:
    /**
     * @brief Inicializa el hardware del dispositivo
     * @return true si la inicialización fue exitosa
     */
    bool initializeHardware();

    /**
     * @brief Obtiene la temperatura de la API de OpenWeatherMap
     * @param city Nombre de la ciudad
     * @param country Código del país (ej: "AR")
     * @return Temperatura en grados Celsius, o -999.0 en caso de error
     */
    double fetchTemperature(const std::string& city, const std::string& country);

    /**
     * @brief Muestra la temperatura en la pantalla OLED
     * @param temperature Temperatura a mostrar
     * @param city Nombre de la ciudad
     */
    void displayOnOLED(double temperature, const std::string& city);

    /**
     * @brief Muestra la temperatura por consola
     * @param temperature Temperatura a mostrar
     * @param city Nombre de la ciudad
     */
    void displayOnConsole(double temperature, const std::string& city);

    /**
     * @brief Realiza una solicitud HTTP GET
     * @param url URL completa de la solicitud
     * @return Respuesta como string, o string vacío en caso de error
     */
    std::string httpGet(const std::string& url);

    /**
     * @brief Parsea la respuesta JSON y extrae la temperatura
     * @param jsonResponse Respuesta JSON de la API
     * @return Temperatura en grados Celsius
     */
    double parseTemperature(const std::string& jsonResponse);

    /** @brief Ciudad configurada para obtener temperatura */
    std::string m_city;
    
    /** @brief País configurado */
    std::string m_country;
    
    /** @brief API key de OpenWeatherMap */
    std::string m_apiKey;
    
    /** @brief Indica si el hardware fue inicializado correctamente */
    bool m_hardwareInitialized;

    /** @brief Intervalo de actualización en milisegundos */
    unsigned int m_updateIntervalMs;
};

} // namespace Device

#endif // DEVICE_T_HPP
