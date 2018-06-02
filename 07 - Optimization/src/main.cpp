/*
  Про оптимизации:
  1) Я убрал GL_DEPTH_TEST.
     +: не используется сравнительно дорогой алгоритм Z-буфера.
     -: в коде уменьшилась читаемость, ушла часть ООП-составляющей.
  2) 
*/

// Для вывода логов.
#include <cstdio>
#include <iostream>
// Для математических расчетов.
#include <cmath>

// Базовая функциональность.
#include "utils/utils.hpp"
// Для света.
#include "light/light.hpp"

// Для отрисовки пружины.
#include "spring/spring.hpp"
// Для отрисовки коробки.
#include "box/box.hpp"

// Обработка нажатий с клавиатуры.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Поворот.
void DoChanges();

int main(int argc, char **argv) {
    // Чтение конфига.
    if (argc >= 2) {
        file_in = std::string(argv[1]);
    }
    if (argc >= 3) {
        file_out = std::string(argv[2]);
    }
    json_config = JSONConfig(file_in);

    // Создание окна;
    GLFWwindow *window = Init(json_config);
    if (window == nullptr) {
        // Закрытие всего.
        glfwTerminate();
        return 1;
    }
    glfwSetKeyCallback(window, KeyCallback);
    
    // Загрузка текстур.
    LoadTexture(json_config.spring_texture);
    LoadTexture(json_config.box_texture);

    // Загрузка списков.
    GLuint list_index = glGenLists(7);

    // Создание коробки.
    box = Box(json_config.ltn, json_config.rbf, json_config.box_color, textures[json_config.box_texture],
              list_index, list_index + 1, list_index + 2, list_index + 3);

    // Создание пружины.
    spring = Spring(json_config.spring_radius, json_config.wire_radius, json_config.pressure_coefficient,
                    json_config.number_of_coils,
                    json_config.model_ambient, json_config.model_diffuse, json_config.model_specular, json_config.model_emission, json_config.shineness,
                    textures[json_config.spring_texture], list_index + 4, list_index + 5, list_index + 6,
                    json_config.quality_spring, json_config.quality_wire,
                    json_config.spring_translation);

    // Создание света.
    Light(json_config);

    // Цикл отрисовки.
    while (! glfwWindowShouldClose(window)) {
        // Время последнего кадра.
        GLfloat current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Рендеринг.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Рисование.
        Draw();
        
        // Смена буферов.
        glfwSwapBuffers(window);
        // Проверка но новые сообщения.
        glfwPollEvents();
        // Изменения.
        DoChanges();
    }
    
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    prefix_unused(scancode);
    prefix_unused(mods);

    // Выход.
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Сохранение конфига.
    if ((key == GLFW_KEY_ENTER) && (action == GLFW_PRESS)) {
        json_config.Save();
    }

    // Остановка/запуск анимации.
    if ((key == GLFW_KEY_BACKSPACE) && (action == GLFW_PRESS)) {
        json_config.show_animation = !json_config.show_animation;
    }
    
    // Остановка/запуск текстур.
    if ((key == GLFW_KEY_TAB) && (action == GLFW_PRESS)) {
        json_config.show_textures = !json_config.show_textures;
    }

    // Проверка нажатия клавиши.
    if ((action == GLFW_PRESS) && (key >= 0) && (key < 1024)) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void DoChanges() {
    if (!json_config.show_animation) {
        return;
    }
    
    // Изменение положения.
    GLfloat tr_x = json_config.speed_translation.X * delta_time;
    GLfloat tr_y = json_config.speed_translation.Y * delta_time;
    GLfloat tr_z = json_config.speed_translation.Z * delta_time;

    Vertex translation = spring.GetTranslation();
    translation.X += tr_x;
    translation.Y += tr_y;
    translation.Z += tr_z;

    spring.SetTranslation(translation);
    json_config.spring_translation = spring.GetTranslation();
    
    // Изменение качества.
    GLint quality_speed = 1;

    GLint change_spring_quality = 0;
    GLint change_wire_quality = 0;

    if (keys[GLFW_KEY_LEFT]) {
        change_spring_quality -= quality_speed;
    }
    if (keys[GLFW_KEY_RIGHT]) {
        change_spring_quality += quality_speed;
    }
    if (keys[GLFW_KEY_UP]) {
        change_wire_quality += quality_speed;
    }
    if (keys[GLFW_KEY_DOWN]) {
        change_wire_quality -= quality_speed;
    }
    
    if (((keys[GLFW_KEY_LEFT]) || (keys[GLFW_KEY_RIGHT]) || (keys[GLFW_KEY_UP]) || (keys[GLFW_KEY_DOWN])) && 
        (!(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT]) || !(keys[GLFW_KEY_UP] && keys[GLFW_KEY_DOWN])))
        spring.ChangeQuality(spring.GetSpringQuality() + change_spring_quality,
                                     spring.GetWireQuality() + change_wire_quality);
    json_config.quality_spring = spring.GetSpringQuality();
    json_config.quality_wire = spring.GetWireQuality();

    Intersection();

    // Изменение параметров источника света.
    GLfloat constant = json_config.speed_change.X * delta_time;
    GLfloat linear = json_config.speed_change.Y * delta_time;
    GLfloat quadratic = json_config.speed_change.Z * delta_time;

    if (keys[GLFW_KEY_Q]) {
        json_config.light_attenuation.X += constant;
    }
    if (keys[GLFW_KEY_A]) {
        json_config.light_attenuation.X -= constant;
        if (json_config.light_attenuation.X < 0.0f) {
            json_config.light_attenuation.X = 0.0f;
        }
    }
    if (keys[GLFW_KEY_W]) {
        json_config.light_attenuation.Y += linear;
    }
    if (keys[GLFW_KEY_S]) {
        json_config.light_attenuation.Y -= linear;
        if (json_config.light_attenuation.Y < 0.0f) {
            json_config.light_attenuation.Y = 0.0f;
        }
    }
    if (keys[GLFW_KEY_E]) {
        json_config.light_attenuation.Z += quadratic;
    }
    if (keys[GLFW_KEY_D]) {
        json_config.light_attenuation.Z -= quadratic;
        if (json_config.light_attenuation.Z < 0.0f) {
            json_config.light_attenuation.Z = 0.0f;
        }
    }

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, json_config.light_attenuation.X);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, json_config.light_attenuation.Y);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, json_config.light_attenuation.Z);
}
