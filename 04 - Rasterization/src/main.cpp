// g++ -g -Wall -Wextra -std=c++14 -O3 main.cpp -o main -lGL -lglfw -lGLU -lGLEW

// Для вывода логов.
#include <cstdio>
#include <iostream>
// Для математических расчетов.
#include <cmath>

// GLEW - для динамической линковки линковки с OpenGL.
// GLEW_STATIC показывает, что библиотека будет подключена статически.
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW - для работы с пользовательским вводом/выводом
// и создания экрана.
#include <GLFW/glfw3.h>

#include <vector>
#include <utility>

// Макрос для неиспользуемых переменных.
// (Подавление -Wall -Wextra).
#define prefix_unused(variable) ((void)variable)

// Параметры монитора.
#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080

// Параметры экрана.
#define WIDTH 1280
#define HEIGHT 720
#define NAME "Lab - 04. Masyagin Mikhail. IU9-42B."

// Точка на плоскости.
struct Point {
    // Конструктор по умолчанию для создания векторов точек.
    Point() : x(0), y(0) {}
    // Конструктор.
    Point(GLint x, GLint y) : x(x), y(y) {}
    // Координаты.
    GLint x;
    GLint y;
};

// Вектор точек фигуры (в нём будут храниться точки фигуры).
std::vector<Point> Points;

// Мой "буфер кадра".
GLfloat *frame_buffer;

// Размеры экрана.
int Width, Height;

// Способ отрисовки линий.
bool Antialiasing = false;

// Инициализация окна.
GLFWwindow *Init();

// Коллбэки.
// Обработка ошибок OpenGL и GLWF.
void ErrorCallback(int error, const char* description);

// Изменение размера экрана.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

// Задание viewport'а, матрицы проекции, очистка моего "буфера кадра" и массива точек.
void SetViewProjBuff();

// Очитска моего "буфера кадра".
void ClearFrameBuffer();

// Обработка нажатий с клавиатуры.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Обработка позиции мыши и нажатий её кнопок.
void MouseCallback(GLFWwindow* window, int button, int action, int mods);

// Отрисовка всех линий.
void DrawLines();

// Обычная отрисовка линии.
void DrawLine(GLint x1, GLint y1, GLint x2, GLint y2);

// Отрисовка линии по Брезенхему.
void DrawLineAntialiasing(GLint x1, GLint y1, GLint x2, GLint y2);

// Функция получения знака числа (нужна в алгоритмах отрисовки линий).
template <typename T> int Sign(T val);

// Заливка областей.
void Fill(GLint x, GLint y);

// Задать цвет пикселя в моем "буфере кадра".
void SetPixelColor(int x, int y, GLfloat r, GLfloat g, GLfloat b);

// Получение индекса в реальном массиве.
int GetPixelIndex(int x, int y);

int main(int argc, char **argv) {
    prefix_unused(argc);
    prefix_unused(argv);

    // Создание окна.
    GLFWwindow *window = Init();
    if (window == nullptr) {
        glfwTerminate();
        return 1;
    }
    
    glfwGetFramebufferSize(window, &Width, &Height);
    SetViewProjBuff();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Цикл отрисовки.
    while (! glfwWindowShouldClose(window)) {
        // Очистка.
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка пикселей из моего "буфера кадра".
        glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, frame_buffer);

        // Смена буферов.
        glfwSwapBuffers(window);
        // Проверка но новые сообщения.
        glfwPollEvents();
    }
    
    return 0;
}

GLFWwindow *Init() {
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, NAME, nullptr, nullptr);    
    if (window == nullptr) {
        printf("Error:\nFailed to create GLFW window.\n");
        return nullptr;
    }
    // Выбор позиции окна.
    glfwSetWindowPos(window, (MONITOR_WIDTH - WIDTH) / 2, (MONITOR_HEIGHT - HEIGHT) / 2);
    // Выбор контекст только что созданного окна.
    glfwMakeContextCurrent(window);

    // Выбор колбэков.
    glfwSetErrorCallback(ErrorCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);

    // Инициализация GLEW.
    // glewExperimental - для самых современных функций управления OpenGL.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Error:\nFailed to initialize GLEW.\n");
        return nullptr;
    }
    
    return window;
}

void ErrorCallback(int error, const char* description) {
    printf("Error:\n(string)  : %s\n(integer) : %d", description, error);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    prefix_unused(window);
    Width = width;
    Height = height;
    SetViewProjBuff();
}

void SetViewProjBuff() {
    // Задания Viewport'а.
    glViewport(0, 0, Width, Height);
    // Задание ортографической прокции.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, (GLfloat)Width, 0.0f, (GLfloat)Height);   
    // Задание модельно-видовой матрицы.
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Очистка моего "буфера кадра".
    ClearFrameBuffer();
    // Очистка точек фигуры.
    Points.clear();
}

