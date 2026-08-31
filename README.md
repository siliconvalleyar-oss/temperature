# RaspberryPi Weather App - Branch All

Aplicación para Raspberry Pi que muestra la temperatura actual de una ciudad usando la API de OpenWeatherMap, con display en pantalla OLED SSD1306 y salida por consola.

## ✅ Esta Rama

**Esta rama funciona tanto en PC como en Raspberry Pi.**

- ✅ Detecta automáticamente la plataforma
- ✅ En Raspberry Pi: compila con OLED y bcm2835
- ✅ En PC: compila sin OLED (modo simulación)
- ✅ Opción de forzar OLED con `make OLED=yes`

Para usar solo en Raspberry Pi, ver la rama **raspberry**.

## Características

- 🌡️ Obtención de temperatura en tiempo real
- 📺 Display OLED SSD1306 (128x64) - solo en RPi
- 💻 Salida por consola
- 🔧 Compatible con Raspberry Pi 32 y 64 bits
- 🖥️ Compatible con PC (x86/x64)
- ⚙️ Configuración flexible
- 📚 Documentación completa

## Requisitos

### Hardware

**Raspberry Pi:**
- Raspberry Pi (32 o 64 bits)
- Conexión a internet
- Display OLED SSD1306 (opcional)

**PC:**
- Cualquier sistema Linux/macOS
- Conexión a internet

### Software

- GCC/G++ 7.0+ (con soporte C++14)
- Make 4.0+
- Git 2.0+
- libcurl
- nlohmann/json
- bcm2835 (solo Raspberry Pi)

## Instalación

### 1. Clonar repositorio (esta rama)

```bash
git clone -b all https://github.com/siliconvalleyar-oss/temperature.git
cd temperature
```

### 2. Instalar dependencias

```bash
sudo ./scripts/install_deps.sh
```

El script detecta automáticamente la plataforma e instala las dependencias correspondientes.

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

**En Raspberry Pi:**
```bash
sudo ./bin/App
```

**En PC:**
```bash
./bin/App
```

### Ver versión

```bash
./bin/App --version
```

Salida:
```
App v0.1.0
```

### Salida en Raspberry Pi

```
========================================
  RaspberryPi Weather App v0.1.0
========================================
  OLED: Habilitado (SSD1306)
  Hardware: Raspberry Pi (bcm2835)
========================================

Device_t: Constructor llamado
Device_t: Ciudad configurada: Buenos Aires, AR
Device_t: Iniciando ejecución...
Device_t: Inicializando hardware...
Device_t: bcm2835 inicializado correctamente
Device_t: Obteniendo temperatura de Buenos Aires...
========================================
  Temperatura Actual
  Ciudad: Buenos Aires
  Temperatura: 22.5 °C
========================================
```

### Salida en PC

```
========================================
  RaspberryPi Weather App v0.1.0
========================================
  OLED: Deshabilitado (modo PC)
  Hardware: Modo PC (sin bcm2835)
========================================

Device_t: Constructor llamado
Device_t: Ciudad configurada: Buenos Aires, AR
Device_t: Iniciando ejecución...
Device_t: Inicializando hardware...
Device_t: Modo PC (no se detectó Raspberry Pi)
Device_t: Obteniendo temperatura de Buenos Aires...
[OLED] No disponible en modo PC
[OLED] Temperatura: 22.5°C en Buenos Aires
========================================
  Temperatura Actual
  Ciudad: Buenos Aires
  Temperatura: 22.5 °C
========================================
```

## Compilación

### Comandos disponibles

```bash
make           # Compilar (detecta automáticamente)
make OLED=yes  # Forzar OLED habilitado
make OLED=no   # Forzar OLED deshabilitado
make oled      # Limpiar y compilar con OLED
make noled     # Limpiar y compilar sin OLED
make clean     # Limpiar archivos objeto
make distclean # Limpiar todo
make install   # Instalar en /usr/local/bin
make help      # Ver ayuda
make info      # Ver información del sistema
```

### Verificar plataforma

```bash
make info
```

Salida:
```
========================================
  Información del Proyecto
========================================
Versión: 0.1.0
Compilador: g++
Arquitectura: x86_64
Raspberry Pi: no
OLED habilitado: no
...
```

## Ramas

| Rama | Descripción |
|------|-------------|
| `raspberry` | Solo Raspberry Pi con OLED |
| `all` | Compatible con PC y Raspberry Pi (esta rama) |

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
│   └── oled/
├── src/                    # Código fuente
│   ├── main.cpp
│   ├── Device_t.cpp
│   └── oled/
├── scripts/                # Scripts
│   ├── install_deps.sh
│   └── setup_git.sh
├── Makefile
├── VERSION
└── README.md
```

## Hardware (Raspberry Pi)

### Display OLED SSD1306

| Pin OLED | Pin Raspberry Pi |
|----------|------------------|
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
