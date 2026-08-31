# DEPLOY - Guía de Despliegue Remoto

## Visión General

Este documento describe cómo desplegar y compilar el proyecto de forma remota en una Raspberry Pi.

---

## Requisitos

### Local (PC de desarrollo)

- Git
- SSH client
- Acco a internet

### Remoto (Raspberry Pi)

- Raspbian/Raspberry Pi OS
- SSH habilitado
- Acceso a internet
- GCC/G++ instalado

---

## Configuración Inicial

### 1. Configurar SSH sin contraseña

Desde tu PC, ejecuta:

```bash
# Generar clave SSH (si no tienes una)
ssh-keygen -t rsa -b 4096

# Copiar clave a la Raspberry Pi
ssh-copy-id joy@raspberry.local
```

**Nota:** Si usas una IP en lugar de hostname:

```bash
ssh-copy-id joy@192.168.1.100
```

### 2. Verificar conexión

```bash
# Probar conexión sin contraseña
ssh joy@raspberry.local "echo '✅ Conexión exitosa'"

# O usar el Makefile
make ssh-test
```

---

## Despliegue con Make

### Comandos disponibles

```bash
# Ver ayuda completa
make help

# Despliegue completo (clonar + compilar)
make deploy-all

# Probar conexión SSH
make ssh-test

# Clonar repositorio en RPi
make deploy-clone

# Compilar en RPi
make deploy-build

# Ejecutar en RPi
make deploy-run

# Ver estado en RPi
make deploy-status

# Conectar a RPi
make ssh
```

### Configurar host/usuario

```bash
# Usar IP específica
make deploy-all RPI_HOST=192.168.1.100

# Usar usuario diferente
make deploy-all RPI_USER=pi

# Combinar
make deploy-all RPI_USER=pi RPI_HOST=192.168.1.50
```

---

## Despliegue con Script

### Uso básico

```bash
# Despliegue completo
./scripts/deploy.sh

# Ver ayuda
./scripts/deploy.sh --help
```

### Opciones del script

```bash
# Probar conexión
./scripts/deploy.sh --test

# Solo clonar
./scripts/deploy.sh --clone

# Solo compilar
./scripts/deploy.sh --build

# Solo ejecutar
./scripts/deploy.sh --run

# Ver estado
./scripts/deploy.sh --status
```

### Configurar host/usuario

```bash
# IP específica
./scripts/deploy.sh --host 192.168.1.100

# Usuario específico
./scripts/deploy.sh --user pi --host 192.168.1.50

# Usando variables de entorno
RPI_USER=pi RPI_HOST=192.168.1.50 ./scripts/deploy.sh
```

---

## Flujo de Trabajo Completo

### Desde cero

```bash
# 1. Clonar repositorio local
git clone https://github.com/siliconvalleyar-oss/temperature.git
cd temperature

# 2. Configurar SSH (una vez)
ssh-keygen -t rsa -b 4096
ssh-copy-id joy@raspberry.local

# 3. Desplegar en RPi
make deploy-all

# 4. Conectar para verificar
make ssh
cd ~/src/temperature
sudo ./bin/App
```

### Actualizaciones

```bash
# En local: hacer cambios
git add .
git commit -m "feat: nueva funcionalidad"
git push

# En RPi: actualizar
make deploy-build

# O ejecutar todo junto
make deploy-all
```

---

## Comando SSH Directo

### Formato del usuario

```
ssh joy@raspberry.local "cd /home/joy/src && git clone temperature && cd temperature && git pull && make clean && make -j4 && sudo ./bin/App"
```

### Desglose

| Paso | Comando |
|------|---------|
| Conectar | `ssh joy@raspberry.local` |
| Ir a src | `cd /home/joy/src` |
| Clonar | `git clone temperature` |
| Entrar al proyecto | `cd temperature` |
| Actualizar | `git pull` |
| Limpiar | `make clean` |
| Compilar | `make -j4` |
| Ejecutar | `sudo ./bin/App` |

---

## Solución de Problemas

### Error: "Permission denied (publickey)"

**Solución:**
```bash
# Copiar clave SSH
ssh-copy-id joy@raspberry.local

# Verificar permisos
chmod 700 ~/.ssh
chmod 600 ~/.ssh/id_rsa
```

### Error: "Host key verification failed"

**Solución:**
```bash
# Agregar host a known_hosts
ssh-keyscan -H raspberry.local >> ~/.ssh/known_hosts

# O conectar una vez manualmente
ssh joy@raspberry.local
```

### Error: "Could not resolve hostname"

**Solución:**
```bash
# Verificar hostname
ping raspberry.local

# Usar IP en lugar de hostname
ping 192.168.1.100

# Configurar en /etc/hosts
echo "192.168.1.100 raspberry.local" | sudo tee -a /etc/hosts
```

### Error: "make: *** No targets specified"

**Solución:**
```bash
# Verificar que el repositorio existe
ssh joy@raspberry.local "ls -la ~/src/temperature"

# Si no existe, clonar primero
make deploy-clone
```

---

## Scripts Útiles

### SSH Config (~/.ssh/config)

```
Host raspberry
    HostName raspberry.local
    User joy
    IdentityFile ~/.ssh/id_rsa

Host raspberry-ip
    HostName 192.168.1.100
    User joy
    IdentityFile ~/.ssh/id_rsa
```

Con esto puedes usar:
```bash
ssh raspberry
make deploy-all RPI_HOST=raspberry
```

### Alias para shell (~/.bashrc)

```bash
# Desplegar en RPi
alias deploy-rpi='make deploy-all RPI_HOST=raspberry.local'

# Conectar a RPi
alias rpi='ssh joy@raspberry.local'

# Ver estado RPi
alias rpi-status='make deploy-status'
```

---

## Seguridad

### Recomendaciones

1. **Usar SSH keys** en lugar de contraseñas
2. **Restringir acceso** por IP en la RPi
3. **No exponer** la RPi a internet
4. **Usar VPN** si es necesario acceder remotamente

### Configurar firewall en RPi

```bash
# Instalar ufw
sudo apt-get install ufw

# Permitir solo SSH
sudo ufw allow ssh

# Habilitar
sudo ufw enable
```

---

## Enlaces

- [Guía de Uso](USAGE.md)
- [Compilación](BUILD.md)
- [Hardware](HARDWARE.md)
- [Solución de Problemas](TROUBLESHOOTING.md)