void ClearFrameBuffer() {
    if (frame_buffer != nullptr) {
        delete[] frame_buffer;
    }
    int frame_buffer_size = Width * Height * 3;
    frame_buffer = new GLfloat[frame_buffer_size];
    for (int i = 0; i < frame_buffer_size; i += 3) {
        frame_buffer[i] = 0.0f;
        frame_buffer[i + 1] = 0.0f;
        frame_buffer[i + 2] = 0.0f;
    }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    prefix_unused(scancode);
    prefix_unused(mods);

    // Выход.
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    // Очистка.
    if ((key == GLFW_KEY_BACKSPACE) && (action == GLFW_PRESS)) {
        ClearFrameBuffer();
        Points.clear();
    }

    // Выбор способа отрисовка линий.
    if ((key == GLFW_KEY_A) && (action == GLFW_PRESS)) {
        Antialiasing = !Antialiasing;
        ClearFrameBuffer();
        DrawLines();
    }
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
    prefix_unused(mods);
    
    GLdouble x, y;
    glfwGetCursorPos(window, &x, &y);

    if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
        Points.push_back(Point(x, y));
        ClearFrameBuffer();
        SetPixelColor(x, y, 1.0f, 1.0f, 1.0f);
        DrawLines();
    }

    if ((button == GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_PRESS)) {
        Fill(x, y);
    }
}

void DrawLines() {
    size_t num = Points.size();
    if (num > 1) {
        for (size_t i = 0; i < num - 1; i++) {
            if (Antialiasing) {
                DrawLineAntialiasing(Points[i].x, Points[i].y, Points[i + 1].x, Points[i + 1].y);
            } else {
                DrawLine(Points[i].x, Points[i].y, Points[i + 1].x, Points[i + 1].y);                
            }
        }
    }
    if (num > 2) {
        if (Antialiasing) {
            DrawLineAntialiasing(Points[num - 1].x, Points[num - 1].y, Points[0].x, Points[0].y);
        } else {
            DrawLine(Points[num - 1].x, Points[num - 1].y, Points[0].x, Points[0].y);                
        }
    }
}

void DrawLine(GLint x1, GLint y1, GLint x2, GLint y2) {
    // Алгоритм Брезенхема (4-связный).

    // Параметры пикселя.
    glPixelZoom(1, 1);

    // Инициализация.
    GLint x = x1;
    GLint y = y1;
    GLint dx = std::abs(x2 - x1);
    GLint dy = std::abs(y2 - y1);
    GLint sign_x = Sign(x2 - x1);
    GLint sign_y = Sign(y2 - y1);
    GLint e = 2 * dy - dx;

    // Вырожденные случаи.
    if (dy == 0) {
        for (GLint i = x1; std::abs(x2 - i) > 0; i += sign_x) {
            SetPixelColor(i, y, 1.0f, 1.0f, 1.0f);
        }
        return;
    }
    if (dx == 0) {
        for (GLint i = y1; std::abs(y2 - i) > 0; i += sign_y) {
            SetPixelColor(x, i, 1.0f, 1.0f, 1.0f);
        }
        return;
    }

    // Сравнение dx и dy.
    bool ch = false;
    if (dy >= dx) {
        std::swap(dx, dy);
        ch = true;
    }
    
    // Отрисовка.
    int i = 0;
    while(true) {
        // Отрисовка одной точки.
        SetPixelColor(x, y, 1.0f, 1.0f, 1.0f);
        if (e < dx) {
            if (ch) {
                y += sign_y;
            } else {
                x += sign_x;
            }
            e += 2 * dy;
        } else {
            if (ch) {
                x += sign_x;
            } else {
                y += sign_y;
            }
            e -= 2 * dx;
        }
        i++;
        if (i > dx + dy) {
            SetPixelColor(x, y, 1.0f, 1.0f, 1.0f);
            break;
        }
    } 
}

