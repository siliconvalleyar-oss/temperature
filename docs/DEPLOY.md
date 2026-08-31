# DEPLOY - Guía de Despliegue

## Visión General

Este documento describe cómo desplegar el proyecto RaspberryPi Weather App
en producción.

---

## Requisitos

### Hardware

- Raspberry Pi (32 o 64 bits)
- Tarjeta microSD (mínimo 8GB)
- Fuente de poder (5V 3A)
- Conexión a internet

### Software

- Raspbian/Raspberry Pi OS
- Dependencias instaladas

---

## Despliegue Manual

### 1. Preparar Raspberry Pi

```bash
# Actualizar sistema
sudo apt-get update && sudo apt-get upgrade -y

# Habilitar I2C
sudo raspi-config
# Interfacing Options → I2C → Enable

# Reiniciar
sudo reboot
```

### 2. Instalar Aplicación

```bash
# Clonar repositorio
git clone https://github.com/USUARIO/REPOSITORIO.git
cd REPOSITORIO

# Instalar dependencias
sudo ./scripts/install_deps.sh

# Compilar
make

# Instalar en /usr/local/bin
sudo make install
```

### 3. Configurar

```bash
# Editar configuración
sudo nano /usr/local/bin/config/config.cfg

# Configurar API key y ciudad
[weather]
api_key = TU_API_KEY
city = Buenos Aires
country = AR
```

### 4. Ejecutar

```bash
# Ejecutar manualmente
sudo /usr/local/bin/App

# O con systemd (ver abajo)
```

---

## Despliegue con systemd

### Crear Servicio

```bash
sudo nano /etc/systemd/system/weather.service
```

Contenido:
```ini
[Unit]
Description=RaspberryPi Weather App
After=network.target

[Service]
Type=simple
User=root
WorkingDirectory=/usr/local/bin
ExecStart=/usr/local/bin/App
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

### Habilitar Servicio

```bash
# Recargar systemd
sudo systemctl daemon-reload

# Habilitar inicio automático
sudo systemctl enable weather.service

# Iniciar servicio
sudo systemctl start weather.service

# Ver estado
sudo systemctl status weather.service
```

### Comandos Útiles

```bash
# Ver logs
sudo journalctl -u weather.service -f

# Detener servicio
sudo systemctl stop weather.service

# Reiniciar servicio
sudo systemctl restart weather.service

# Deshabilitar servicio
sudo systemctl disable weather.service
```

---

## Despliegue Remoto

### Usando SSH

```bash
# Copiar archivos a Raspberry Pi
scp -r ./bin/App pi@raspberrypi:/home/pi/

# Conectar
ssh pi@raspberrypi

# Ejecutar
sudo ./App
```

### Usando Ansible

Crear playbook:
```yaml
---
- hosts: raspberrypi
  become: yes
  tasks:
    - name: Instalar dependencias
      script: scripts/install_deps.sh
    
    - name: Copiar binario
      copy:
        src: bin/App
        dest: /usr/local/bin/App
        mode: '0755'
    
    - name: Copiar configuración
      copy:
        src: config/config.cfg
        dest: /usr/local/bin/config/config.cfg
        mode: '0600'
    
    - name: Iniciar servicio
      systemd:
        name: weather
        state: started
        enabled: yes
```

Ejecutar:
```bash
ansible-playbook -i inventory deploy.yml
```

---

## Despliegue con Docker (Experimental)

### Dockerfile

```dockerfile
FROM arm32v7/debian:bullseye

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libbcm2835-dev \
    libcurl4-openssl-dev \
    nlohmann-json3-dev

WORKDIR /app
COPY . .

RUN make

CMD ["./bin/App"]
```

### Construir y Ejecutar

```bash
# Construir imagen
docker build -t weather-app .

# Ejecutar
docker run --privileged weather-app
```

**Nota**: Docker en Raspberry Pi requiere `--privileged` para acceso a GPIO.

---

## Monitoreo

### Ver Logs

```bash
# Logs de systemd
sudo journalctl -u weather.service -f

# Logs de la aplicación
tail -f /var/log/weather.log

# Logs del sistema
tail -f /var/log/syslog
```

### Monitoreo de Recursos

```bash
# Ver uso de CPU y memoria
htop

# Ver uso de memoria
free -h

# Ver procesos
ps aux | grep App
```

### Monitoreo de Red

```bash
# Ver conexiones activas
netstat -tuln

# Ver tráfico
iftop
```

---

## Actualizaciones

### Actualizar Aplicación

```bash
# Detener servicio
sudo systemctl stop weather.service

# Actualizar código
cd /usr/local/bin
git pull origin main

# Recompilar
make clean && make

# Reiniciar servicio
sudo systemctl start weather.service
```

### Actualizar Dependencias

```bash
# Actualizar sistema
sudo apt-get update && sudo apt-get upgrade -y

# Reinstalar dependencias del proyecto
sudo ./scripts/install_deps.sh

# Recompilar
make clean && make
```

---

## Backup

### Respaldar Configuración

```bash
# Respaldar configuración
tar -czvf backup_config_$(date +%Y%m%d).tar.gz config/

# Respaldar binario
cp bin/App bin/App.backup
```

### Restaurar Configuración

```bash
# Restaurar configuración
tar -xzvf backup_config_20260830.tar.gz

# Reiniciar servicio
sudo systemctl restart weather.service
```

---

## Rollback

### Revertir a Versión Anterior

```bash
# Detener servicio
sudo systemctl stop weather.service

# Restaurar binario anterior
cp bin/App.backup bin/App

# Reiniciar servicio
sudo systemctl start weather.service
```

### Revertir Cambios de Código

```bash
# Ver historial
git log --oneline

# Revertir a commit específico
git revert <commit-hash>

# Recompilar
make clean && make

# Reiniciar servicio
sudo systemctl restart weather.service
```

---

## Troubleshooting

### Servicio no inicia

```bash
# Ver logs de error
sudo journalctl -u weather.service -n 50

# Verificar estado
sudo systemctl status weather.service

# Probar ejecución manual
sudo /usr/local/bin/App
```

### No hay conexión a API

```bash
# Verificar red
ping google.com

# Probar API directamente
curl "http://api.openweathermap.org/data/2.5/weather?q=Buenos Aires,AR&appid=TU_API_KEY"

# Verificar DNS
nslookup api.openweathermap.org
```

### GPIO no funciona

```bash
# Verificar permisos
ls -l /dev/gpiomem

# Verificar I2C
sudo i2cdetect -y 1

# Verificar módulos
lsmod | grep i2c_bcm2835
```

---

## Seguridad

### Permisos

```bash
# Configuración protegida
chmod 600 /usr/local/bin/config/config.cfg

# Binario ejecutable solo por root
chmod 700 /usr/local/bin/App
```

### Firewall

```bash
# Instalar ufw
sudo apt-get install ufw

# Permitir solo SSH
sudo ufw allow ssh

# Habilitar firewall
sudo ufw enable
```

---

## Enlaces

- [Guía de Uso](USAGE.md)
- [Compilación](BUILD.md)
- [Solución de Problemas](TROUBLESHOOTING.md)
