#ifndef SPRING_HPP
#define SPRING_HPP

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
       ambient, diffuse, specular, shineness - параметры материала;
       texture - номер текстуры OpenGL;
       quality_spring - качество самой пружины;
       quality_wire - качество проволоки;
       translation - смещение;
     */
    Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
           GLint number_of_coils,
           Color ambient, Color diffuse, Color specular, Color emission, GLfloat shineness,
           GLuint texture, GLuint with_texture, GLuint without_texture, GLuint material,
           GLint quality_spring, GLint quality_wire,
           Vertex translation);

    Spring() = default;

    // Методы изменения качества пружины.
    
    /*
      Смена качества пружины:
      quality_spring - качество самой пружины;
      quality_wire - качество проволоки;
     */
    void ChangeQuality(GLint quality_spring, GLint quality_wire);

    // Геттеры положения.
    Vertex GetTranslation() { return translation; }

    // Сеттеры положения.
    void SetTranslation(Vertex translation) { 
        this->translation = translation;
    }

    // Геттеры качества.
    int GetSpringQuality() {
        return quality_spring;
    }
    int GetWireQuality() {
        return quality_wire;
    }
    
    // Геттеры колец.
    std::vector<Vertex> GetTopCircle() {
        return top_circle;
    }
    std::vector<Vertex> GetBottomCircle() {
        return bottom_circle;
    }
    
    /*
      Генерирует массивы вершин.
    */
    void GenerateVertices();

    void Compile();

    // Коэффициенты.
    GLfloat spring_radius, wire_radius, pressure_coefficient;
    
    // Кол-во витков.
    GLint number_of_coils;

    // Параметры материала.
    Color ambient;
    Color diffuse;
    Color specular;
    Color emission;
    GLfloat shineness;
    
    // Текстура.
    GLuint texture;

    // Списки отрисовки.
    GLuint with_texture;
    GLuint without_texture;
    GLuint material;

    // Качество.
    GLint quality_spring, quality_wire;

    // Перемещение.
    Vertex translation;
    
    // Вектор векторов точек.
    std::vector<std::vector<Vertex>> vertecies;
    // Вектор векторов нормалей.
    std::vector<std::vector<Vertex>> normals;
    // Вектор векторов координат текстуры.
    std::vector<std::vector<Vertex>> texels;

    // Кольца анализа столкновений.
    std::vector<Vertex> top_circle;
    std::vector<Vertex> bottom_circle;
};

#endif // SPRING_HPP
