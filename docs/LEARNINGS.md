# LEARNINGS - Aprendizajes del Proyecto RaspberryPi Weather App

## Git / Versionado

### Flujo de versionado

1. Obtener el último tag publicado (ej: `v1.0.0`).
2. El archivo `VERSION` debe coincidir con ese tag (sin `v`).
3. Calcular la siguiente versión: `tag + 0.0.1` (ej: `v1.0.0` → `1.0.1`).
4. Actualizar `VERSION` a la nueva versión.
5. Confirmar cambios y pushear con el nuevo tag.

### Reglas

- **Los mensajes de commit deben seguir conventional commits**: `feat:`, `fix:`, `docs:`, `chore:`, `refactor:`, `test:`.
- **No eliminar tags publicados.** Si hay error, crear nuevo tag.
- **El tag y `VERSION` siempre deben coincidir** (tag con `v`, `VERSION` sin `v`).

---

## Hardware

### bcm2835

- **Permisos**: La librería bcm2835 requiere permisos de root para acceder a los pines GPIO.
- **Instalación**: Se puede instalar desde apt (`libbcm2835-dev`) o compilar desde fuente.
- **Compatibilidad**: Funciona tanto en Raspberry Pi 32 bits como 64 bits.

### OLED SSD1306

- **Dirección I2C**: Por defecto es 0x3C, pero puede ser 0x3D según el módulo.
- **Inicialización**: Requiere una secuencia de inicialización específica para configurar los registros internos.
- **Buffer**: El display usa un buffer de 1024 bytes (128x64/8) para manejar los píxeles.

### HMC5883L

- **Comunicación**: Usa I2C a 100kHz o 400kHz.
- **ID del sensor**: El registro de identificación retorna "H43" para HMC5883L válido.
- **Calibración**: Para precisión, se recomienda calibrar el sensor rotándolo en todos los ejes.

---

## Red

### OpenWeatherMap API

- **API Key**: Se necesita registrarse en openweathermap.org para obtener una API key.
- **Rate Limit**: El plan gratuito permite 60 llamadas por minuto.
- **Formato de respuesta**: JSON con estructura `main.temp` para temperatura.
- **Unidades**: Usar `units=metric` para grados Celsius.

### libcurl

- **Inicialización**: Siempre llamar a `curl_easy_init()` antes de usar y `curl_easy_cleanup()` después.
- **Timeout**: Configurar `CURLOPT_TIMEOUT` para evitar bloqueos infinitos.
- **Errores**: Verificar tanto el código de retorno de curl como el código HTTP.

---

## Compilación

### Makefile

- **Versión**: Leer del archivo VERSION y pasar como `-DVERSION` al compilador.
- **Jerarquía**: Mantener la estructura de directorios en obj/ para facilitar la depuración.
- **Dependencias**: Usar `| directorio` para crear directorios antes de compilar.

### Errores comunes

- **bcm2835 no encontrado**: Ejecutar `sudo ./scripts/install_deps.sh`
- **Permisos**: Ejecutar `sudo ./bin/App` para acceder a GPIO
- **API key**: Verificar que la API key de OpenWeatherMap sea válida

---

## Mejores prácticas

1. **Gestión de memoria**: Usar `std::unique_ptr` para evitar fugas de memoria.
2. **RAII**: Inicializar y limpiar recursos en constructores/destructores.
3. **Namespace**: Usar namespaces para organizar el código.
4. **Documentación**: Comentar funciones con Doxygen para generación automática.
5. **Constantes**: Usar `const` y `constexpr` para valores fijos.
6. **Manejo de errores**: Retornar códigos de error en lugar de excepciones para código embebido.
