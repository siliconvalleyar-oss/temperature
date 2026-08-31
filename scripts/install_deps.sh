#!/bin/bash
# =============================================================================
# Script de instalación de dependencias para RaspberryPi Weather App
# =============================================================================
# Este script instala todas las dependencias necesarias para compilar
# el proyecto. Detecta si es Raspberry Pi o PC y instala lo correspondiente.
# =============================================================================

set -e  # Salir si hay errores

# Colores para mensajes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para imprimir mensajes
print_msg() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  Instalación de Dependencias${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Verificar si se ejecuta como root
if [ "$EUID" -ne 0 ]; then
    print_error "Este script debe ejecutarse como root (sudo)"
    exit 1
fi

print_header

# =============================================================================
# Detectar plataforma
# =============================================================================

ARCH=$(uname -m)
IS_RASPBERRY_PI=false

if grep -q "Raspberry Pi" /proc/cpuinfo 2>/dev/null; then
    IS_RASPBERRY_PI=true
elif grep -q "BCM" /proc/cpuinfo 2>/dev/null; then
    IS_RASPBERRY_PI=true
elif [ -f /sys/firmware/devicetree/base/model ] && grep -q "Raspberry Pi" /sys/firmware/devicetree/base/model 2>/dev/null; then
    IS_RASPBERRY_PI=true
fi

echo ""
print_msg "Plataforma detectada:"
print_msg "  Arquitectura: $ARCH"
if [ "$IS_RASPBERRY_PI" = true ]; then
    print_msg "  Tipo: Raspberry Pi"
else
    print_msg "  Tipo: PC (no Raspberry Pi)"
fi
echo ""

# =============================================================================
# Actualizar repositorios
# =============================================================================

print_msg "Actualizando repositorios..."
apt-get update

# =============================================================================
# Instalar herramientas de compilación (siempre necesarias)
# =============================================================================

print_msg "Instalando compilador y herramientas..."
apt-get install -y \
    g++ \
    gcc \
    make \
    cmake \
    git \
    curl \
    pkg-config

# =============================================================================
# Instalar dependencias siempre necesarias
# =============================================================================

print_msg "Instalando libcurl..."
apt-get install -y \
    libcurl4-openssl-dev

print_msg "Instalando nlohmann/json..."
apt-get install -y \
    nlohmann-json3-dev

# =============================================================================
# Instalar bcm2835 (solo en Raspberry Pi)
# =============================================================================

if [ "$IS_RASPBERRY_PI" = true ]; then
    print_msg "Detectado Raspberry Pi - Instalando bcm2835..."
    
    # Verificar si bcm2835 ya está instalado
    if ! ldconfig -p | grep -q libbcm2835; then
        print_msg "bcm2835 no encontrado, compilando desde fuente..."
        
        # Crear directorio temporal
        TEMP_DIR=$(mktemp -d)
        cd "$TEMP_DIR"
        
        # Descargar bcm2835
        print_msg "Descargando bcm2835..."
        curl -sL http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz -o bcm2835.tar.gz
        
        # Extraer
        tar -xzf bcm2835.tar.gz
        cd bcm2835-1.71
        
        # Compilar e instalar
        ./configure
        make
        make check
        make install
        
        # Limpiar
        cd /
        rm -rf "$TEMP_DIR"
        
        print_msg "bcm2835 instalado correctamente"
    else
        print_msg "bcm2835 ya está instalado"
    fi
else
    print_msg "No es Raspberry Pi - Omitiendo bcm2835"
    print_msg "El proyecto se compilará en modo PC (sin soporte OLED)"
fi

# =============================================================================
# Instalar dependencias adicionales
# =============================================================================

print_msg "Instalando dependencias adicionales..."
apt-get install -y \
    libssl-dev \
    ca-certificates

# =============================================================================
# Verificar instalación
# =============================================================================

print_msg "Verificando instalación..."

# Verificar gcc
if command -v g++ &> /dev/null; then
    GXX_VERSION=$(g++ --version | head -n1)
    print_msg "g++ instalado: $GXX_VERSION"
else
    print_error "g++ no encontrado"
    exit 1
fi

# Verificar make
if command -v make &> /dev/null; then
    MAKE_VERSION=$(make --version | head -n1)
    print_msg "make instalado: $MAKE_VERSION"
else
    print_error "make no encontrado"
    exit 1
fi

# Verificar libcurl
if ldconfig -p | grep -q libcurl; then
    print_msg "libcurl instalada correctamente"
else
    print_error "libcurl no encontrada"
    exit 1
fi

# Verificar bcm2835 (solo en RPi)
if [ "$IS_RASPBERRY_PI" = true ]; then
    if ldconfig -p | grep -q libbcm2835; then
        print_msg "bcm2835 instalado correctamente"
    else
        print_error "bcm2835 no encontrado"
        exit 1
    fi
fi

# =============================================================================
# Resumen
# =============================================================================

echo ""
print_msg "========================================"
print_msg "  ¡Todas las dependencias instaladas!"
print_msg "========================================"
echo ""

if [ "$IS_RASPBERRY_PI" = true ]; then
    print_msg "Plataforma: Raspberry Pi"
    print_msg "Compilación recomendada:"
    echo "  make OLED=yes    # Con soporte OLED"
    echo "  make OLED=no     # Sin soporte OLED"
else
    print_msg "Plataforma: PC"
    print_msg "Compilación recomendada:"
    echo "  make OLED=no     # Sin soporte OLED"
    echo ""
    print_warn "El soporte OLED no está disponible en PC"
fi

echo ""
print_msg "Otros comandos:"
echo "  make              # Compilar (detecta automáticamente)"
echo "  make help         # Ver ayuda completa"
echo "  make info         # Ver información del sistema"
echo ""
