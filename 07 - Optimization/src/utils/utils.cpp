#include "utils.hpp"
#include <iostream>

std::string file_in;
std::string file_out;

JSONConfig json_config;

Spring spring;
Box box;

bool keys[1024];

GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;

GLFWwindow *Init(const JSONConfig &json_config) {
    // Инициализация GLFW.
    if (! glfwInit()) {
        printf("Error:\nFailed to initialize GLFW.\n");
        return nullptr;
    }
    // Настройка параметров GLFW.
    // Выберем версию OpenGL (2.0);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // Основная версия.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); // Суб-версия.

    // Создание окна.
    GLFWwindow* window = glfwCreateWindow(json_config.window_width, json_config.window_height, json_config.window_name.c_str(), nullptr, nullptr);    
    if (window == nullptr) {
        printf("Error:\nFailed to create GLFW window.\n");
        return nullptr;
    }
    // Выбор позиции окна.
    glfwSetWindowPos(window, (json_config.screen_width - json_config.window_width) / 2, (json_config.screen_height - json_config.window_height) / 2);
    // Выбор контекст только что созданного окна.
    glfwMakeContextCurrent(window);

    // Выбор колбэков.
    glfwSetErrorCallback(ErrorCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Инициализация GLEW.
    // glewExperimental - для самых современных функций управления OpenGL.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Error:\nFailed to initialize GLEW.\n");
        return nullptr;
    }
    
    // Включение альфа-канала.
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Включение текстур.
    glEnable(GL_TEXTURE_2D);

    // Включение света.
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    // Задание ViewPort'а.
    int view_port_width, view_port_height;
    glfwGetFramebufferSize(window, &view_port_width, &view_port_height);
    int size = std::min<int>(view_port_width, view_port_height);
    glViewport(0, 0, size, size);

    // Очистка буфера.
    glClearColor(json_config.clear_color.R, json_config.clear_color.G, json_config.clear_color.B, json_config.clear_color.A);

    // Проекция.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0f, 0.1, 1000);
    glRotatef(5.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-5.0f, 0.0f, 1.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    
    return window;
}

void ErrorCallback(int error, const char* description) {
    printf("Error:\n(string)  : %s\n(integer) : %d", description, error);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    prefix_unused(window);
    int size = std::min<int>(width, height);
    glViewport(0, 0, size, size);
}

void Draw() {
   if (json_config.show_textures) {
       glCallList(box.box_far_with_texture);
       glPushMatrix();
       glTranslatef(spring.translation.X, spring.translation.Y, spring.translation.Z);
       glCallList(spring.material);
       glCallList(spring.with_texture);
       glPopMatrix();
       glCallList(box.box_near_with_texture);
   } else {
       glCallList(box.box_far_without_texture);
       glPushMatrix();
       glTranslatef(spring.translation.X, spring.translation.Y, spring.translation.Z);
       glCallList(spring.material);
       glCallList(spring.without_texture);
       glPopMatrix();
       glCallList(box.box_near_without_texture);
   }
}

void Intersection() {
    // Проверим, было ли пересечение коробки и пружины.
    // Если да, то отразим вектор скорости последней.
    auto top = spring.GetTopCircle();
    auto bottom = spring.GetBottomCircle();
    auto tr  = spring.GetTranslation();
    
    bool flag_x = false;
    bool flag_y = false;
    bool flag_z = false;
    
    for (size_t i = 0; i < top.size(); i++) {
        if (((top[i].X + tr.X < box.left[0].X) || (top[i].X + tr.X > box.right[0].X)) && !flag_x) {
            json_config.speed_translation.X *= -1.0f;
            flag_x = true;
        }
        
        if (((top[i].Y + tr.Y > box.top[0].Y) || (bottom[i].Y + tr.Y < box.bottom[0].Y)) && !flag_y) {
            json_config.speed_translation.Y *= -1.0f;
            flag_y = true;
        }
        
        if (((top[i].Z + tr.Z > box.near[0].Z) || (top[i].Z + tr.Z < box.far[0].Z)) && !flag_z) {
            json_config.speed_translation.Z *= -1.0f;
            flag_z = true;
        }
    }
}
