#ifndef SHADER_CPP
#define SHADER_CPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.hpp"


Shader::Shader() : id{0} { }
Shader::Shader(const char *vertexPath, const char *fragmentPath){
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Get Vertex/Fragment source code
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        // Open Files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's bufer contents into stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //close file
        vShaderFile.close();
        fShaderFile.close();
        // convert stream to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure &e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    //Compile Shader
    unsigned int vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Check compile status
    CheckCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Check compile status
    CheckCompileErrors(fragment, "FRAGMENT");

    // Create Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    // Check Link Status
    CheckCompileErrors(id, "PROGRAM");

    // Delete shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Destroy(){
    if (id) { glDeleteProgram(id); id = 0; }
}


void Shader::Use(){
    glUseProgram(id);
}

void Shader::SetBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloatp(const std::string &name, int count, float* value) const{
    glUniform1fv(glGetUniformLocation(id, name.c_str()), count, value);
}

void Shader::SetInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetIvec2(const std::string &name, glm::ivec2 value) const{
    glUniform2i(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void Shader::SetVec3(const std::string &name, glm::vec3 value) const{
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec3p(const std::string &name, int count, const float* value) const{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), count, value);
}

void Shader::SetMat4(const std::string &name, GLboolean transpose, const GLfloat *value) const{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, transpose, value);
}


void Shader::CheckCompileErrors(unsigned int shader, std::string type){
    int success;
    char infoLog[512];
    if (type != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;

        }
    }
}

void ShaderList::Destroy(){
    std::vector<Shader>::iterator it;
    for (it = shaders.begin(); it != shaders.end(); ++it){
        it->Destroy();
    }
    shaders.clear();
}

void ShaderList::LoadShader(){
    Destroy();
    shaders.push_back(Shader{"res/shaders/shader.vert", "res/shaders/planet.frag"});
    shaders.push_back(Shader{"res/shaders/shader.vert", "res/shaders/outlinePlanet.frag"});
    shaders.push_back(Shader{"res/shaders/shader.vert", "res/shaders/text.frag"});
    shaders.push_back(Shader{"res/shaders/shader.vert", "res/shaders/sky.frag"});
    shaders.push_back(Shader{"res/shaders/fade.vert", "res/shaders/fade.frag"});
}

Shader &ShaderList::operator[](int pos){
    if (pos < 0 || pos >= (int)shaders.size()) exit(EXIT_FAILURE);
    return shaders[pos];
}




#endif
