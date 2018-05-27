#ifndef COLOR_HPP
#define COLOR_HPP

#include <GLFW/glfw3.h>

// Цвет в формате RGBA
// (цвета - в процентном соотношении).
struct Color {
    Color(GLfloat R, GLfloat G, GLfloat B, GLfloat A) : R(R), G(G), B(B), A(A) {};
    Color() = default;
    GLfloat R;
    GLfloat G;
    GLfloat B;
    GLfloat A;
};

#endif  // COLOR_HPP
