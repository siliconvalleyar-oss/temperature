# HARDWARE - Documentación de Hardware

## Raspberry Pi Compatible

### Modelos Soportados

| Modelo | Arquitectura | Estado |
|--------|--------------|--------|
| Raspberry Pi 4 Model B | ARMv8 (64-bit) | ✅ Soportado |
| Raspberry Pi 3 Model B+ | ARMv8 (64-bit) | ✅ Soportado |
| Raspberry Pi 3 Model B | ARMv8 (64-bit) | ✅ Soportado |
| Raspberry Pi Zero W | ARMv6 (32-bit) | ✅ Soportado |
| Raspberry Pi Zero 2 W | ARMv8 (64-bit) | ✅ Soportado |
| Raspberry Pi 400 | ARMv8 (64-bit) | ✅ Soportado |

### Requisitos Mínimos

- CPU: ARMv6 o superior
- RAM: 256 MB mínimo (512 MB recomendado)
- Almacenamiento: 8 GB microSD
- Conexión a internet

---

## Display OLED SSD1306

### Especificaciones

| Parámetro | Valor |
|-----------|-------|
| Controlador | SSD1306 |
| Resolución | 128x64 pixels |
| Interfaz | I2C |
| Dirección I2C | 0x3C (default) o 0x3D |
| Voltaje | 3.3V - 5V |
| Corriente | ~20mA |

### Conexiones

| Pin OLED | Pin Raspberry Pi | Descripción |
|----------|------------------|-------------|
| VCC | 3.3V (Pin 1) | Alimentación |
| GND | GND (Pin 6) | Tierra |
| SDA | GPIO 0 (Pin 3) | Datos I2C |
| SCL | GPIO 1 (Pin 5) | Reloj I2C |

### Diagrama de Conexión

```
Raspberry Pi          OLED SSD1306
┌────────────┐        ┌────────────┐
│      3.3V  │───────▶│ VCC        │
│       GND  │───────▶│ GND        │
│  GPIO 0    │───────▶│ SDA        │
│  GPIO 1    │───────▶│ SCL        │
└────────────┘        └────────────┘
```

### Configuración I2C

Habilitar I2C en Raspberry Pi:

```bash
sudo raspi-config
# Interfacing Options → I2C → Enable
```

Verificar dispositivos I2C:

```bash
sudo i2cdetect -y 1
```

Debería mostrar:
```
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- 3c -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
```

---

## Magnetómetro HMC5883L

### Especificaciones

| Parámetro | Valor |
|-----------|-------|
| Rango | ±0.88 a ±8.1 Gauss |
| Resolución | 2 mGauss (0.88G range) |
| Interfaz | I2C |
| Dirección I2C | 0x1E |
| Voltaje | 2.16V - 3.6V |
| Corriente | 100µA (modo continuo) |

### Conexiones

| Pin HMC5883L | Pin Raspberry Pi | Descripción |
|--------------|------------------|-------------|
| VCC | 3.3V (Pin 1) | Alimentación |
| GND | GND (Pin 6) | Tierra |
| SDA | GPIO 0 (Pin 3) | Datos I2C |
| SCL | GPIO 1 (Pin 5) | Reloj I2C |

### Configuración

El HMC5883L comparte el bus I2C con el OLED SSD1306.
No requiere configuración adicional ya que usa direcciones diferentes.

---

## Pinout de Raspberry Pi

### Raspberry Pi 4 (40-pin)

