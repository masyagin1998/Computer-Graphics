#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <GLFW/glfw3.h>

// Одна вершин.
// (w = 1).
struct Vertex {
    Vertex(GLfloat X, GLfloat Y, GLfloat Z) : X(X), Y(Y), Z(Z) {};
    Vertex() = default;
    GLfloat X;
    GLfloat Y;
    GLfloat Z;
};

#endif  // VERTEX_HPP
