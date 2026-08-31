# BUILD - Guía de Compilación

## Requisitos Previos

### Hardware

- Raspberry Pi (32 o 64 bits)
  - Raspberry Pi 3 Model B/B+
  - Raspberry Pi 4 Model B
  - Raspberry Pi Zero/Zero W (con limitaciones)

### Software

- **Sistema Operativo**: Raspbian/Raspberry Pi OS (Bullseye o posterior)
- **Compilador**: GCC/G++ 7.0 o posterior
- **Make**: GNU Make 4.0 o posterior
- **Git**: 2.0 o posterior

---

## Instalación de Dependencias

### Script automático

```bash
sudo ./scripts/install_deps.sh
```

Este script instala:
- g++ y gcc
- make
- cmake
- git
- libbcm2835-dev
- libcurl4-openssl-dev
- nlohmann-json3-dev

### Instalación manual

```bash
# Actualizar repositorios
sudo apt-get update

# Instalar compilador y herramientas
sudo apt-get install -y g++ gcc make cmake git

# Instalar bcm2835
sudo apt-get install -y libbcm2835-dev

# Instalar libcurl
sudo apt-get install -y libcurl4-openssl-dev

# Instalar nlohmann/json
sudo apt-get install -y nlohmann-json3-dev
```

---

## Compilación

### Compilación estándar

```bash
make
```

Esto generará el binario en `bin/App`. El Makefile detecta automáticamente si es Raspberry Pi y habilita/deshabilita OLED accordingly.

### Compilación con OLED

**En Raspberry Pi:**

```bash
# Compilar con soporte OLED
make OLED=yes

# Compilar sin soporte OLED
make OLED=no

# O usar atajos
make oled    # Limpia y compila con OLED
make noled   # Limpia y compila sin OLED
```

**En PC:**

```bash
# OLED se deshabilita automáticamente
make

# O forzar sin OLED
make OLED=no
```

### Compilación con opciones

```bash
# Ver opciones disponibles
make help

# Ver información del proyecto
make info

# Compilar con debug
make CXXFLAGS="-g -O0"

# Limpiar y recompilar
make clean && make
```

### Estructura de salida

```
bin/
└── App              # Binario final

obj/
├── src/
│   ├── main.o
│   ├── Device_t.o
│   ├── HMC5883L.o
│   └── oled/
│       ├── SSD1306_OLED.o
│       ├── SSD1306_OLED_font.o
│       ├── SSD1306_OLED_graphics.o
│       └── SSD1306_OLED_Print.o
```

---

## Ejecución

### Ejecutar aplicación

```bash
sudo ./bin/App
```

**Nota**: Se requiere `sudo` para acceder a los pines GPIO.

### Ejecutar con opciones

```bash
# Ver versión
./bin/App --version

# Ver ayuda (próximamente)
./bin/App --help
```

---

## Errores Comunes de Compilación

### Error: "bcm2835.h: No such file or directory"

**Solución**:
```bash
sudo apt-get install libbcm2835-dev
# o
sudo ./scripts/install_deps.sh
```

### Error: "curl/curl.h: No such file or directory"

**Solución**:
```bash
sudo apt-get install libcurl4-openssl-dev
```

### Error: "json.hpp: No such file or directory"

**Solución**:
```bash
sudo apt-get install nlohmann-json3-dev
```

### Error: "VERSION: No such file or directory"

**Solución**: Verificar que el archivo VERSION existe:
```bash
cat VERSION
```

Si no existe, crearlo:
```bash
echo "0.1.0" > VERSION
```

---

## Limpieza

### Limpiar archivos objeto

```bash
make clean
```

### Limpiar todo

```bash
make distclean
```

---

## Instalación (Opcional)

### Instalar en /usr/local/bin

```bash
sudo make install
```

### Desinstalar

```bash
sudo rm /usr/local/bin/App
```

---

## Compilación Cruzada (Cross-Compilation)

Para compilar en un sistema diferente (ej: x86) y ejecutar en Raspberry Pi:

```bash
# Instalar herramientas de cross-compilation
sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

# Compilar
make CXX=arm-linux-gnueabihf-g++ CC=arm-linux-gnueabihf-gcc
```

---

## Debugging

### Compilar con symbols de debug

```bash
make CXXFLAGS="-g -O0 -DDEBUG"
```

### Usar gdb

```bash
gdb ./bin/App
```

### Usar valgrind

```bash
valgrind --leak-check=full ./bin/App
```

---

## Optimizaciones

### Flags de optimización

El Makefile usa `-O2` por defecto. Para producción:

```bash
make CXXFLAGS="-O3 -march=native"
```

Para debug:

```bash
make CXXFLAGS="-g -O0"
```

---

## Soporte

Si tiene problemas de compilación:

1. Revisar esta documentación
2. Ejecutar `sudo ./scripts/install_deps.sh`
3. Verificar versión de gcc: `gcc --version`
4. Revisar logs de error
5. Consultar [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
