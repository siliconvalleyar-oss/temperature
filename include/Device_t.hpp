/**
 * @file Device_t.hpp
 * @brief Clase principal del dispositivo
 * 
 * Usa Open-Meteo API (gratis, sin API key) para obtener clima.
 * 
 * @author Proyecto RaspberryPi
 * @version 0.2.0
 * @date 2026
 */

#ifndef DEVICE_T_HPP
#define DEVICE_T_HPP

#include <memory>
#include <string>
#include <functional>
#include <thread>
#include <chrono>

namespace Device {

class Device_t {
public:
    Device_t();
    ~Device_t();
    void run();

private:
    bool initializeHardware();
    std::string getCoordinates(const std::string& city);
    double fetchTemperature(const std::string& city, const std::string& country);
    void displayOnOLED(double temperature, const std::string& city);
    void displayOnConsole(double temperature, const std::string& city);
    std::string httpGet(const std::string& url);
    double parseTemperature(const std::string& jsonResponse);

    std::string m_city;
    std::string m_country;
    std::string m_apiKey;  // No se usa con Open-Meteo
    bool m_hardwareInitialized;
    unsigned int m_updateIntervalMs;
};

} // namespace Device

#endif // DEVICE_T_HPP
