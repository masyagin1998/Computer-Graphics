#include "spring.hpp"

#include <cstdio>
#include <cmath>
#include <iostream>

Spring::Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
               GLint number_of_coils,
               Color ambient, Color diffuse, Color specular, Color emission, GLfloat shineness,
               GLuint texture,
               GLint quality_spring, GLint quality_wire,
               Vertex translation) :
    spring_radius(spring_radius), wire_radius(wire_radius), pressure_coefficient(pressure_coefficient),
    number_of_coils(number_of_coils),
    ambient(ambient), diffuse(diffuse), specular(specular), emission(emission), shineness(shineness),
    texture(texture),
    quality_spring(quality_spring), quality_wire(quality_wire),
    translation(translation) {
    GenerateVertices();
}

void Spring::ChangeQuality(GLint quality_spring, GLint quality_wire) {
    this->quality_spring = quality_spring > 50 ? quality_spring : 50;
    this->quality_wire = quality_wire > 3 ? quality_wire : 3;
    GenerateVertices();
}

void Spring::Draw(bool with_texture) {
    if (with_texture) {
        DrawWithTexture();
    } else {
        DrawWithoutTexture();
    }
}

void Spring::DrawWithoutTexture() {
    // Установка свойств материала.
    GLfloat model_ambient[] = { ambient.R,
                                ambient.G,
                                ambient.B,
                                ambient.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, model_ambient);
    
    GLfloat model_diffuse[] = { diffuse.R,
                                diffuse.G,
                                diffuse.B,
                                diffuse.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, model_diffuse);
    
    GLfloat model_specular[] = { specular.R,
                                 specular.G,
                                 specular.B,
                                 specular.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, model_specular);
    
    GLfloat model_emission[] = { emission.R,
                                 emission.G,
                                 emission.B,
                                 emission.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, model_emission);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shineness);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // Перемещение.
    glTranslatef(translation.X, translation.Y, translation.Z);
    // Отрисовка.
    for (size_t i = 0; i < vertecies.size(); i++) {
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(vertecies[i][0].first.X, vertecies[i][0].first.Y, vertecies[i][0].first.Z);
            glVertex3f(vertecies[i][0].second.X, vertecies[i][0].second.Y, vertecies[i][0].second.Z);
            for (size_t j = 1; j < vertecies[i].size(); j++) {
                // В "переизбытке" переменных нет ничего страшного, так как это делает код понятнее,
                // плюс компилятор переделывает этот код (особенно при -O3 ключе).
                auto a = vertecies[i][j - 1].first;
                auto b = vertecies[i][j - 1].second;
                auto c = vertecies[i][j].first;

                // Векторное произведение.
                // ___________________
                // |  i  |  j  |  k  |
                // ___________________
                // | cbx | cby | cbz |
                // ___________________
                // | bax | bay | baz |
                // ___________________

                auto cbx = b.X - c.X;
                auto cby = b.Y - c.Y;
                auto cbz = b.Z - c.Z;
                
                auto bax = a.X - b.X;
                auto bay = a.Y - b.Y;
                auto baz = a.Z - b.Z;
                
                glNormal3f(baz * cby - cbz * bay, bax * cbz - cbx * baz, bay * cbx - cby * bax);
                glVertex3f(c.X, c.Y, c.Z);
                auto d = vertecies[i][j].second;

                // Векторное произведение.
                // ___________________
                // |  i  |  j  |  k  |
                // ___________________
                // | bcx | bcy | bcz |
                // ___________________
                // | cdx | cdy | cdz | 
                // ___________________
                
                auto bcx = c.X - b.X;
                auto bcy = c.Y - b.Y;
                auto bcz = c.Z - b.Z;

                auto cdx = d.X - c.X;
                auto cdy = d.Y - c.Y;
                auto cdz = d.Z - c.Z;
                
                glNormal3f(cdz * bcy - cdy * bcz, cdx * bcz - bcx * cdz, cdy * bcx - bcy * cdx);
                glVertex3f(vertecies[i][j].second.X, vertecies[i][j].second.Y, vertecies[i][j].second.Z);
            }
        glEnd();
    }
    glPopMatrix();
}

