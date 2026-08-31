# ARCHITECTURE - Arquitectura del Sistema

## Visión General

El proyecto RaspberryPi Weather App está diseñado para ejecutarse en Raspberry Pi
y mostrar la temperatura actual de una ciudad usando la API de OpenWeatherMap.

## Diagrama de Componentes

```
┌─────────────────────────────────────────────────────────────┐
│                      main.cpp                               │
│                    (Punto de entrada)                        │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│                     Device_t                                │
│                (Clase principal)                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │  Hardware    │  │    API      │  │     Display         │  │
│  │  Manager     │  │  Client     │  │     Manager         │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
└─────────────────────────┬───────────────────────────────────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
┌─────────────┐  ┌─────────────┐  ┌─────────────┐
│  bcm2835    │  │   libcurl   │  │  SSD1306    │
│  (GPIO/I2C) │  │  (HTTP)     │  │  (OLED)     │
└─────────────┘  └─────────────┘  └─────────────┘
```

## Componentes Principales

### 1. Device_t (Clase Principal)

La clase `Device_t` es el núcleo de la aplicación. Gestiona:

- Inicialización del hardware
- Obtención de temperatura desde la API
- Mostrar datos en OLED y consola
- Bucle principal de ejecución

**Responsabilidades**:
- Coordinar todos los componentes
- Manejar el ciclo de vida de la aplicación
- Gestionar errores y excepciones

### 2. Hardware Manager

Gestiona la inicialización y comunicación con el hardware:

- **bcm2835**: Biblioteca para acceder a GPIO/I2C/SPI
- **I2C**: Comunicación con sensores y displays

### 3. API Client

Maneja la comunicación con OpenWeatherMap API:

- **libcurl**: Realiza solicitudes HTTP
- **nlohmann/json**: Parsea respuestas JSON

### 4. Display Manager

Controla el display OLED SSD1306:

- **SSD1306**: Driver para el display
- **Buffer**: Gestiona el buffer de pantalla

---

## Flujo de Datos

```
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│  OpenWeather │────▶│   Device_t   │────▶│    OLED      │
│     API      │     │   (Procesa)  │     │   Display    │
└──────────────┘     └──────────────┘     └──────────────┘
                            │
                            ▼
                     ┌──────────────┐
                     │   Consola    │
                     │   (stdout)   │
                     └──────────────┘
```

### Secuencia de Ejecución

1. **Inicialización**:
   - Crear instancia de Device_t
   - Inicializar hardware (bcm2835)
   - Configurar I2C

2. **Obtención de Datos**:
   - Realizar solicitud HTTP a OpenWeatherMap
   - Parsear respuesta JSON
   - Extraer temperatura

3. **Visualización**:
   - Mostrar en OLED (si disponible)
   - Mostrar en consola

4. **Espera**:
   - Dormir por intervalo configurado
   - Repetir desde paso 2

---

## Estructura de Directorios

```
project/
├── bin/                    # Binarios compilados
│   └── App
├── config/                 # Archivos de configuración
│   ├── config.cfg
│   └── hardware.cfg
├── docs/                   # Documentación
│   ├── ARCHITECTURE.md
│   ├── API.md
│   └── ...
├── include/                # Cabeceras
│   ├── Device_t.hpp
│   ├── HMC5883L.hpp
│   ├── nlohmann/
│   │   └── json.hpp
│   └── oled/
│       └── SSD1306_OLED.hpp
├── src/                    # Código fuente
│   ├── main.cpp
│   ├── Device_t.cpp
│   ├── HMC5883L.cpp
│   └── oled/
│       └── SSD1306_OLED.cpp
├── scripts/                # Scripts auxiliares
│   ├── install_deps.sh
│   └── setup_git.sh
├── Makefile
└── VERSION
```

---

## Patrones de Diseño

### 1. RAII (Resource Acquisition Is Initialization)

Los recursos se adquieren en constructores y liberan en destructores:

```cpp
class Device_t {
public:
    Device_t();   // Adquiere recursos
    ~Device_t();  // Libera recursos
};
```

### 2. PImpl (Pointer to Implementation)

Ocultar detalles de implementación:

```cpp
class Device_t {
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
```

### 3. Singleton (Opcional)

Para gestionar acceso a hardware compartido:

```cpp
class HardwareManager {
public:
    static HardwareManager& instance();
};
```

---

## Dependencias

### Librerías Externas

| Librería | Versión | Propósito |
|----------|---------|-----------|
| bcm2835 | 1.71+ | Acceso a GPIO/I2C |
| libcurl | 7.68+ | Solicitudes HTTP |
| nlohmann/json | 3.11+ | Parseo de JSON |

### Dependencias del Sistema

- GCC/G++ 7.0+
- Make 4.0+
- Git 2.0+
- Raspbian/Raspberry Pi OS

---

## Seguridad

### API Key

- Almacenar en `config/config.cfg`
- No commitear en Git (agregar a .gitignore)
- Rotar periódicamente

### Permisos

- La aplicación requiere permisos de root para GPIO
- Usar `sudo` solo cuando sea necesario
- Considerar usar `capabilities` de Linux

---

## Rendimiento

### Optimizaciones

1. **Buffer OLED**: Actualizar solo cambios
2. **Cache**: Almacenar temperatura en memoria
3. **Timeout**: Limitar tiempo de espera HTTP

### Métricas

- Tiempo de inicio: < 1 segundo
- Tiempo de actualización: < 2 segundos
- Uso de memoria: < 10 MB

---

## Escalabilidad

### Futuras Mejoras

1. Múltiples sensores
2. Dashboard web
3. Almacenamiento en base de datos
4. Alertas por email/SMS
5. Actualización OTA

---

## Referencias

- [bcm2835 Library Documentation](http://www.airspayce.com/mikem/bcm2835/)
- [OpenWeatherMap API Documentation](https://openweathermap.org/api)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [HMC5883L Datasheet](https://www.adafruit.com/datasheets/HMC5883L.pdf)
