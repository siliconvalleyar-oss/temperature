# ROADMAP - Hoja de Ruta

## Versión 0.1.0 (Actual)

### Completado

- [x] Estructura inicial del proyecto
- [x] Clase Device_t con método run()
- [x] Integración con OpenWeatherMap API
- [x] Soporte para display OLED SSD1306
- [x] Driver para magnetómetro HMC5883L
- [x] Makefile con soporte de versiones
- [x] Scripts de instalación
- [x] Documentación completa

---

## Versión 0.2.0 (Próxima)

### Planeado

- [ ] Manejo completo de errores
- [ ] Configuración persistente
- [ ] Modo de ahorro de energía
- [ ] Soporte para múltiples ciudades
- [ ] Tests unitarios

### Fecha Estimada

- **Inicio**: Septiembre 2026
- **Fin**: Octubre 2026

---

## Versión 0.3.0

### Planeado

- [ ] Gráficas de temperatura histórica en OLED
- [ ] Alarma de temperatura
- [ ] Soporte para múltiples sensores
- [ ] Modo de prueba (test mode)
- [ ] Tests de integración

### Fecha Estimada

- **Inicio**: Octubre 2026
- **Fin**: Noviembre 2026

---

## Versión 0.4.0

### Planeado

- [ ] Dashboard web local
- [ ] Almacenamiento en SQLite
- [ ] Exportación de datos (CSV/JSON)
- [ ] Alertas por email
- [ ] CI/CD con GitHub Actions

### Fecha Estimada

- **Inicio**: Noviembre 2026
- **Fin**: Diciembre 2026

---

## Versión 1.0.0

### Planeado

- [ ] Soporte completo para múltiples sensores
- [ ] Interfaz web completa
- [ ] Actualización OTA
- [ ] Documentación de API completa
- [ ] Benchmark y optimización

### Fecha Estimada

- **Inicio**: Diciembre 2026
- **Fin**: Enero 2027

---

## Funcionalidades Futuras

### Corto Plazo (3-6 meses)

1. **Manejo de errores robusto**
   - Retry automático
   - Fallback a modo offline
   - Logging detallado

2. **Múltiples fuentes de datos**
   - OpenWeatherMap
   - WeatherAPI
   - AccuWeather

3. **Almacenamiento local**
   - SQLite para datos históricos
   - Exportación a CSV

### Mediano Plazo (6-12 meses)

1. **Interfaz web**
   - Dashboard en tiempo real
   - Gráficas interactivas
   - Configuración remota

2. **Alertas**
   - Email
   - SMS (vía API)
   - Notificaciones push

3. **Múltiples dispositivos**
   - Monitoreo centralizado
   - Comparación entre estaciones

### Largo Plazo (12+ meses)

1. **Machine Learning**
   - Predicción de clima
   - Detección de anomalías
   - Optimización automática

2. **Integración IoT**
   - MQTT
   - Home Assistant
   - Node-RED

3. **Hardware avanzado**
   - Soporte para más sensores
   - Conectividad LoRa
   - Energía solar

---

## Prioridades

### Alta

1. Estabilidad y confiabilidad
2. Manejo de errores
3. Documentación

### Media

1. Rendimiento
2. Nuevas funcionalidades
3. Tests

### Baja

1. Optimizaciones avanzadas
2. Features experimentales
3. Soporte para hardware exótico

---

## Contribuir

Para contribuir a alguna funcionalidad:

1. Revisar el roadmap
2. Crear un issue con la etiqueta `enhancement`
3. Discutir implementación
4. Enviar Pull Request

---

## Feedback

El roadmap está sujeto a cambios según:

- Feedback de usuarios
- Disponibilidad de tiempo
- Cambios en dependencias
- Nuevas oportunidades

Para sugerir cambios, crear un issue con la etiqueta `roadmap`.

---

## Enlaces

- [CHANGELOG](CHANGELOG.md)
- [TODO](TODO.md)
- [CONTRIBUTING](CONTRIBUTING.md)
