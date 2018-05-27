#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../json_config/json_config.hpp"

// Инициализация света.
void Light(const JSONConfig &json_config);
// Точечный свет (GL_LIGHT0)
void PointLight(const JSONConfig &json_config);
// Глобальный свет (GL_LIGHT_MODEL_*)
void GlobalLight(const JSONConfig &json_config);

#endif  // LIGHT_HPP
