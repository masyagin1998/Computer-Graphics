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

// Для упрощения отрисовки.
#include <vector>
#include <utility>
#include <algorithm>

// Макрос для неиспользуемых переменных.
// (Подавление -Wall -Wextra).
#define prefix_unused(variable) ((void)variable)

using namespace std;
int window_width;
int window_height;

// Параметры монитора.
#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080

// Параметры экрана.
#define WIDTH 1280
#define HEIGHT 720
#define NAME "Lab - 05. Masyagin Mikhail. IU9-42B."

// Инициализация окна.
GLFWwindow *Init();

// Задание viewport'а, матрицы проекции, очистка векторов точек, сброс состояния.
void SetViewProjBuff();

// Коллбэки.
// Обработка ошибок OpenGL и GLWF.
void ErrorCallback(int error, const char* description);

void FramebufferSizeCallback(GLFWwindow * window, int width, int height);

// Обработка нажатий с клавиатуры.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Обработка позиции мыши и нажатий её кyопок.
void MouseCallback(GLFWwindow* window, int button, int action, int mods);

// ##########################################################################################
// ############################## Все, что нужно для 5 лабы. ################################
// ##########################################################################################

// Всю программу можно представить как конечный автомат с несколькими состояними.
// (FIGURE - состояние ввода фигуры, FIGURE_HOLE - состояние ввода отверстий фигуры,
// CLIPPER - состояние ввода отсекателя, CLIPPER_HOLE - состояние ввода отверстий отсекателя,
// INTERSECTION - состояние отрисовки пересечения,
// DIFFERENCE - состояние отрисовки разности,
// UNION - состояние отрисовки объединения).
// Между отрисовками можно переходить с помощью Enter.
// Между состояними можно переходить с помощью Space.
// INTERSECTION - I, DIFFERENCE - D, UNION - U.
enum class State {
    FIGURE,
    FIGURE_HOLE,
    CLIPPER,
    CLIPPER_HOLE,
    INTERSECTION,
    DIFFERENCE,
    UNION
};
// Текущее состояние.
State state = State::FIGURE;

// Точка на плоскости.
struct Point {
    // Конструктор.
    Point(GLdouble x, GLdouble y, int index_figure, int index_clipper) : 
        x(x), y(y), 
        index_figure(index_figure), index_clipper(index_clipper),
        enter(false), exit(false) {}
    // Конструктор по умолчанию для создания векторов точек.
    Point() :
        x(-1.0), y(-1.0),
        index_figure(-1), index_clipper(-1),
        enter(false), exit(false) {}
    
    // Координаты точки.
    GLdouble x, y;
    // Индексация точки в векторах вершин.
    int index_figure, index_clipper;
    // Является ли она входной или выходной.
    bool enter, exit;
};

// Полигон.
struct Polygon {
    // Конструктор по умолчанию для создания векторов полигонов.
    Polygon() : intersection(false) {}

    // Вектор точек.
    vector<Point> points;
    // Есть ли пересечение.
    bool intersection;
};

// Реализация алгоритма Вейлера-Азертона для разности.
vector<Polygon> WeilerAtherton(vector<Polygon> figure, vector<Polygon> clipper);

// Направление фигуры.
// (CW - по часовой стрелке, CCW - против часовой стрелки).
enum class Direction {
    CW,
    CCW
};

// Изменение направления фигуры.
void ChangeDirection(Polygon &polygon, Direction direction);

// Проверяем, есть ли текущая точка среди точек полигона. Если да,
// то вовзращаем ее индекс, иначе -1.
int Find(GLdouble x, GLdouble y, Polygon polygon);

// Вставка.
void Insert(vector<Point> &points, GLdouble x, GLdouble y, Point inter);

// Проверка точки на принадлежность полигону.
bool IsInside(Point point, Polygon &polygon);

// Функция отрисовки всего и вся.
void Draw();

// Нужно ли заливать выделенную область цветом или нет.
bool colors = false;

// Закраска.
void Colorify();

// Задать цвет пикселя в моем "буфере кадра".
void SetPixelColor(int x, int y, GLubyte r, GLubyte g, GLubyte b);

// Получение индекса в реальном массиве.
int GetPixelIndex(int x, int y);

// ##########################################################################################

// ##########################################################################################
// ################################## "Наследие" 4 лабы. ####################################
// ##########################################################################################

