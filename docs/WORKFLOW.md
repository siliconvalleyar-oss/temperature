# WORKFLOW - Flujo de Trabajo

## Flujo de Desarrollo

### 1. Configuración Inicial

```bash
# Clonar repositorio
git clone https://github.com/USUARIO/REPOSITORIO.git
cd REPOSITORIO

# Instalar dependencias
sudo ./scripts/install_deps.sh

# Compilar
make
```

### 2. Ciclo de Desarrollo

```
┌─────────────┐
│   Inicio    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Crear     │
│   Rama      │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Desarrollar│◀──────────────┐
└──────┬──────┘               │
       │                      │
       ▼                      │
┌─────────────┐               │
│   Compilar  │               │
│   y Testear │               │
└──────┬──────┘               │
       │                      │
       ├── Error ─────────────┘
       │
       ▼ (Éxito)
┌─────────────┐
│   Commit    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Push      │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Pull Request│
└──────┬──────┘
       │
       ▼
┌─────────────┐
│Code Review  │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Merge     │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   Deploy    │
└─────────────┘
```

---

## Comandos Frecuentes

### Desarrollo

```bash
# Crear rama de features
git checkout -b feature/nueva-funcionalidad

# Compilar
make

# Ejecutar
sudo ./bin/App

# Limpiar
make clean
```

### Git

```bash
# Ver estado
git status

# Ver cambios
git diff

# Agregar cambios
git add .

# Commit
git commit -m "feat: descripción"

# Push
git push origin feature/nueva-funcionalidad
```

### Mantenimiento

```bash
# Actualizar dependencias
sudo apt-get update && sudo apt-get upgrade

# Reinstalar dependencias del proyecto
sudo ./scripts/install_deps.sh

# Rebuild completo
make distclean && make
```

---

## Versionado

### Semántico

- **MAJOR**: Cambios incompatibles
- **MINOR**: Nuevas funcionalidades (compatibles)
- **PATCH**: Corrección de bugs

### Flujo de Versionado

1. Actualizar archivo `VERSION`
2. Actualizar `CHANGELOG.md`
3. Commit con mensaje `chore: bump version X.Y.Z`
4. Crear tag: `git tag vX.Y.Z`
5. Push con tags: `git push origin main --tags`

---

## Pull Requests

### Antes de Crear PR

1. Compilar sin errores: `make`
2. Ejecutar aplicación: `sudo ./bin/App`
3. Verificar que no hay warnings
4. Actualizar documentación si es necesario
5. Actualizar CHANGELOG.md

### Formato de PR

```
## Descripción
Breve descripción de los cambios

## Tipo de Cambio
- [ ] Bug fix
- [ ] Nueva funcionalidad
- [ ] Cambio de documentación
- [ ] Refactor
- [ ] Otro

## Checklist
- [ ] Código compila sin errores
- [ ] Tests pasan (si aplica)
- [ ] Documentación actualizada
- [ ] CHANGELOG actualizado
```

---

## Code Review

### Criterios de Revisión

1. **Correctitud**: ¿El código hace lo que se espera?
2. **Legibilidad**: ¿El código es fácil de entender?
3. **Mantenibilidad**: ¿Es fácil de modificar?
4. **Rendimiento**: ¿Es eficiente?
5. **Seguridad**: ¿Es seguro?

### Comentarios

- Ser constructivo y respetuoso
- Explicar por qué, no solo qué
- Sugerir alternativas cuando sea posible
- Agradecer el esfuerzo

---

## Testing

### Tipos de Tests

1. **Unit Tests**: Probar funciones individuales
2. **Integration Tests**: Probar componentes juntos
3. **System Tests**: Probar todo el sistema

### Ejecutar Tests

```bash
# Tests unitarios
make test-unit

# Tests de integración
make test-integration

# Todos los tests
make test
```

---

## Deploy

### Producción

1. Merge a `main`
2. Crear tag de versión
3. Push tag: `git push origin main --tags`
4. Ejecutar en Raspberry Pi:
   ```bash
   git pull origin main
   make clean && make
   sudo ./bin/App
   ```

### Desarrollo

1. Ejecutar directamente:
   ```bash
   make
   sudo ./bin/App
   ```

---

## Herramientas Recomendadas

### Editor

- VS Code con extensiones:
  - C/C++
  - CMake Tools
  - GitLens

### Terminal

- tmux para sesiones múltiples
- htop para monitoreo

### Debug

- gdb para debugging
- valgrind para memory leaks

---

## Comunicación

### Canales

- **Issues**: Bugs y features
- **Pull Requests**: Revisiones de código
- **Discussions**: Preguntas generales

### Etiquetas

- `bug`: Reporte de bug
- `enhancement`: Nueva funcionalidad
- `documentation`: Cambios en docs
- `good first issue`: Buenos issues para empezar
- `help wanted`: Se necesita ayuda

---

## Recursos

- [Conventional Commits](https://www.conventionalcommits.org/)
- [Git Flow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)
- [Code Review Best Practices](https://github.com/thoughtbot/guides/tree/main/code-review)
