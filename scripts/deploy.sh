#!/bin/bash
# =============================================================================
# Script de Despliegue Remoto para Raspberry Pi
# =============================================================================
# Este script clona, compila y ejecuta el proyecto en una Raspberry Pi remota.
# Requiere acceso SSH sin contraseña configurado.
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
    echo -e "${BLUE}  Despliegue Remoto - Raspberry Pi${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Configuración por defecto
RPI_USER="${RPI_USER:-joy}"
RPI_HOST="${RPI_HOST:-raspberry.local}"
RPI_SRC_DIR="${RPI_SRC_DIR:-/home/${RPI_USER}/src}"
REPO_URL="https://github.com/siliconvalleyar-oss/temperature.git"
PROJECT_NAME="temperature"

# Verificar argumentos
show_help() {
    echo "Uso: $0 [opciones]"
    echo ""
    echo "Opciones:"
    echo "  -h, --help         Mostrar esta ayuda"
    echo "  -u, --user USER    Usuario SSH (default: $RPI_USER)"
    echo "  -H, --host HOST    Hostname o IP (default: $RPI_HOST)"
    echo "  -d, --dir DIR      Directorio src en RPi (default: $RPI_SRC_DIR)"
    echo "  -c, --clone        Solo clonar repositorio"
    echo "  -b, --build        Solo compilar"
    echo "  -r, --run          Solo ejecutar"
    echo "  -a, --all          Clonar + Compilar + Ejecutar (default)"
    echo "  -s, --status       Ver estado del repositorio"
    echo "  -t, --test         Probar conexión SSH"
    echo ""
    echo "Ejemplos:"
    echo "  $0                          # Despliegue completo"
    echo "  $0 -H 192.168.1.100         # Usar IP específica"
    echo "  $0 -u pi -H 192.168.1.50    # Usuario y IP específicos"
    echo "  $0 --test                   # Probar conexión"
    echo ""
}

# Valores por defecto para acciones
DO_CLONE=false
DO_BUILD=false
DO_RUN=false
DO_ALL=true
DO_STATUS=false
DO_TEST=false

# Parsear argumentos
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -u|--user)
            RPI_USER="$2"
            shift 2
            ;;
        -H|--host)
            RPI_HOST="$2"
            shift 2
            ;;
        -d|--dir)
            RPI_SRC_DIR="$2"
            shift 2
            ;;
        -c|--clone)
            DO_CLONE=true
            DO_ALL=false
            shift
            ;;
        -b|--build)
            DO_BUILD=true
            DO_ALL=false
            shift
            ;;
        -r|--run)
            DO_RUN=true
            DO_ALL=false
            shift
            ;;
        -a|--all)
            DO_ALL=true
            shift
            ;;
        -s|--status)
            DO_STATUS=true
            DO_ALL=false
            shift
            ;;
        -t|--test)
            DO_TEST=true
            DO_ALL=false
            shift
            ;;
        *)
            print_error "Opción desconocida: $1"
            show_help
            exit 1
            ;;
    esac
done

# Comando SSH
SSH_CMD="ssh ${RPI_USER}@${RPI_HOST}"

print_header

echo ""
print_msg "Configuración:"
print_msg "  Usuario: $RPI_USER"
print_msg "  Host: $RPI_HOST"
print_msg "  Directorio: $RPI_SRC_DIR"
print_msg "  Repositorio: $REPO_URL"
echo ""

# =============================================================================
# Probar conexión SSH
# =============================================================================

test_ssh() {
    print_msg "Probando conexión SSH..."
    if $SSH_CMD "echo '✅ Conexión SSH exitosa'" 2>/dev/null; then
        return 0
    else
        print_error "No se pudo conectar a $RPI_USER@$RPI_HOST"
        echo ""
        echo "Verifique:"
        echo "  1. La Raspberry Pi está encendida y en la red"
        echo "  2. SSH keys están configuradas:"
        echo "     ssh-copy-id $RPI_USER@$RPI_HOST"
        echo "  3. El hostname/IP es correcto:"
        echo "     ping $RPI_HOST"
        return 1
    fi
}

# =============================================================================
# Clonar repositorio
# =============================================================================

clone_repo() {
    print_msg "Clonando repositorio en Raspberry Pi..."
    
    $SSH_CMD "mkdir -p $RPI_SRC_DIR"
    
    $SSH_CMD "cd $RPI_SRC_DIR && \
        if [ -d $PROJECT_NAME ]; then \
            echo '📦 Repositorio ya existe, actualizando...'; \
            cd $PROJECT_NAME && git pull; \
        else \
            echo '📦 Clonando repositorio...'; \
            git clone $REPO_URL; \
            cd $PROJECT_NAME; \
        fi"
    
    print_msg "✅ Repositorio clonado/actualizado"
}

# =============================================================================
# Compilar
# =============================================================================

build_project() {
    print_msg "Compilando en Raspberry Pi..."
    
    $SSH_CMD "cd $RPI_SRC_DIR/$PROJECT_NAME && \
        echo '🔧 Instalando dependencias...' && \
        sudo ./scripts/install_deps.sh && \
        echo '' && \
        echo '🔨 Compilando...' && \
        make clean && make -j4"
    
    print_msg "✅ Compilación exitosa"
}

# =============================================================================
# Ejecutar
# =============================================================================

run_project() {
    print_msg "Ejecutando en Raspberry Pi..."
    
    $SSH_CMD "cd $RPI_SRC_DIR/$PROJECT_NAME && \
        sudo ./bin/App"
}

# =============================================================================
# Ver estado
# =============================================================================

show_status() {
    print_msg "Estado del repositorio en Raspberry Pi..."
    
    $SSH_CMD "cd $RPI_SRC_DIR/$PROJECT_NAME && \
        echo '📂 Directorio:' && pwd && \
        echo '' && \
        echo '🌿 Ramas:' && git branch && \
        echo '' && \
        echo '📝 Último commit:' && git log --oneline -1 && \
        echo '' && \
        echo '📊 Estado:' && git status --short"
}

# =============================================================================
# Ejecutar acciones
# =============================================================================

if [ "$DO_TEST" = true ]; then
    test_ssh
elif [ "$DO_STATUS" = true ]; then
    test_ssh && show_status
elif [ "$DO_ALL" = true ]; then
    test_ssh && clone_repo && build_project
elif [ "$DO_CLONE" = true ]; then
    test_ssh && clone_repo
elif [ "$DO_BUILD" = true ]; then
    test_ssh && build_project
elif [ "$DO_RUN" = true ]; then
    test_ssh && run_project
fi

echo ""
print_msg "========================================"
print_msg "  Operación completada"
print_msg "========================================"
