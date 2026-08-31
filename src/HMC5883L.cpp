/**
 * @file HMC5883L.cpp
 * @brief Implementación del driver para el magnetómetro HMC5883L
 * 
 * Implementa los métodos básicos de comunicación I2C con el sensor.
 * Solo funciona en Raspberry Pi con bcm2835 habilitado.
 * 
 * @author Proyecto RaspberryPi
 * @version 0.1.0
 * @date 2026
 */

#include "HMC5883L.hpp"

#ifdef HAS_BCM2835
    #include <bcm2835.h>
#endif

#include <cmath>

/**
 * @brief Constructor del HMC5883L
 * @param address Dirección I2C del sensor
 */
HMC5883L::HMC5883L(uint8_t address)
    : m_address(address)
    , m_initialized(false)
    , m_currentRange(RANGE_1_3_G)
    , m_currentMode(MODE_IDLE)
{
}

/**
 * @brief Destructor
 */
HMC5883L::~HMC5883L() {
    if (m_initialized) {
        setMode(MODE_IDLE);
    }
}

/**
 * @brief Inicializa el sensor HMC5883L
 * 
 * Configura el sensor con valores por defecto:
 * - Rango: 1.3G
 * - Oversampling: 512
 * - Modo: continuo
 * 
 * @return true si la inicialización fue exitosa
 */
bool HMC5883L::begin() {
    #ifdef HAS_BCM2835
        // Inicializar I2C
        if (!bcm2835_i2c_begin()) {
            return false;
        }
        
        bcm2835_i2c_setSlaveAddress(m_address);
        bcm2835_i2c_set_baudrate(100000);  // 100kHz
        
        // Verificar ID del sensor
        uint8_t idA, idB, idC;
        readRegister(REG_Identification_A, idA);
        readRegister(REG_Identification_B, idB);
        readRegister(REG_Identification_C, idC);
        
        // HMC5883L retorna "H43" como ID
        if (idA != 'H' || idB != '4' || idC != '3') {
            return false;
        }
        
        // Configurar sensor
        setRange(m_currentRange);
        setMode(MODE_CONTINUOUS);
        
        m_initialized = true;
        return true;
    #else
        // En modo PC, simular inicialización exitosa
        std::cout << "HMC5883L: Modo simulación (no disponible en PC)" << std::endl;
        m_initialized = false;
        return false;
    #endif
}

/**
 * @brief Lee los datos de campo magnético de los 3 ejes
 * 
 * @param x Referencia donde almacenar el valor X
 * @param y Referencia donde almacenar el valor Y
 * @param z Referencia donde almacenar el valor Z
 * @return true si la lectura fue exitosa
 */
bool HMC5883L::readMag(int16_t& x, int16_t& y, int16_t& z) {
    if (!m_initialized) {
        return false;
    }
    
    #ifdef HAS_BCM2835
        uint8_t buffer[6];
        if (!readRegisters(REG_Data_Output_X_MSB, buffer, 6)) {
            return false;
        }
        
        // Los datos están en formato MSB first
        x = (int16_t)((buffer[0] << 8) | buffer[1]);
        z = (int16_t)((buffer[2] << 8) | buffer[3]);
        y = (int16_t)((buffer[4] << 8) | buffer[5]);
        
        return true;
    #else
        // En modo PC, retornar valores simulados
        x = 0;
        y = 0;
        z = 0;
        return false;
    #endif
}

/**
 * @brief Calcula el ángulo de dirección (azimut)
 * 
 * @return Ángulo en grados (0-360), donde 0 es Norte
 */
float HMC5883L::getHeading() {
    int16_t x, y, z;
    
    if (!readMag(x, y, z)) {
        return -1.0f;
    }
    
    // Calcular heading en radianes
    float heading = atan2((float)y, (float)x);
    
    // Convertir a grados
    heading = heading * 180.0f / M_PI;
    
    // Ajustar a rango 0-360
    if (heading < 0) {
        heading += 360.0f;
    }
    
    return heading;
}

/**
 * @brief Configura el rango de medición
 * @param range Rango deseado
 */
void HMC5883L::setRange(Range range) {
    m_currentRange = range;
    
    #ifdef HAS_BCM2835
        uint8_t value = 0;
        readRegister(REG_Configuration_B, value);
        value = (value & 0xE0) | (range << 5);
        writeRegister(REG_Configuration_B, value);
    #endif
}

/**
 * @brief Configura el modo de operación
 * @param mode Modo deseado
 */
void HMC5883L::setMode(Mode mode) {
    m_currentMode = mode;
    
    #ifdef HAS_BCM2835
        writeRegister(REG_Mode, static_cast<uint8_t>(mode));
    #endif
}

/**
 * @brief Verifica si hay nuevos datos disponibles
 * @return true si hay datos nuevos
 */
bool HMC5883L::dataReady() {
    #ifdef HAS_BCM2835
        uint8_t status = 0;
        readRegister(REG_Status, status);
        return (status & 0x01) != 0;
    #else
        return false;
    #endif
}

/**
 * @brief Escribe un byte en un registro del sensor
 * @param reg Registro a escribir
 * @param value Valor a escribir
 * @return true si la escritura fue exitosa
 */
bool HMC5883L::writeRegister(uint8_t reg, uint8_t value) {
    #ifdef HAS_BCM2835
        char buf[2] = {static_cast<char>(reg), static_cast<char>(value)};
        return bcm2835_i2c_write(buf, 2) == BCM2835_I2C_REASON_OK;
    #else
        (void)reg;
        (void)value;
        return false;
    #endif
}

/**
 * @brief Lee un byte de un registro del sensor
 * @param reg Registro a leer
 * @param value Referencia donde almacenar el valor leído
 * @return true si la lectura fue exitosa
 */
bool HMC5883L::readRegister(uint8_t reg, uint8_t& value) {
    #ifdef HAS_BCM2835
        char buf[1] = {static_cast<char>(reg)};
        char data[1] = {0};
        
        // Escribir dirección de registro, luego leer
        if (bcm2835_i2c_write(buf, 1) != BCM2835_I2C_REASON_OK) {
            return false;
        }
        if (bcm2835_i2c_read(data, 1) != BCM2835_I2C_REASON_OK) {
            return false;
        }
        
        value = static_cast<uint8_t>(data[0]);
        return true;
    #else
        (void)reg;
        value = 0;
        return false;
    #endif
}

/**
 * @brief Lee múltiples bytes desde un registro
 * @param reg Registro inicial
 * @param buffer Buffer donde almacenar los datos
 * @param count Número de bytes a leer
 * @return true si la lectura fue exitosa
 */
bool HMC5883L::readRegisters(uint8_t reg, uint8_t* buffer, size_t count) {
    #ifdef HAS_BCM2835
        char buf[1] = {static_cast<char>(reg)};
        
        // Escribir dirección de registro
        if (bcm2835_i2c_write(buf, 1) != BCM2835_I2C_REASON_OK) {
            return false;
        }
        
        // Leer datos
        if (bcm2835_i2c_read(reinterpret_cast<char*>(buffer), count) != BCM2835_I2C_REASON_OK) {
            return false;
        }
        
        return true;
    #else
        (void)reg;
        (void)buffer;
        (void)count;
        return false;
    #endif
}
