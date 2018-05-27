#ifndef UTILS_HPP
#define UTILS_HPP

// GLEW - для динамической линковки линковки с OpenGL.
// GLEW_STATIC показывает, что библиотека будет подключена статически.
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW - для работы с пользовательским вводом/выводом
// и создания экрана.
#include <GLFW/glfw3.h>

#include <string>

// Для работы с конфигурационным файлом.
#include "../json_config/json_config.hpp"

// Макрос для неиспользуемых переменных.
// (Подавление -Wall -Wextra).
#define prefix_unused(variable) ((void)variable)

// Куда читать/писать конфиг.
extern std::string file_in;
extern std::string file_out;

// Конфиг.
extern JSONConfig json_config;

// Клавиши.
extern bool keys[1024];

// Параметры раскадровки.
extern GLfloat delta_time;
extern GLfloat last_frame;

// Инициализация окна.
GLFWwindow *Init(const JSONConfig &json_config);

// Коллбэки.
// Обработка ошибок OpenGL и GLWF.
void ErrorCallback(int error, const char* description);

// Изменение размера экрана.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif  // UTILS_HPP
