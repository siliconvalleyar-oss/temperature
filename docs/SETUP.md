# SETUP - Guía de Configuración

## Configuración Inicial

### 1. Preparar Raspberry Pi

```bash
# Actualizar sistema
sudo apt-get update && sudo apt-get upgrade -y

# Habilitar I2C
sudo raspi-config
# Interfacing Options → I2C → Enable

# Reiniciar
sudo reboot
```

### 2. Instalar Dependencias

```bash
# Clonar repositorio
git clone https://github.com/USUARIO/REPOSITORIO.git
cd REPOSITORIO

# Instalar dependencias
sudo ./scripts/install_deps.sh
```

### 3. Configurar API Key

```bash
# Editar configuración
nano config/config.cfg
```

Cambiar:
```ini
[weather]
api_key = TU_API_KEY_DE_OPENWEATHERMAP
city = Buenos Aires
country = AR
```

### 4. Compilar

```bash
make
```

### 5. Probar

```bash
sudo ./bin/App --version
```

---

## Configuración de Hardware

### Display OLED SSD1306

1. **Conectar cables**:
   - VCC → 3.3V (Pin 1)
   - GND → GND (Pin 6)
   - SDA → GPIO 0 (Pin 3)
   - SCL → GPIO 1 (Pin 5)

2. **Verificar conexión**:
   ```bash
   sudo i2cdetect -y 1
   # Debe mostrar 0x3C
   ```

3. **Configurar en config.cfg**:
   ```ini
   [oled]
   i2c_address = 0x3C
   width = 128
   height = 64
   ```

### Magnetómetro HMC5883L

1. **Conectar cables**:
   - VCC → 3.3V (Pin 1)
   - GND → GND (Pin 6)
   - SDA → GPIO 0 (Pin 3)
   - SCL → GPIO 1 (Pin 5)

2. **Verificar conexión**:
   ```bash
   sudo i2cdetect -y 1
   # Debe mostrar 0x1E
   ```

3. **Configurar en config.cfg**:
   ```ini
   [compass]
   enabled = true
   i2c_address = 0x1E
   ```

---

## Configuración de Red

### WiFi

```bash
# Configurar WiFi
sudo raspi-config
# System Options → Wireless LAN

# Verificar conexión
ping google.com
```

### Proxy

Si usas proxy, configurar en config.cfg:
```ini
[network]
proxy = http://proxy:8080
```

### DNS

```bash
# Verificar DNS
nslookup api.openweathermap.org

# Configurar DNS
sudo nano /etc/resolv.conf
```

---

## Configuración de Permisos

### GPIO sin sudo (Opcional)

```bash
# Agregar usuario al grupo gpio
sudo usermod -aG gpio $USER

# Reiniciar sesión
sudo reboot
```

**Nota**: Algunas operaciones requieren sudo independientemente.

### Archivos de Configuración

```bash
# Proteger configuración
chmod 600 config/config.cfg
chmod 700 config/
```

---

## Verificación

### Checklist

- [ ] Sistema actualizado
- [ ] I2C habilitado
- [ ] Dependencias instaladas
- [ ] API key configurada
- [ ] Hardware conectado
- [ ] Permisos configurados
- [ ] Compilación exitosa

### Pruebas

```bash
# Verificar versión
./bin/App --version

# Ejecutar aplicación
sudo ./bin/App

# Verificar I2C
sudo i2cdetect -y 1

# Verificar logs
tail -f /var/log/syslog
```

---

## Configuración Avanzada

### Cambiar Intervalo de Actualización

Editar config.cfg:
```ini
[update]
interval_seconds = 300  # 5 minutos
```

### Cambiar Ciudad por Defecto

Editar config.cfg:
```ini
[weather]
city = London
country = GB
```

### Habilitar Logging Detallado

Editar config.cfg:
```ini
[logging]
level = DEBUG
file = /var/log/weather.log
```

### Configurar GPIO

Editar config/hardware.cfg:
```ini
[gpio]
led_status = 17
button_reset = 27
```

---

## Solución de Problemas

### I2C no funciona

```bash
# Verificar módulo
lsmod | grep i2c_bcm2835

# Cargar módulo
sudo modprobe i2c-bcm2835

# Verificar dispositivos
sudo i2cdetect -y 1
```

### Permisos denegados

```bash
# Ejecutar con sudo
sudo ./bin/App

# O configurar sudoers
sudo visudo
# Agregar: usuario ALL=(ALL) NOPASSWD: /path/to/App
```

### Compilación falla

```bash
# Limpiar y recompilar
make clean
make

# Verificar dependencias
sudo ./scripts/install_deps.sh
```

---

## Enlaces

- [Guía de Uso](USAGE.md)
- [Compilación](BUILD.md)
- [Hardware](HARDWARE.md)
- [Solución de Problemas](TROUBLESHOOTING.md)
