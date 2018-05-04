// g++ -g -Wall -Wextra -std=c++14 -O3 main.cpp spring/spring.cpp -o main -lGL -lglfw -lGLU -lGLEW


// Для вывода логов.
#include <cstdio>
// Для математических расчетов.
#include <cmath>

// GLEW - для динамической линковки линковки с OpenGL.
// GLEW_STATIC показывает, что библиотека будет подключена статически.
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW - для работы с пользовательским вводом/выводом
// и создания экрана.
#include <GLFW/glfw3.h>

// Для отрисовки пружины.
#include "spring/spring.hpp"

// Макрос для неиспользуемых переменных.
// (Подавление -Wall -Wextra).
#define prefix_unused(variable) ((void)variable)

// Параметры монитора.
#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080

// Параметры экрана.
#define WIDTH 900
#define HEIGHT 900
#define NAME "Lab - 03. Masyagin Mikhail. IU9-42B."

// Клавиши.
bool keys[1024];

// Параметры полигонов.
static bool polygon_mode = true;

// Параметры раскадровки.
static GLfloat delta_time = 0.0f; // Время, прошедшее между последним и текущим кадром.
static GLfloat last_frame = 0.0f; // Время вывода последнего кадра.

/*
// Статическая пружина.
static Spring static_spring = Spring(1.0f, 0.1f, 2.0f,
                                     6,
                                     1.0f, 1.0f, 1.0f, 0.5f,
                                     500.0f, 50.0f,
                                     2.0f, -2.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f,
                                     0.5f);
*/

// Динамическая пружина.
static Spring dynamic_spring = Spring(1.0f, 0.1f, 2.0f,
                                      6,
                                      1.0f, 1.0f, 1.0f, 0.5f,
                                      50.0f, 50.0f);

// Инициализация окна.
GLFWwindow *Init(GLuint width, GLuint height, const char *name);

// Инициализация камеры.
void Camera(int viewPortWidth, int viewPortHeight);

// Коллбэки.
// Обработка ошибок OpenGL и GLWF.
void ErrorCallback(int error, const char* description);

// Изменение размера экрана.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

// Обработка нажатий с клавиатуры.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Выбор способо отображения полигонов.
void ChoosePolygonMode();

// Выбор того, какие переменные углов и масштаба надо сбросить.
void ChooseWhatToRestoreToDefaults();

// Поворот.
void DoChanges();

// Отрисовка куба.
void DrawCube();

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

    // Проекция.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0f, 0.1, 1000);
    glTranslatef(0.0f, 0.0f, -15.0f);
    glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(200.0f, 0.0f, 1.0f, 0.0f);

    // Цикл отрисовки.
    while (! glfwWindowShouldClose(window)) {
        // Время последнего кадра.
        GLfloat current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Рендеринг.
        // Очистка буфера.
        glClearColor(0.22f, 0.24f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Рисование.
        // Статическая пружина.
        //static_spring.Draw();
        // Динамическая пружина.
        dynamic_spring.Draw();
        // Кубик.
        glPushMatrix();
        glTranslatef(3.0f, -3.0f, 3.0f);
        DrawCube();
        glPopMatrix();
        // Смена буферов.
        glfwSwapBuffers(window);
        // Проверка но новые сообщения.
        glfwPollEvents();
        // Изменения.
        DoChanges();
    }
    
    return 0;
}

GLFWwindow *Init(GLuint width, GLuint height, const char *name) {
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
    GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);    
    if (window == nullptr) {
        printf("Error:\nFailed to create GLFW window.\n");
        return nullptr;
    }
    // Выбор позиции окна.
    glfwSetWindowPos(window, (MONITOR_WIDTH - WIDTH) / 2, (MONITOR_HEIGHT - HEIGHT) / 2);
    // Выбор иконки окна.
    //glfwSetWindowIcon(window, 1, LoadIcon("icon/icon.png"));
    // Выбор контекст только что созданного окна.
    glfwMakeContextCurrent(window);

    // Выбор колбэков.
    glfwSetErrorCallback(ErrorCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);

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
    
    // Задание ViewPort'а.
    int view_port_width, view_port_height;
    glfwGetFramebufferSize(window, &view_port_width, &view_port_height);
    int size = std::min<int>(view_port_width, view_port_height);
    glViewport(0, 0, size, size);
    
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
    if (polygon_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        polygon_mode = false;
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        polygon_mode = true;
    }
}

void DoChanges() {
    // Скорости.
    GLfloat translation_speed = 10.0f * delta_time;
    GLfloat rotation_speed = 40.0f * delta_time;
    GLfloat scale_speed = 1.0f * delta_time;
    GLint quality_speed = 1;

    // Изменение положения.
    if (keys[GLFW_KEY_A]) {
        dynamic_spring.SetTranslationX(dynamic_spring.GetTranslationX() + translation_speed);
    }
    if (keys[GLFW_KEY_D]) {
        dynamic_spring.SetTranslationX(dynamic_spring.GetTranslationX() - translation_speed);
    }
    if (keys[GLFW_KEY_W]) {
        dynamic_spring.SetTranslationY(dynamic_spring.GetTranslationY() + translation_speed);
    }
    if (keys[GLFW_KEY_S]) {
        dynamic_spring.SetTranslationY(dynamic_spring.GetTranslationY() - translation_speed);
    }

    // Изменение углов.
    if (keys[GLFW_KEY_J]) {
        dynamic_spring.SetRotationX(dynamic_spring.GetRotationX() - rotation_speed);
    }
    if (keys[GLFW_KEY_L]) {
        dynamic_spring.SetRotationX(dynamic_spring.GetRotationX() + rotation_speed);
    }
    if (keys[GLFW_KEY_I]) {
        dynamic_spring.SetRotationY(dynamic_spring.GetRotationY() + rotation_speed);
    }
    if (keys[GLFW_KEY_K]) {
        dynamic_spring.SetRotationY(dynamic_spring.GetRotationY() - rotation_speed);
    }

    // Изменение масштаба.
    if (keys[GLFW_KEY_T]) {
        dynamic_spring.SetScale(dynamic_spring.GetScale() + scale_speed);
    }
    if (keys[GLFW_KEY_G]) {
        dynamic_spring.SetScale(dynamic_spring.GetScale() - scale_speed);
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
        dynamic_spring.ChangeQuality(dynamic_spring.GetSpringQuality() + change_spring_quality,
                                     dynamic_spring.GetWireQuality() + change_wire_quality);
}

void DrawCube() {
    GLfloat size = 1.0f / 2.0f;

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-size, size, size);
        glVertex3f(-size, -size, size);
        glVertex3f(size, size, size);
        glVertex3f(size, -size, size);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-size, size, -size);
        glVertex3f(-size, -size, -size);
        glVertex3f(size, size, -size);
        glVertex3f(size, -size, -size);
    glEnd();

    glColor3f(1.0f, 0.65f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-size, size, size);
        glVertex3f(-size, -size, size);
        glVertex3f(-size, size, -size);
        glVertex3f(-size, -size, -size);
    glEnd();
    
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(size, size, size);
        glVertex3f(size, -size, size);
        glVertex3f(size, size, -size);
        glVertex3f(size, -size, -size);
    glEnd();

    glColor3f(0.26f, 0.67f, 1.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-size, size, -size);
        glVertex3f(-size, size, size);
        glVertex3f(size, size, -size);
        glVertex3f(size, size, size);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-size, -size, -size);
        glVertex3f(-size, -size, size);
        glVertex3f(size, -size, -size);
        glVertex3f(size, -size, size);
    glEnd();
}
