#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"

class Shader {
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);
        // use/activate and delete the shader
        void use();
        void del();
        // functions for uniforms
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, glm::vec2 &vector) const;
        void setVec2(const std::string& name, float x, float y) const;
        void setVec3(const std::string& name, glm::vec3 &vector) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setVec4(const std::string& name, glm::vec4 &vector) const;
        void setVec4(const std::string& name, float x, float y, float z, float w) const;
        void setMat2(const std::string& name, glm::mat4& matrix) const;
        void setMat3(const std::string& name, glm::mat4& matrix) const;
        void setMat4(const std::string& name, glm::mat4& matrix) const;
};

#endif
