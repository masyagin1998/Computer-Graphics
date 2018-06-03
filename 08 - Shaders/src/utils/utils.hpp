#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdio>
#include <cmath>

// GLEW - для динамической линковки линковки с OpenGL.
// GLEW_STATIC показывает, что библиотека будет подключена статически.
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW - для работы с пользовательским вводом/выводом
// и создания экрана.
#include <GLFW/glfw3.h>

// GLM - для работы с матрицами.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Загрузчик шейдеров.
#include "../shaders/shader.hpp"

// Пружина.
#include "../spring/spring.hpp"
// Куб.
#include "../cube/cube.hpp"

// Макрос для неиспользуемых переменных.
// (Подавление -Wall -Wextra).
#define prefix_unused(variable) ((void)variable)

// Отрисовывать ли полигоны.
extern bool draw_polygons;

// Клавиши.
extern bool keys[1024];

// Параметры раскадровки.
extern GLfloat delta_time;
extern GLfloat last_frame;

// Буферы.
extern GLuint VAOs[2];
extern GLuint VBOs[2];

// Куб.
extern Shader cube_shader;
extern Cube cube;

// Пружина.
extern Shader spring_shader;
extern Spring spring;

// Матрица проекции.
extern glm::mat4 projection;

// Инициализация окна.
GLFWwindow *Init();

// Коллбэки.
// Обработка ошибок OpenGL и GLWF.
void ErrorCallback(int error, const char* description);

// Изменение размера экрана.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif  // UTILS_HPP
