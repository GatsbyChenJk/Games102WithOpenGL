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

	// create GLFW window
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
	std::vector<float> vertices = 
	{
		// --------------axis init----------------
		// positions         // colors
		 10.0f, -5.0f, 0.0f,  0.0f, 0.0f, 0.0f,  
		 -5.0f, -5.0f, 0.0f,  0.0f, 0.0f, 0.0f, 
		 -5.0f,  10.0f, 0.0f,  0.0f, 0.0f, 0.0f  
	};

	std::vector<glm::vec3> functionPoints;
	for (int i = 0;i < 10;i++)
	{
		for (float j = static_cast<float>(i);j < static_cast<float>(i + 1);j += 0.1f)
		{
			functionPoints.push_back(glm::vec3(j, static_cast<float>(sqrt(j)), 0.0f));
			functionPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
	vectorSize = functionPoints.size();
	// position adjustment
	for (auto it = functionPoints.begin();it != functionPoints.end();it+=2)
	{
		it->x -= 5.0f;
		it->y -= 5.0f;
	}

	std::vector<glm::vec2> inputPoints;
	inputPoints.push_back(glm::vec2(1.0f,1.0f));
	inputPoints.push_back(glm::vec2(4.0f,2.0f));
	inputPoints.push_back(glm::vec2(9.0f,3.0f));
	// position adjustment
	for (auto it = inputPoints.begin();it != inputPoints.end();it++)
	{
		it->x -= 5.0f;
		it->y -= 5.0f;
	}
	Interpolation interpolater(inputPoints);
	for (auto &input : inputPoints)
	{
		interpolater.GetEstimateNum2D(input.x, NewTon);
	}
	std::vector<glm::vec3> estimatePoints;
	for (auto it = interpolater.interpolatePoints.begin();it != interpolater.interpolatePoints.end();it++)
	{
		estimatePoints.push_back(glm::vec3(it->x, it->y, 0.0f));
		estimatePoints.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	// ---------------- axis line objects -------------------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO);
	glGenBuffers(1, &GL_VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// -------------------------------------------------------------------------------------------------------------------

	// ------------------ func line objects -------------------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_Func);
	glGenBuffers(1, &GL_VBO_Func);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_Func);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_Func);
	glBufferData(GL_ARRAY_BUFFER, functionPoints.size() * sizeof(glm::vec3) * 2, functionPoints.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	// -------------------------------------------------------------------------------------------------------------------

	// ------------------ interpolate func line objects ------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_Inter);
	glGenBuffers(1, &GL_VBO_Inter);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_Inter);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_Inter);
	glBufferData(GL_ARRAY_BUFFER, estimatePoints.size() * sizeof(glm::vec3) * 2, estimatePoints.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	// -------------------------------------------------------------------------------------------------------------------

	return true;
}

void OpenGLApp::UpdateWindow()
{
	glfwSetFramebufferSizeCallback(GL_window.get(), OpenGLApp::ResizeWindow);

	while (!glfwWindowShouldClose(GL_window.get()))
	{
		// set background color	
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// render
		GL_shader.get()->use();
		// set vertex range by projection matrix
		glm::mat4 projection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, -1.0f, 1.0f);
		GL_shader.get()->setMat4("projection", projection);
		
		glBindVertexArray(GL_VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 3);

		glBindVertexArray(GL_VAO_Func);
		glDrawArrays(GL_LINE_STRIP, 0, vectorSize/2);

		glBindVertexArray(GL_VAO_Inter);
		glDrawArrays(GL_LINE_STRIP, 0, 3);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(GL_window.get());
		glfwPollEvents();
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &GL_VAO);
	glDeleteBuffers(1, &GL_VBO);
	glDeleteVertexArrays(1, &GL_VAO_Func);
	glDeleteBuffers(1, &GL_VBO_Func);
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

