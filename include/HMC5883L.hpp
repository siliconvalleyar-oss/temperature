/**
 * @file HMC5883L.hpp
 * @brief Driver para el magnetómetro HMC5883L
 * 
 * Esta clase proporciona una interfaz para comunicarse con el
 * magnetómetro HMC5883L a través de I2C.
 * 
 * El HMC5883L es un sensor de campo magnético de 3 ejes
 * utilizado para determinación de dirección (brújula digital).
 * 
 * @author Proyecto RaspberryPi
 * @version 0.1.0
 * @date 2026
 * 
 * @note Este es un esqueleto básico. La implementación completa
 *       requiere configuración de registros I2C y cálculos de
 *       angulación.
 */

#ifndef HMC5883L_HPP
#define HMC5883L_HPP

#include <cstdint>
#include <string>
#include <iostream>

/**
 * @class HMC5883L
 * @brief Driver para el magnetómetro HMC5883L
 * 
 * Proporciona métodos para:
 * - Inicializar el sensor
 * - Leer datos de campo magnético (X, Y, Z)
 * - Calcular dirección/azimut
 * - Configurar rango y oversampling
 */
class HMC5883L {
public:
    /**
     * @brief Dirección I2C por defecto del HMC5883L
     */
    static constexpr uint8_t DEFAULT_ADDRESS = 0x1E;
    
    /**
     * @brief Registros del HMC5883L
     */
    enum Register : uint8_t {
        REG_Configuration_A   = 0x00,
        REG_Configuration_B   = 0x01,
        REG_Mode              = 0x02,
        REG_Data_Output_X_MSB = 0x03,
        REG_Data_Output_X_LSB = 0x04,
        REG_Data_Output_Z_MSB = 0x05,
        REG_Data_Output_Z_LSB = 0x06,
        REG_Data_Output_Y_MSB = 0x07,
        REG_Data_Output_Y_LSB = 0x08,
        REG_Status            = 0x09,
        REG_Identification_A  = 0x0A,
        REG_Identification_B  = 0x0B,
        REG_Identification_C  = 0x0C
    };
    
    /**
     * @brief Modos de operación
     */
    enum Mode : uint8_t {
        MODE_CONTINUOUS = 0x00,
        MODE_SINGLE     = 0x01,
        MODE_IDLE       = 0x02,
        MODE_SLEEP      = 0x03
    };
    
    /**
     * @brief Rangos de medición (gauss)
     */
    enum Range : uint8_t {
        RANGE_0_88_G = 0x00,
        RANGE_1_3_G  = 0x01,
        RANGE_1_9_G  = 0x02,
        RANGE_2_5_G  = 0x03,
        RANGE_4_0_G  = 0x04,
        RANGE_4_7_G  = 0x05,
        RANGE_5_6_G  = 0x06,
        RANGE_8_1_G  = 0x07
    };
    
    /**
     * @brief Constructor por defecto
     * @param address Dirección I2C del sensor (default: 0x1E)
     */
    HMC5883L(uint8_t address = DEFAULT_ADDRESS);
    
    /**
     * @brief Destructor
     */
    ~HMC5883L();
    
    /**
     * @brief Inicializa el sensor HMC5883L
     * @return true si la inicialización fue exitosa
     */
    bool begin();
    
    /**
     * @brief Lee los datos de campo magnético de los 3 ejes
     * @param x Referencia donde almacenar el valor X
     * @param y Referencia donde almacenar el valor Y
     * @param z Referencia donde almacenar el valor Z
     * @return true si la lectura fue exitosa
     */
    bool readMag(int16_t& x, int16_t& y, int16_t& z);
    
    /**
     * @brief Calcula el ángulo de dirección (azimut)
     * @return Ángulo en grados (0-360), donde 0 es Norte
     */
    float getHeading();
    
    /**
     * @brief Configura el rango de medición
     * @param range Rango deseado
     */
    void setRange(Range range);
    
    /**
     * @brief Configura el modo de operación
     * @param mode Modo deseado
     */
    void setMode(Mode mode);
    
    /**
     * @brief Verifica si hay nuevos datos disponibles
     * @return true si hay datos nuevos
     */
    bool dataReady();
    
    /**
     * @brief Obtiene el estado de inicialización
     * @return true si el sensor fue inicializado correctamente
     */
    bool isInitialized() const { return m_initialized; }
    
    /**
     * @brief Obtiene la dirección I2C del sensor
     * @return Dirección I2C
     */
    uint8_t getAddress() const { return m_address; }

private:
    /** @brief Dirección I2C del sensor */
    uint8_t m_address;
    
    /** @brief Estado de inicialización */
    bool m_initialized;
    
    /** @brief Rango actual configurado */
    Range m_currentRange;
    
    /** @brief Modo actual configurado */
    Mode m_currentMode;
    
    /**
     * @brief Escribe un byte en un registro del sensor
     * @param reg Registro a escribir
     * @param value Valor a escribir
     * @return true si la escritura fue exitosa
     */
    bool writeRegister(uint8_t reg, uint8_t value);
    
    /**
     * @brief Lee un byte de un registro del sensor
     * @param reg Registro a leer
     * @param value Referencia donde almacenar el valor leído
     * @return true si la lectura fue exitosa
     */
    bool readRegister(uint8_t reg, uint8_t& value);
    
    /**
     * @brief Lee múltiples bytes desde un registro
     * @param reg Registro inicial
     * @param buffer Buffer donde almacenar los datos
     * @param count Número de bytes a leer
     * @return true si la lectura fue exitosa
     */
    bool readRegisters(uint8_t reg, uint8_t* buffer, size_t count);
};

#endif // HMC5883L_HPP
