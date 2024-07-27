#pragma once
#ifndef OPENGLAPP_H
#define OPENGLAPP_H

#include "GladApp.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Shader.h"
#include "ImGuiApp.h"
#include "RBFNetwork.h"

class OpenGLApp
{
private:
	int vectorSize;
public:
	int GL_MajorVersion;
	int GL_MinorVersion;
	
	GLuint GL_VAO;                                                                                         // Vertex Array Object For Axis
	GLuint GL_VBO;                                                                                         // Vertex Buffer Object For Axis
	GLuint GL_VAO_Func;                                                                                    // Vertex Array Object For Func Curve
	GLuint GL_VBO_Func;                                                                                    // Vertex Buffer Object For Func Curve 
	GLuint GL_VAO_RBF;																					// Vertex Array Object For Input points (DataSet)
	GLuint GL_VBO_RBF;																					// Vertex Buffer Object For Input points (DataSet)
	GLuint GL_VAO_Estimate;																			// Vertex Array Object For Estimate points
	GLuint GL_VBO_Estimate;																			// Vertex Buffer Object For Estimate points

	glm::mat4 GL_Projection;                                                                               // Projection Matrix
	glm::mat4 GL_Viewing;                                                                                  // Camera Transform (Viewing) Matrix
	
	struct GLFWWindowDeleter                                                                               // Window Pointer Deleter
	{
		void operator()(GLFWwindow* window) const 
		{
			glfwDestroyWindow(window);
		}
	};

	std::unique_ptr<GLFWwindow, GLFWWindowDeleter> GL_window;                                                     // Window Pointer
	std::string GL_windowName;                                                                                    
	int GL_windowHeight;
	int GL_windowWidth;

	std::unique_ptr<Shader, Shader::ShaderDeleter> GL_shader;                                                     // Shader Pointer

	std::unique_ptr<ImGuiApp, ImGuiApp::ImGuiAppDeleter> GL_ImGuiObject;
public:
	OpenGLApp(const std::string& WindowName,int initHeight,int initWidth,int MajorVersion,int MinorVersion);
	~OpenGLApp();
	bool Init();
	bool InitWindow();
	void UpdateWindow();
	// set as static func : cause normal member func dont has implict "this" ptr
	static void ResizeWindow(GLFWwindow* RawWindow,int resizeHeight,int resizeWidth);
	int Run();

private:
	bool InitShader();
	bool InitResources();
	bool InitImGui();
};

#endif // !OPENGLAPP_H
