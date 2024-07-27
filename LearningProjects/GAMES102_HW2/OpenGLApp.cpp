#pragma once
#include "OpenGLApp.h"

// packging implement

OpenGLApp::OpenGLApp(const std::string& WindowName, int initWidth, int initHeight, int MajorVersion, int MinorVersion)
	:GL_windowName(WindowName), 
	GL_windowHeight(initHeight), 
	GL_windowWidth(initWidth),
	GL_MajorVersion(MajorVersion),
	GL_MinorVersion(MinorVersion),
	GL_Projection(glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, -1.0f, 1.0f)),
	GL_Viewing(glm::mat4(1.0f))
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
	if (!InitImGui())
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

bool OpenGLApp::InitImGui()
{
	GL_ImGuiObject = ImGuiApp::createImGuiPtr("ImGui Out!", "#version 150");
	if (GL_ImGuiObject != nullptr)
		return true;
	else
		return false;
}

bool OpenGLApp::InitResources()
{
	auto PointAdjuster_2D = [](std::vector<glm::vec2>& input)
	{
		for (auto it = input.begin();it != input.end();it++)
		{
			it->x -= 5.0f;
			it->y -= 5.0f;
		}
		return input;
	};

	auto PointAdjuster_3D = [](std::vector<glm::vec3>& input)
	{
		for (auto it = input.begin();it != input.end();it += 2)
		{
			it->x -= 5.0f;
			it->y -= 5.0f;
		}
		return input;
	};
	
	// --------------axis init----------------
	std::vector<glm::vec3> vertices = 
	{	      		          
		 glm::vec3(10.0f, -5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),  
		 glm::vec3(-5.0f, -5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		 glm::vec3(-5.0f, 10.0f, 0.0f), glm::vec3( 0.0f, 0.0f, 0.0f)
	};
	// generate func points which want to be interpolated
	std::vector<glm::vec3> functionPoints;
	for (int i = 0;i < 10;i++)
	{
		for (float j = static_cast<float>(i);j < static_cast<float>(i + 1);j += 0.1f)
		{
			functionPoints.push_back(glm::vec3(j, static_cast<float>(sin(j)), 0.0f));
			functionPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
	vectorSize = static_cast<int>(functionPoints.size());
	// position adjustment
	functionPoints = PointAdjuster_3D(functionPoints);
	//--------------------------------------
	

	// ---------------- axis line objects -------------------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO);
	glGenBuffers(1, &GL_VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) * 2, vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	// -------------------------------------------------------------------------------------------------------------------

	// ------------------ func line objects -------------------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_Func);
	glGenBuffers(1, &GL_VBO_Func);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_Func);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_Func);
	glBufferData(GL_ARRAY_BUFFER, functionPoints.size() * sizeof(glm::vec3) * 2, functionPoints.data(), GL_STATIC_DRAW);
	glLineWidth(2.5f);
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
	
	GL_ImGuiObject.get()->Init(GL_window.get());

	while (!glfwWindowShouldClose(GL_window.get()))
	{		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		GL_ImGuiObject.get()->SetDebugTransform(GL_Viewing, GL_Projection);
		GL_ImGuiObject.get()->RenderOnWindow(GL_window.get());
		// render
		GL_shader.get()->use();
		// set vertex range by projection matrix		
		GL_shader.get()->setMat4("viewing", GL_Viewing);
		GL_shader.get()->setMat4("projection", GL_Projection);
		// draw
		glBindVertexArray(GL_VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 3);

		glBindVertexArray(GL_VAO_Func);
		glDrawArrays(GL_LINE_STRIP, 0, vectorSize / 2);

		
		glfwPollEvents();
		glfwSwapBuffers(GL_window.get());
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &GL_VAO);
	glDeleteBuffers(1, &GL_VBO);
	glDeleteVertexArrays(1, &GL_VAO_Func);
	glDeleteBuffers(1, &GL_VBO_Func);

	GL_ImGuiObject.get()->Destroy();
	glfwTerminate();

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
	std::cout << "duration time :" << timeDuration.count()/static_cast<float>(10e9) <<"s" << std::endl;
	return 0;
}

