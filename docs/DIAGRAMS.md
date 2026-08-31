# DIAGRAMS - Diagramas del Proyecto

## Diagrama de Componentes

```
┌─────────────────────────────────────────────────────────────┐
│                      main.cpp                               │
│                    (Punto de entrada)                        │
└─────────────────────────┬───────────────────────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│                     Device_t                                │
│                (Clase principal)                             │
│                                                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │  Hardware    │  │    API      │  │     Display         │  │
│  │  Manager     │  │  Client     │  │     Manager         │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
└─────────────────────────┬───────────────────────────────────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
┌─────────────┐  ┌─────────────┐  ┌─────────────┐
│  bcm2835    │  │   libcurl   │  │  SSD1306    │
│  (GPIO/I2C) │  │  (HTTP)     │  │  (OLED)     │
└─────────────┘  └─────────────┘  └─────────────┘
```

---

## Diagrama de Secuencia

```
┌──────┐          ┌──────────┐          ┌─────────┐          ┌──────┐
│ User │          │ Device_t │          │ OpenWeather│        │ OLED │
└──┬───┘          └────┬─────┘          └────┬──────┘        └──┬───┘
   │                   │                    │                   │
   │  Ejecutar App     │                    │                   │
   │──────────────────▶│                    │                   │
   │                   │                    │                   │
   │                   │  GET /weather      │                   │
   │                   │───────────────────▶│                   │
   │                   │                    │                   │
   │                   │  200 OK {temp}     │                   │
   │                   │◀───────────────────│                   │
   │                   │                    │                   │
   │                   │  Display temp      │                   │
   │                   │──────────────────────────────────────▶│
   │                   │                    │                   │
   │                   │  print(temp)       │                   │
   │  Temp: 22.5°C     │                    │                   │
   │◀──────────────────│                    │                   │
   │                   │                    │                   │
   │                   │  delay(60s)        │                   │
   │                   │────────┐           │                   │
   │                   │        │           │                   │
   │                   │◀───────┘           │                   │
   │                   │                    │                   │
   │                   │  (repetir)         │                   │
   │                   │───────────────────▶│                   │
   │                   │                    │                   │
```

---

## Diagrama de Estados

```
                    ┌───────────────┐
                    │   Inicio      │
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐
                    │ Inicializar   │
                    │ Hardware      │
                    └───────┬───────┘
                            │
                ┌───────────┴───────────┐
                │                       │
                ▼                       ▼
        ┌───────────────┐       ┌───────────────┐
        │   Hardware    │       │   Hardware    │
        │   OK          │       │   Error       │
        └───────┬───────┘       └───────┬───────┘
                │                       │
                │                       ▼
                │               ┌───────────────┐
                │               │ Modo          │
                │               │ Simulación    │
                │               └───────┬───────┘
                │                       │
                ▼                       ▼
        ┌───────────────────────────────────────┐
        │         Bucle Principal               │
        └───────────────┬───────────────────────┘
                        │
                        ▼
                ┌───────────────┐
                │ Obtener       │
                │ Temperatura   │
                └───────┬───────┘
                        │
            ┌───────────┴───────────┐
            │                       │
            ▼                       ▼
    ┌───────────────┐       ┌───────────────┐
    │   Temperatura │       │   Error       │
    │   Válida      │       │   API         │
    └───────┬───────┘       └───────┬───────┘
            │                       │
            ▼                       ▼
    ┌───────────────┐       ┌───────────────┐
    │ Mostrar en    │       │ Log error     │
    │ OLED/Console  │       │               │
    └───────┬───────┘       └───────┬───────┘
            │                       │
            └───────────┬───────────┘
                        │
                        ▼
                ┌───────────────┐
                │ Esperar       │
                │ 60 segundos   │
                └───────┬───────┘
                        │
                        └──────────────▶ (repetir)
```

---

## Diagrama de Clases

```
┌─────────────────────────────────────┐
│            Device_t                 │
├─────────────────────────────────────┤
│ - m_city: string                    │
│ - m_country: string                 │
│ - m_apiKey: string                  │
│ - m_hardwareInitialized: bool       │
│ - m_updateIntervalMs: unsigned int  │
├─────────────────────────────────────┤
│ + Device_t()                        │
│ + ~Device_t()                       │
│ + run(): void                       │
├─────────────────────────────────────┤
│ - initializeHardware(): bool        │
│ - fetchTemperature(city, country): double │
│ - displayOnOLED(temp, city): void   │
│ - displayOnConsole(temp, city): void│
│ - httpGet(url): string              │
│ - parseTemperature(json): double    │
└─────────────────────────────────────┘
                        │
                        │ usa
                        ▼
┌─────────────────────────────────────┐
│            HMC5883L                 │
├─────────────────────────────────────┤
│ - m_address: uint8_t               │
│ - m_initialized: bool              │
│ - m_currentRange: Range            │
│ - m_currentMode: Mode              │
├─────────────────────────────────────┤
│ + HMC5883L(address: uint8_t)       │
│ + ~HMC5883L()                      │
│ + begin(): bool                    │
│ + readMag(x, y, z): bool           │
│ + getHeading(): float              │
│ + setRange(range: Range): void     │
│ + setMode(mode: Mode): void        │
│ + dataReady(): bool                │
└─────────────────────────────────────┘
                        │
                        │ usa
                        ▼
┌─────────────────────────────────────┐
│            SSD1306                  │
├─────────────────────────────────────┤
│ + OLEDbegin(speed, address): void  │
│ + OLEDupdate(): void               │
│ + OLEDclearBuffer(): void          │
│ + drawPixel(x, y, color): void     │
│ + setCursor(x, y): void            │
│ + print(text): void                │
└─────────────────────────────────────┘
```

