# SKILLS - Habilidades del Proyecto

## Habilidades Técnicas

### Programación

- **C++11/14/17**: Lenguaje principal del proyecto
- **POO**: Programación orientada a objetos
- **Gestión de memoria**: unique_ptr, RAII
- **Templates**: Uso de plantillas C++
- **Namespaces**: Organización de código

### Hardware

- **Raspberry Pi**: Configuración y uso
- **GPIO**: Acceso a pines de entrada/salida
- **I2C**: Comunicación con sensores y displays
- **SPI**: Comunicación de alta velocidad
- **bcm2835**: Librería de acceso a hardware

### Redes

- **HTTP/HTTPS**: Protocolos de comunicación
- **REST APIs**: Consumo de servicios web
- **libcurl**: Cliente HTTP en C++
- **JSON**: Formato de intercambio de datos
- **nlohmann/json**: Librería JSON para C++

### Desarrollo

- **Git**: Control de versiones
- **Make**: Sistema de compilación
- **Linux**: Administración de sistemas
- **Shell scripting**: Automatización de tareas
- **Debugging**: Depuración de código

---

## Herramientas

### Compilación

| Herramienta | Uso | Versión |
|-------------|-----|---------|
| GCC/G++ | Compilador C/C++ | 7.0+ |
| Make | Build automation | 4.0+ |
| CMake | Build system alternativo | 3.10+ |

### Desarrollo

| Herramienta | Uso | Alternativa |
|-------------|-----|-------------|
| VS Code | Editor de código | Vim, Emacs |
| Git | Control de versiones | Mercurial |
| Valgrind | Memory debugging | AddressSanitizer |
| GDB | Depurador | LLDB |

### Monitoreo

| Herramienta | Uso | Comando |
|-------------|-----|---------|
| htop | Monitor de procesos | `htop` |
| iotop | Monitor de I/O | `sudo iotop` |
| iftop | Monitor de red | `sudo iftop` |
| vmstat | Estadísticas VM | `vmstat 1` |

---

## Conocimientos

### Arduino (Transferible)

- Lógica de programación embebida
- Uso de sensores y displays
- Comunicación I2C/SPI
- Biblioteca Wire (similar a bcm2835)

### Raspberry Pi

- Instalación de SO
- Configuración de hardware
- Administración de servicios
- Seguridad básica

### Redes

- Protocolos HTTP/HTTPS
- DNS y resolución de nombres
- Firewalls y proxies
- Certificados SSL/TLS

---

## Aprendizajes

###bcm2835

1. **Permisos**: Requiere root para GPIO
2. **Inicialización**: Siempre llamar a `bcm2835_init()` y `bcm2835_close()`
3. **I2C**: Configurar dirección antes de comunicar
4. **Errores**: Verificar retorno de funciones

### OpenWeatherMap API

1. **Rate limiting**: 60 req/min en plan gratuito
2. **Formato**: Respuesta JSON con estructura específica
3. **Unidades**: Usar `units=metric` para Celsius
4. **Errores**: Verificar código HTTP y campo `cod`

### OLED SSD1306

1. **Inicialización**: Secuencia específica de registros
2. **Buffer**: 1024 bytes para 128x64
3. **I2C**: Dirección 0x3C o 0x3D
4. **Contrast**: Ajustar según iluminación

### HMC5883L

1. **Calibración**: Necesaria para precisión
2. **ID**: Registro 0x0A retorna 'H', 0x0B '4', 0x0C '3'
3. **Modo**: Continuo o single-shot
4. **Rango**: Configurable según necesidad

---

## Competencias

### Resolución de Problemas

- Debugging sistemático
- Análisis de logs
- Prueba de hipótesis
- Búsqueda de documentación

### Documentación

- Doxygen para código
- Markdown para docs
- Diagramas con ASCII art
- Comentarios descriptivos

### Optimización

- Profiling de código
- Optimización de memoria
- Reducción de llamadas a API
- Cache de datos

---

## Certificaciones (Futuras)

### Objetivos

- [ ] Raspberry Pi Certified Educator
- [ ] Linux Foundation Certified
- [ ] AWS IoT Specialty

### Recursos

- [Raspberry Pi Training](https://www.raspberrypi.org/training/)
- [Linux Foundation Courses](https://training.linuxfoundation.org/)
- [AWS IoT Training](https://aws.amazon.com/training/)

---

## Enlaces

### Documentación

- [bcm2835 Library](http://www.airspayce.com/mikem/bcm2835/)
- [OpenWeatherMap API](https://openweathermap.org/api)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [HMC5883L Datasheet](https://www.adafruit.com/datasheets/HMC5883L.pdf)

### Comunidad

- [Raspberry Pi Forums](https://www.raspberrypi.org/forums/)
- [Stack Overflow](https://stackoverflow.com/questions/tagged/raspberry-pi)
- [GitHub Discussions](https://github.com/USUARIO/REPOSITORIO/discussions)

### Tutoriales

- [Adafruit Tutorials](https://learn.adafruit.com/)
- [SparkFun Tutorials](https://learn.sparkfun.com/)
- [Instructables](https://www.instructables.com/)
