#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <string>
#include <unordered_map>

// Словарь <имя текстуры, сама текстура>
extern std::unordered_map<std::string, GLuint> textures;

// Загрузка текстуры.
void LoadTexture(std::string texture_name);

#endif  // TEXTURE_HPP
