#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#ifndef GLADAPP_H
#define GLADAPP_H

class GladApp
{
public:
	static bool Init()
	{
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "[ERROR]:Failed to initialize GLAD" << std::endl;
            return false;
        }

        return true;
	}
};

#endif // !GLADAPP_H
