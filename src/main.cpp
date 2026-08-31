/**
 * @file main.cpp
 * @brief Punto de entrada principal del proyecto RaspberryPi
 * 
 * Este archivo contiene la función main que inicializa el dispositivo
 * y ejecuta la aplicación principal.
 * 
 * Uso:
 *   ./App              - Ejecutar aplicación
 *   ./App --version    - Mostrar versión y salir
 * 
 * @author Proyecto RaspberryPi
 * @version 0.1.0
 * @date 2026
 */

#include <iostream>
#include <string>
#include <memory>
#include "Device_t.hpp"

/**
 * @brief Punto de entrada principal
 * 
 * Crea un dispositivo Device_t usando unique_ptr para gestión
 * automática de memoria y ejecuta el método run().
 * 
 * Soporta argumento --version para mostrar la versión.
 * 
 * @param argc Número de argumentos
 * @param argv Array de argumentos
 * @return 0 si éxito, 1 en caso de error
 */
int main(int argc, char* argv[]) {
    // Verificar argumento --version
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--version" || arg == "-v") {
            std::cout << "App v" << VERSION << std::endl;
            return 0;
        }
    }
    
    // Mostrar versión y información del sistema
    std::cout << "========================================" << std::endl;
    std::cout << "  RaspberryPi Weather App v" << VERSION << std::endl;
    std::cout << "========================================" << std::endl;
    
    #ifdef HAS_OLED
        std::cout << "  OLED: Habilitado (SSD1306)" << std::endl;
    #else
        std::cout << "  OLED: Deshabilitado (modo PC)" << std::endl;
    #endif
    
    #ifdef HAS_BCM2835
        std::cout << "  Hardware: Raspberry Pi (bcm2835)" << std::endl;
    #else
        std::cout << "  Hardware: Modo PC (sin bcm2835)" << std::endl;
    #endif
    
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Crear dispositivo con unique_ptr para gestión automática de memoria
    auto device = std::make_unique<Device::Device_t>();
    
    // Ejecutar aplicación
    device->run();
    
    return 0;
}
