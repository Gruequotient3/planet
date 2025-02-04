#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

class Shader{
    public:
        unsigned int id;


        Shader();
        Shader (const char* vertexPath, const char* fragmentPath);

        void use();

        // Uniform access fonction
        void setBool(const std::string &name, bool value) const;
        void setFloat(const std::string &name, float value) const;
        void setInt(const std::string &name, int value) const;
        void setMat4(const std::string &name, GLboolean transpose, const GLfloat *value) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);

};






#endif