// Мой "буфер кадра".
GLfloat *frame_buffer;

// Очитска моего "буфера кадра".
void ClearFrameBuffer();

// Отрисовка всех линий полигона.
void DrawLines(Polygon polygon, GLfloat R, GLfloat G, GLfloat B);

// Отрисовка одной линии.
void DrawLine(GLint x1, GLint y1, GLint x2, GLint y2, GLfloat R, GLfloat G, GLfloat B);

// Функция получения знака числа (нужна в алгоритмах отрисовки линий).
template <typename T> int Sign(T val);

// Задать цвет пикселя в моем "буфере кадра".
void SetPixelColor(int x, int y, GLfloat R, GLfloat G, GLfloat B);

// Получение индекса в реальном массиве.
int GetPixelIndex(int x, int y);

// ##########################################################################################

vector<Polygon> figure;
vector<Polygon> clipper;
vector<Polygon> result;

int main(int argc, char **argv) {
    prefix_unused(argc);
    prefix_unused(argv);

    // Создание окна.
    GLFWwindow *window = Init();
    if (window == nullptr) {
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(window, &window_width, &window_height);
    SetViewProjBuff();
    
    while (!glfwWindowShouldClose(window)) {
        // Очистка буфера кадра.
        glClearColor(0.22f, 0.23f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка.
        glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, frame_buffer);

        // Смена буферов.
        glfwSwapBuffers(window);
        // Проверка но новые сообщения.
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
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

void SetViewProjBuff() {
    // Задания Viewport'а.
    glViewport(0, 0, window_width, window_height);

    // Задание ортографической прокции.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, (GLdouble)window_width, 0.0f, (GLdouble)window_height);   
    // Задание модельно-видовой матрицы.
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Очистка моего "буфера кадра".
    ClearFrameBuffer();
    
    // Очистка точек.
    for (size_t i=0; i<figure.size(); i++) {
        figure[i].points.clear();
    }
    figure.clear();
    for (size_t i=0; i<clipper.size(); i++) {
        clipper[i].points.clear();
    }
    clipper.clear();
    for (size_t i = 0; i < result.size(); i++) {
        result[i].points.clear();
    }
    result.clear();
    
    // Сброс состояния.
    state = State::FIGURE;
    colors = false;
}

void ErrorCallback(int error, const char* description) {
    printf("Error:\n(string)  : %s\n(integer) : %d", description, error);
}

void FramebufferSizeCallback(GLFWwindow * window, int width, int height){
    prefix_unused(window);
    window_width = width;
    window_height = height;
    SetViewProjBuff();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    prefix_unused(scancode);
    prefix_unused(mods);

    // Выход.
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }
    
    // Очистка.
    if ((key == GLFW_KEY_BACKSPACE) && (action == GLFW_PRESS)) {
        SetViewProjBuff();
        return;
    }

    // Завершение ввода полигона.
    if ((key == GLFW_KEY_ENTER) && (action == GLFW_PRESS)) {
        switch (state) {
        case State::FIGURE: {
            figure.push_back(Polygon());
            state = State::FIGURE_HOLE;
            break;
        }
        case State::FIGURE_HOLE: {
            figure.push_back(Polygon());
            break;
        }
        case State::CLIPPER: {
            clipper.push_back(Polygon());
            state = State::CLIPPER_HOLE;
            break;
        }
        case State::CLIPPER_HOLE: {
            clipper.push_back(Polygon());
            break;
        }
        default: {
        }
        }
    } 

    // Выбор состояния автомата.
    if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS)) {
        switch (state) {
        case State::FIGURE: {
            figure.push_back(Polygon());
            state = State::FIGURE_HOLE;
            break;
        }
        case State::FIGURE_HOLE: {
            state = State::CLIPPER;
            break;
        }
        case State::CLIPPER: {
            clipper.push_back(Polygon());
            state = State::CLIPPER_HOLE;
            break;
        }
        default: {
        }
        }
    }

    // Выбор отрисовки.
    // Пересечение.
    if ((key == GLFW_KEY_I) && (action == GLFW_PRESS) && (state == State::CLIPPER_HOLE || state == State::INTERSECTION || state == State::DIFFERENCE || state == State::UNION)) {
        if (figure[figure.size() - 1].points.empty()) {
            figure.pop_back();
        }
        if (clipper[clipper.size() - 1].points.empty()) {
            clipper.pop_back();
        }
        state = State::INTERSECTION;
        result = WeilerAtherton(figure, clipper);
    }
    // Разность.
    if ((key == GLFW_KEY_D) && (action == GLFW_PRESS) && (state == State::CLIPPER_HOLE || state == State::INTERSECTION || state == State::DIFFERENCE || state == State::UNION)) {
        if (figure[figure.size() - 1].points.empty()) {
            figure.pop_back();
        }
        if (clipper[clipper.size() - 1].points.empty()) {
            clipper.pop_back();
        }
        state = State::DIFFERENCE;
        result = WeilerAtherton(figure, clipper);
    }
    // Объединение.
    if ((key == GLFW_KEY_U) && (action == GLFW_PRESS) && (state == State::CLIPPER_HOLE || state == State::INTERSECTION || state == State::DIFFERENCE || state == State::UNION)) {
        if (figure[figure.size() - 1].points.empty()) {
            figure.pop_back();
        }
        if (clipper[clipper.size() - 1].points.empty()) {
            clipper.pop_back();
        }
        state = State::UNION;
        result = WeilerAtherton(figure, clipper);
    }
    if ((key == GLFW_KEY_C) && (action == GLFW_PRESS)) {
        colors = !colors;
    }

    // Отрисовка.
    ClearFrameBuffer();
    Draw();
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
    prefix_unused(mods);
    
    // Получаем координаты точки.
    GLdouble x, y;
    glfwGetCursorPos(window, &x, &y);
    // Выбор того, куда нужно добавить очередную точку.
    if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
        switch (state) {
        case State::FIGURE: {
            if (figure.empty()) {
                figure.push_back(Polygon());
                figure[0].points.push_back(Point(x, y, 0, 0));
            } else {
                figure[0].points.push_back(Point(x, y, 0, 0));
            }
            break;
        }
        case State::FIGURE_HOLE: {
            figure[figure.size() - 1].points.push_back(Point(x, y, figure.size() - 1, 0));
            break;
        }
        case State::CLIPPER: {
            if (clipper.empty()) {
                clipper.push_back(Polygon());
                clipper[0].points.push_back(Point(x, y, 0, 0));
            } else {
                clipper[0].points.push_back(Point(x, y, 0, 0));
            }
            break;
        }
        case State::CLIPPER_HOLE: {
            clipper[clipper.size() - 1].points.push_back(Point(x, y, 0, clipper.size() - 1));
            break;
        }
        default: {
            return;
        }
        }
    }

    // Отрисовка.
    ClearFrameBuffer();
    Draw();
}

