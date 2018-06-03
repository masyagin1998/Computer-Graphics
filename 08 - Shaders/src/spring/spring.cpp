#include "spring.hpp"
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <iostream>

Spring::Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
                GLint number_of_coils,
                Color color,
                GLint quality_spring, GLint quality_wire,
                Vertex translation,
                Vertex rotation,
                GLfloat scale,
               GLuint VAO, GLuint VBO) :
    spring_radius(spring_radius), wire_radius(wire_radius), pressure_coefficient(pressure_coefficient),
    number_of_coils(number_of_coils),
    color(color),
    quality_spring(quality_spring), quality_wire(quality_wire),
    translation(translation),
    rotation(rotation),
    scale(scale),
    VAO(VAO), VBO(VBO) {
    ChangeQuality(quality_spring, quality_wire);
    GenerateVertices();
}

void Spring::ChangeQuality(GLint quality_spring, GLint quality_wire) {
    this->quality_spring = quality_spring > 8 * number_of_coils ? quality_spring : 8 * number_of_coils;
    this->quality_wire = quality_wire > 3 ? quality_wire : 3;
    this->quality_spring = this->quality_spring < 400 * number_of_coils ? this->quality_spring : 400 * number_of_coils;
    this->quality_wire = this->quality_wire < 150 ? this->quality_wire : 150;
    GenerateVertices();
}

void Spring::GenerateVertices() {
    // Если мы генерируем вершины впервые, то ничего очищать не надо.
    // Если мы в очередной раз генерируем вершины, то всю информацию,
    // оставшуюся с прошлого раза, нужно удалить.
    if (!vertecies.empty()) {
        vertecies.clear();
    }

    // Задание качества.
    GLfloat wire_angle_max = 2.0f * M_PI;
    GLfloat wire_step = wire_angle_max / (GLfloat) quality_wire;

    GLfloat spring_angle_max = (GLfloat) number_of_coils * 2.0f * M_PI;
    GLfloat spring_step = spring_angle_max / (GLfloat) quality_spring;

    // Задание координат пружины.
    for (int i = 0; i <= quality_spring; i++) {
        vertecies.push_back(std::vector<Vertex>());
        for (int j = 0; j <= quality_wire; j++) {
            GLfloat current_x = (spring_radius + wire_radius * cos(j * wire_step)) * sin(i * spring_step);
            GLfloat current_y = -(spring_radius + wire_radius * cos(j * wire_step)) * cos(i * spring_step);
            GLfloat current_z = (i * spring_step / M_PI) / pressure_coefficient + wire_radius * sin(j * wire_step);
            Vertex current = {current_x, current_y, current_z};
            vertecies[vertecies.size() - 1].push_back(current);
        }
    }

    for (size_t i = 0; i < vertecies.size(); i++) {
        std::reverse(vertecies[i].begin(), vertecies[i].end());
    }

    std::vector<GLfloat> arr;
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        auto b = vertecies[i + 1][0];
        arr.push_back(b.X);
        arr.push_back(b.Y);
        arr.push_back(b.Z);
        auto a = vertecies[i][0];
        arr.push_back(a.X);
        arr.push_back(a.Y);
        arr.push_back(a.Z);
        for (size_t j = 0; j < vertecies[i].size() - 2; j++) {
            auto c = vertecies[i + 1][j + 1];
            arr.push_back(c.X);
            arr.push_back(c.Y);
            arr.push_back(c.Z);
            auto d = vertecies[i][j + 1];
            arr.push_back(d.X);
            arr.push_back(d.Y);
            arr.push_back(d.Z);
        }
        arr.push_back(b.X);
        arr.push_back(b.Y);
        arr.push_back(b.Z);
        arr.push_back(a.X);
        arr.push_back(a.Y);
        arr.push_back(a.Z);
    }

    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(GLfloat), arr.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    printf("Debug Log: generated array of vertecies.\n\n");
}

void Spring::Draw() {
    glBindVertexArray(VAO);
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * vertecies[0].size() * 2, vertecies[0].size() * 2);
    }
    glBindVertexArray(0);
}
