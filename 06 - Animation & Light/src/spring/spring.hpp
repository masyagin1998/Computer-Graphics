#ifndef SPRING_HPP
#define SPRING_HPP

#include <GLFW/glfw3.h>

#include <vector>
#include <utility>

#include "../color/color.hpp"
#include "../vertex/vertex.hpp"

class Spring {
 public:

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
           GLuint texture,
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

    // Методы отрисовки.
    
    /*
      Рисует пружины с текущими параметрами.
     */
    void Draw(bool with_texture = true);

    // Геттеры положения.
    Vertex GetTranslation() { return translation; }

    // Сеттеры положения.
    void SetTranslation(Vertex translation) { this->translation = translation; }

    // Геттеры качества.
    int GetSpringQuality() { return quality_spring; }
    int GetWireQuality() { return quality_wire; }

    // Сеттеры качества.
    void SetSpingQuality(int quality_spring) { this->quality_spring = quality_spring > 10 * number_of_coils ? quality_spring : 10 * number_of_coils; GenerateVertices(); }
    void SetWireQuality(int quality_wire) { this->quality_wire = quality_wire > 3 ? quality_wire : 3; GenerateVertices(); }
    
    // Геттеры колец.
    std::vector<Vertex> GetTopCircle() { return top_circle; }
    std::vector<Vertex> GetBottomCircle() { return bottom_circle; }
 private:
    /*
      Генерирует массивы вершин.
    */
    void GenerateVertices();

    /*
      Функции отрисовки.
    */
    void DrawWithTexture();
    void DrawWithoutTexture();

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

    // Качество.
    GLint quality_spring, quality_wire;

    // Перемещение.
    Vertex translation;
    
    // Вектор векторов пар точек.
    std::vector<std::vector<std::pair<Vertex, Vertex>>> vertecies;

    // Кольца анализа столкновений.
    std::vector<Vertex> top_circle;
    std::vector<Vertex> bottom_circle;
};

#endif // SPRING_HPP
