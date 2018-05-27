#ifndef BOX_HPP
#define BOX_HPP

#include <GLFW/glfw3.h>

#include <vector>

#include "../vertex/vertex.hpp"
#include "../color/color.hpp"

// Коробка.
class Box {
 public:
    
    // Конструкторы.

    // ltn_ (left top nearest) - левая верхняя ближняя вершина,
    // rbf_ (rigth bottom far) - правая нижняя дальняя вершина,
    // color - цвет.
    Box(Vertex ltn_, Vertex rbf_, Color color, GLuint texture);
    Box() = default;

    // Метод отрисовки.
    void Draw(bool with_texture = true);
    
    // 6 полигонов.
    std::vector<Vertex> top;
    std::vector<Vertex> bottom;
    std::vector<Vertex> left;
    std::vector<Vertex> right;
    std::vector<Vertex> near;
    std::vector<Vertex> far;
    
 private:
    // Отрисовка с текстурой.
    void DrawWithTexture();
    
    // Отрисовка без текстуры.
    void DrawWithoutTexture();

    // Цвет.
    Color color;
    // Текстура.
    GLuint texture;
};

#endif  // BOX_HPP
