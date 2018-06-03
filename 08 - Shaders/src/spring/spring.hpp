#ifndef SPRING_HPP
#define SPRING_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <utility>

#include "../color/color.hpp"
#include "../vertex/vertex.hpp"

struct Spring {
    // Конструкторы.

    /* 
       Конструктор c выбором начального положения
       и масштаба:
       spring_radius - радиус самой пружины;
       wire_radius - радиус проволоки, из которой
                     сделана пружина;
       pressure_coefficient - коэффициент сжатия
                              пружины;
       number_of_coils - кол-во витков;
       quality_spring - качество самой пружины;
       quality_wire - качество проволоки;
       translation - смещение;
       rotation - повороты вокруг осей x, y, z;
       scale - масштаб.
     */
    Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
           GLint number_of_coils,
           Color color,
           GLint quality_spring, GLint quality_wire,
           Vertex translation, Vertex rotation,
           GLfloat scale,
           GLuint VAO, GLuint VBO);

    Spring() = default;
    
    // Смена качества пружины:
    void ChangeQuality(GLint quality_spring, GLint quality_wire);
    
    // Генерирует массивы вершин.
    void GenerateVertices();

    // Отрисовка.
    void Draw();

    // Коэффициенты.
    GLfloat spring_radius, wire_radius, pressure_coefficient;
    
    // Кол-во витков.
    GLint number_of_coils;

    // Цвет.
    Color color;

    // Качество.
    GLint quality_spring, quality_wire;

    // Перемещение.
    Vertex translation;
    // Повороты.
    Vertex rotation;
    // Масштаб.
    GLfloat scale;

    // Буфферы.
    GLuint VAO;
    GLuint VBO;
    
    // Вектор векторов точек.
    std::vector<std::vector<Vertex>> vertecies;
};

#endif // SPRING_HPP