void Spring::DrawWithTexture() {
    // Установка свойств материала.
    GLfloat model_ambient[] = { ambient.R,
                                ambient.G,
                                ambient.B,
                                ambient.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, model_ambient);
    
    GLfloat model_diffuse[] = { diffuse.R,
                                diffuse.G,
                                diffuse.B,
                                diffuse.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, model_diffuse);
    
    GLfloat model_specular[] = { specular.R,
                                 specular.G,
                                 specular.B,
                                 specular.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, model_specular);
    
    GLfloat model_emission[] = { emission.R,
                                 emission.G,
                                 emission.B,
                                 emission.A };
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, model_emission);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shineness);

    glBindTexture(GL_TEXTURE_2D, texture);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // Перемещение.
    glTranslatef(translation.X, translation.Y, translation.Z);
    // Отрисовка.

    GLfloat spring_step = 1.0f / ((GLfloat) quality_spring / (GLfloat) number_of_coils);
    GLfloat wire_step = 1.0f / (GLfloat) quality_wire;
    
    GLfloat spring_tex = 0.0f;
    GLfloat wire_tex = 0.0f;

    for (size_t i = 0; i < vertecies.size(); i++) {
        glBegin(GL_QUADS);
            spring_tex = 0.0f;
            for (size_t j = 0; j < vertecies[i].size() - 1; j++) {
                auto a = vertecies[i][j].first;
                auto b = vertecies[i][j].second;
                auto c = vertecies[i][j + 1].second;

                // Векторное произведение.
                // ___________________
                // |  i  |  j  |  k  |
                // ___________________
                // | cbx | cby | cbz |
                // ___________________
                // | bax | bay | baz |
                // ___________________

                auto cbx = b.X - c.X;
                auto cby = b.Y - c.Y;
                auto cbz = b.Z - c.Z;
                
                auto bax = a.X - b.X;
                auto bay = a.Y - b.Y;
                auto baz = a.Z - b.Z;

                glTexCoord2f(wire_tex, spring_tex);
                glVertex3f(vertecies[i][j].first.X, vertecies[i][j].first.Y, vertecies[i][j].first.Z);
                glNormal3f(baz * cby - cbz * bay, bax * cbz - cbx * baz, bay * cbx - cby * bax);
                glTexCoord2f(wire_tex + wire_step, spring_tex);
                glVertex3f(vertecies[i][j].second.X, vertecies[i][j].second.Y, vertecies[i][j].second.Z);
                glTexCoord2f(wire_tex + wire_step, spring_tex + spring_step);
                glVertex3f(vertecies[i][j + 1].second.X, vertecies[i][j + 1].second.Y, vertecies[i][j + 1].second.Z);
                glTexCoord2f(wire_tex, spring_tex + spring_step);
                glVertex3f(vertecies[i][j + 1].first.X, vertecies[i][j + 1].first.Y, vertecies[i][j + 1].first.Z);
                spring_tex += spring_step;
            }
        glEnd();

        wire_tex += wire_step;
    }
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Spring::GenerateVertices() {
    // Очистка текущего вектора.
    for (size_t i = 0; i < vertecies.size(); i++) {
        vertecies[i].clear();
    }
    vertecies.clear();
    top_circle.clear();
    bottom_circle.clear();

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
    
    spring_angle_max = 2.0f * M_PI;
    spring_step = spring_angle_max / (GLfloat) quality_spring;
    for (GLfloat spring_angle = spring_angle_min; spring_angle < spring_angle_max; spring_angle += spring_step) {
        GLfloat X = (spring_radius + wire_radius) * sin(spring_angle);
        GLfloat Y = -(spring_radius + wire_radius) * cos(spring_angle);
        GLfloat Z = (spring_angle_max * number_of_coils / M_PI) / pressure_coefficient + wire_radius * sin(wire_angle_max);
        top_circle.emplace_back(Vertex(X, Y, Z));
        bottom_circle.emplace_back(Vertex(X, Y, 0));
    }

    printf("Debug Log: generated array of vertecies.\n");
}