// ##########################################################################################
// ############################## Все, что нужно для 5 лабы. ################################
// ##########################################################################################

vector<Polygon> WeilerAtherton(vector<Polygon> figure, vector<Polygon> clipper) {
    // В процессе ввода данных мы могли перепутать ориентация фигуры и отсекателя.
    // Восстановим их.
    ChangeDirection(figure[0], Direction::CW);
    for (size_t i = 1; i < figure.size(); i++) {
        ChangeDirection(figure[i], Direction::CCW);
    }
    ChangeDirection(clipper[0], Direction::CW);
    for (size_t i = 1; i < clipper.size(); i++) {
        ChangeDirection(clipper[i], Direction::CCW);
    }
    
    // Найдем все точки персечения, вставим их в нужные места массива.
    // Входы.
    vector<Point> enters;
    // Выходы.
    vector<Point> exits;
    // Обновленная фигура.
    vector<Polygon> figure1;
    // Обновленный отсекатель.
    vector<Polygon> clipper1;
    clipper1 = clipper;
    for (size_t k = 0; k < figure.size(); k++) {
        figure1.push_back(figure[k]);
        for (size_t i = 0; i < figure[k].points.size(); i++) {
            // Обеспечим цикличность.
            int next1 = (i == figure[k].points.size() - 1) ? 0 : (i + 1);
            // Посчитаем коэффициенты при прямой, содержащей текущее ребро фигуры.
            // A * x + B * y + C = 0.
            GLdouble a1 = figure[k].points[i].y - figure[k].points[next1].y;
            GLdouble b1 = figure[k].points[next1].x - figure[k].points[i].x;
            GLdouble c1 = figure[k].points[i].x * figure[k].points[next1].y - 
                figure[k].points[next1].x * figure[k].points[i].y;
            for (size_t l = 0; l < clipper.size(); l++){
                for (size_t j = 0; j < clipper[l].points.size(); j++) {
                    // Обеспечим цикличность.
                    int next2  = (j == clipper[l].points.size() - 1) ? 0 : (j + 1);
                    // Посчитаем коэффициенты при прямой, содержащей текущее ребро отсекателя.
                    // A * x + B * y + C = 0.
                    GLdouble a2 = clipper[l].points[j].y - clipper[l].points[next2].y;
                    GLdouble b2 = clipper[l].points[next2].x - clipper[l].points[j].x;
                    GLdouble c2 = clipper[l].points[j].x * clipper[l].points[next2].y - 
                        clipper[l].points[next2].x * clipper[l].points[j].y;
                    // Найдем точку пересечения 2-х отрезков.
                    GLdouble k1 = a2 * figure[k].points[i].x + b2 * figure[k].points[i].y + c2;
                    GLdouble k2 = a2 * figure[k].points[next1].x + b2 * figure[k].points[next1].y  + c2;
                    GLdouble k3 = a1 * clipper[l].points[j].x + b1 * clipper[l].points[j].y  + c1;
                    GLdouble k4 = a1 * clipper[l].points[next2].x + b1 * clipper[l].points[next2].y  + c1;
                    // Если точка не принадлежит отрезку, то перейдем на следующую итерацию цикла.
                    if ((k1 * k2 >= 0) || (k3 * k4 >= 0)) {
                        continue;
                    }
                    // Иначе добавим ее в обновленные фигуру и отсекатель.
                    GLdouble vec = k1 / (k1 - k2);
                    Point intersection(figure[k].points[i].x + vec * b1, figure[k].points[i].y - vec * a1, k, l);
                    figure1.back().intersection = true;
                    clipper1[l].intersection = true;
                    Insert(clipper1[l].points, clipper[l].points[j].x, clipper[l].points[j].y, intersection);
                    Insert(figure1.back().points, figure[k].points[i].x, figure[k].points[i].y, intersection);
                }
            }
        }
    }
    
    // Определяем входные и выходные точки для всех последовательностей пересечений.
    bool exit = false, flag = false, inside = false;
    for (size_t j = 0; j < figure1.size(); j++) {
        for (size_t i = 0; i < figure1[j].points.size(); i++) {
            // Проверяем, есть ли искомая точка в массие (index != -1 - есть).
            int index = Find(figure1[j].points[i].x, figure1[j].points[i].y, figure[j]);
            // Если есть, то...
            if (index >= 0) {
                for (size_t k = 0; k < clipper1.size(); k++) {
                    // Если текущая точка лежит внути отсекателя.
                    inside = IsInside(figure1[j].points[i], clipper[k]);
                    // Если это дыра в отсекателе, то...
                    if (k > 0) {
                        if (inside) {
                            flag = true;
                            exit = false;
                            break;
                        }
                        // Иначе...
                    } else if( inside) {
                        flag = exit = true;
                    }
                }
                if (!flag) {
                    exit = false;
                }
                flag = false;
                // Иначе...
            } else {
                // Проверяем, есть ли искомая точка в другом массие (index != -1 - есть).
                index = Find(figure1[j].points[i].x, figure1[j].points[i].y, clipper1[figure1[j].points[i].index_clipper]);
                if (exit) {
                    figure1[j].points[i].exit = true;
                    clipper1[figure1[j].points[i].index_clipper].points[index].exit = true;
                    exits.push_back(figure1[j].points[i]);
                } else {
                    figure1[j].points[i].enter = true;
                    clipper1[figure1[j].points[i].index_clipper].points[index].enter = true;
                    enters.push_back(figure1[j].points[i]);
                }
                exit = !exit;
            }
        }
    }
    // Сохраняем обновленные фигуру и отсекатель.
    clipper = clipper1;
    figure = figure1;

    // Результирующие полигоны.
    vector<Polygon> result;
    // Флаги, показывающие, где мы находимся.
    // В полигоне или в дырке.
    bool in_polygon, in_hole;

    // Существует 3 варианта алгоритма Вейлера-Азертона: пересечение, разности и объединения.
    // В зависимости от выбранного алгоритм будем выбирать различные функции нахождения результирующих полигонов и задания точек.
    if (state == State::INTERSECTION) {
        // Сначала бегаеам по фигуре и добавляем в результат нужные полигоны.
        for (size_t i = 0; i < figure.size(); i++) {
            if (!figure[i].intersection) {
                in_polygon = in_hole = false;
                for (size_t j = 0; j < clipper.size(); j++) {
                    if (j == 0 && IsInside(figure[i].points[0], clipper[j])) {
                        in_polygon = true;
                    } else if (j > 0 && IsInside(figure[i].points[0], clipper[j])) {
                        in_hole = true;
                    }
                }
                if (in_polygon && !in_hole) {
                    result.push_back(figure[i]);
                }
            }
        }
    } else {
        // Сначала бегаеам по фигуре и добавляем в результат нужные полигоны.
        for (size_t i = 0; i < figure.size(); i++) {
            if (!figure[i].intersection) {
                in_polygon = in_hole = false;
                for (size_t j = 0; j < clipper.size(); j++) {
                    if (j == 0 && IsInside(figure[i].points[0], clipper[j])) {
                        in_polygon = true;
                    } else if (j > 0 && IsInside(figure[i].points[0], clipper[j])) {
                        in_hole = true;
                    }
                }
                if (!(in_polygon && !in_hole)) {
                    result.push_back(figure[i]);
                }
            }
        }
    }

    if (state == State::INTERSECTION || state == State::DIFFERENCE) {
        // Потом тоже самое по отсекателю.
        for (size_t i = 0; i < clipper.size(); i++){
            if (!clipper[i].intersection) {
                in_polygon = in_hole = false;
                for (size_t j = 0; j < figure.size(); j++) {
                    if (j == 0 && IsInside(clipper[i].points[0], figure[j])) {
                        in_polygon = true;
                    } else if (j > 0 && IsInside(clipper[i].points[0], figure[j])) {
                        in_hole = true;
                    }
                }
                if (in_polygon && !in_hole) {
                    result.push_back(clipper[i]);
                }
            }
        }
    } else {
        // Потом тоже самое по отсекателю.
        for (size_t i = 0; i < clipper.size(); i++){
            if (!clipper[i].intersection) {
                in_polygon = in_hole = false;
                for (size_t j = 0; j < figure.size(); j++) {
                    if (j == 0 && IsInside(clipper[i].points[0], figure[j])) {
                        in_polygon = true;
                    } else if (j > 0 && IsInside(clipper[i].points[0], figure[j])) {
                        in_hole = true;
                    }
                }
                if (!(in_polygon && !in_hole)) {
                    result.push_back(clipper[i]);
                }
            }
        }
    }

    if (state == State::DIFFERENCE) {
        for (size_t i = 0; i < clipper.size(); i++) {
            std::reverse(clipper[i].points.begin(), clipper[i].points.end());
        }
    }

    if (state == State::DIFFERENCE || state == State::UNION) {
        // Пока стек exits не пуст...
        while (!exits.empty()) {
            // Заводим новый вектор точек.
            vector<Point> points;
            // Стартуем с конца.
            Point start = exits.back();
            Point point;
            exits.pop_back();
            points.push_back(start);
            Polygon s = figure[start.index_figure];
            int index = Find(start.x, start.y, s);
            if (index < (int) s.points.size() - 1) {
                point = s.points[index+1];
            } else {
                point = s.points[0];
            }
            while (!((point.x == start.x) && (point.y == start.y))) {
                index = Find(point.x, point.y, s);
                point = s.points[index];
                while (point.enter == 0) {
                    if (point.x == start.x && point.y == start.y) {
                        break;
                    }
                    points.push_back(point);
                    index += 1;
                    if (index < (int) s.points.size()) {
                        point = s.points[index];
                    } else{
                        point = s.points[0];
                        index = 0;
                    }
                }
                Polygon c = clipper[point.index_clipper];
                index = Find(point.x, point.y, c);
                point = c.points[index];
                while (point.exit == 0) {
                    if (point.x == start.x && point.y == start.y) {
                        break;
                    }
                    points.push_back(point);
                    index += 1;
                    if (index < (int) c.points.size()) {
                        point = c.points[index];
                    } else {
                        point = c.points[0];
                        index = 0;
                    }
                }
                s = figure[point.index_figure];
            }
            Polygon polygon;
            polygon.points = points;
            result.push_back(polygon);
        }
    } else {
        // Пока стек enters не пуст...
        while (!enters.empty()) {
            // Заводим новый вектор точек.
            vector<Point> points;
            // Стартуем с конца.
            Point start = enters.back();
            Point point;
            enters.pop_back();
            points.push_back(start);
            Polygon s = figure[start.index_figure];
            int index = Find(start.x, start.y, s);
            if (index < (int) s.points.size() - 1) {
                point = s.points[index+1];
            } else {
                point = s.points[0];
            }
            while (!((point.x == start.x) && (point.y == start.y))) {
                index = Find(point.x, point.y, s);
                point = s.points[index];
                while (point.exit == 0) {
                    if (point.x == start.x && point.y == start.y) {
                        break;
                    }
                    points.push_back(point);
                    index += 1;
                    if (index < (int) s.points.size()) {
                        point = s.points[index];
                    } else{
                        point = s.points[0];
                        index = 0;
                    }
                }
                Polygon c = clipper[point.index_clipper];
                index = Find(point.x, point.y, c);
                point = c.points[index];
                while (point.enter == 0) {
                    if (point.x == start.x && point.y == start.y) {
                        break;
                    }
                    points.push_back(point);
                    index += 1;
                    if (index < (int) c.points.size()) {
                        point = c.points[index];
                    } else {
                        point = c.points[0];
                        index = 0;
                    }
                }
                s = figure[point.index_figure];
            }
            Polygon polygon;
            polygon.points = points;
            result.push_back(polygon);
        }
    }
    
    return result;
}

