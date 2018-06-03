#include <cstdio>
#include <cmath>

// GLEW - для динамической линковки линковки с OpenGL.
// GLEW_STATIC показывает, что библиотека будет подключена статически.
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW - для работы с пользовательским вводом/выводом
// и создания экрана.
#include <GLFW/glfw3.h>

// SOIL - для загрузки текстур.
#include <SOIL/SOIL.h>

// GLM - для работы с матрицами.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Загрузчик шейдеров.
#include "shaders/shader.hpp"
// Загрузчик камеры.

// Макрос для неиспользуемых переменных.
// (Подавление -Wall -Wextra).
#define prefix_unused(variable) ((void)variable)

// Инициализация окна.
GLFWwindow *Init(GLuint width, GLuint height, const char *name);

// Информация об OpenGL в консоль.
void GetOpenGlInfo();

// Загрузка текстуры.
GLuint LoadTexture(char *texture_name);

// Колбэк для клавиатуры.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Колбэк для мыши.
void MouseCallback(GLFWwindow* window, double xpos, double ypos);

// Перемещение.
void Move();

// Разница во времени (чтобы на всех системах скорость была одинаковой).
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Размеры окна.
const GLuint WIDTH = 1280, HEIGHT = 720;
// Название окна.
const char *NAME = "Лабораторная работа - 01. Введение в OpenGL. Масягин Михаил. ИУ9-42Б.";

