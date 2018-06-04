#include "spring.hpp"

#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>

Spring::Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
               GLint number_of_coils,
               Color ambient, Color diffuse, Color specular, Color emission, GLfloat shineness,
               GLuint texture, GLuint with_texture, GLuint without_texture, GLuint material,
               GLint quality_spring, GLint quality_wire,
               Vertex translation) :
    spring_radius(spring_radius), wire_radius(wire_radius), pressure_coefficient(pressure_coefficient),
    number_of_coils(number_of_coils),
    ambient(ambient), diffuse(diffuse), specular(specular), emission(emission), shineness(shineness),
    texture(texture), with_texture(with_texture), without_texture(without_texture), material(material),
    quality_spring(quality_spring), quality_wire(quality_wire),
    translation(translation) {
    ChangeQuality(quality_spring, quality_wire);
    GenerateVertices();

    // Установка свойств материала.
    GLfloat model_ambient[] = { ambient.R,
                                ambient.G,
                                ambient.B,
                                ambient.A };
    
    GLfloat model_diffuse[] = { diffuse.R,
                                diffuse.G,
                                diffuse.B,
                                diffuse.A };
    
    GLfloat model_specular[] = { specular.R,
                                 specular.G,
                                 specular.B,
                                 specular.A };
    
    GLfloat model_emission[] = { emission.R,
                                 emission.G,
                                 emission.B,
                                 emission.A };
    
    glNewList(material, GL_COMPILE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, model_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, model_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, model_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, model_emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shineness);
    glEndList();
}

void Spring::ChangeQuality(GLint quality_spring, GLint quality_wire) {
    this->quality_spring = quality_spring > 8 * number_of_coils ? quality_spring : 8 * number_of_coils;
    this->quality_wire = quality_wire > 6 ? quality_wire : 6;
    this->quality_spring = this->quality_spring < 40 * number_of_coils ? this->quality_spring : 40 * number_of_coils;
    this->quality_wire = this->quality_wire < 15 ? this->quality_wire : 15;
    GenerateVertices();
}

void Spring::GenerateVertices() {
    // Если мы генерируем вершины впервые, то ничего очищать не надо.
    // Если мы в очередной раз генерируем вершины, то всю информацию,
    // оставшуюся с прошлого раза, нужно удалить.
    if (!vertecies.empty()) {
        vertecies.clear();
        top_circle.clear();
        bottom_circle.clear();
        normals.clear();
        texels.clear();
        glDeleteLists(without_texture, 1);
        glDeleteLists(with_texture, 1);
    }

    // Задание качества.
    GLfloat wire_angle_max = 2.0f * M_PI;
    GLfloat wire_step = wire_angle_max / (GLfloat) quality_wire;

    GLfloat spring_angle_min = 0.0f;
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

    // Задание векторов нормалей.
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        normals.push_back(std::vector<Vertex>());
        for (size_t j = 0; j < vertecies[i].size() / 2 - 1; j++) {
            auto a = vertecies[i][j];
            auto b = vertecies[i + 1][j];
            auto c = vertecies[i + 1][j + 1];
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
            GLfloat length = std::sqrt((baz * cby - cbz * bay) * (baz * cby - cbz * bay) +
                                       (bax * cbz - cbx * baz) * (bax * cbz - cbx * baz) +
                                       (bay * cbx - cby * bax) * (bay * cbx - cby * bax));
            normals[i].push_back(Vertex{(baz * cby - cbz * bay) / length, (bax * cbz - cbx * baz) / length, (bay * cbx - cby * bax) / length });
        }        
    }
    
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        for (size_t j = vertecies[i].size() / 2 - 1; j < vertecies[i].size() - 1; j++) {
            auto a = vertecies[i][j];
            auto b = vertecies[i + 1][j];
            auto c = vertecies[i + 1][j + 1];
            
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
            GLfloat length = std::sqrt((baz * cby - cbz * bay) * (baz * cby - cbz * bay) +
                                       (bax * cbz - cbx * baz) * (bax * cbz - cbx * baz) +
                                       (bay * cbx - cby * bax) * (bay * cbx - cby * bax));
            normals[i].push_back(Vertex{(baz * cby - cbz * bay) / length, (bax * cbz - cbx * baz) / length, (bay * cbx - cby * bax) / length });
        }
    }
    
    // Задание текстур.
    GLfloat spring_tex = 0.0f;
    GLfloat spring_tex_step = 1.0f / ((GLfloat) quality_spring / (GLfloat) number_of_coils);
    GLfloat wire_tex = 0.0f;
    GLfloat wire_tex_step = 1.0f / (GLfloat) quality_wire;
    
    for (size_t i = 0; i < vertecies.size(); i++) {
        texels.push_back(std::vector<Vertex>());
        wire_tex = 0.0f;
        for (size_t j = 0; j < vertecies[i].size() - 1; j++) {
            texels[i].push_back(Vertex{wire_tex, spring_tex, 0});
            wire_tex += wire_tex_step;
        }
        spring_tex += spring_tex_step;
    }

    // Задание колец-ограничителей.
    spring_angle_max = 2.0f * M_PI;
    spring_step = spring_angle_max / (GLfloat) quality_spring;
    for (GLfloat spring_angle = spring_angle_min; spring_angle < spring_angle_max; spring_angle += spring_step) {
        GLfloat X = (spring_radius + wire_radius) * sin(spring_angle);
        GLfloat Y = -(spring_radius + wire_radius) * cos(spring_angle);
        GLfloat Z = (spring_angle_max * number_of_coils / M_PI) / pressure_coefficient + wire_radius * sin(wire_angle_max);
        top_circle.emplace_back(Vertex(X, Y, Z));
        bottom_circle.emplace_back(Vertex(X, Y, 0));
    }
    
    // Компиляция списков OpenGL.
    Compile();

    printf("Debug Log: generated array of vertecies.\n\n");
}

