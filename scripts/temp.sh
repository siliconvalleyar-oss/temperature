#!/bin/bash

# Reemplaza con tu clave de API de OpenWeatherMap
API_KEY="b77f8502eb10bb93c41913239e281ef3"

# Verifica que el usuario haya proporcionado una ciudad como argumento
if [ "$#" -ne 1 ]; then
    echo "Uso: $0 <nombre_de_la_ciudad>"
    exit 1
fi

CITY="$1"
COUNTRY="AR" # Puedes cambiar esto si necesitas especificar otro país
URL="http://api.openweathermap.org/data/2.5/weather?q=${CITY},${COUNTRY}&appid=${API_KEY}&units=metric"

# Realiza la solicitud a la API
RESPONSE=$(curl -s "${URL}")

# Verifica si la respuesta contiene un mensaje de error
if echo "$RESPONSE" | jq -e '.cod != 200' > /dev/null; then
    echo "Error al obtener los datos: $(echo "$RESPONSE" | jq -r '.message')"
    exit 1
fi

# Extrae la temperatura de la respuesta
TEMPERATURE=$(echo "$RESPONSE" | jq -r '.main.temp')

# Imprime la temperatura
echo "La temperatura actual en ${CITY} es ${TEMPERATURE} °C"
