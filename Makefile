# =============================================================================
# Makefile para Proyecto RaspberryPi Weather App
# =============================================================================
# Este Makefile detecta si se ejecuta en Raspberry Pi y ofrece la opción
# de compilar con/sin soporte OLED.
# =============================================================================

# Leer versión del archivo VERSION
VERSION := $(shell cat VERSION 2>/dev/null || echo "0.1.0")

# Compilador
CXX = g++
CC = gcc

# Flags de compilación
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
CFLAGS = -Wall -Wextra -O2

# =============================================================================
# Detección de Raspberry Pi
# =============================================================================

# Detectar si es Raspberry Pi leyendo /proc/cpuinfo o /sys/firmware
IS_RASPBERRY_PI := $(shell \
    if grep -q "Raspberry Pi" /proc/cpuinfo 2>/dev/null; then \
        echo "yes"; \
    elif grep -q "BCM" /proc/cpuinfo 2>/dev/null; then \
        echo "yes"; \
    elif [ -f /sys/firmware/devicetree/base/model ] && grep -q "Raspberry Pi" /sys/firmware/devicetree/base/model 2>/dev/null; then \
        echo "yes"; \
    else \
        echo "no"; \
    fi \
)

# Arquitectura del sistema
ARCH := $(shell uname -m)

# =============================================================================
# Opción OLED (solo disponible en Raspberry Pi)
# =============================================================================

# Por defecto: OLED habilitado si es Raspberry Pi, deshabilitado si no
ifeq ($(IS_RASPBERRY_PI),yes)
    OLED_DEFAULT ?= yes
else
    OLED_DEFAULT ?= no
endif

# Permitir override desde línea de comandos: make OLED=yes o make OLED=no
OLED ?= $(OLED_DEFAULT)

# Validar opción OLED
ifneq ($(OLED),yes)
    ifneq ($(OLED),no)
        $(error Opción OLED inválida. Use OLED=yes o OLED=no)
    endif
endif

# =============================================================================
# Configuración según opción OLED
# =============================================================================

# Flags para defines
DEFINES = -DVERSION=\"$(VERSION)\"

# Librerías base (siempre necesarias)
LIBS = -lcurl -lpthread

ifeq ($(OLED),yes)
    # Modo Raspberry Pi con OLED
    DEFINES += -DHAS_OLED -DHAS_BCM2835
    LIBS += -lbcm2835
    OLED_SOURCES = $(SRCDIR)/oled/SSD1306_OLED.cpp \
                   $(SRCDIR)/oled/SSD1306_OLED_font.cpp \
                   $(SRCDIR)/oled/SSD1306_OLED_graphics.cpp \
                   $(SRCDIR)/oled/SSD1306_OLED_Print.cpp
    OLED_OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(OLED_SOURCES))
    OLED_MSG = "OLED: Habilitado (SSD1306)"
else
    # Modo PC sin OLED
    OLED_SOURCES =
    OLED_OBJECTS =
    OLED_MSG = "OLED: Deshabilitado"
endif

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DOCDIR = docs

# Archivos fuente base (siempre se compilan)
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/Device_t.cpp \
          $(SRCDIR)/HMC5883L.cpp \
          $(OLED_SOURCES)

# Archivos objeto (manteniendo jerarquía)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Archivo binario final
TARGET = $(BINDIR)/App

# Include paths
INCLUDES = -I$(INCDIR)

# =============================================================================
# Objetivos
# =============================================================================

.PHONY: all clean distclean install help info yes no

# Objetivo principal
all: $(TARGET)
	@echo ""
	@echo "========================================"
	@echo "  Compilación exitosa!"
	@echo "  Versión: $(VERSION)"
	@echo "  Binario: $(TARGET)"
	@echo "  Plataforma: $(ARCH)"
	@echo "  Raspberry Pi: $(IS_RASPBERRY_PI)"
	@echo "  $(OLED_MSG)"
	@echo "========================================"
	@echo ""

# Crear directorio bin si no existe
$(BINDIR):
	mkdir -p $(BINDIR)

# Crear directorio obj si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Crear subdirectorios de obj manteniendo jerarquía
$(OBJDIR)/oled:
	mkdir -p $(OBJDIR)/oled

# Compilar archivos fuente
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) $(OBJDIR)/oled
	@echo "Compilando: $<"
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

