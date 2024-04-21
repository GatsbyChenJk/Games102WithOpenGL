#pragma once
#include "OpenGLApp.h"

// packging implement

OpenGLApp::OpenGLApp(const std::string& WindowName, int initWidth, int initHeight, int MajorVersion, int MinorVersion)
	:GL_windowName(WindowName), 
	GL_windowHeight(initHeight), 
	GL_windowWidth(initWidth),
	GL_MajorVersion(MajorVersion),
	GL_MinorVersion(MinorVersion)
{
}

OpenGLApp::~OpenGLApp()
{
}

bool OpenGLApp::Init()
{
	if (!InitWindow())
		return false;
	if (!GladApp::Init())
		return false;
	if (!InitShader())
		return false;
	if (!InitResources())
		return false;

	return true;
}

bool OpenGLApp::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建 GLFW 窗口
	GLFWwindow* rawWindow = glfwCreateWindow(GL_windowWidth, GL_windowHeight, GL_windowName.c_str(), nullptr, nullptr);
	if (!rawWindow) 
	{
		std::cerr << "[ERROR]:Failed to create GLFW window" << std::endl;
		return false;
	}
	// IMPORTANT!! Call this func to ensure that GLAD can be initialized
	glfwMakeContextCurrent(rawWindow);
	//------------------------------------------------------------------
	GL_window.reset(rawWindow);
	if (GL_window != nullptr)
		return true;
	else
		return false;
}

bool OpenGLApp::InitShader()
{
	GL_shader = Shader::create("vertexCode.txt", "fragmentCode.txt");
	if (GL_shader != nullptr)
		return true;
	else
		return false;
}

bool OpenGLApp::InitResources()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	glGenVertexArrays(1, &GL_VAO);
	glGenBuffers(1, &GL_VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return true;
}

void OpenGLApp::UpdateWindow()
{
	glfwSetFramebufferSizeCallback(GL_window.get(), OpenGLApp::ResizeWindow);

	while (!glfwWindowShouldClose(GL_window.get()))
	{
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render the triangle
		GL_shader.get()->use();
		//ourShader.setFloat("offset", 0.3f);
		glBindVertexArray(GL_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(GL_window.get());
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &GL_VAO);
	glDeleteBuffers(1, &GL_VBO);
}

void OpenGLApp::ResizeWindow(GLFWwindow* RawWindow, int resizeWidth, int resizeHeight)
{
	glViewport(0, 0, resizeWidth, resizeHeight);
}

int OpenGLApp::Run()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	this->UpdateWindow();
	auto endTime = std::chrono::high_resolution_clock::now();
	auto timeDuration = endTime - startTime;
	std::cout << "duration time :" << timeDuration.count() <<"ns" << std::endl;
	return 0;
}