void ChangeDirection(Polygon &polygon, Direction direction) {
    // Вычисляем ориентированную площадь многоугольника.
    GLdouble sum = 0.0;
    for (size_t i = 0; i < polygon.points.size() - 1; i++) {
        sum += polygon.points[i].x * polygon.points[i + 1].y - polygon.points[i + 1].x * polygon.points[i].y;
    }
    sum += polygon.points[polygon.points.size() - 1].x * polygon.points[0].y - polygon.points[0].x * polygon.points[polygon.points.size() - 1].y;

    // Выбираем, нужно ли нам менять направление обхода или нет, и, если да, то переворачиваем.
    if (((sum > 0.0) && (direction == Direction::CCW)) || ((sum < 0.0) && (direction == Direction::CW))) {
        std::reverse(polygon.points.begin(), polygon.points.end());
    }
}

int Find(GLdouble x, GLdouble y, Polygon polygon) {
    // Пробегаем весь вектор точек.
    // Если найдем нужную, то вернем ее индекс, иначе -1.
    for (size_t i = 0; i < polygon.points.size(); i++) {
        if ((polygon.points[i].x == x) && (polygon.points[i].y == y)) {
            return i;
        }
    }
    return -1;
}

bool IsInside(Point point, Polygon &polygon) {
    // Проверка точки на принадлежность полигону методом трассировки лучей.
    bool inside = false;
    for (size_t i = 0, j = polygon.points.size() - 1; i < polygon.points.size(); j = i++){
        if ((((polygon.points[i].y<=point.y) && (point.y<polygon.points[j].y)) ||
             ((polygon.points[j].y<=point.y) && (point.y<polygon.points[i].y))) &&
            (point.x > (polygon.points[j].x - polygon.points[i].x) * (point.y - polygon.points[i].y) / 
             (polygon.points[j].y - polygon.points[i].y) + polygon.points[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

void Insert(vector<Point> &points, GLdouble x, GLdouble y, Point point) {
    size_t i;
    for (i = 0; i < points.size(); i++) {
        if ((points[i].x == x) && (points[i].y == y)) {
            break;
        }
    }
    while ((i + 1 != points.size()) && (sqrt((point.x-x)*(point.x-x)+(point.y-y)*(point.y-y)) >
          sqrt((points[i+1].x-x)*(points[i+1].x-x)+(points[i+1].y-y)*(points[i+1].y-y)))) {
        i++;
    }
    points.insert(points.begin() + (i + 1), point);
}

void Draw() {
    // Отрисовка фигуры и отверстий в ней.
    for (size_t i = 0; i < figure.size(); i++) {
        DrawLines(figure[i], 0.96f, 0.75f, 0.46f);
    }

    // Отрисовка отсекателя и отверстий в нем.
    for (size_t i = 0; i < clipper.size(); i++) {
        DrawLines(clipper[i], 0.56f, 0.66f, 0.35f);
    }
    
    // Отрисовка результата клиппинга.
    for (size_t i = 0; i < result.size(); i++) {
        DrawLines(result[i], 1.0f, 1.0f, 1.0f);
    }

    if (colors && result.size() != 0) {
        Colorify();
    }
}

void Colorify() {
    // Алгоритм заполнения со списком ребер и флагом.
    for (int y = 0; y < window_height; y++) {
        bool flag = false;
        int x = 0;
        while (x < window_width) {
            int index = GetPixelIndex(x, y);
            GLfloat R = frame_buffer[index];
            GLfloat G = frame_buffer[index + 1];
            GLfloat B = frame_buffer[index + 2];
            if ((R == 1.0f) && (G == 1.0f) && (B == 1.0f)) {
                while (true) {
                    x++;
                    index = GetPixelIndex(x, y);
                    R = frame_buffer[index];
                    G = frame_buffer[index + 1];
                    B = frame_buffer[index + 2];
                    if ((R != 1.0f) || (G != 1.0f) || (B != 1.0f)) {
                        break;
                    }
                }
                flag = !flag;
            }
            if (flag) {
                bool flag1 = false;
                int x_tmp = x;
                while (x_tmp < window_width) {
                    index = GetPixelIndex(x_tmp, y);
                    R = frame_buffer[index];
                    G = frame_buffer[index + 1];
                    B = frame_buffer[index + 2];
                    if ((R == 1.0f) && (G == 1.0f) && (B == 1.0f)) {
                        flag1 = true;
                    }
                    x_tmp++;
                }
                if (flag1) {
                    SetPixelColor(x, y, 1.0f, 1.0f, 1.0f);
                }
            }
            x++;
        }
    }
}

// ##########################################################################################

// ##########################################################################################
// ################################## "Наследие" 4 лабы. ####################################
// ##########################################################################################

void ClearFrameBuffer() {
    if (frame_buffer != nullptr) {
        delete[] frame_buffer;
    }
    int frame_buffer_size = window_width * window_height * 3;
    frame_buffer = new GLfloat[frame_buffer_size];
    for (int i = 0; i < frame_buffer_size; i += 3) {
        frame_buffer[i] = 0.22f;
        frame_buffer[i + 1] = 0.23f;
        frame_buffer[i + 2] = 0.29f;
    }
}

void DrawLines(Polygon polygon, GLfloat R, GLfloat G, GLfloat B) {
    size_t num = polygon.points.size();
    if (num > 0) {
        SetPixelColor(polygon.points[0].x, polygon.points[0].y, R, G, B);
    }
    if (num > 1) {
        for (size_t i = 0; i < num - 1; i++) {
            DrawLine(polygon.points[i].x, polygon.points[i].y, polygon.points[i + 1].x, polygon.points[i + 1].y, R, G, B);
        }
    }
    if (num > 2) {
        DrawLine(polygon.points[num - 1].x, polygon.points[num - 1].y, polygon.points[0].x, polygon.points[0].y, R, G, B);                
    }
}

void DrawLine(GLint x1, GLint y1, GLint x2, GLint y2, GLfloat R, GLfloat G, GLfloat B) {
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
            SetPixelColor(i, y, R, G, B);
        }
        return;
    }
    if (dx == 0) {
        for (GLint i = y1; std::abs(y2 - i) > 0; i += sign_y) {
            SetPixelColor(x, i, R, G, B);
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
        SetPixelColor(x, y, R, G, B);
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
            SetPixelColor(x, y, R, G, B);
            break;
        }
    } 
}


template <typename T> int Sign(T val) {
    return (T(0) < val) - (val < T(0));
}

void SetPixelColor(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
    int index = (window_height - y) * 3 * window_width + 3 * x;
    frame_buffer[index] = r;
    frame_buffer[index + 1] = g;
    frame_buffer[index + 2] = b;
}

int GetPixelIndex(int x, int y) {
    return (window_height - y) * 3 * window_width + 3 * x;
}

// ##########################################################################################
