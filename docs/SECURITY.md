# SECURITY - Documentación de Seguridad

## Visión General

Este documento describe las consideraciones de seguridad del proyecto
RaspberryPi Weather App.

---

## API Key de OpenWeatherMap

### Riesgos

- **Exposición**: La API key puede ser interceptada si se almacena en texto plano
- **Uso no autorizado**: Personas no autorizadas pueden usar tu API key
- **Límites**: Se pueden agotar las cuotas de la API

### Mitigaciones

1. **No commitear la API key**:
   ```bash
   # Agregar a .gitignore
   echo "config/config.cfg" >> .gitignore
   ```

2. **Usar variables de entorno**:
   ```bash
   export OPENWEATHER_API_KEY="tu_api_key"
   ```

3. **Rotar la key periódicamente**:
   - Crear nueva key en OpenWeatherMap
   - Actualizar configuración
   - Eliminar key antigua

4. **Restringir uso**:
   - Configurar IP en OpenWeatherMap
   - Monitorear uso

### Almacenamiento Seguro

```cpp
// Mala práctica
std::string api_key = "tu_api_key_aqui";

// Buena práctica
std::string api_key = std::getenv("OPENWEATHER_API_KEY");
```

---

## Acceso a GPIO

### Riesgos

- **Daño硬件**: Acceso incorrecto puede dañar la Raspberry Pi
- **Privilegios**: La aplicación requiere permisos de root
- **Seguridad física**: Acceso físico al dispositivo

### Mitigaciones

1. **Ejecutar con mínimo privilegio**:
   ```bash
   # En lugar de sudo
   sudo setcap cap_net_raw+ep ./bin/App
   ```

2. **Validar entradas**:
   ```cpp
   // Validar pin antes de usar
   if (pin < 0 || pin > 27) {
       return false;
   }
   ```

3. **Limpieza al salir**:
   ```cpp
   Device_t::~Device_t() {
       // Restaurar pines a estado inicial
       bcm2835_close();
   }
   ```

4. **Monitoreo**:
   - Log de accesos a GPIO
   - Alertas de uso inusual

---

## Red

### Riesgos

- **Intercepción**: Datos transmitidos pueden ser interceptados
- **Man-in-the-middle**: Atacante puede modificar solicitudes
- **DNS spoofing**: Redireccionar a servidor falso

### Mitigaciones

1. **Usar HTTPS** (cuando sea posible):
   ```cpp
   curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/...");
   ```

2. **Verificar certificados**:
   ```cpp
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
   ```

3. **Timeout**:
   ```cpp
   curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
   ```

4. **Validar respuestas**:
   ```cpp
   if (http_code != 200) {
       // Manejar error
   }
   ```

---

## Datos Locales

### Riesgos

- **Almacenamiento inseguro**: Configuración con datos sensibles
- **Permisos incorrectos**: Archivos legibles por otros usuarios
- **Residuos**: Datos no borrados correctamente

### Mitigaciones

1. **Permisos de archivos**:
   ```bash
   chmod 600 config/config.cfg
   chmod 700 config/
   ```

2. **Encriptación** (opcional):
   ```cpp
   // Usar OpenSSL para encriptar
   #include <openssl/evp.h>
   ```

3. **Limpieza de memoria**:
   ```cpp
   // Borrar datos sensibles
   memset(api_key.data(), 0, api_key.size());
   ```

---

## Actualizaciones

### Riesgos

- **Dependencias obsoletas**: Librerías con vulnerabilidades conocidas
- **Actualizaciones maliciosas**: Paquetes comprometidos

### Mitigaciones

1. **Mantener dependencias actualizadas**:
   ```bash
   sudo apt-get update && sudo apt-get upgrade
   ```

2. **Verificar hashes**:
   ```bash
   # Verificar integridad de descargas
   sha256sum bcm2835-1.71.tar.gz
   ```

3. **Usar repositorios oficiales**:
   ```bash
   sudo apt-get install libbcm2835-dev  # En lugar de compilar desde fuente
   ```

---

## Logs

### Riesgos

- **Información sensible**: Logs pueden contener API keys o datos personales
- **Acceso no autorizado**: Logs legibles por otros usuarios

### Mitigaciones

1. **No loggear datos sensibles**:
   ```cpp
   // Mala práctica
   std::cout << "API Key: " << api_key << std::endl;
   
   // Buena práctica
   std::cout << "Conectando a API..." << std::endl;
   ```

2. **Permisos de archivos de log**:
   ```bash
   chmod 640 /var/log/weather.log
   ```

3. **Rotación de logs**:
   ```bash
   # Usar logrotate
   /etc/logrotate.d/weather
   ```

---

## Auditoría

### Checklist de Seguridad

- [ ] API key no está en código fuente
- [ ] API key no está en repositorio Git
- [ ] Permisos de archivos son correctos
- [ ] No se loguean datos sensibles
- [ ] Dependencias están actualizadas
- [ ] Se usa HTTPS cuando es posible
- [ ] Se validan entradas externas
- [ ] Se manejan errores adecuadamente

### Herramientas

```bash
# Escanear dependencias
sudo apt-get install clamav
clamscan -r .

# Verificar permidos
find . -type f -perm /o+w

# Buscar API keys
grep -r "api_key" --include="*.cpp" --include="*.hpp"
```

---

## Incidentes

### Reportar Vulnerabilidades

Si encuentras una vulnerabilidad:

1. **No la publiques** públicamente
2. **Contacta** al maintainers
3. **Proporciona** detalles para reproducción
4. **Espera** confirmación antes de disclosure

### Respuesta a Incidentes

1. **Identificar**: Determinar alcance del problema
2. **Contener**: Limitar daños
3. **Erradicar**: Corregir vulnerabilidad
4. **Recuperar**: Restaurar normalidad
5. **Lecciones**: Documentar para futuro

---

## Recursos

- [OWASP IoT Top 10](https://owasp.org/www-project-internet-of-things/)
- [Raspberry Pi Security](https://www.raspberrypi.org/documentation/configuration/security.md)
- [CWE/SANS Top 25](https://cwe.mitre.org/top25/)
