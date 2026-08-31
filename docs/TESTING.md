# TESTING - Documentación de Testing

## Visión General

Este documento describe cómo probar el proyecto RaspberryPi Weather App.

---

## Tipos de Tests

### 1. Tests Unitarios

Prueban funciones individuales en aislamiento.

**Ejemplo**:
```cpp
#include <cassert>
#include "Device_t.hpp"

void test_parseTemperature() {
    Device::Device_t device;
    std::string json = R"({"main":{"temp":22.5}})";
    double temp = device.parseTemperature(json);
    assert(temp == 22.5);
}

int main() {
    test_parseTemperature();
    return 0;
}
```

### 2. Tests de Integración

Prueban la interacción entre componentes.

**Ejemplo**:
```cpp
void test_weather_fetch() {
    Device::Device_t device;
    double temp = device.fetchTemperature("Buenos Aires", "AR");
    assert(temp > -100 && temp < 100);
}
```

### 3. Tests de Hardware

Prueban la comunicación con hardware real.

**Ejemplo**:
```cpp
void test_oled_init() {
    SSD1306 oled(128, 64);
    bool result = oled.OLEDbegin();
    assert(result == true);
}
```

---

## Ejecutar Tests

### Compilar Tests

```bash
# Compilar todos los tests
make test

# Compilar test específico
g++ -o test_device tests/test_device.cpp -Iinclude -std=c++11
```

### Ejecutar Tests

```bash
# Ejecutar todos los tests
./tests/run_all.sh

# Ejecutar test específico
./bin/test_device
```

---

## Tests Manuales

### Verificar Conexión API

```bash
curl "http://api.openweathermap.org/data/2.5/weather?q=Buenos Aires,AR&appid=TU_API_KEY&units=metric"
```

### Verificar OLED

```bash
# Probar display básico
sudo ./bin/App --test-oled
```

### Verificar I2C

```bash
# Escanear dispositivos
sudo i2cdetect -y 1

# Leer registro
sudo i2cget -y 1 0x3C 0x00
```

---

## Cobertura de Código

### Generar Reporte

```bash
# Instalar gcov
sudo apt-get install lcov

# Compilar con cobertura
make CXXFLAGS="--coverage"

# Ejecutar tests
./tests/run_all.sh

# Generar reporte
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage/
```

### Ver Reporte

```bash
# Abrir en navegador
xdg-open coverage/index.html
```

---

## Tests de Rendimiento

### Benchmark API

```bash
# Medir tiempo de respuesta
time curl "http://api.openweathermap.org/data/2.5/weather?q=Buenos Aires,AR&appid=TU_API_KEY&units=metric"
```

### Benchmark App

```bash
# Medir tiempo de ejecución
time sudo ./bin/App --once
```

---

## Tests de Estrés

### Múltiples Solicitudes

```bash
# 100 solicitudes en paralelo
for i in {1..100}; do
    curl -s "http://api.openweathermap.org/data/2.5/weather?q=Buenos Aires,AR&appid=TU_API_KEY&units=metric" > /dev/null &
done
wait
```

### Memoria

```bash
# Verificar memory leaks
valgrind --leak-check=full sudo ./bin/App --once
```

---

## Tests de Compatibilidad

### Raspberry Pi 32-bit

```bash
# Compilar en 32-bit
make CXXFLAGS="-m32"
```

### Raspberry Pi 64-bit

```bash
# Compilar en 64-bit
make CXXFLAGS="-m64"
```

---

## CI/CD (Futuro)

### GitHub Actions

```yaml
name: CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: sudo ./scripts/install_deps.sh
      - name: Build
        run: make
      - name: Test
        run: make test
```

---

## Reporting

### Bug Reports

Al reportar bugs, incluir:

1. **Descripción** del problema
2. **Pasos** para reproducir
3. **Comportamiento** esperado vs actual
4. **Entorno** (RPi modelo, SO, versión)
5. **Logs** de error

### Template

```
## Bug Report

**Descripción**: [Descripción breve]

**Pasos para reproducir**:
1. [Paso 1]
2. [Paso 2]

**Comportamiento esperado**: [Qué debería pasar]

**Comportamiento actual**: [Qué pasa]

**Entorno**:
- Raspberry Pi: [Modelo]
- SO: [Versión]
- Versión App: [Versión]

**Logs**:
```
[Logs de error]
```
```

---

## Checklist de Testing

### Antes de Cada Release

- [ ] Todos los tests unitarios pasan
- [ ] Tests de integración pasan
- [ ] No hay memory leaks (valgrind)
- [ ] Compila sin warnings
- [ ] Funciona en RPi 32-bit
- [ ] Funciona en RPi 64-bit
- [ ] API key no está hardcodeada
- [ ] Documentación actualizada

### Pruebas Manuales

- [ ] App inicia correctamente
- [ ] Obtiene temperatura de API
- [ ] Muestra en consola
- [ ] Muestra en OLED (si disponible)
- [ ] --version funciona
- [ ] Ctrl+C limpia correctamente

---

## Enlaces

- [Guía de Uso](USAGE.md)
- [Compilación](BUILD.md)
- [Contribución](CONTRIBUTING.md)
