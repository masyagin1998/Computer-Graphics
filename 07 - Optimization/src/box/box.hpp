#ifndef BOX_HPP
#define BOX_HPP

#include <GLFW/glfw3.h>

#include <vector>

#include "../vertex/vertex.hpp"
#include "../color/color.hpp"

// Коробка.
struct Box {
    // Конструкторы.

    // ltn_ (left top nearest) - левая верхняя ближняя вершина,
    // rbf_ (rigth bottom far) - правая нижняя дальняя вершина,
    // color - цвет.
    Box(Vertex ltn_, Vertex rbf_, Color color, GLuint texture,
        GLuint box_far_without_texture, GLuint box_near_without_texture,
        GLuint box_far_with_texture, GLuint box_near_with_texture);
    Box() = default;

    void Compile();
    
    // 6 полигонов.
    std::vector<Vertex> top;
    std::vector<Vertex> bottom;
    std::vector<Vertex> left;
    std::vector<Vertex> right;
    std::vector<Vertex> near;
    std::vector<Vertex> far;
    
    // Цвет.
    Color color;
    // Текстура.
    GLuint texture;

    // Списки.
    GLuint box_far_without_texture;
    GLuint box_near_without_texture;

    GLuint box_far_with_texture;
    GLuint box_near_with_texture;
};

#endif  // BOX_HPP
