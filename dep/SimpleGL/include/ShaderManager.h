#include <glad/glad.h>  // Manages function pointers
#include "ShaderProgram.h"
#include <vector>
#include <string>
class ShaderManager {
private:
    std::vector<ShaderProgram> shaders;
public:
    ShaderProgram* getShader(std::string shadername);
    bool checkShader(std::string shadername);

    void load_shader(std::string shadername);
    //void load_compute_shader(std::string shadername);
    //bool unload_shader(std::string shadername);
    ShaderManager();
};