# Enlazar binario final
$(TARGET): $(OBJECTS) | $(BINDIR)
	@echo "Enlazando: $@"
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@
	@echo "Binario generado: $@"

# =============================================================================
# Objetivos de limpieza
# =============================================================================

# Limpiar archivos objeto
clean:
	@echo "Limpiando archivos objeto..."
	rm -rf $(OBJDIR)
	@echo "Limpieza completada."

# Limpiar todo (objetos + binario)
distclean: clean
	@echo "Limpiando binario..."
	rm -rf $(BINDIR)
	@echo "Limpieza completa."

# =============================================================================
# Instalación (opcional)
# =============================================================================

install: $(TARGET)
	@echo "Instalando en /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Instalación completada."

# =============================================================================
# Atajos de compilación
# =============================================================================

# Compilar con OLED (solo Raspberry Pi)
oled: clean
	@echo "Compilando con soporte OLED..."
	$(MAKE) OLED=yes

# Compilar sin OLED (PC o RPi sin OLED)
noled: clean
	@echo "Compilando sin soporte OLED..."
	$(MAKE) OLED=no

# =============================================================================
# Ayuda
# =============================================================================

help:
	@echo "========================================"
	@echo "  Makefile - RaspberryPi Weather App"
	@echo "========================================"
	@echo ""
	@echo "Uso: make [objetivo] [OLED=yes|no]"
	@echo ""
	@echo "Objetivos:"
	@echo "  all        - Compilar el proyecto (por defecto)"
	@echo "  clean      - Limpiar archivos objeto"
	@echo "  distclean  - Limpiar todo (objetos + binario)"
	@echo "  install    - Instalar en /usr/local/bin"
	@echo "  oled       - Compilar CON soporte OLED"
	@echo "  noled      - Compilar SIN soporte OLED"
	@echo "  help       - Mostrar esta ayuda"
	@echo "  info       - Mostrar información del sistema"
	@echo ""
	@echo "Opciones:"
	@echo "  OLED=yes   - Habilitar soporte OLED (requiere bcm2835)"
	@echo "  OLED=no    - Deshabilitar soporte OLED"
	@echo ""
	@echo "Ejemplos:"
	@echo "  make                    # Compilar (detecta automáticamente)"
	@echo "  make OLED=yes           # Forzar OLED habilitado"
	@echo "  make OLED=no            # Forzar OLED deshabilitado"
	@echo "  make oled               # Limpiar y compilar con OLED"
	@echo "  make noled              # Limpiar y compilar sin OLED"
	@echo ""
	@echo "Plataforma detectada:"
	@echo "  Arquitectura: $(ARCH)"
	@echo "  Raspberry Pi: $(IS_RASPBERRY_PI)"
	@echo "  OLED por defecto: $(OLED_DEFAULT)"
	@echo ""
	@echo "Versión: $(VERSION)"
	@echo ""

# =============================================================================
# Información del sistema
# =============================================================================

info:
	@echo "========================================"
	@echo "  Información del Proyecto"
	@echo "========================================"
	@echo "Versión: $(VERSION)"
	@echo "Compilador: $(CXX)"
	@echo "Arquitectura: $(ARCH)"
	@echo "Raspberry Pi: $(IS_RASPBERRY_PI)"
	@echo "OLED habilitado: $(OLED)"
	@echo ""
	@echo "Flags:"
	@echo "  CXXFLAGS: $(CXXFLAGS)"
	@echo "  DEFINES: $(DEFINES)"
	@echo "  LIBS: $(LIBS)"
	@echo ""
	@echo "Archivos:"
	@echo "  Fuentes: $(words $(SOURCES)) archivos"
	@echo "  Objetos: $(words $(OBJECTS)) archivos"
	@echo "  Binario: $(TARGET)"
	@echo ""
	@echo "Dependencias:"
ifeq ($(OLED),yes)
	@echo "  - bcm2835 (librería GPIO)"
	@echo "  - libcurl (cliente HTTP)"
	@echo "  - nlohmann/json (parser JSON)"
	@echo "  - SSD1306 OLED driver"
else
	@echo "  - libcurl (cliente HTTP)"
	@echo "  - nlohmann/json (parser JSON)"
	@echo "  (bcm2835 y OLED deshabilitados)"
	@echo ""
	@echo "  NOTA: Algunas funciones de hardware estarán"
	@echo "  deshabilitadas en modo PC."
endif
	@echo ""
