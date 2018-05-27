#include "utils.hpp"

std::string file_in;
std::string file_out;

JSONConfig json_config;

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

    // Включение теста глубины.
    glEnable(GL_DEPTH_TEST);

    // Включение альфа-канала.
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Включение текстур.
    glEnable(GL_TEXTURE_2D);

    // Включение нормализации.
    glEnable(GL_NORMALIZE);

    // Включение света.
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);
    
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
    //glRotatef(5.0f, 1.0f, 0.0f, 0.0f);
    //glRotatef(-5.0f, 0.0f, 1.0f, 0.0f);
    
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
