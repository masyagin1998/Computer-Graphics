
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <stdio.h>

#include <string>
#include <fstream>
#include <sstream>

class Shader {
 public:
    // Конструктор считывает и собирает шейдер
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

    // Использование программы.
    void Use();

    // Идентификатор программы
    GLuint program;

 private:
    // Считывание файла.
    std::string ReadFile(const GLchar *file_name);
};

#endif
