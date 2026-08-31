# RULES - Reglas del Proyecto

## Reglas de Código

### 1. Estilo de Código

- **Indentación**: 4 espacios (no tabs)
- **Línea máxima**: 100 caracteres
- **Braces**: Estilo K&R
- **Espacios**: Después de keywords, antes de operadores

### 2. Convenciones de Nombres

| Tipo | Formato | Ejemplo |
|------|---------|---------|
| Clases | PascalCase | `Device_t` |
| Funciones | camelCase | `fetchTemperature()` |
| Variables | camelCase | `m_city` |
| Constantes | UPPER_SNAKE | `DEFAULT_ADDRESS` |
| Archivos | PascalCase | `Device_t.cpp` |
| Namespaces | lowercase | `Device` |

### 3. Comentarios

- **Funciones**: Doxygen style
- **Clases**: Doxygen style
- **Código complejo**: Explicar la lógica
- **TODOs**: Usar formato `// TODO: descripción`

### 4. Include Order

```cpp
// 1. Header propio
#include "Device_t.hpp"

// 2. Headers de sistema
#include <iostream>
#include <string>
#include <memory>

// 3. Headers de terceros
#include <curl/curl.h>
#include "nlohmann/json.hpp"

// 4. Headers del proyecto
#include "oled/SSD1306_OLED.hpp"
```

---

## Reglas de Git

### 1. Mensajes de Commit

Usar [Conventional Commits](https://www.conventionalcommits.org/):

```
tipo(alcance): descripción

[corpo opcional]

[footer opcional]
```

**Tipos**:
- `feat`: Nueva funcionalidad
- `fix`: Corrección de bug
- `docs`: Documentación
- `style`: Formato (no afecta código)
- `refactor`: Refactorización
- `test`: Tests
- `chore`: Mantenimiento

**Ejemplos**:
```
feat: agregar soporte para múltiples ciudades
fix: corregir error en parsing JSON
docs: actualizar guía de uso
```

### 2. Ramas

- `main`: Código estable
- `develop`: Desarrollo
- `feature/*`: Nuevas funcionalidades
- `fix/*`: Correcciones
- `release/*`: Pre-release

### 3. Pull Requests

1. Describir cambios
2. Tipo de cambio
3. Checklist:
   - [ ] Código compila
   - [ ] Tests pasan
   - [ ] Documentación actualizada

---

## Reglas de Build

### 1. Makefile

- **Target principal**: `all`
- **Limpieza**: `clean`, `distclean`
- **Instalación**: `install`
- **Ayuda**: `help`

### 2. Compilación

```bash
# Siempre limpiar antes de compilar
make clean && make

# Verificar warnings
make CXXFLAGS="-Wall -Wextra"
```

### 3. Versionado

- Actualizar `VERSION` antes de release
- Tags con formato `vX.Y.Z`
- CHANGELOG actualizado

---

## Reglas de Seguridad

### 1. API Keys

- **No commitear** en repositorio
- **Usar variables de entorno** o config
- **Rotar periódicamente**
- **Restringir uso** por IP si es posible

### 2. Permisos

- **Mínimo privilegio**: Solo lo necesario
- **sudo**: Solo cuando sea requerido
- **Archivos**: Permisos restrictivos

### 3. Datos

- **No loggear** datos sensibles
- **Encriptar** datos en tránsito
- **Validar** todas las entradas

---

## Reglas de Documentación

### 1. Archivos Obligatorios

- `README.md`: Descripción y uso
- `CHANGELOG.md`: Historial de cambios
- `CONTRIBUTING.md`: Cómo contribuir
- `LICENSE`: Licencia

### 2. Código

- **Funciones públicas**: Doxygen
- **Clases**: Doxygen
- **Archivos**: Encabezado con descripción
- **TODOs**: Formato estándar

### 3. Diagramas

- ASCII art para diagrams simples
- Mermaid o PlantUML para complejos
- Actualizar cuando cambie arquitectura

---

## Reglas de Testing

### 1. Tipos de Tests

- **Unitarios**: Funciones individuales
- **Integración**: Componentes juntos
- **Sistema**: Todo el sistema
- **Regresión**: No romper funcionalidad

### 2. Cobertura

- **Objetivo**: > 80%
- **Mínimo**: > 50%
- **Crítico**: 100%

### 3. Ejecución

```bash
# Antes de commit
make test

# Antes de merge
make test-all

# En CI/CD
make test-coverage
```

---

## Reglas de Deploy

### 1. Producción

1. **Testing completo**
2. **Code review aprobado**
3. **Documentación actualizada**
4. **Versión actualizada**
5. **Tag creado**

### 2. Rollback

1. **Identificar** versión anterior
2. **Restaurar** código
3. **Recompilar**
4. **Verificar** funcionamiento
5. **Actualizar** documentación

### 3. Monitoreo

1. **Logs**: Revisar regularmente
2. **Métricas**: CPU, memoria, red
3. **Alertas**: Configurar notificaciones
4. **Backup**: Automático y verificado

---

## Reglas de Comunicación

### 1. Issues

- **Bug report**: Template específico
- **Feature request**: Descripción clara
- **Question**: Contexto completo

### 2. Pull Requests

- **Título**: Conventional commit
- **Descripción**: Qué y por qué
- **Tests**: Incluir si aplica
- **Docs**: Actualizar si es necesario

### 3. Reviews

- **Constructivo**: Sugerir, no criticar
- **Específico**: Apuntar a líneas
- **Respetuoso**: Agradecer el esfuerzo

---

## Enlaces

- [CONTRIBUTING](CONTRIBUTING.md)
- [CODING_STANDARDS](CODING_STANDARDS.md)
- [SECURITY](SECURITY.md)
