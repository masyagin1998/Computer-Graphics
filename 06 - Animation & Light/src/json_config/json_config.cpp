#include "json_config.hpp"

#include <cinttypes>

#include <fstream>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

JSONConfig::JSONConfig(std::string filename) : 
    start_filename(filename),
    was_error(false) {
    if (filename == "") {
        was_error = true;
        return;
    }
    
    // Создание JSON-дерева.
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        was_error = true;
        return;
    }
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    boost::property_tree::ptree json_tree;
    boost::property_tree::read_json(stream, json_tree);

    // Чтение конфигов.

    // Показывать/не показывать анимацию.
    show_animation = json_tree.get<bool>("showAnimation");
    
    show_textures = json_tree.get<bool>("showTextures");
    
    // Конфиг окна.
    window_name = json_tree.get<std::string>("window.windowName");
    screen_width = json_tree.get<GLint>("window.screenWidth");
    screen_height = json_tree.get<GLint>("window.screenHeight");
    window_width = json_tree.get<GLint>("window.windowWidth");
    window_height = json_tree.get<GLint>("window.windowHeight");
    is_resizable = json_tree.get<bool>("window.isResizable");
    
    clear_color.R = json_tree.get<GLfloat>("window.color.R");
    clear_color.G = json_tree.get<GLfloat>("window.color.G");
    clear_color.B = json_tree.get<GLfloat>("window.color.B");
    clear_color.A = json_tree.get<GLfloat>("window.color.A");

    // Конфиг коробки.
    ltn.X = json_tree.get<GLfloat>("box.ltn.X");
    ltn.Y = json_tree.get<GLfloat>("box.ltn.Y");
    ltn.Z = json_tree.get<GLfloat>("box.ltn.Z");
    
    rbf.X = json_tree.get<GLfloat>("box.rbf.X");
    rbf.Y = json_tree.get<GLfloat>("box.rbf.Y");
    rbf.Z = json_tree.get<GLfloat>("box.rbf.Z");

    box_color.R = json_tree.get<GLfloat>("box.color.R");
    box_color.G = json_tree.get<GLfloat>("box.color.G");
    box_color.B = json_tree.get<GLfloat>("box.color.B");
    box_color.A = json_tree.get<GLfloat>("box.color.A");

    box_texture = json_tree.get<std::string>("box.texture");

    // Конфиг пружины.
    spring_radius = json_tree.get<GLfloat>("spring.sizes.springRadius");
    wire_radius = json_tree.get<GLfloat>("spring.sizes.wireRadius");
    pressure_coefficient = json_tree.get<GLfloat>("spring.sizes.pressureCoefficient");
    number_of_coils = json_tree.get<GLint>("spring.sizes.numberOfCoils");

    quality_wire = json_tree.get<GLint>("spring.quality.qualityWire");
    quality_spring = json_tree.get<GLint>("spring.quality.qualitySpring");

    spring_translation.X = json_tree.get<GLfloat>("spring.translation.X");
    spring_translation.Y = json_tree.get<GLfloat>("spring.translation.Y");
    spring_translation.Z = json_tree.get<GLfloat>("spring.translation.Z");

    speed_translation.X = json_tree.get<GLfloat>("spring.speed_translation.X");
    speed_translation.Y = json_tree.get<GLfloat>("spring.speed_translation.Y");
    speed_translation.Z = json_tree.get<GLfloat>("spring.speed_translation.Z");

    model_ambient.R = json_tree.get<GLfloat>("spring.model.ambient.R");
    model_ambient.G = json_tree.get<GLfloat>("spring.model.ambient.G");
    model_ambient.B = json_tree.get<GLfloat>("spring.model.ambient.B");
    model_ambient.A = json_tree.get<GLfloat>("spring.model.ambient.A");

    model_diffuse.R = json_tree.get<GLfloat>("spring.model.diffuse.R");
    model_diffuse.G = json_tree.get<GLfloat>("spring.model.diffuse.G");
    model_diffuse.B = json_tree.get<GLfloat>("spring.model.diffuse.B");
    model_diffuse.A = json_tree.get<GLfloat>("spring.model.diffuse.A");

    model_specular.R = json_tree.get<GLfloat>("spring.model.specular.R");
    model_specular.G = json_tree.get<GLfloat>("spring.model.specular.G");
    model_specular.B = json_tree.get<GLfloat>("spring.model.specular.B");
    model_specular.A = json_tree.get<GLfloat>("spring.model.specular.A");

    model_emission.R = json_tree.get<GLfloat>("spring.model.emission.R");
    model_emission.G = json_tree.get<GLfloat>("spring.model.emission.G");
    model_emission.B = json_tree.get<GLfloat>("spring.model.emission.B");
    model_emission.A = json_tree.get<GLfloat>("spring.model.emission.A");


    shineness = json_tree.get<GLfloat>("spring.model.shineness");

    spring_texture = json_tree.get<std::string>("spring.texture");

    // Конфиг источника света.
    light_position.X = json_tree.get<GLfloat>("light.position.X");
    light_position.Y = json_tree.get<GLfloat>("light.position.Y");
    light_position.Z = json_tree.get<GLfloat>("light.position.Z");

    light_ambient.R = json_tree.get<GLfloat>("light.ambient.R");
    light_ambient.G = json_tree.get<GLfloat>("light.ambient.G");
    light_ambient.B = json_tree.get<GLfloat>("light.ambient.B");
    light_ambient.A = json_tree.get<GLfloat>("light.ambient.A");

    light_diffuse.R = json_tree.get<GLfloat>("light.diffuse.R");
    light_diffuse.G = json_tree.get<GLfloat>("light.diffuse.G");
    light_diffuse.B = json_tree.get<GLfloat>("light.diffuse.B");
    light_diffuse.A = json_tree.get<GLfloat>("light.diffuse.A");

    light_specular.R = json_tree.get<GLfloat>("light.specular.R");
    light_specular.G = json_tree.get<GLfloat>("light.specular.G");
    light_specular.B = json_tree.get<GLfloat>("light.specular.B");
    light_specular.A = json_tree.get<GLfloat>("light.specular.A");

    light_attenuation.X = json_tree.get<GLfloat>("light.constantAttenuation");
    light_attenuation.Y = json_tree.get<GLfloat>("light.linearAttenuation");
    light_attenuation.Z = json_tree.get<GLfloat>("light.quadraticAttenuation");
    
    speed_change.X = json_tree.get<GLfloat>("light.speedChange.X");
    speed_change.Y = json_tree.get<GLfloat>("light.speedChange.Y");
    speed_change.Z = json_tree.get<GLfloat>("light.speedChange.Z");

    // Конфиг глобального света.
    global_light_ambient.R = json_tree.get<GLfloat>("globalLight.ambient.R");
    global_light_ambient.G = json_tree.get<GLfloat>("globalLight.ambient.G");
    global_light_ambient.B = json_tree.get<GLfloat>("globalLight.ambient.B");
    global_light_ambient.A = json_tree.get<GLfloat>("globalLight.ambient.A");
}

