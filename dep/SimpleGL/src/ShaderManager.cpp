#include "ShaderManager.h"

ShaderProgram* ShaderManager::getShader(std::string shadername) {
    for (ShaderProgram& sp : shaders) {
        if (sp.getName() == shadername) {
            return &sp;
        }
    }
    std::cout << "[WARNING] Shader " << shadername << " was not found\n"; 
    return nullptr;
}

bool ShaderManager::checkShader(std::string shadername) {
    for (ShaderProgram& sp : shaders) {
        if (sp.getName() == shadername) {
            return true;
        }
    }
    return false;
}
void ShaderManager::load_shader(std::string shadername) {
    // check if already loaded
    if (checkShader(shadername)) return;
    ShaderProgram sp;
    sp.load(ShaderProgram::get_shader_file("vertex\\" + shadername + ".glsl").c_str(),
            ShaderProgram::get_shader_file("fragment\\" + shadername + ".glsl").c_str());

    // WARNING: cannot have dynamically allocated members on Shader Program.
    if (sp.isLoaded()) {
        sp.setName(shadername);
        this->shaders.push_back(sp);
        }
}
// void ShaderManager::load_compute_shader(std::string shadername) {
// }

ShaderManager::ShaderManager() {
}