#!/bin/bash
time g++ -g -Wall -Wextra -std=c++14 -O3 src/main.cpp src/shaders/shader.cpp -o bin/main -lGL -lglfw -lGLU -lGLEW -lSOIL

