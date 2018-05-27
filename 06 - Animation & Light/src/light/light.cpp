#include "light.hpp"

void Light(const JSONConfig &json_config) {
    PointLight(json_config);
    GlobalLight(json_config);
}

void PointLight(const JSONConfig &json_config) {
    // Создание источника света.
    GLfloat light_position[] = { json_config.light_position.X,
                                 json_config.light_position.Y,
                                 json_config.light_position.Z,
                                 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat light_ambient[] = { json_config.light_ambient.R,
                                json_config.light_ambient.G,
                                json_config.light_ambient.B,
                                json_config.light_ambient.A };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    GLfloat light_diffuse[] = { json_config.light_diffuse.R,
                                json_config.light_diffuse.G,
                                json_config.light_diffuse.B,
                                json_config.light_diffuse.A };
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    GLfloat light_specular[] = { json_config.light_specular.R,
                                 json_config.light_specular.G,
                                 json_config.light_specular.B,
                                 json_config.light_specular.A };
    
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, json_config.light_attenuation.X);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, json_config.light_attenuation.Y);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, json_config.light_attenuation.Z);
}

void GlobalLight(const JSONConfig &json_config) {
    // Создание глобального света.
    GLfloat global_light_ambient[] = { json_config.global_light_ambient.R,
                                       json_config.global_light_ambient.G,
                                       json_config.global_light_ambient.B,
                                       json_config.global_light_ambient.A };
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_light_ambient);
}
