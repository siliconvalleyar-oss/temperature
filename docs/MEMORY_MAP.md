# MEMORY_MAP - Mapa de Memoria

## Visión General

Este documento describe el uso de memoria del proyecto RaspberryPi Weather App.

---

## Uso de Memoria por Componente

### Device_t

| Variable | Tipo | Tamaño | Descripción |
|----------|------|--------|-------------|
| m_city | string | 24 bytes | Ciudad configurada |
| m_country | string | 24 bytes | País configurado |
| m_apiKey | string | 24 bytes | API key |
| m_hardwareInitialized | bool | 1 byte | Estado de hardware |
| m_updateIntervalMs | unsigned int | 4 bytes | Intervalo de actualización |

**Total por instancia**: ~77 bytes + datos internos

### HMC5883L

| Variable | Tipo | Tamaño | Descripción |
|----------|------|--------|-------------|
| m_address | uint8_t | 1 byte | Dirección I2C |
| m_initialized | bool | 1 byte | Estado de inicialización |
| m_currentRange | Range | 4 bytes | Rango actual |
| m_currentMode | Mode | 4 bytes | Modo actual |

**Total por instancia**: ~10 bytes

### SSD1306

| Variable | Tipo | Tamaño | Descripción |
|----------|------|--------|-------------|
| buffer | uint8_t* | 8 bytes | Puntero a buffer |
| bufferWidth | uint8_t | 1 byte | Ancho del buffer |
| bufferHeight | uint8_t | 1 byte | Alto del buffer |
| _OLED_WIDTH | int16_t | 2 bytes | Ancho del display |
| _OLED_HEIGHT | int16_t | 2 bytes | Alto del display |
| _OLED_PAGE_NUM | int8_t | 1 byte | Número de páginas |

**Total por instancia**: ~15 bytes + buffer (1024 bytes para 128x64)

---

## Buffer OLED

### Configuración 128x64

```
Tamaño: 128 × 64 / 8 = 1024 bytes
```

### Estructura del Buffer

```
┌─────────────────────────────────────┐
│           Buffer OLED               │
│  ┌─────────────────────────────┐    │
│  │  Página 0 (filas 0-7)      │    │
│  │  128 bytes                 │    │
│  ├─────────────────────────────┤    │
│  │  Página 1 (filas 8-15)     │    │
│  │  128 bytes                 │    │
│  ├─────────────────────────────┤    │
│  │  ...                       │    │
│  ├─────────────────────────────┤    │
│  │  Página 7 (filas 56-63)    │    │
│  │  128 bytes                 │    │
│  └─────────────────────────────┘    │
│  Total: 1024 bytes                  │
└─────────────────────────────────────┘
```

---

## Memoria Dinámica

### Asignaciones

```cpp
// Device_t
std::string m_city;           // 24 bytes (string small object)
std::string m_country;        // 24 bytes
std::string m_apiKey;         // 24 bytes

// SSD1306
uint8_t* buffer = new uint8_t[1024];  // 1024 bytes

// curl
CURL* curl = curl_easy_init();  // ~1KB estructura interna
```

### Liberación

```cpp
// Device_t destructor
~Device_t() {
    // strings se liberan automáticamente
}

// SSD1306 destructor
~SSD1306() {
    delete[] buffer;
}

// curl
curl_easy_cleanup(curl);
```

---

## Memoria Estática

### Constantes

```cpp
// HMC5883L
static constexpr uint8_t DEFAULT_ADDRESS = 0x1E;

// SSD1306
#define SSD1306_ADDR 0x3C
#define BLACK 0
#define WHITE 1
#define INVERSE 2
```

### Tablas de Fuentes

```cpp
// Font_One[] - 5x8 ASCII
static const unsigned char Font_One[] = { ... };  // ~768 bytes

// Font_Two[] - 7x8 Thick
static const unsigned char Font_Two[] = { ... };  // ~512 bytes
```

---

## Stack Usage

### main()

```
┌─────────────────────────────┐
│  main()                     │
│  ├── argc: int (4 bytes)    │
│  ├── argv: char** (8 bytes) │
│  ├── arg: string (24 bytes) │
│  └── device: unique_ptr (8) │
│  Total: ~44 bytes           │
└─────────────────────────────┘
```

### Device_t::run()

```
┌─────────────────────────────┐
│  run()                      │
│  ├── temperature: double (8)│
│  └── ...                    │
│  Total: ~50 bytes           │
└─────────────────────────────┘
```

---

## Límites de Memoria

### Raspberry Pi por Modelo

| Modelo | RAM | Límite Recomendado |
|--------|-----|-------------------|
| Pi Zero | 512 MB | < 100 MB |
| Pi 3 | 1 GB | < 256 MB |
| Pi 4 | 2-8 GB | < 512 MB |

### Uso Estimado de esta App

- **Idle**: ~5 MB
- **Activo**: ~8 MB
- **Pico**: ~12 MB

---

## Optimización de Memoria

### Técnicas Aplicadas

1. **unique_ptr**: Evita fugas de memoria
2. **String interning**: Reutilizar strings comunes
3. **Buffer estático**: OLED buffer en heap
4. **Lazy initialization**: Inicializar solo cuando se necesita

### Técnicas Futuras

1. **Pool allocation**: Alocación en bloque
2. **Memory-mapped files**: Para datos grandes
3. **Compression**: Comprimir datos en memoria
4. **Stack allocation**: Mover objetos al stack

---

## Monitoreo de Memoria

### Herramientas

```bash
# Ver uso de memoria
free -h

# Ver proceso específico
ps -p <PID> -o pid,rss,vsz

# Valgrind para memory leaks
valgrind --leak-check=full ./bin/App
```

### Métricas

- **RSS (Resident Set Size)**: Memoria física usada
- **VSZ (Virtual Size)**: Memoria virtual total
- **Heap**: Memoria dinámica
- **Stack**: Memoria de llamadas

---

## Enlaces

- [Arquitectura](ARCHITECTURE.md)
- [Hardware](HARDWARE.md)
- [Performance](PERFORMANCE.md)