```
        ┌─────────────────────────────────┐
        │  Raspberry Pi 4                 │
        │  ┌───────────────────────────┐  │
3.3V  1 ─┤  │                           │├─ 2  5V
GPIO0 3 ─┤  │                           │├─ 4  5V
GPIO1 5 ─┤  │     ┌─────────────┐       │├─ 6  GND
GPIO4 7 ─┤  │     │             │       │├─ 8  GPIO14
  GND 9 ─┤  │     │   RPi 4B    │       │├─ 10 GPIO15
GPIO17 11┤  │     │             │       │├─ 12 GPIO18
GPIO27 13┤  │     └─────────────┘       │├─ 14 GND
GPIO22 15┤  │                           │├─ 16 GPIO23
3.3V  17 ─┤  │                           │├─ 18 GPIO24
GPIO10 19┤  │                           │├─ 20 GND
GPIO9  21┤  │                           │├─ 22 GPIO25
GPIO11 23┤  │                           │├─ 24 GPIO8
  GND 25 ─┤  │                           │├─ 26 GPIO7
GPIO0  27┤  │                           │├─ 28 GPIO1
GPIO5  29┤  │                           │├─ 30 GND
GPIO6  31┤  │                           │├─ 32 GPIO12
GPIO13 33┤  │                           │├─ 34 GND
GPIO19 35┤  │                           │├─ 36 GPIO16
GPIO26 37┤  │                           │├─ 38 GPIO20
  GND 39 ─┤  │                           │├─ 40 GPIO21
        │  └───────────────────────────┘  │
        └─────────────────────────────────┘
```

### Pines Utilizados en Este Proyecto

| Pin | GPIO | Uso |
|-----|------|-----|
| 1 | 3.3V | Alimentación sensores |
| 3 | GPIO 0 | I2C SDA |
| 5 | GPIO 1 | I2C SCL |
| 6 | GND | Tierra |
| 11 | GPIO 17 | LED estado (opcional) |
| 13 | GPIO 27 | Botón reset (opcional) |

---

## Requisitos Eléctricos

### Alimentación

| Componente | Voltaje | Corriente |
|------------|---------|-----------|
| Raspberry Pi 4 | 5V | 2.5A-3A |
| OLED SSD1306 | 3.3V | ~20mA |
| HMC5883L | 3.3V | ~100µA |

### Consumo Total Estimado

- **Idle**: ~300mA
- **Load**: ~700mA
- **Pico**: ~1.2A

### Recomendaciones

1. Usar fuente de poder adecuada (5V 3A)
2. No exceder 500mA por pin GPIO
3. Usar capacitores de descoupling en circuitos externos

---

## Protección

### Recomendaciones

1. **ESD**: Usar pulsera antiestática
2. **Alimentación**: No conectar/disconectar con power-on
3. **I2C**: Usar resistencias pull-up (4.7kΩ)
4. **GPIO**: No exceder voltaje máximo (3.3V)

### Circuitos de Protección

```
Raspberry Pi          Circuito Externo
┌────────────┐        ┌────────────┐
│  GPIO 0    │───┬───▶│ SDA        │
│            │   │    │            │
│            │  ─┴─   │            │
│            │  4.7kΩ │            │
│            │   │    │            │
│  3.3V      │───┴───▶│ VCC        │
└────────────┘        └────────────┘
```

---

## Troubleshooting Hardware

### Display OLED no muestra nada

1. Verificar conexiones físicas
2. Verificar dirección I2C: `sudo i2cdetect -y 1`
3. Probar con ejemplo básico de bcm2835
4. Verificar alimentación (3.3V estable)

### Magnetómetro no responde

1. Verificar conexiones I2C
2. Verificar dirección: `sudo i2cdetect -y 1` (debe mostrar 0x1E)
3. Leer registro ID: `sudo i2cget -y 1 0x1E 0x0A` (debe retornar 0x48)

### Errores de I2C

```bash
# Verificar si I2C está habilitado
lsmod | grep i2c_bcm2835

# Ver dispositivos I2C
sudo i2cdetect -y 1

# Probar comunicación
sudo i2cget -y 1 0x3C 0x00
```

---

## Enlaces

- [Raspberry Pi Pinout](https://pinout.xyz/)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [HMC5883L Datasheet](https://www.adafruit.com/datasheets/HMC5883L.pdf)
- [bcm2835 Library](http://www.airspayce.com/mikem/bcm2835/)