void DrawLineAntialiasing(GLint x1, GLint y1, GLint x2, GLint y2) {
    // Алгоритм Брезенхема с устранением ступенчатости (4-связный).

    // Параметры пикселя.
    glPixelZoom(1, 1);
    // Кол-во разбиений цвета.
    const GLint I = 8;
    // Коэффициент.
    GLfloat k = 0.1f;

    // Вычисление дельт.
    GLint dx = x2 - x1;
    GLint dy = y2 - y1;

    // Вырожденные случаи.  
    if (std::abs(dx) == std::abs(dy) || dy * dx == 0) {
        DrawLine(x1, y1, x2, y2);
    }
    
    // Инициализация.
    GLfloat tgx = std::abs((GLfloat) dy / (GLfloat) dx);
    GLfloat m = std::abs((GLfloat) I * (GLfloat) dy / (GLfloat)dx);
    if (std::abs(dy) > std::abs(dx)) {
        m = std::abs((GLfloat) I * (GLfloat) dx / (GLfloat) dy);
    }
    GLfloat e = m / 2;
    GLfloat de = m;
    GLfloat w = std::abs(I - m);
    GLint sign_x = Sign(dx);
    GLint sign_y = Sign(dy);
    
    // Закраска самого первого пикселя.
    SetPixelColor(x1, y1, 1.0f, 1.0f, 1.0f);
    
    // Пошли по октантам.
    while (std::abs(x1) != std::abs(x2)) {
        if (tgx < 1.0f) x1 += sign_x; else y1 += sign_y;
        if (e >= w) {
            if (tgx < 1.0f) y1 += sign_y; else x1 += sign_x;
            e -= w;
            SetPixelColor(x1 - (dx * dy > 0 ? sign_x : 0), y1 - (dx * dy < 0 ? sign_y : 0), 1.0f, 1.0f, 1.0f);
        } else if (e < w) {
            e += de;
        }
        GLfloat color = ((dx * dy > 0) && (tgx < 1.0f)) || ((dx * dy < 0) && (tgx > 1.0f)) ?
            (1.0f - k * e > 0.0f ? (1.0f - k * e < 1.0f ? 1.0f - k * e : 1.0f) : 0.1f) :
            (k * e > 0.0f ? (k * e < 1.0f ? k * e : 1.0f) : 0.1f);
        SetPixelColor(x1, y1, color, color, color);
    }
    return;
}

template <typename T> int Sign(T val) {
    return (T(0) < val) - (val < T(0));
}

void Fill(GLint x, GLint y) {
    // Алгоритм построчной заливки с затравкой (8-связный).

    // Инициализация стека.
    std::vector<Point> stack;
    stack.push_back(Point(x, y));

    // Пока стек не пуст...
    while (stack.size() != 0) {
        // Достали из стека текущую точку.
        Point p = stack[stack.size() - 1]; stack.pop_back();
        
        // Закраска текущей строки.
        GLint x_min = p.x;
        GLint x_max = p.x;
        GLint y = p.y;
        while (true) {
            int index = GetPixelIndex(x_min - 1, y);
            GLfloat r = frame_buffer[index];
            GLfloat g = frame_buffer[index + 1];
            GLfloat b = frame_buffer[index + 2];
            if ((r != 0.0f) && (g != 0.0f) && (b != 0.0f)) {
                break;
            }
            x_min--;
            if (x_min < 0) {
                x_min = 0;
                break;
            }
        }
        while (true) {
            int index = GetPixelIndex(x_max + 1, y);
            GLfloat r = frame_buffer[index];
            GLfloat g = frame_buffer[index + 1];
            GLfloat b = frame_buffer[index + 2];
            if ((r != 0.0f) && (g != 0.0f) && (b != 0.0f)) {
                break;
            }
            x_max++;
            if (x_max >= Width) {
                x_max = Width - 1;
                break;
            }
        }
        for (int i = x_min; i <= x_max; i++) {
            SetPixelColor(i, y, 1.0f, 1.0f, 1.0f);
        }
        
        // Закраска верхней строки.
        if (y - 1 > 0) {
            bool flag = true;
            for (int i = x_min > 0 ? x_min - 1 : 0; i <= (x_max < Width - 2 ? x_max + 1 : Width - 1); i++) {
                int index = GetPixelIndex(i, y - 1);
                GLfloat r = frame_buffer[index];
                GLfloat g = frame_buffer[index + 1];
                GLfloat b = frame_buffer[index + 2];
                if ((r == 0.0f) && (g == 0.0f) && (b == 0.0f)) {
                    if (flag) {
                        stack.push_back(Point(i, y - 1));
                        flag = false;
                    }
                } else {
                    flag = true;
                }
            }
        }

        // Обработка нижней строки.
        if (y + 1 < Height) {
            bool flag = true;
            for (int i = x_min > 0 ? x_min - 1 : 0; i <= (x_max < Width - 2 ? x_max + 1 : Width - 1); i++) {
                int index = GetPixelIndex(i, y + 1);
                GLfloat r = frame_buffer[index];
                GLfloat g = frame_buffer[index + 1];
                GLfloat b = frame_buffer[index + 2];
                if ((r == 0.0f) && (g == 0.0f) && (b == 0.0f)) {
                    if (flag) {
                        stack.push_back(Point(i, y + 1));
                        flag = false;
                    }
                } else {
                    flag = true;
                }
            }
        }
    }
}

void SetPixelColor(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
    int index = (Height - (GLint) y) * 3 * Width + 3 * ((GLint) x);
    frame_buffer[index] = r;
    frame_buffer[index + 1] = g;
    frame_buffer[index + 2] = b;
}

int GetPixelIndex(int x, int y) {
    return (Height - (GLint) y) * 3 * Width + 3 * ((GLint) x);
}
