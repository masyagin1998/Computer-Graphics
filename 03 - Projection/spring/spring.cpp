#include "spring.hpp"

#include <cstdio>
#include <cmath>

Spring::Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
           GLint number_of_coils,
           GLfloat color_r, GLfloat color_g, GLfloat color_b, GLfloat transparency, 
           GLint quality_spring, GLint quality_wire,
           GLfloat translation_x, GLfloat translation_y, GLfloat translation_z,
           GLfloat rotation_x, GLfloat rotation_y, GLfloat rotation_z,
           GLfloat scale) :
    spring_radius(spring_radius), wire_radius(wire_radius), pressure_coefficient(pressure_coefficient),
    number_of_coils(number_of_coils),
    color_r(color_r), color_g(color_g), color_b(color_b), transparency(transparency),
    quality_spring(quality_spring), quality_wire(quality_wire),
    translation_x(translation_x), translation_y(translation_y), translation_z(translation_z),
    rotation_x(rotation_x), rotation_y(rotation_y), rotation_z(rotation_z),
    scale(scale) {
    GenerateVertices();
}

Spring::Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
           GLint number_of_coils,
           GLfloat color_r, GLfloat color_g, GLfloat color_b, GLfloat transparency,
           GLint quality_spring, GLint quality_wire) :
    spring_radius(spring_radius), wire_radius(wire_radius), pressure_coefficient(pressure_coefficient),
    number_of_coils(number_of_coils),
    color_r(color_r), color_g(color_g), color_b(color_b), transparency(transparency),
    quality_spring(quality_spring), quality_wire(quality_wire),
    translation_x(0.0f), translation_y(0.0f), translation_z(0.0f),
    rotation_x(0.0f), rotation_y(0.0f), rotation_z(0.0f),
    scale(1.0f) {
    GenerateVertices();
}

Spring::Spring() :
    spring_radius(1.0f), wire_radius(0.1f), pressure_coefficient(2.0f),
    number_of_coils(6),
    color_r(0.75f), color_g(0.75f), color_b(0.75f), transparency(0.5f),
    quality_spring(50.0f), quality_wire(50.0f),
    translation_x(0.0f), translation_y(0.0f), translation_z(0.0f),
    rotation_x(0.0f), rotation_y(0.0f), rotation_z(0.0f),
    scale(1.0f) {
    GenerateVertices();
}

void Spring::ChangeLocation(GLfloat translation_x, GLfloat translation_y, GLfloat translation_z,
                            GLfloat rotation_x, GLfloat rotation_y, GLfloat rotation_z,
                            GLfloat scale) {
    this->translation_x = translation_x;
    this->translation_y = translation_y;
    this->translation_z = translation_z;
    this->rotation_x = (rotation_x >= 360.0f) || (rotation_x <= -360.0f) ? 0.0f : rotation_x;
    this->rotation_y = (rotation_y >= 360.0f) || (rotation_y <= -360.0f) ? 0.0f : rotation_y;
    this->rotation_z = (rotation_z >= 360.0f) || (rotation_z <= -360.0f) ? 0.0f : rotation_z;
    this->scale = scale;
}

void Spring::ChangeQuality(GLint quality_spring, GLint quality_wire) {
    this->quality_spring = quality_spring > 50 ? quality_spring : 50;
    this->quality_wire = quality_wire > 3 ? quality_wire : 3;
    GenerateVertices();
}

void Spring::Draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    // Перемещение.
    glTranslatef(translation_x, translation_y, translation_z);

    // Вращение.
    glRotatef(rotation_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation_z, 0.0f, 0.0f, 1.0f);

    // Масштабирование.
    glScalef(scale, scale, scale);

    // Закраска.
    glColor4f(color_r, color_g, color_b, transparency);

    // Отрисовка.
    for (size_t i = 0; i < vertecies.size(); i++) {
        glBegin(GL_QUAD_STRIP);
            for (size_t j = 0; j < vertecies[i].size(); j++) {
                glVertex3f(vertecies[i][j].first.x, vertecies[i][j].first.y, vertecies[i][j].first.z);
                glVertex3f(vertecies[i][j].second.x, vertecies[i][j].second.y, vertecies[i][j].second.z);
            }
        glEnd();
    }
    glPopMatrix();
}

void Spring::GenerateVertices() {
    // Очистка текущего вектора.
    for (size_t i = 0; i < vertecies.size(); i++) {
        vertecies[i].clear();
    }
    vertecies.clear();

    // Задание качества.
    GLfloat wire_angle_min = 0.0f;
    GLfloat wire_angle_max = 2.0f * M_PI;
    GLfloat wire_step = wire_angle_max / (GLfloat) quality_wire;

    GLfloat spring_angle_min = 0.0f;
    GLfloat spring_angle_max = (GLfloat) number_of_coils * 2.0f * M_PI;
    GLfloat spring_step = spring_angle_max / (GLfloat) quality_spring;
    
    // Создание нового массива.
    for (GLfloat wire_angle = wire_angle_min; wire_angle <= wire_angle_max; wire_angle += wire_step) {
        vertecies.push_back(std::vector<std::pair<Vertex, Vertex>>());
        size_t i = vertecies.size() - 1;
        for (GLfloat spring_angle = spring_angle_min; spring_angle < spring_angle_max; spring_angle += spring_step) {
            GLfloat current_x = (spring_radius + wire_radius * cos(wire_angle)) * sin(spring_angle);
            GLfloat current_y = -(spring_radius + wire_radius * cos(wire_angle)) * cos(spring_angle);
            GLfloat current_z = (spring_angle / M_PI) / pressure_coefficient + wire_radius * sin(wire_angle);
            Vertex current = {current_x, current_y, current_z};

            GLfloat next_x = (spring_radius + wire_radius * cos(wire_angle + wire_step)) * sin(spring_angle);
            GLfloat next_y = -(spring_radius + wire_radius * cos(wire_angle + wire_step)) * cos(spring_angle);
            GLfloat next_z = (spring_angle / M_PI) / pressure_coefficient + wire_radius * sin(wire_angle + wire_step);
            Vertex next = {next_x, next_y, next_z};

            vertecies[i].push_back(std::make_pair(current, next));
        }
        GLfloat current_x = (spring_radius + wire_radius * cos(wire_angle)) * sin(spring_angle_max);
        GLfloat current_y = -(spring_radius + wire_radius * cos(wire_angle)) * cos(spring_angle_max);
        GLfloat current_z = (spring_angle_max / M_PI) / pressure_coefficient + wire_radius * sin(wire_angle);
        Vertex current = {current_x, current_y, current_z};
        
        GLfloat next_x = (spring_radius + wire_radius * cos(wire_angle + wire_step)) * sin(spring_angle_max);
        GLfloat next_y = -(spring_radius + wire_radius * cos(wire_angle + wire_step)) * cos(spring_angle_max);
        GLfloat next_z = (spring_angle_max / M_PI) / pressure_coefficient + wire_radius * sin(wire_angle + wire_step);
        Vertex next = {next_x, next_y, next_z};
        
        vertecies[i].push_back(std::make_pair(current, next)); 
    }

    printf("Debug Log: generated array of vertecies.\n");
}
