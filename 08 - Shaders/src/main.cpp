#include "utils/utils.hpp"

// Колбэк для клавиатуры.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Выбор способо отображения полигонов.
void ChoosePolygonMode();

// Изменения.
void DoChanges();

int main(int argc, char **argv) {
    prefix_unused(argc);
    prefix_unused(argv);

    // Создание окна;
    GLFWwindow *window = Init();
    if (window == nullptr) {
        // Закрытие всего.
        glfwTerminate();
        return 1;
    }
    glfwSetKeyCallback(window, KeyCallback);

    // Загрузка шейдеров и создание фигур.
    cube_shader = Shader("src/cube/shader.vs", "src/cube/shader.frag");
    cube = Cube(VAOs[0], VBOs[0]);

    spring_shader = Shader("src/spring/shader.vs", "src/spring/shader.frag");
    spring = Spring(1.0f, 0.1f, 2.0f,
                    6,
                    Color{1.0f, 1.0f, 0.0f, 0.6f},
                    7, 8,
                    Vertex{-4.0f, 0.0f, -15.0f},
                    Vertex{0.0f, 0.0f, 0.0f},
                    1.0f,
                    VAOs[1],
                    VBOs[1]);
    
    // Цикл отрисовки.
    while (!glfwWindowShouldClose(window)) {
        GLfloat current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Очистка буфера.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model;

        // Кубик.
        cube_shader.Use();
        model = glm::translate(model, glm::vec3(-7.0f, -2.0f, -5.0f));
        glUniformMatrix4fv(glGetUniformLocation(cube_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(cube_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        cube.Draw();

        // Пружина.
        spring_shader.Use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(spring.translation.X, spring.translation.Y, spring.translation.Z));
        model = glm::rotate(model, glm::radians(spring.rotation.X), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(spring.rotation.Y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(spring.rotation.Z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(spring.scale, spring.scale, spring.scale));
        glUniformMatrix4fv(glGetUniformLocation(cube_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(cube_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        spring.Draw();
        
        // Изменения.
        DoChanges();
        // Смена буферов.
        glfwSwapBuffers(window);
        // Провекра на новые сообщения.
        glfwPollEvents();
    }

    // Очистка ресурсов.
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    // Закрытие всего.
    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    prefix_unused(scancode);
    prefix_unused(mods);

    // Выход.
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    // Изменение способа отображения полигонов.
    if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS)) {
        ChoosePolygonMode();
    }

    // Проверка нажатия клавиши.
    if ((action == GLFW_PRESS) && (key >= 0) && (key < 1024)) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void ChoosePolygonMode() {
    // Изменение способа отображения полигонов.
    if (draw_polygons) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        draw_polygons = false;
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        draw_polygons = true;
    }
}

void DoChanges() {
    GLfloat translation_speed = 10.0f * delta_time;
    GLfloat rotation_speed = 40.0f * delta_time;
    GLfloat scale_speed = 1.0f * delta_time;
    GLint quality_speed = 1;

    // Изменение положения.
    if (keys[GLFW_KEY_A]) {
        spring.translation.X -= translation_speed;
    }
    if (keys[GLFW_KEY_Q]) {
        spring.translation.X += translation_speed;
    }
    if (keys[GLFW_KEY_S]) {
        spring.translation.Y -= translation_speed;
    }
    if (keys[GLFW_KEY_W]) {
        spring.translation.Y += translation_speed;
    }
    if (keys[GLFW_KEY_D]) {
        spring.translation.Z -= translation_speed;
    }
    if (keys[GLFW_KEY_E]) {
        spring.translation.Z += translation_speed;
    }

    // Изменение углов.
    if (keys[GLFW_KEY_J]) {
        spring.rotation.X -= rotation_speed;
    }
    if (keys[GLFW_KEY_U]) {
        spring.rotation.X += rotation_speed;
    }
    if (keys[GLFW_KEY_K]) {
        spring.rotation.Y -= rotation_speed;
    }
    if (keys[GLFW_KEY_I]) {
        spring.rotation.Y += rotation_speed;
    }
    if (keys[GLFW_KEY_L]) {
        spring.rotation.Z -= rotation_speed;
    }
    if (keys[GLFW_KEY_O]) {
        spring.rotation.Z += rotation_speed;
    }
    
    if ((spring.rotation.X >= 360.0f) || (spring.rotation.X <= -360.0f)) {
        spring.rotation.X = 0.0f;
    }
    if ((spring.rotation.Y >= 360.0f) || (spring.rotation.Y <= -360.0f)) {
        spring.rotation.Y = 0.0f;
    }
    if ((spring.rotation.Z >= 360.0f) || (spring.rotation.Z <= -360.0f)) {
        spring.rotation.Z = 0.0f;
    }

    // Изменение масштаба.
    if (keys[GLFW_KEY_T]) {
        spring.scale += scale_speed;
    }
    if (keys[GLFW_KEY_G]) {
        spring.scale -= scale_speed;
    }
    
    // Изменение качества.
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
        spring.ChangeQuality(spring.quality_spring + change_spring_quality,
                             spring.quality_wire + change_wire_quality);
}
