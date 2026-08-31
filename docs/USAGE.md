# USAGE - Guía de Uso

## Inicio Rápido

### 1. Compilar el proyecto

```bash
# Instalar dependencias
sudo ./scripts/install_deps.sh

# Compilar
make
```

### 2. Ejecutar la aplicación

```bash
# Ejecutar con permisos de root (requerido para GPIO)
sudo ./bin/App
```

### 3. Ver la versión

```bash
./bin/App --version
# o
./bin/App -v
```

---

## Comandos Disponibles

### Ejecutar aplicación

```bash
sudo ./bin/App
```

La aplicación mostrará:
- Temperatura actual de la ciudad configurada
- Actualización automática cada 60 segundos
- Salida en consola y en display OLED (si está conectado)

### Ver versión

```bash
./bin/App --version
```

Salida:
```
App v0.1.0
```

---

## Configuración

### Archivo de configuración

La configuración se encuentra en `config/config.cfg`:

```ini
[weather]
api_key = TU_API_KEY
city = Buenos Aires
country = AR
units = metric

[update]
interval_seconds = 60
```

### Cambiar ciudad

Editar `config/config.cfg`:
```ini
[weather]
city = London
country = GB
```

### Cambiar intervalo de actualización

Editar `config/config.cfg`:
```ini
[update]
interval_seconds = 300  # 5 minutos
```

---

## Hardware Requerido

### Mínimo

- Raspberry Pi (32 o 64 bits)
- Conexión a internet
- Tarjeta SD con Raspbian/Raspberry Pi OS

### Opcional

- Display OLED SSD1306 (128x64) vía I2C
- Magnetómetro HMC5883L vía I2C

### Conexiones I2C

| Dispositivo | Dirección | Pin SDA | Pin SCL |
|-------------|-----------|---------|---------|
| OLED SSD1306 | 0x3C | GPIO 0 (Pin 3) | GPIO 1 (Pin 5) |
| HMC5883L | 0x1E | GPIO 0 (Pin 3) | GPIO 1 (Pin 5) |

---

## Ejemplos de Uso

### Ejemplo 1: Ejecución básica

```bash
sudo ./bin/App
```

Salida:
```
========================================
  RaspberryPi Weather App v0.1.0
========================================
Device_t: Constructor llamado
Device_t: Ciudad configurada: Buenos Aires, AR
Device_t: Iniciando ejecución...
Device_t: Modo simulación (no se detectó ARM)
Device_t: Obteniendo temperatura de Buenos Aires...
========================================
  Temperatura Actual
  Ciudad: Buenos Aires
  Temperatura: 22.5 °C
========================================
```

### Ejemplo 2: Ver versión

```bash
./bin/App --version
```

Salida:
```
App v0.1.0
```

### Ejemplo 3: Compilar con make

```bash
# Ver opciones disponibles
make help

# Compilar (detecta automáticamente)
make

# Compilar con OLED (solo Raspberry Pi)
make OLED=yes

# Compilar sin OLED (PC o RPi sin OLED)
make OLED=no

# Limpiar
make clean

# Ver información del proyecto
make info
```

---

## Solución de Problemas

Si tiene problemas, consulte [TROUBLESHOOTING.md](TROUBLESHOOTING.md).

---

## Enlaces Útiles

- [OpenWeatherMap API](https://openweathermap.org/api)
- [bcm2835 Library](http://www.airspayce.com/mikem/bcm2835/)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [HMC5883L Datasheet](https://www.adafruit.com/datasheets/HMC5883L.pdf)
