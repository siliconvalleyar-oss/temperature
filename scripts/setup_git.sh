#!/bin/bash
# =============================================================================
# Script de configuración de repositorio Git
# =============================================================================
# Este script configura el repositorio Git de forma interactiva,
# incluyendo la creación del repositorio en GitHub/GitLab.
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
    echo -e "${BLUE}  Configuración de Repositorio Git${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Verificar si Git está instalado
if ! command -v git &> /dev/null; then
    print_error "Git no está instalado"
    print_msg "Instale Git con: sudo apt-get install git"
    exit 1
fi

print_header

# Preguntar datos al usuario
echo ""
read -p "Nombre de usuario en GitHub/GitLab: " GIT_USERNAME
read -p "Email: " GIT_EMAIL
read -p "Nombre del repositorio: " REPO_NAME
read -p "¿Es público? (s/n, default: s): " IS_PUBLIC

# Configurar valores por defecto
IS_PUBLIC=${IS_PUBLIC:-s}
if [ "$IS_PUBLIC" = "s" ] || [ "$IS_PUBLIC" = "S" ]; then
    REPO_VISIBILITY="public"
    GH_FLAG="--public"
else
    REPO_VISIBILITY="private"
    GH_FLAG="--private"
fi

echo ""
print_msg "Configurando repositorio..."
print_msg "  Usuario: $GIT_USERNAME"
print_msg "  Email: $GIT_EMAIL"
print_msg "  Repositorio: $REPO_NAME"
print_msg "  Visibilidad: $REPO_VISIBILITY"

# Configurar Git
print_msg "Configurando Git..."
git config --global user.name "$GIT_USERNAME"
git config --global user.email "$GIT_EMAIL"

# Verificar si ya hay un repositorio Git
if [ -d ".git" ]; then
    print_warn "Ya existe un repositorio Git en este directorio"
    read -p "¿Desea continuar? (s/n): " CONTINUE
    if [ "$CONTINUE" != "s" ] && [ "$CONTINUE" != "S" ]; then
        print_msg "Operación cancelada"
        exit 0
    fi
else
    # Inicializar repositorio
    print_msg "Inicializando repositorio Git..."
    git init
fi

# Agregar archivos
print_msg "Agregando archivos al repositorio..."
git add .

# Crear commit inicial
print_msg "Creando commit inicial..."
git commit -m "feat: initial commit - RaspberryPi Weather App v0.1.0"

# Verificar si gh está instalado
if command -v gh &> /dev/null; then
    print_msg "GitHub CLI (gh) detectado"
    read -p "¿Desea crear el repositorio en GitHub? (s/n): " CREATE_GH
    
    if [ "$CREATE_GH" = "s" ] || [ "$CREATE_GH" = "S" ]; then
        # Verificar autenticación
        if gh auth status &> /dev/null; then
            print_msg "Autenticado en GitHub"
            
            # Crear repositorio
            print_msg "Creando repositorio en GitHub..."
            gh repo create "$REPO_NAME" $GH_FLAG --source=. --remote=origin --push
            
            print_msg "¡Repositorio creado exitosamente!"
            print_msg "URL: https://github.com/$GIT_USERNAME/$REPO_NAME"
        else
            print_warn "No está autenticado en GitHub"
            print_msg "Ejecute: gh auth login"
            print_msg "Luego vuelva a ejecutar este script"
        fi
    else
        print_msg "Omitiendo creación en GitHub"
    fi
else
    print_warn "GitHub CLI (gh) no está instalado"
    echo ""
    print_msg "Para crear el repositorio manualmente:"
    echo "  1. Cree el repositorio en GitHub/GitLab"
    echo "  2. Ejecute:"
    echo "     git remote add origin https://github.com/$GIT_USERNAME/$REPO_NAME.git"
    echo "     git push -u origin main"
    echo ""
fi

print_msg "========================================"
print_msg "  ¡Configuración completada!"
print_msg "========================================"
print_msg ""
print_msg "Próximos pasos:"
print_msg "  1. Compile el proyecto: make"
print_msg "  2. Ejecute la aplicación: sudo ./bin/App"
print_msg ""