void Spring::Compile() {
    std::vector<GLfloat> vertecies_back;
    std::vector<GLfloat> normals_back;
    std::vector<GLfloat> tex_back;

    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        auto b = vertecies[i + 1][0];
        vertecies_back.push_back(b.X);
        vertecies_back.push_back(b.Y);
        vertecies_back.push_back(b.Z);
        normals_back.push_back(0.0f);
        normals_back.push_back(0.0f);
        normals_back.push_back(0.0f);
        tex_back.push_back(texels[i][1].X);
        tex_back.push_back(texels[i][1].Y);                                               
        auto a = vertecies[i][0];
        vertecies_back.push_back(a.X);
        vertecies_back.push_back(a.Y);
        vertecies_back.push_back(a.Z);
        normals_back.push_back(0.0f);
        normals_back.push_back(0.0f);
        normals_back.push_back(0.0f);
        tex_back.push_back(texels[i + 1][1].X);
        tex_back.push_back(texels[i + 1][1].Y);
        for (size_t j = 0; j < vertecies[i].size() / 2 - 1; j++) {
            auto c = vertecies[i + 1][j + 1];
            vertecies_back.push_back(c.X);
            vertecies_back.push_back(c.Y);
            vertecies_back.push_back(c.Z);
            normals_back.push_back(normals[i][j].X);
            normals_back.push_back(normals[i][j].Y);
            normals_back.push_back(normals[i][j].Z);
            tex_back.push_back(texels[i][j].X);
            tex_back.push_back(texels[i][j].Y);
            auto d = vertecies[i][j + 1];
            vertecies_back.push_back(d.X);
            vertecies_back.push_back(d.Y);
            vertecies_back.push_back(d.Z);
            normals_back.push_back(normals[i][j].X);
            normals_back.push_back(normals[i][j].Y);
            normals_back.push_back(normals[i][j].Z);
            tex_back.push_back(texels[i + 1][j].X);
            tex_back.push_back(texels[i + 1][j].Y);
        }
    }

    std::vector<GLfloat> vertecies_front;
    std::vector<GLfloat> normals_front;
    std::vector<GLfloat> tex_front;
    
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        auto b = vertecies[i + 1][vertecies[i].size() / 2 - 1];
        vertecies_front.push_back(b.X);
        vertecies_front.push_back(b.Y);
        vertecies_front.push_back(b.Z);
        normals_front.push_back(0.0f);
        normals_front.push_back(0.0f);
        normals_front.push_back(0.0f);
        tex_front.push_back(texels[i][vertecies[i].size() / 2 + 1].X);
        tex_front.push_back(texels[i][vertecies[i].size() / 2 + 1].Y);
        auto a = vertecies[i][vertecies[i].size() / 2 - 1];
        vertecies_front.push_back(a.X);
        vertecies_front.push_back(a.Y);
        vertecies_front.push_back(a.Z);
        normals_front.push_back(0.0f);
        normals_front.push_back(0.0f);
        normals_front.push_back(0.0f);
        tex_front.push_back(texels[i + 1][vertecies[i].size() / 2 + 1].X);
        tex_front.push_back(texels[i + 1][vertecies[i].size() / 2 + 1].Y);
        for (size_t j = vertecies[i].size() / 2 - 1; j < vertecies[i].size() - 1; j++) {
            auto c = vertecies[i + 1][j + 1];
            vertecies_front.push_back(c.X);
            vertecies_front.push_back(c.Y);
            vertecies_front.push_back(c.Z);
            normals_front.push_back(normals[i][j].X);
            normals_front.push_back(normals[i][j].Y);
            normals_front.push_back(normals[i][j].Z);
            tex_front.push_back(texels[i][j].X);
            tex_front.push_back(texels[i][j].Y);
            auto d = vertecies[i][j + 1];
            vertecies_front.push_back(d.X);
            vertecies_front.push_back(d.Y);
            vertecies_front.push_back(d.Z);
            normals_front.push_back(normals[i][j].X);
            normals_front.push_back(normals[i][j].Y);
            normals_front.push_back(normals[i][j].Z);
            tex_front.push_back(texels[i + 1][j].Y);
            tex_front.push_back(texels[i + 1][j].Y);
        }
    }

    glNewList(without_texture, GL_COMPILE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, vertecies_back.data());
    glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 3, normals_back.data());
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * (vertecies[0].size() / 2) * 2, (vertecies[0].size() / 2) * 2);
    }
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, vertecies_front.data());
    glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 3, normals_front.data());
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, ((vertecies[0].size() - 1) - (vertecies[0].size() / 2 - 1) + 1) * 2 * i, ((vertecies[0].size() - 1) - (vertecies[0].size() / 2 - 1) + 1) * 2); 
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    /*
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        auto a = vertecies[i][0];
        auto b = vertecies[i + 1][0];
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(a.X, a.Y, a.Z);
        glVertex3f(b.X, b.Y, b.Z);
        for (size_t j = 0; j < vertecies[i].size() / 2 - 1; j++) {
            auto c = vertecies[i + 1][j + 1];
            auto d = vertecies[i][j + 1];
            Vertex v = normals[i][j];
            glNormal3f(v.X, v.Y, v.Z);
            glVertex3f(d.X, d.Y, d.Z);
            glNormal3f(v.X, v.Y, v.Z);
            glVertex3f(c.X, c.Y, c.Z);
        }        
        glEnd();
    }

    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        auto a = vertecies[i][vertecies[i].size() / 2];
        auto b = vertecies[i + 1][vertecies[i].size() / 2];
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(a.X, a.Y, a.Z);
        glVertex3f(b.X, b.Y, b.Z);
        for (size_t j = vertecies[i].size() / 2; j < vertecies[i].size() - 2; j++) {
            auto c = vertecies[i + 1][j + 1];
            auto d = vertecies[i][j + 1];
            Vertex v = normals[i][j];
            glNormal3f(v.X, v.Y, v.Z);
            glVertex3f(d.X, d.Y, d.Z);
            glNormal3f(v.X, v.Y, v.Z);
            glVertex3f(c.X, c.Y, c.Z);
        }        
        glEnd();
    }
    */

    glEndList();
    glNewList(with_texture, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, vertecies_back.data());
    glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 3, normals_back.data());
    glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, tex_back.data());
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * (vertecies[0].size() / 2) * 2, (vertecies[0].size() / 2) * 2);
    }
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, vertecies_front.data());
    glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 3, normals_front.data());
    glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, tex_front.data());
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, ((vertecies[0].size() - 1) - (vertecies[0].size() / 2 - 1) + 1) * 2 * i, ((vertecies[0].size() - 1) - (vertecies[0].size() / 2 - 1) + 1) * 2); 
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    /*
    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        auto a = vertecies[i][0];
        auto b = vertecies[i + 1][0];
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(texels[i][1].X, texels[i][1].Y);
        glVertex3f(a.X, a.Y, a.Z);
        glTexCoord2f(texels[i + 1][1].X, texels[i + 1][1].Y);
        glVertex3f(b.X, b.Y, b.Z);
        for (size_t j = 0; j < vertecies[i].size() / 2 - 1; j++) {
            auto c = vertecies[i + 1][j + 1];
            auto d = vertecies[i][j + 1];
            Vertex v = normals[i][j];
            glNormal3f(v.X, v.Y, v.Z);
            glTexCoord2f(texels[i][j].X, texels[i][j].Y);
            glVertex3f(d.X, d.Y, d.Z);
            glNormal3f(v.X, v.Y, v.Z);
            glTexCoord2f(texels[i + 1][j].X, texels[i + 1][j].Y);
            glVertex3f(c.X, c.Y, c.Z);
        }
        glEnd();
    }

    for (size_t i = 0; i < vertecies.size() - 1; i++) {
        auto a = vertecies[i][vertecies[i].size() / 2];
        auto b = vertecies[i + 1][vertecies[i].size() / 2];
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(texels[i][vertecies[i].size() / 2 + 1].X, texels[i][vertecies[i].size() / 2 + 1].Y);
        glVertex3f(a.X, a.Y, a.Z);
        glTexCoord2f(texels[i + 1][vertecies[i].size() / 2 + 1].X, texels[i + 1][vertecies[i + 1].size() / 2 + 1].Y);
        glVertex3f(b.X, b.Y, b.Z);
        for (size_t j = vertecies[i].size() / 2 - 1; j < vertecies[i].size() - 2; j++) {
            auto c = vertecies[i + 1][j + 1];
            auto d = vertecies[i][j + 1];
            Vertex v = normals[i][j];
            glNormal3f(v.X, v.Y, v.Z);
            glTexCoord2f(texels[i][j].X, texels[i][j].Y);
            glVertex3f(d.X, d.Y, d.Z);
            glNormal3f(v.X, v.Y, v.Z);
            glTexCoord2f(texels[i + 1][j].X, texels[i + 1][j].Y);
            glVertex3f(c.X, c.Y, c.Z);
        }        
        glEnd();
    }
    */
    glBindTexture(GL_TEXTURE_2D, 0);
    glEndList();
}
