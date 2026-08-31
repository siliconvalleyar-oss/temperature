# RaspberryPi Weather App

Aplicación para Raspberry Pi que muestra la temperatura actual de una ciudad usando la API de OpenWeatherMap, con display en pantalla OLED SSD1306 y salida por consola.

## Características

- 🌡️ Obtención de temperatura en tiempo real
- 📺 Display OLED SSD1306 (128x64)
- 💻 Salida por consola
- 🔧 Compatible con Raspberry Pi 32 y 64 bits
- 📱 Soporte para magnetómetro HMC5883L
- ⚙️ Configuración flexible
- 📚 Documentación completa

## Requisitos

### Hardware

- Raspberry Pi (32 o 64 bits)
  - Raspberry Pi 3 Model B/B+
  - Raspberry Pi 4 Model B
  - Raspberry Pi Zero/Zero W
- Conexión a internet
- Display OLED SSD1306 (opcional)
- Magnetómetro HMC5883L (opcional)

### Software

- Raspbian/Raspberry Pi OS (Bullseye o posterior)
- GCC/G++ 7.0+
- Make 4.0+
- Git 2.0+

## Instalación

### 1. Clonar repositorio

```bash
git clone https://github.com/USUARIO/REPOSITORIO.git
cd REPOSITORIO
```

### 2. Instalar dependencias

```bash
sudo ./scripts/install_deps.sh
```

### 3. Compilar

```bash
# Detecta automáticamente (RPi con OLED, PC sin OLED)
make

# O especificar opción OLED
make OLED=yes   # Con soporte OLED (solo RPi)
make OLED=no    # Sin soporte OLED
```

### 4. Configurar API Key

Editar `config/config.cfg`:

```ini
[weather]
api_key = TU_API_KEY_DE_OPENWEATHERMAP
city = Buenos Aires
country = AR
```

Obtener API key gratuita en: https://openweathermap.org/api

## Uso

### Ejecutar aplicación

```bash
sudo ./bin/App
```

**Nota**: Se requiere `sudo` para acceder a los pines GPIO.

### Ver versión

```bash
./bin/App --version
```

Salida:
```
App v0.1.0
```

### Salida esperada

```
========================================
  RaspberryPi Weather App v0.1.0
========================================
Device_t: Iniciando ejecución...
Device_t: Ciudad configurada: Buenos Aires, AR
Device_t: Obteniendo temperatura...
========================================
  Temperatura Actual
  Ciudad: Buenos Aires
  Temperatura: 22.5 °C
========================================
Device_t: Esperando 60 segundos...
```

## Hardware

### Display OLED SSD1306

| Pin OLED | Pin Raspberry Pi |
|----------|------------------|
| VCC | 3.3V (Pin 1) |
| GND | GND (Pin 6) |
| SDA | GPIO 0 (Pin 3) |
| SCL | GPIO 1 (Pin 5) |

### Magnetómetro HMC5883L

| Pin HMC5883L | Pin Raspberry Pi |
|--------------|------------------|
| VCC | 3.3V (Pin 1) |
| GND | GND (Pin 6) |
| SDA | GPIO 0 (Pin 3) |
| SCL | GPIO 1 (Pin 5) |

## Configuración

### Archivos de configuración

- `config/config.cfg`: Configuración general
- `config/hardware.cfg`: Configuración de hardware

### Opciones principales

```ini
[weather]
api_key = TU_API_KEY
city = Buenos Aires
country = AR
units = metric

[update]
interval_seconds = 60
```

## Estructura del Proyecto

```
├── bin/                    # Binarios compilados
│   └── App
├── config/                 # Configuración
│   ├── config.cfg
│   └── hardware.cfg
├── docs/                   # Documentación
│   ├── ARCHITECTURE.md
│   ├── API.md
│   └── ...
├── include/                # Cabeceras
│   ├── Device_t.hpp
│   ├── HMC5883L.hpp
│   └── oled/
├── src/                    # Código fuente
│   ├── main.cpp
│   ├── Device_t.cpp
│   ├── HMC5883L.cpp
│   └── oled/
├── scripts/                # Scripts
│   ├── install_deps.sh
│   └── setup_git.sh
├── Makefile
├── VERSION
└── README.md
```

## Compilación

### Comandos disponibles

```bash
make           # Compilar proyecto
make clean     # Limpiar archivos objeto
make distclean # Limpiar todo
make install   # Instalar en /usr/local/bin
make help      # Ver ayuda
make info      # Ver información del proyecto
```

### Compilar con opciones

```bash
# Debug
make CXXFLAGS="-g -O0"

# Optimizado
make CXXFLAGS="-O3 -march=native"
```

## Documentación

- [Arquitectura](docs/ARCHITECTURE.md)
- [API](docs/API.md)
- [Hardware](docs/HARDWARE.md)
- [Instalación](docs/INSTALL.md)
- [Uso](docs/USAGE.md)
- [Compilación](docs/BUILD.md)
- [Contribución](docs/CONTRIBUTING.md)
- [Changelog](docs/CHANGELOG.md)

## Solución de Problemas

Ver [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)

## Contribuir

Ver [CONTRIBUTING.md](docs/CONTRIBUTING.md)

## Licencia

MIT License - Ver [LICENSE](LICENSE)

## Enlaces

- [OpenWeatherMap API](https://openweathermap.org/api)
- [bcm2835 Library](http://www.airspayce.com/mikem/bcm2835/)
- [Raspberry Pi](https://www.raspberrypi.org/)

## Soporte

Si tienes problemas:

1. Revisar [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)
2. Ejecutar `sudo ./scripts/install_deps.sh`
3. Crear un issue en GitHub

## Versión

Actual: **0.1.0**

Ver [CHANGELOG.md](docs/CHANGELOG.md) para historial de cambios.
