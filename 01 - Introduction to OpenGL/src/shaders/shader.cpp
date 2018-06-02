
#include "shader.hpp"

#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const GLchar *vertex_path, const GLchar *fragment_path) {
    // Загрузка шейдеров;
    std::string vertex_string = ReadFile(vertex_path);
    std::string fragment_string = ReadFile(fragment_path);
    const GLchar* vertex_shader = vertex_string.c_str();
    const GLchar * fragment_shader = fragment_string.c_str();

    // Компиляция шейдеров;
    GLint success;
    GLchar log[512];

    // Вершинный шейдер.
    GLuint vertex; vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (! success) {
        glGetShaderInfoLog(vertex, 512, nullptr, log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", log);
    }
    
    // Фрагментный шейдер.
    GLuint fragment; fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (! success) {
        glGetShaderInfoLog(fragment, 512, nullptr, log);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", log);
    }
    
    // Шейдерная программа.
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (! success) {
        glGetProgramInfoLog(program, 512, nullptr, log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", log);
    }

    // Теперь можно и удалить шейдеры.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(program);
}

std::string Shader::ReadFile(const GLchar *file_name) {
    std::ifstream file;
    
    file.exceptions (std::ifstream::badbit);
    try {
        file.open(file_name);
        std::stringstream file_stream;
        file_stream << file.rdbuf();
        file.close();
        return file_stream.str();
    } catch (std::ifstream::failure e) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
        return "";
    }
}

