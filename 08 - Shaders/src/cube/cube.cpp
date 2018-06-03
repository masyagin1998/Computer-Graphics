#include "cube.hpp"

Cube::Cube(GLuint VAO, GLuint VBO) :
    VAO(VAO),
    VBO(VBO) {
    GLfloat s = 1.0f / 2.0f;

    GLfloat cube[] = {
        -s, s, s, 1.0f, 0.0f, 0.0f,
        -s, -s, s, 1.0f, 0.0f, 0.0f,
        s, s, s, 1.0f, 0.0f, 0.0f,
        s, -s, s, 1.0f, 0.0f, 0.0f,

        -s, s, -s, 1.0f, 1.0f, 0.0f,
        -s, -s, -s, 1.0f, 1.0f, 0.0f,
        s, s, -s, 1.0f, 1.0f, 0.0f,
        s, -s, -s, 1.0f, 1.0f, 0.0f,

        -s, s, s, 0.0f, 1.0f, 0.0f,
        -s, -s, s, 0.0f, 1.0f, 0.0f,
        -s, s, -s, 0.0f, 1.0f, 0.0f,
        -s, -s, -s, 0.0f, 1.0f, 0.0f,

        s, s, s, 0.0f, 1.0f, 1.0f,
        s, -s, s, 0.0f, 1.0f, 1.0f,
        s, s, -s, 0.0f, 1.0f, 1.0f,
        s, -s, -s, 0.0f, 1.0f, 1.0f,

        -s, s, -s, 0.0f, 0.0f, 1.0f,
        -s, s, s, 0.0f, 0.0f, 1.0f,
        s, s, -s, 0.0f, 0.0f, 1.0f,
        s, s, s, 0.0f, 0.0f, 1.0f,
        
        -s, -s, -s, 1.0f, 0.0f, 1.0f,
        -s, -s, s, 1.0f, 0.0f, 1.0f,
        s, -s, -s, 1.0f, 0.0f, 1.0f,
        s, -s, s, 1.0f, 0.0f, 1.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
    glBindVertexArray(0);
}
