cmake_minimum_required(VERSION 3.10)

project(WeatherProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS} include)

add_executable(weather_project main.cc)

target_link_libraries(weather_project PRIVATE ${CURL_LIBRARIES})

# Incluir nlohmann/json (suponiendo que json.hpp está en include/nlohmann/)
target_include_directories(weather_project PRIVATE include)
