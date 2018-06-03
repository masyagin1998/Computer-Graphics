#pragma once

#ifndef SPRING_HPP
#define SPRING_HPP

#include <GLFW/glfw3.h>

#include <vector>
#include <utility>

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
       quality_spring - качество самой пружины;
       quality_wire - качество проволоки;
       translation_x - смещение по X;
       translation_y - смещение по Y;
       rotation_x - вращение по X;
       rotation_y - вращение по Y;
       scale - масштаб;
     */
    Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
           GLint number_of_coils,
           GLfloat color_r, GLfloat color_g, GLfloat color_b, GLfloat transparency, 
           GLint quality_spring, GLint quality_wire,
           GLfloat translation_x, GLfloat translation_y, GLfloat translation_z,
           GLfloat rotation_x, GLfloat rotation_y, GLfloat rotation_z,
           GLfloat scale);
    
    /* 
       Конструктор без выбора начального положения:
       spring_radius - радиус самой пружины;
       wire_radius - радиус проволоки, из которой
                     сделана пружина;
       pressure_coefficient - коэффициент сжатия
                              пружины;
       number_of_coils - кол-во витков;
       quality_spring - качество самой пружины;
       quality_wire - качество проволоки;
     */
    Spring(GLfloat spring_radius, GLfloat wire_radius, GLfloat pressure_coefficient,
           GLint number_of_coils,
           GLfloat color_r, GLfloat color_g, GLfloat color_b, GLfloat transparency,
           GLint quality_spring, GLint quality_wire);

    /*
      Конструктор по умолчанию.
     */
    Spring();

    // Методы изменения положения пружины и её масштаба.

    /*
      Смена положения и масштаба:
      translation_x - смещение по X;
      translation_y - смещение по Y;
      rotation_x - вращение по X;
      rotation_y - вращение по Y;
      scale - масштаб;
    */
    void ChangeLocation(GLfloat translation_x, GLfloat translation_y, GLfloat translation_z,
                        GLfloat rotation_x, GLfloat rotation_y, GLfloat rotation_z,
                        GLfloat scale);

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
    void Draw();

    // Геттеры положения.

    GLfloat GetTranslationX() { return translation_x; }
    GLfloat GetTranslationY() { return translation_y; }
    GLfloat GetTranslationZ() { return translation_z; }
    GLfloat GetRotationX() { return rotation_x; }
    GLfloat GetRotationY() { return rotation_y; }
    GLfloat GetRotationZ() { return rotation_z; }
    GLfloat GetScale() { return scale; }

    // Сеттеры положения.

    void SetTranslationX(GLfloat translation_x) { this->translation_x = translation_x; }
    void SetTranslationY(GLfloat translation_y) { this->translation_y = translation_y; }
    void SetTranslationZ(GLfloat translation_z) {this->translation_z = translation_z; }
    void SetRotationX(GLfloat rotation_x) { this->rotation_x = (rotation_x >= 360.0f) || (rotation_x <= -360.0f) ? 0.0f : rotation_x; }
    void SetRotationY(GLfloat rotation_y) { this->rotation_y = (rotation_y >= 360.0f) || (rotation_y <= -360.0f) ? 0.0f : rotation_y; }
    void SetRotationZ(GLfloat rotation_z) { this->rotation_z = (rotation_z >= 360.0f) || (rotation_z <= -360.0f) ? 0.0f : rotation_z; }
    void SetScale(GLfloat scale) { this->scale = scale; }

    // Геттеры качества.
    int GetSpringQuality() { return quality_spring; }
    int GetWireQuality() { return quality_wire; }

    // Сеттеры качества.
    void SetSpingQuality(int quality_spring) { this->quality_spring = quality_spring > 50 ? quality_spring : 50; GenerateVertices(); }
    void SetWireQuality(int quality_wire) { this->quality_wire = quality_wire > 3 ? quality_wire : 3; GenerateVertices(); }

 private:
    /*
      Генерирует массивы вершин.
     */
    void GenerateVertices();

    // Коэффициенты.
    GLfloat spring_radius, wire_radius, pressure_coefficient;
    
    // Кол-во витков.
    GLint number_of_coils;

    // Цвет.
    GLfloat color_r, color_g, color_b, transparency;

    // Качество.
    GLint quality_spring, quality_wire;

    // Перемещение.
    GLfloat translation_x, translation_y, translation_z;

    // Поворот.
    GLfloat rotation_x, rotation_y, rotation_z;

    // Масштаб.
    GLfloat scale;
    
    // Структура, содержащая описание одной вершины.
    struct Vertex {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    
    // Вектор векторов пар точек.
    std::vector<std::vector<std::pair<Vertex, Vertex>>> vertecies;
};

#endif // SPRING_HPP
