# CONTRIBUTING - Guía de Contribución

## Bienvenido

¡Gracias por considerar contribuir a este proyecto! Este documento explica
cómo puedes ayudar.

---

## Cómo Contribuir

### 1. Reportar Bugs

Si encuentras un bug, por favor crea un issue con:

- **Descripción clara** del problema
- **Pasos para reproducir**
- **Comportamiento esperado**
- **Comportamiento actual**
- **Entorno** (versión de Raspberry Pi, SO, etc.)
- **Logs de error** (si aplica)

### 2. Sugerir Funcionalidades

Para sugerir nuevas funcionalidades:

1. Crea un issue con la etiqueta `enhancement`
2. Describe la funcionalidad
3. Explica por qué es útil
4. Proporciona ejemplos de uso

### 3. Enviar Pull Requests

#### Proceso

1. **Fork** el repositorio
2. **Crea una rama** para tu funcionalidad:
   ```bash
   git checkout -b feature/nueva-funcionalidad
   ```
3. **Haz tus cambios**
4. **Escribe tests** si es posible
5. **Compila y prueba**:
   ```bash
   make clean && make
   sudo ./bin/App
   ```
6. **Commit** con mensajes descriptivos:
   ```bash
   git commit -m "feat: agregar nueva funcionalidad X"
   ```
7. **Push** a tu rama:
   ```bash
   git push origin feature/nueva-funcionalidad
   ```
8. **Crea un Pull Request**

---

## Convenciones de Código

### Mensajes de Commit

Usar [Conventional Commits](https://www.conventionalcommits.org/):

```
feat: agregar soporte para múltiples ciudades
fix: corregir error en parsing JSON
docs: actualizar documentación de uso
chore: actualizar dependencias
refactor: refactorizar Device_t
test: agregar tests para Device_t
```

### Estilo de Código

- **Indentación**: 4 espacios
- **Línea máxima**: 100 caracteres
- **Nombres**: camelCase para variables, PascalCase para clases
- **Comentarios**: Documentar funciones públicas con Doxygen

### Ejemplo

```cpp
/**
 * @brief Obtiene la temperatura de la API
 * 
 * @param city Nombre de la ciudad
 * @return Temperatura en grados Celsius
 */
double getTemperature(const std::string& city);
```

---

## Estructura del Proyecto

```
src/            # Código fuente
include/        # Cabeceras
docs/           # Documentación
scripts/        # Scripts auxiliares
config/         # Configuración
```

---

## Tests

### Ejecutar tests

```bash
make test
```

### Escribir tests

Los tests van en `tests/`:

```cpp
#include "Device_t.hpp"
#include <cassert>

void test_getTemperature() {
    Device::Device_t device;
    double temp = device.getTemperature("Buenos Aires", "AR");
    assert(temp > -100 && temp < 100);
}

int main() {
    test_getTemperature();
    return 0;
}
```

---

## Code Review

Todos los Pull Requests pasarán por code review:

1. **Compilación**: Debe compilar sin errores
2. **Tests**: Todos los tests deben pasar
3. **Documentación**: Código debe estar documentado
4. **Estilo**: Seguir convenciones del proyecto

---

## Preguntas

Si tienes preguntas:

1. Revisa la documentación en `docs/`
2. Busca issues existentes
3. Crea un issue con la etiqueta `question`

---

## Reconocimiento

Los contribuidores serán reconocidos en:

- Archivo AUTHORS
- README.md
- Release notes

---

## Licencia

Al contribuir, aceptas que tus contribuciones serán licenciadas bajo
la misma licencia del proyecto (MIT).

---

## Enlaces

- [README](../README.md)
- [CHANGELOG](CHANGELOG.md)
- [TODO](TODO.md)