// Камера.
glm::vec3 cameraPos  = glm::vec3(0.0f, 0.0f,  5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp  = glm::vec3(0.0f, 1.0f,  0.0f);

GLfloat yaw = -90.0f; // Тангаж.
GLfloat pitch = 0.0f; // Рыскание.

GLfloat lastX = WIDTH  / 2.0; // Положение мыши X.
GLfloat lastY = HEIGHT / 2.0; // Положения мыши Y.
GLfloat fov = 45.0f; // Угол обзора (человеческий - 45).

// Буфер клавиш.
bool keys[1024];

int main(int argc, char **argv) {
    prefix_unused(argc);
    prefix_unused(argv);

    // Создание окна;
    GLFWwindow *window = Init(WIDTH, HEIGHT, NAME);
    if (window == nullptr) {
        // Закрытие всего.
        glfwTerminate();
        return 1;
    }

    // Получение информации об OpenGL;
    GetOpenGlInfo();

    // Загрузка шейдеров;
    // Программа 0.
    Shader program0 = Shader("src/shaders/shader0/shader.vs", "src/shaders/shader0/shader.frag");
    // Программа 1.
    Shader program1 = Shader("src/shaders/shader1/shader.vs", "src/shaders/shader1/shader.frag");

    // Треугольная пирамида.
    GLfloat pyramid[] = {
        // Позиции              // Цвета        
        0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.29f,    0.0f, 1.0f, 0.0f,
        0.5f,  -0.5f, 0.29f,    0.0f, 0.0f, 1.0f,
        
        0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.29f,    0.0f, 1.0f, 0.0f,
        0.0f,  -0.5f, -0.58f,   0.0f, 0.0f, 1.0f,
        
        0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.29f,     0.0f, 0.0f, 1.0f,
        0.0f,  -0.5f, -0.58f,   0.0f, 0.0f, 1.0f,
        
        0.5f, -0.5f, 0.29f,     0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.29f,    0.0f, 1.0f, 0.0f,
        0.0f,  -0.5f, -0.58f,   0.0f, 0.0f, 1.0f,
    };

    // Куб.
    float cube[] = {
        // Позиции             // Цвета
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f
    };

    // Позиции кубов.
    glm::vec3 cube_positions[] = {
        glm::vec3(2.0f,  0.0f, 0.0f),
        glm::vec3(-2.0f,  0.0f, 0.0f),
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, -2.0f, 0.0f),
        glm::vec3(0.0f,  0.0f, 2.0f),
        glm::vec3(0.0f,  0.0f, -2.0f)
    };


    GLuint VBOs[2];
    glGenBuffers(2, VBOs);

    GLuint VAOs[2];
    glGenVertexArrays(2, VAOs);

    // Треугольная пирамида.
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Куб.
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Загрузка текстур.
    GLuint texture_asuka = LoadTexture((char *)"src/textures/asuka.png");
    GLuint texture_rey = LoadTexture((char *)"src/textures/rey.png");

    // Установка миксера текстур.
    GLfloat mix_value = 0.0f;
    bool mix_flag = false;
    
    // Цикл отрисовки.
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Провекра на новые сообщения.
        glfwPollEvents();
        Move();

        // Рендеринг.

        // Очистка буфера.
        glClearColor(0.22f, 0.24f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Работаем с кубами.
        program1.Use();

        // Привязка текстур.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_asuka);
        glUniform1i(glGetUniformLocation(program1.program, "texture_asuka"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_rey);
        glUniform1i(glGetUniformLocation(program1.program, "texture_rey"), 1);

        // Матрица вида.
        glm::mat4 view; view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(program1.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // Матрица проекции.
        glm::mat4 projection; projection = glm::perspective(fov, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(program1.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Начинаем отрисовку.
        glBindVertexArray(VAOs[1]);
        for (GLuint i = 0; i < 6; i++) {
            // Матрица модели.
            glm::mat4 model;
            model = glm::translate(model, cube_positions[i]);
            GLfloat angle = 20.0f * i + (GLfloat)glfwGetTime() * 10.0f * (i + 1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(program1.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            // Миксер текстур.
            if (mix_value > 1.0f) {
                mix_value = 1.0f;
                mix_flag = false;
            }
            if (mix_value < 0.0f) {
                mix_value = 0.0f;
                mix_flag = true;
            }
            mix_value += (mix_flag ? 0.1f * deltaTime : -0.1f * deltaTime);
            glUniform1f(glGetUniformLocation(program1.program, "mix_value"), mix_value);

            // Отрисовка.
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // Работаем с треугольником.
        program0.Use();
        
        // Матрица модели.
        glm::mat4 model;
        model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(program0.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // Матрица вида.
        glUniformMatrix4fv(glGetUniformLocation(program0.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // Матрица проекции.
        glUniformMatrix4fv(glGetUniformLocation(program0.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAOs[0]);
        // Отрисовка.
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(0);

        // Смена буферов.
        glfwSwapBuffers(window);
    }

    // Очистка ресурсов.
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // Закрытие всего.
    glfwTerminate();
    return 0;
}

GLFWwindow *Init(GLuint width, GLuint height, const char *name) {
    // Инициализация GLFW.
    glfwInit();
    // Настройка параметров GLFW:
    // Выберем версию OpenGL (4.5);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // Будем работать в режиме "Основного профиля";
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Запретим изменять размер окна;
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Создание окна.
    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);    
    if (window == nullptr) {
        printf("Failed to create GLFW window.\n");
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    // Выбор колбэков.
    // Выберем колбэк для клавиатуры;
    glfwSetKeyCallback(window, KeyCallback);
    // Выберем колбэк для мыши.
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Инициализация GLEW.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW.\n");
        return nullptr;
    }

    // Выбор разрешения ViewPort'а.
    int viewPortWidth, viewPortHeight;
    glfwGetFramebufferSize(window, &viewPortWidth, &viewPortHeight);  
    glViewport(0, 0, viewPortWidth, viewPortHeight);

    // Тест глубины.
    glEnable(GL_DEPTH_TEST);

    return window;
}

GLuint LoadTexture(char *texture_name) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image(texture_name, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return texture;
}

void GetOpenGlInfo() {
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor            : %s\n", vendor);
    printf("GL Renderer          : %s\n", renderer);
    printf("GL Version (string)  : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version         : %s\n", glsl_version);
    printf("\n");

    GLint number_extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &number_extensions);

    for (int i = 0; i < number_extensions; i++) {
        printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
    }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    prefix_unused(scancode);
    prefix_unused(mode);

    if ((key == GLFW_KEY_ESCAPE && action) == (GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if ((key >= 0) && (key < 1024)) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

bool firstMouse = true;
void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    prefix_unused(window);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void Move() {
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W]) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_S]) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_A]) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (keys[GLFW_KEY_D]) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}
