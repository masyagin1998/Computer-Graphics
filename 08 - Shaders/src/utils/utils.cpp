#include "utils.hpp"

bool draw_polygons = true;

bool keys[1024];

GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;

GLuint VBOs[2];
GLuint VAOs[2];

glm::mat4 projection;

Shader cube_shader;
Cube cube;

Shader spring_shader;
Spring spring;

GLFWwindow *Init() {
    // Инициализация GLFW.
    glfwInit();
    // Настройка параметров GLFW:
    // Выберем версию OpenGL (4.5);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // Будем работать в режиме "Основного профиля";
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна.
    GLFWwindow* window = glfwCreateWindow(720, 720, "Lab-08. Masyagin Mikhail. IU9-42B.", nullptr, nullptr);    
    if (window == nullptr) {
        printf("Failed to create GLFW window.\n");
        return nullptr;
    }
    // Выбор позиции окна.
    glfwSetWindowPos(window, (1920 - 720) / 2, (1080 - 720) / 2);
    // Выбор контекст только что созданного окна.
    glfwMakeContextCurrent(window);

    // Инициализация GLEW.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW.\n");
        return nullptr;
    }

    // Выбор колбэков.
    glfwSetErrorCallback(ErrorCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Включение теста глубины.
    glEnable(GL_DEPTH_TEST);

    // Включение альфа-канала.
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Выбор разрешения ViewPort'а.
    int view_port_width, view_port_height;
    glfwGetFramebufferSize(window, &view_port_width, &view_port_height);
    int size = std::min<int>(view_port_width, view_port_height);
    glViewport(0, 0, size, size);

    // Очистка буфера.
    glClearColor(0.22f, 0.24f, 0.29f, 1.0f);

    // Инициализация буферов.
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    // Матрица проекции.
    projection = glm::perspective(45.0f, 1.0f / 1.0f, 0.1f, 100.0f);
    projection = glm::rotate(projection, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    projection = glm::rotate(projection, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
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
