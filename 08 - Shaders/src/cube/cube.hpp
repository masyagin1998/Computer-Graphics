#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Cube {
    // Конструктор.
    Cube(GLuint VAO, GLuint VBO);
    // Конструктор по умолчанию.
    Cube() = default;
    // Отрисовка.
    void Draw();
    
    GLuint VAO;
    GLuint VBO;
};

#endif  // CUBE_HPP
