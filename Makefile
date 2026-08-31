# =============================================================================
# Makefile para Proyecto RaspberryPi Weather App - Branch Raspberry
# =============================================================================
# Este Makefile está diseñado específicamente para Raspberry Pi.
# Siempre compila con soporte OLED (usando librería del sistema) y bcm2835.
# =============================================================================

# Leer versión del archivo VERSION
VERSION := $(shell cat VERSION 2>/dev/null || echo "0.1.0")

# Compilador
CXX = g++
CC = gcc

# Flags de compilación (C++14 para make_unique)
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
CFLAGS = -Wall -Wextra -O2

# Flags para defines - Siempre habilitar OLED y BCM2835
DEFINES = -DVERSION=\"$(VERSION)\" -DHAS_OLED -DHAS_BCM2835

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
DOCDIR = docs

# Archivos fuente - NO incluir OLED local (usar librería del sistema)
SOURCES = $(SRCDIR)/main.cpp \
          $(SRCDIR)/Device_t.cpp

# Archivos objeto (manteniendo jerarquía)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Archivo binario final
TARGET = $(BINDIR)/App

# Librerías - bcm2835, libcurl y SSD1306 del sistema
LIBS = -lbcm2835 -lcurl -lpthread -lSSD1306_OLED_RPI

# Include paths - usar librería del sistema
INCLUDES = -I$(INCDIR)

# =============================================================================
# Objetivos
# =============================================================================

.PHONY: all clean distclean install help info

# Objetivo principal
all: $(TARGET)
	@echo ""
	@echo "========================================"
	@echo "  Compilación exitosa!"
	@echo "  Versión: $(VERSION)"
	@echo "  Binario: $(TARGET)"
	@echo "  OLED: Habilitado (SSD1306 del sistema)"
	@echo "  Hardware: Raspberry Pi (bcm2835)"
	@echo "========================================"
	@echo ""

# Crear directorio bin si no existe
$(BINDIR):
	mkdir -p $(BINDIR)

# Crear directorio obj si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compilar archivos fuente
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
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
# Ejecutar
# =============================================================================

run: $(TARGET)
	@echo "Ejecutando aplicación..."
	sudo ./$(TARGET)

# =============================================================================
# Ayuda
# =============================================================================

help:
	@echo "========================================"
	@echo "  Makefile - RaspberryPi Weather App"
	@echo "  Branch: raspberry"
	@echo "========================================"
	@echo ""
	@echo "Uso: make [objetivo]"
	@echo ""
	@echo "Objetivos:"
	@echo "  all        - Compilar el proyecto (por defecto)"
	@echo "  clean      - Limpiar archivos objeto"
	@echo "  distclean  - Limpiar todo (objetos + binario)"
	@echo "  install    - Instalar en /usr/local/bin"
	@echo "  run        - Ejecutar la aplicación"
	@echo "  help       - Mostrar esta ayuda"
	@echo "  info       - Mostrar información del sistema"
	@echo ""
	@echo "Esta rama está diseñada para Raspberry Pi."
	@echo "Usa la librería SSD1306 del sistema."
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
	@echo "Rama: raspberry"
	@echo "OLED: Habilitado (librería del sistema)"
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
	@echo "  - bcm2835 (librería GPIO)"
	@echo "  - libcurl (cliente HTTP)"
	@echo "  - nlohmann/json (parser JSON)"
	@echo "  - SSD1306 OLED (librería del sistema)"
	@echo ""
