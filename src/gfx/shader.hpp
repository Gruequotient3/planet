#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader{
    public:
        unsigned int id;

        Shader();
        Shader (const char* vertexPath, const char* fragmentPath);
        ~Shader();
        void use();

        // Uniform access fonction
        void setBool(const std::string &name, bool value) const;
        void setFloat(const std::string &name, float value) const;
        void setFloatp(const std::string &name, int count, float* value) const;
        void setInt(const std::string &name, int value) const;
        void setVec3(const std::string &name, glm::vec3 value) const;
        void setVec3p(const std::string &name, int count, const float* value) const;
        void setMat4(const std::string &name, GLboolean transpose, const GLfloat *value) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);

};






#endif
