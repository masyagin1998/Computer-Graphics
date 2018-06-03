#!/bin/bash
time g++ -g -Wall -Wextra -std=c++14 -O3 src/main.cpp src/shaders/shader.cpp src/utils/utils.cpp src/cube/cube.cpp src/spring/spring.cpp -o bin/main -lGL -lglfw -lGLU -lGLEW -lSOIL

