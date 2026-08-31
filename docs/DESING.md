# DESING - Diseño del Sistema

## Visión General

Este documento describe el diseño del sistema RaspberryPi Weather App.

---

## Principios de Diseño

### 1. Simplicidad

- Código claro y legible
- Funciones con responsabilidad única
- Nombres descriptivos

### 2. Modularidad

- Componentes independientes
- Interfaces bien definidas
- Bajo acoplamiento

### 3. Extensibilidad

- Fácil agregar nuevas funcionalidades
- Patrones de diseño apropiados
- Código reutilizable

### 4. Confiabilidad

- Manejo robusto de errores
- Fallbacks apropiados
- Logging adecuado

---

## Patrones de Diseño

### 1. Singleton (Opcional)

Para gestionar acceso a hardware compartido:

```cpp
class HardwareManager {
public:
    static HardwareManager& instance() {
        static HardwareManager instance;
        return instance;
    }
private:
    HardwareManager() = default;
};
```

### 2. Strategy

Para intercambiar algoritmos (ej: fuentes de datos):

```cpp
class WeatherSource {
public:
    virtual double getTemperature() = 0;
};

class OpenWeatherMap : public WeatherSource {
    double getTemperature() override;
};
```

### 3. Observer

Para notificar cambios (ej: nueva temperatura):

```cpp
class Observer {
public:
    virtual void onTemperatureUpdate(double temp) = 0;
};

class Subject {
    std::vector<Observer*> observers;
    void notify(double temp);
};
```

---

## Estructura de Datos

### Configuración

```cpp
struct Config {
    std::string apiKey;
    std::string city;
    std::string country;
    unsigned int updateInterval;
};
```

### Datos Meteorológicos

```cpp
struct WeatherData {
    double temperature;
    double humidity;
    double pressure;
    std::string description;
    std::string city;
    time_t timestamp;
};
```

---

## Flujo de Control

```
┌─────────────┐
│   Inicio    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Inicializar │
│ Hardware    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Cargar      │
│ Config      │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Bucle       │◀──────────┐
│ Principal   │           │
└──────┬──────┘           │
       │                  │
       ▼                  │
┌─────────────┐           │
│ Obtener     │           │
│ Datos       │           │
└──────┬──────┘           │
       │                  │
       ▼                  │
┌─────────────┐           │
│ Procesar    │           │
│ Datos       │           │
└──────┬──────┘           │
       │                  │
       ▼                  │
┌─────────────┐           │
│ Mostrar     │           │
│ Resultados  │           │
└──────┬──────┘           │
       │                  │
       ▼                  │
┌─────────────┐           │
│ Esperar     │───────────┘
│ Intervalo   │
└─────────────┘
```

---

## Interfaz de Usuario

### Consola

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

### OLED

```
┌────────────────────────┐
│ Ciudad: Buenos Aires   │
│                        │
│ Temp: 22.5 C           │
│                        │
│                        │
└────────────────────────┘
```

---

## Manejo de Errores

### Errores de Red

```cpp
if (response.empty()) {
    std::cerr << "Error: Sin respuesta de API" << std::endl;
    // Fallback: usar datos en cache
}
```

### Errores de Hardware

```cpp
if (!bcm2835_init()) {
    std::cerr << "Error: bcm2835 no disponible" << std::endl;
    // Modo simulación
}
```

### Errores de Parsing

```cpp
try {
    auto data = json::parse(response);
} catch (json::parse_error& e) {
    std::cerr << "Error JSON: " << e.what() << std::endl;
}
```

---

## Rendimiento

### Optimizaciones

1. **Buffer OLED**: Actualizar solo cambios
2. **Cache**: Almacenar última temperatura
3. **Timeout**: Limitar espera HTTP
4. **Async**: Usar hilos para network (futuro)

### Métricas

- **Latencia API**: < 2s
- **Renderizado OLED**: < 100ms
- **Uso CPU**: < 5%
- **Memoria**: < 10MB

---

## Seguridad

### Consideraciones

1. **API Key**: No expuesta en logs
2. **HTTPS**: Para conexiones (futuro)
3. **Validación**: Entradas sanitizadas
4. **Permisos**: Mínimo necesario

---

## Escalabilidad

### Horizontal

- Múltiples dispositivos
- Centralización de datos
- Balanceo de carga

### Vertical

- Más sensores
- Más funcionalidades
- Más datos históricos

---

## Enlaces

- [ARCHITECTURE](ARCHITECTURE.md)
- [HARDWARE](HARDWARE.md)
- [API](API.md)
