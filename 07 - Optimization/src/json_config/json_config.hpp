#ifndef _JSON_CONFIG_HPP_
#define _JSON_CONFIG_HPP_

#include <GL/glew.h>

#include <string>

#include "../vertex/vertex.hpp"
#include "../color/color.hpp"

/*
  Вся конфигурация программы будет храниться в JSON-файле,
  Который может быть прочитан посредством класса JSONConfig.
*/
struct JSONConfig {
 public:
    // Чтение стартовой конфигурации.
    JSONConfig(std::string filename = "");
    // Сохранение текущей конфигурации.
    void Save(std::string filename = "");
    // Дебаг.
    void Debug();
    
    // Имя файла стартовой конфигурации.
    std::string start_filename;
    // Была ли ошибка при чтении JSON-файла конфигурации.
    bool was_error;

    // Отображать ли анимация.
    bool show_animation;

    // Отображать ли текстуры.
    bool show_textures;
    
    // Конфиг окна.
    // Имя окна.
    std::string window_name;
    // Размеры экрана.
    GLint screen_width;
    GLint screen_height;
    // Размеры окна.
    GLint window_width;
    GLint window_height;
    // Можно ли изменять размер экрана.
    bool is_resizable;
    // Цвет очистки экрана.
    Color clear_color;

    // Конфиг коробки.
    // Левая, верхняя, ближайшая точка.
    Vertex ltn;
    // Правая, нижняя, наиболее удаленная точка.
    Vertex rbf;
    // Цвет.
    Color box_color;
    // Текстура.
    std::string box_texture;

    // Конфиг пружины.
    // Размеры.
    GLfloat spring_radius;
    GLfloat wire_radius;
    GLfloat pressure_coefficient;
    GLint number_of_coils;
    
    // Качество.
    GLint quality_wire;
    GLint quality_spring;
    
    // Начальная позиция.
    Vertex spring_translation;

    // Скорости.
    Vertex speed_translation;

    // Материал.
    Color model_ambient;
    Color model_diffuse;
    Color model_specular;
    Color model_emission;
    GLfloat shineness;

    // Текстура.
    std::string spring_texture;

    // Конфиг источника света.
    Vertex light_position;
    Color light_ambient;
    Color light_diffuse;
    Color light_specular;
    Vertex light_attenuation;

    Vertex speed_change;

    // Конфиг глобального освещения.
    Color global_light_ambient;

};

#endif  // _JSON_CONFIG_HPP_
