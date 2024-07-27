#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLApp.h"

int main()
{
    // set window name  
    std::string AppName = "Man,what can I say? RBF Neural Network OUT!";
    // set basic info
    OpenGLApp myOpenGLApp(AppName, 1000, 800, 3, 3);
    // initialization
    if(!myOpenGLApp.Init())
        return 0;   
    // execute
    return myOpenGLApp.Run();
}


