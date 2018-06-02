#!/bin/bash
time g++ -g -Wall -Wextra -std=c++14 -O3 src/main.cpp src/spring/spring.cpp src/box/box.cpp src/json_config/json_config.cpp src/utils/utils.cpp src/texture/texture.cpp src/light/light.cpp -o bin/main -lGL -lglfw -lGLU -lGLEW -lSOIL

