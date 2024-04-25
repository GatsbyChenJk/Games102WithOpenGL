#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;
    Shader() = default;
    ~Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath);	
    // 自定义deleter
    struct ShaderDeleter 
    {
        void operator()(Shader* shader) const 
        {
            delete shader;
        }
    };

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4 matValue) const;
    // use this func avoid delete error when delete the pointer of shader class 
    static std::unique_ptr<Shader, ShaderDeleter> create(const char* vertexPath, const char* fragmentPath)
    {
        return std::unique_ptr<Shader, ShaderDeleter>(new Shader(vertexPath, fragmentPath));
    }
    // ---------------------------------------------------------------------------
private:
	void checkCompileErrors(unsigned int shader, std::string type);	
};

#endif