void JSONConfig::Save(std::string filename) {
    // Создание JSON-дерева.
    if (filename == "") {
        filename = start_filename;
    }
    boost::property_tree::ptree json_tree;

    // Показывать/не показывать анимацию.
    json_tree.put("showAnimation", show_animation);

    // Показывать/не показывать текстуры.
    json_tree.put("showTextures", show_textures);

    // Конфиг окна.
    json_tree.put("window.windowName", window_name);
    json_tree.put("window.screenWidth", screen_width);
    json_tree.put("window.screenHeight", screen_height);
    json_tree.put("window.windowWidth", window_width);
    json_tree.put("window.windowHeight", window_height);
    json_tree.put("window.isResizable", is_resizable);

    json_tree.put("window.color.R", clear_color.R);
    json_tree.put("window.color.G", clear_color.G);
    json_tree.put("window.color.B", clear_color.B);
    json_tree.put("window.color.A", clear_color.A);

    // Конфиг коробки.
    json_tree.put("box.ltn.X", ltn.X);
    json_tree.put("box.ltn.Y", ltn.Y);
    json_tree.put("box.ltn.Z", ltn.Z);
    
    json_tree.put("box.rbf.X", rbf.X);
    json_tree.put("box.rbf.Y", rbf.Y);
    json_tree.put("box.rbf.Z", rbf.Z);
        
    json_tree.put("box.color.R", box_color.R);
    json_tree.put("box.color.G", box_color.G);
    json_tree.put("box.color.B", box_color.B);
    json_tree.put("box.color.A", box_color.A);

    json_tree.put("box.texture", box_texture);

    // Конфиг пружины.
    json_tree.put("spring.sizes.springRadius", spring_radius);
    json_tree.put("spring.sizes.wireRadius", wire_radius);
    json_tree.put("spring.sizes.pressureCoefficient", pressure_coefficient);
    json_tree.put("spring.sizes.numberOfCoils", number_of_coils);

    json_tree.put("spring.quality.qualityWire", quality_wire);
    json_tree.put("spring.quality.qualitySpring", quality_spring);

    json_tree.put("spring.translation.X", spring_translation.X);
    json_tree.put("spring.translation.Y", spring_translation.Y);
    json_tree.put("spring.translation.Z", spring_translation.Z);

    json_tree.put("spring.speed_translation.X", speed_translation.X);
    json_tree.put("spring.speed_translation.Y", speed_translation.Y);
    json_tree.put("spring.speed_translation.Z", speed_translation.Z);

    json_tree.put("spring.texture", spring_texture);

    json_tree.put("spring.model.ambient.R", model_ambient.R);
    json_tree.put("spring.model.ambient.G", model_ambient.G);
    json_tree.put("spring.model.ambient.B", model_ambient.B);
    json_tree.put("spring.model.ambient.A", model_ambient.A);

    json_tree.put("spring.model.diffuse.R", model_diffuse.R);
    json_tree.put("spring.model.diffuse.G", model_diffuse.G);
    json_tree.put("spring.model.diffuse.B", model_diffuse.B);
    json_tree.put("spring.model.diffuse.A", model_diffuse.A);

    json_tree.put("spring.model.specular.R", model_specular.R);
    json_tree.put("spring.model.specular.G", model_specular.G);
    json_tree.put("spring.model.specular.B", model_specular.B);
    json_tree.put("spring.model.specular.A", model_specular.A);

    json_tree.put("spring.model.emission.R", model_emission.R);
    json_tree.put("spring.model.emission.G", model_emission.G);
    json_tree.put("spring.model.emission.B", model_emission.B);
    json_tree.put("spring.model.emission.A", model_emission.A);

    json_tree.put("spring.model.shineness", shineness);

    // Конфиг источника света.
    json_tree.put("light.position.X", light_position.X);
    json_tree.put("light.position.Y", light_position.Y);
    json_tree.put("light.position.Z", light_position.Z);

    json_tree.put("light.ambient.R", light_ambient.R);
    json_tree.put("light.ambient.G", light_ambient.G);
    json_tree.put("light.ambient.B", light_ambient.B);
    json_tree.put("light.ambient.A", light_ambient.A);

    json_tree.put("light.diffuse.R", light_diffuse.R);
    json_tree.put("light.diffuse.G", light_diffuse.G);
    json_tree.put("light.diffuse.B", light_diffuse.B);
    json_tree.put("light.diffuse.A", light_diffuse.A);

    json_tree.put("light.specular.R", light_specular.R);
    json_tree.put("light.specular.G", light_specular.G);
    json_tree.put("light.specular.B", light_specular.B);
    json_tree.put("light.specular.A", light_specular.A);

    json_tree.put("light.constantAttenuation", light_attenuation.X);
    json_tree.put("light.linearAttenuation", light_attenuation.Y);
    json_tree.put("light.quadraticAttenuation", light_attenuation.Z);

    json_tree.put("light.speedChange.X", speed_change.X);
    json_tree.put("light.speedChange.Y", speed_change.Y);
    json_tree.put("light.speedChange.Z", speed_change.Z);    

    // Конфиг глобального света.
    json_tree.put("globalLight.ambient.R", global_light_ambient.R);
    json_tree.put("globalLight.ambient.G", global_light_ambient.G);
    json_tree.put("globalLight.ambient.B", global_light_ambient.B);
    json_tree.put("globalLight.ambient.A", global_light_ambient.A);

    // Запись конфигов.
    std::ofstream file(filename);
    boost::property_tree::write_json(file, json_tree);
    file.close();

    std::cout << "Config was successfully saved to: " << filename << std::endl;
    std::cout << std::endl;
}

void JSONConfig::Debug() {
    std::cout << "Config filename: " << start_filename << std::endl;
    std::cout << std::endl;
    
    std::cout << "Window config:" << std::endl;
    std::cout << "Screen Width:     " << screen_width << std::endl;
    std::cout << "Screen Height:    " << screen_height << std::endl;
    std::cout << "Window Width:     " << window_width << std::endl;
    std::cout << "Window Width:     " << window_width << std::endl;
    std::cout << "Is resizable:     " << is_resizable << std::endl;
    std::cout << "Clear color:      " << "(" <<
        clear_color.R << "; " <<
        clear_color.G << "; " <<
        clear_color.B << "; " <<
        clear_color.A << ")" << std::endl;
    std::cout << std::endl;
}
