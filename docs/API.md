# API - Documentación de la API

## OpenWeatherMap API

### Endpoint Base

```
http://api.openweathermap.org/data/2.5/
```

### Autenticación

Todas las solicitudes requieren un parámetro `appid` con la API key.

### Obtener Clima Actual

```
GET /weather?q={city},{country}&appid={api_key}&units=metric
```

**Parámetros**:

| Parámetro | Tipo | Requerido | Descripción |
|-----------|------|-----------|-------------|
| q | string | Sí | Ciudad y país (ej: "Buenos Aires,AR") |
| appid | string | Sí | API key |
| units | string | No | Unidades: metric, imperial, standard |
| lang | string | No | Idioma: es, en, pt, etc. |

**Respuesta**:

```json
{
  "main": {
    "temp": 22.5,
    "feels_like": 21.8,
    "temp_min": 20.0,
    "temp_max": 25.0,
    "pressure": 1013,
    "humidity": 65
  },
  "weather": [
    {
      "id": 800,
      "main": "Clear",
      "description": "clear sky",
      "icon": "01d"
    }
  ],
  "name": "Buenos Aires",
  "cod": 200
}
```

**Campos Principales**:

| Campo | Tipo | Descripción |
|-------|------|-------------|
| main.temp | float | Temperatura actual |
| main.feels_like | float | Sensación térmica |
| main.humidity | float | Humedad porcentaje |
| main.pressure | float | Presión hPa |
| weather[].description | string | Descripción del clima |
| name | string | Nombre de la ciudad |

### Ejemplo de Uso

```bash
curl "http://api.openweathermap.org/data/2.5/weather?q=Buenos Aires,AR&appid=TU_API_KEY&units=metric"
```

### Límites

- **Rate Limit**: 60 llamadas/minuto (plan gratuito)
- **Quota diaria**: 1,000,000 llamadas (plan gratuito)

---

## API Interna (Device_t)

### Device_t

Clase principal que gestiona el dispositivo.

#### Constructor

```cpp
Device_t();
```

Crea una instancia del dispositivo con valores por defecto.

#### run()

```cpp
void run();
```

Ejecuta la aplicación principal. Método bloqueante.

**Flujo**:
1. Inicializa hardware
2. Obtiene temperatura
3. Muestra en OLED y consola
4. Espera intervalo de actualización
5. Repite desde paso 2

---

### HMC5883L

Driver para el magnetómetro HMC5883L.

#### Constructor

```cpp
HMC5883L(uint8_t address = 0x1E);
```

**Parámetros**:

| Parámetro | Tipo | Default | Descripción |
|-----------|------|---------|-------------|
| address | uint8_t | 0x1E | Dirección I2C |

#### begin()

```cpp
bool begin();
```

Inicializa el sensor. Retorna `true` si fue exitoso.

#### readMag()

```cpp
bool readMag(int16_t& x, int16_t& y, int16_t& z);
```

Lee los datos de campo magnético de los 3 ejes.

**Parámetros**:

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| x | int16_t& | Valor del eje X |
| y | int16_t& | Valor del eje Y |
| z | int16_t& | Valor del eje Z |

**Retorna**: `true` si la lectura fue exitosa

#### getHeading()

```cpp
float getHeading();
```

Calcula el ángulo de dirección (azimut).

**Retorna**: Ángulo en grados (0-360), donde 0 es Norte

---

### SSD1306

Driver para el display OLED SSD1306.

#### Constructor

```cpp
SSD1306(int16_t width, int16_t height);
```

**Parámetros**:

| Parámetro | Tipo | Descripción |
|-----------|------|-------------|
| width | int16_t | Ancho del display (128) |
| height | int16_t | Alto del display (64) |

#### OLEDbegin()

```cpp
void OLEDbegin(uint16_t I2C_speed = 0, uint8_t address = 0x3C);
```

Inicializa el display.

**Parámetros**:

| Parámetro | Tipo | Default | Descripción |
|-----------|------|---------|-------------|
| I2C_speed | uint16_t | 0 | Velocidad I2C (0=100kHz) |
| address | uint8_t | 0x3C | Dirección I2C |

#### OLEDupdate()

```cpp
void OLEDupdate();
```

Actualiza el display con el contenido del buffer.

#### OLEDclearBuffer()

```cpp
void OLEDclearBuffer();
```

Limpia el buffer del display.

---

## Errores Comunes

### HTTP Errors

| Código | Descripción | Solución |
|--------|-------------|----------|
| 401 | Unauthorized | Verificar API key |
| 404 | Not Found | Verificar ciudad/país |
| 429 | Too Many Requests | Esperar 1 minuto |
| 500 | Internal Server Error | Intentar más tarde |

### Errores de Hardware

| Error | Descripción | Solución |
|-------|-------------|----------|
| Cannot start I2C | Permisos insuficientes | Ejecutar con sudo |
| bcm2835_init failed | bcm2835 no instalado | Instalar dependencias |

---

## Ejemplos en Código

### Obtener Temperatura

```cpp
#include "Device_t.hpp"

Device::Device_t device;
// La temperatura se obtiene internamente en run()
device.run();
```

### Usar HMC5883L

```cpp
#include "HMC5883L.hpp"

HMC5883L compass;
if (compass.begin()) {
    float heading = compass.getHeading();
    std::cout << "Dirección: " << heading << "°" << std::endl;
}
```

### Usar SSD1306

```cpp
#include "oled/SSD1306_OLED.hpp"

SSD1306 oled(128, 64);
oled.OLEDbegin();
oled.OLEDclearBuffer();
oled.setTextSize(1);
oled.setTextColor(WHITE);
oled.setCursor(0, 0);
oled.print("Hello World");
oled.OLEDupdate();
```

---

## Referencias

- [OpenWeatherMap API Docs](https://openweathermap.org/api)
- [HMC5883L Datasheet](https://www.adafruit.com/datasheets/HMC5883L.pdf)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [bcm2835 Library](http://www.airspayce.com/mikem/bcm2835/)