---

## Diagrama de Deployment

```
┌─────────────────────────────────────┐
│        Raspberry Pi 4B              │
│  ┌─────────────────────────────┐    │
│  │      Raspbian/RPi OS        │    │
│  │  ┌─────────────────────┐    │    │
│  │  │   Weather App       │    │    │
│  │  │   (bin/App)         │    │    │
│  │  └─────────┬───────────┘    │    │
│  │            │                │    │
│  │  ┌─────────▼───────────┐    │    │
│  │  │   bcm2835           │    │    │
│  │  │   (GPIO/I2C)        │    │    │
│  │  └─────────┬───────────┘    │    │
│  │            │                │    │
│  └────────────┼────────────────┘    │
│               │                     │
│       ┌───────┴───────┐             │
│       │               │             │
│       ▼               ▼             │
│  ┌─────────┐    ┌─────────┐        │
│  │  OLED   │    │ HMC5883L│        │
│  │ SSD1306 │    │ (Brújula)│       │
│  └─────────┘    └─────────┘        │
│                                     │
│  ┌─────────────────────────────┐    │
│  │        Red (WiFi)           │    │
│  │            │                │    │
│  └────────────┼────────────────┘    │
│               │                     │
└───────────────┼─────────────────────┘
                │
                ▼
        ┌───────────────┐
        │ OpenWeather   │
        │   Map API     │
        └───────────────┘
```

---

## Diagrama de Datos

```
┌──────────────────┐
│  OpenWeatherMap  │
│      API         │
└────────┬─────────┘
         │
         │ JSON Response
         │ {
         │   "main": {
         │     "temp": 22.5,
         │     "humidity": 65,
         │     "pressure": 1013
         │   },
         │   "weather": [{
         │     "description": "clear sky"
         │   }],
         │   "name": "Buenos Aires"
         │ }
         │
         ▼
┌──────────────────┐
│   parseTemp()    │
│   (extract temp) │
└────────┬─────────┘
         │
         │ double: 22.5
         │
         ▼
┌──────────────────┐
│   displayOnOLED  │
│   displayOnConsole│
└──────────────────┘
```

---

## Diagrama de Red

```
┌─────────────────────────────────────┐
│          LAN (192.168.1.0/24)       │
│                                     │
│  ┌─────────────┐    ┌─────────────┐ │
│  │ Raspberry Pi│    │  Router     │ │
│  │ 192.168.1.10│◀──▶│ 192.168.1.1 │ │
│  └──────┬──────┘    └──────┬──────┘ │
│         │                  │        │
│         │                  │        │
└─────────┼──────────────────┼────────┘
          │                  │
          │                  │
          ▼                  ▼
   ┌─────────────┐    ┌─────────────┐
   │ OpenWeather │    │   Internet  │
   │    Map      │    │             │
   │    API      │    │             │
   └─────────────┘    └─────────────┘
```

---

## Secuencia de Inicialización

```
1. main()
   │
   ├──▶ Device_t()
   │      │
   │      ├──▶ inicializar valores
   │      └──▶ retorno
   │
   ├──▶ device->run()
   │      │
   │      ├──▶ initializeHardware()
   │      │      │
   │      │      ├──▶ bcm2835_init()
   │      │      └──▶ retorno
   │      │
   │      ├──▶ fetchTemperature()
   │      │      │
   │      │      ├──▶ httpGet()
   │      │      │      │
   │      │      │      ├──▶ curl_easy_init()
   │      │      │      ├──▶ curl_easy_perform()
   │      │      │      └──▶ curl_easy_cleanup()
   │      │      │
   │      │      └──▶ parseTemperature()
   │      │             │
   │      │             └──▶ json::parse()
   │      │
   │      ├──▶ displayOnOLED()
   │      │      │
   │      │      └──▶ oled.print()
   │      │
   │      ├──▶ displayOnConsole()
   │      │      │
   │      │      └──▶ std::cout
   │      │
   │      └──▶ delay(60s)
   │
   └──▶ retorno 0
```

---

## Enlaces

- [Arquitectura](ARCHITECTURE.md)
- [Hardware](HARDWARE.md)
- [API](API.md)
