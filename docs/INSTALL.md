# INSTALL - Guía de Instalación

## Requisitos

### Hardware

- Raspberry Pi (32 o 64 bits)
- Tarjeta microSD (mínimo 8GB)
- Fuente de poder USB (5V 2.5A mínimo)
- Conexión a internet (WiFi o Ethernet)

### Software

- Raspbian/Raspberry Pi OS (Bullseye o posterior)
- Git
- Conexión SSH (opcional)

---

## Instalación en Raspberry Pi

### Paso 1: Preparar el sistema

```bash
# Actualizar sistema
sudo apt-get update && sudo apt-get upgrade -y

# Instalar git (si no está instalado)
sudo apt-get install -y git
```

### Paso 2: Clonar el repositorio

```bash
git clone https://github.com/USUARIO/REPOSITORIO.git
cd REPOSITORIO
```

### Paso 3: Instalar dependencias

```bash
sudo ./scripts/install_deps.sh
```

### Paso 4: Compilar

```bash
make
```

### Paso 5: Ejecutar

```bash
sudo ./bin/App
```

---

## Instalación Manual

### Instalar bcm2835

```bash
# Descargar
cd /tmp
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz

# Extraer
tar -xzf bcm2835-1.71.tar.gz
cd bcm2835-1.71

# Compilar e instalar
./configure
make
sudo make install
```

### Instalar libcurl

```bash
sudo apt-get install -y libcurl4-openssl-dev
```

### Instalar nlohmann/json

```bash
sudo apt-get install -y nlohmann-json3-dev
```

---

## Configuración Post-Instalación

### Archivo de configuración

Editar `config/config.cfg`:

```ini
[weather]
api_key = TU_API_KEY_DE_OPENWEATHERMAP
city = Buenos Aires
country = AR
```

### Obtener API Key

1. Registrarse en [OpenWeatherMap](https://openweathermap.org/api)
2. Obtener API Key gratuita
3. Configurar en `config/config.cfg`

### Permisos GPIO

Para acceder a GPIO sin sudo:

```bash
# Agregar usuario al grupo gpio
sudo usermod -aG gpio $USER

# Reiniciar sesión
```

**Nota**: Algunas operaciones requieren sudo independientemente.

---

## Verificación

### Verificar instalación

```bash
# Verificar que bcm2835 está instalado
ldconfig -p | grep bcm2835

# Verificar que libcurl está instalada
ldconfig -p | grep libcurl

# Verificar compilación
make info
```

### Ejecutar prueba

```bash
sudo ./bin/App --version
```

Salida esperada:
```
App v0.1.0
```

---

## Desinstalación

### Eliminar binario

```bash
make distclean
```

### Eliminar dependencias (opcional)

```bash
sudo apt-get remove -y libbcm2835-dev libcurl4-openssl-dev nlohmann-json3-dev
```

### Eliminar repositorio

```bash
cd ..
rm -rf REPOSITORIO
```

---

## Actualización

### Actualizar código

```bash
git pull origin main
make clean && make
```

### Actualizar dependencias

```bash
sudo apt-get update && sudo apt-get upgrade -y
```

---

## Soporte

Si tiene problemas:

1. Revisar [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
2. Verificar requisitos mínimos
3. Ejecutar `sudo ./scripts/install_deps.sh`
4. Revisar logs de error

---

## Enlaces

- [Guía de Uso](USAGE.md)
- [Compilación](BUILD.md)
- [Solución de Problemas](TROUBLESHOOTING.md)
