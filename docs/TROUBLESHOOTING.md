# TROUBLESHOOTING - Solución de Problemas

## Problemas de Compilación

### Error: "bcm2835.h: No such file or directory"

**Solución**: Instalar la librería bcm2835:
```bash
sudo ./scripts/install_deps.sh
```

O instalar manualmente:
```bash
sudo apt-get install libbcm2835-dev
```

### Error: "curl/curl.h: No such file or directory"

**Solución**: Instalar libcurl:
```bash
sudo apt-get install libcurl4-openssl-dev
```

### Error: "json.hpp: No such file or directory"

**Solución**: Instalar nlohmann/json:
```bash
sudo apt-get install nlohmann-json3-dev
```

### Error: "VERSION: No such file or directory"

**Solución**: Asegurarse de que el archivo VERSION existe en la raíz del proyecto.

---

## Problemas de Ejecución

### Error: "Cannot start I2C, Running root?"

**Solución**: Ejecutar con permisos de root:
```bash
sudo ./bin/App
```

### Error: "bcm2835_init() failed"

**Causas posibles**:
1. No se ejecuta como root
2. No se ejecuta en Raspberry Pi
3. bcm2835 no está instalado correctamente

**Solución**:
```bash
# Verificar permisos
sudo ./bin/App

# Verificar si bcm2835 está instalado
ldconfig -p | grep bcm2835

# Reinstalar bcm2835
sudo ./scripts/install_deps.sh
```

### Error: "curl_easy_perform() failed"

**Causas posibles**:
1. Sin conexión a internet
2. Firewall bloqueando la conexión
3. API key inválida

**Solución**:
```bash
# Verificar conexión a internet
ping google.com

# Probar la API directamente
curl "http://api.openweathermap.org/data/2.5/weather?q=Buenos Aires,AR&appid=TU_API_KEY&units=metric"
```

### Error: "No se pudo obtener la temperatura"

**Causas posibles**:
1. API key inválida o expirada
2. Ciudad no encontrada
3. Límite de solicitudes alcanzado

**Solución**:
1. Verificar API key en config/config.cfg
2. Probar con ciudad conocida: "London,GB"
3. Esperar 1 minuto (rate limit: 60/min)

---

## Problemas de Hardware

### Display OLED no muestra nada

**Verificaciones**:
1. Verificar conexiones I2C (SDA, SCL)
2. Verificar dirección I2C del display
3. Verificar alimentación (3.3V)

**Solución**:
```bash
# Escanear dispositivos I2C
sudo i2cdetect -y 1

# Debería mostrar 0x3C o 0x3D
```

### Magnetómetro no responde

**Verificaciones**:
1. Verificar conexiones I2C
2. Verificar dirección I2C (0x1E por defecto)
3. Verificar que el sensor esté soldado correctamente

**Solución**:
```bash
# Escanear dispositivos I2C
sudo i2cdetect -y 1

# Leer registro de identificación
sudo i2cget -y 1 0x1E 0x0A
# Debería retornar 0x48 ('H')
```

---

## Problemas de Red

### No se puede conectar a OpenWeatherMap API

**Verificaciones**:
1. Verificar conexión a internet
2. Verificar DNS
3. Verificar firewall

**Solución**:
```bash
# Verificar DNS
nslookup api.openweathermap.org

# Verificar conectividad
curl -v http://api.openweathermap.org

# Verificar firewall
sudo iptables -L -n
```

---

## Logs y Depuración

### Habilitar logs detallados

Modificar config/config.cfg:
```ini
[logging]
level = DEBUG
file = /var/log/weather.log
```

### Ver logs del sistema

```bash
# Logs de la aplicación
tail -f /var/log/weather.log

# Logs del sistema
journalctl -f
```

---

## Soporte

Si el problema persiste:

1. Revisar documentación en docs/
2. Buscar issues en GitHub
3. Crear un issue con:
   - Descripción del problema
   - Pasos para reproducir
   - Logs de error
   - Versión del software y hardware
