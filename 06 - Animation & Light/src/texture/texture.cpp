#include "texture.hpp"

std::unordered_map<std::string, GLuint> textures;

void LoadTexture(std::string texture_name) {
    // Загрузка изображения.
    int width, height;
    unsigned char* image = SOIL_load_image(texture_name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    
    // Генерация текстуры.
    GLuint texture;
    glGenTextures(1, &texture);

    // Привязка текстуры.
    glBindTexture(GL_TEXTURE_2D, texture);

    // Настройки.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Установка текстуры.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    
    // Очищаем память программы.
    SOIL_free_image_data(image);
    // Отвязываем текстуру.
    glBindTexture(GL_TEXTURE_2D, 0);

    // Запоминаем текстуру.
    textures[texture_name] = texture;
}
