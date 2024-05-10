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

	// points for interpolation
	std::vector<glm::vec2> inputPoints;
	inputPoints.push_back(glm::vec2(1.5f,1.0f));
	inputPoints.push_back(glm::vec2(4.7f,-1.0f));
	inputPoints.push_back(glm::vec2(8.0f,0.1f));

	// position adjustment
	inputPoints = PointAdjuster_2D(inputPoints);
	//--------------------------------------

	Interpolation interpolater(inputPoints);
	for (auto &input : inputPoints)
	{
		interpolater.GetEstimateNum2D(input.x, NewTon);
	}
	std::vector<glm::vec3> estimatePointsForInter;
	for (auto it = interpolater.interpolatePoints.begin();it != interpolater.interpolatePoints.end();it++)
	{
		estimatePointsForInter.push_back(glm::vec3(it->x, it->y, 0.0f));
		estimatePointsForInter.push_back(glm::vec3(0.0f, 1.0f, 0.5f));
	}

   // points for fitting
	std::vector<glm::vec2> discretePoints =
	{
		 glm::vec2(0.1f, 0.7f),
		 glm::vec2(0.7f, 1.8f),
		 glm::vec2(1.6f, 2.7f),
		 glm::vec2(2.5f, 3.9f),
		 glm::vec2(3.3f, 5.2f),
		 glm::vec2(4.5f, 6.3f),
		 glm::vec2(5.0f, 6.7f),
		 glm::vec2(6.2f, 7.1f),
		 glm::vec2(7.4f, 8.4f),
		 glm::vec2(8.0f, 9.2f),
		 glm::vec2(9.6f, 11.0f),
		 glm::vec2(11.8f, 13.0f)
	};
	// position adjustment
	discretePoints = PointAdjuster_2D(discretePoints);
	//--------------------------------------
	Fitting Fitter(discretePoints);
	for (auto& input : discretePoints)
	{
		Fitter.GetEstimateNum2D(input.x, Linear);
	}
	std::vector<glm::vec3> estimatePointsForFitting;
	for (auto it = Fitter.fittingPoints.begin();it != Fitter.fittingPoints.end();it++)
	{
		estimatePointsForFitting.push_back(glm::vec3(it->x, it->y, 0.0f));
		estimatePointsForFitting.push_back(glm::vec3(1.0f, 0.5f, 0.5f));
	}

	std::vector<glm::vec3> originalDiscretePoints;
	for (auto it = discretePoints.begin();it != discretePoints.end();it++)
	{
		originalDiscretePoints.push_back(glm::vec3(it->x, it->y, 0.0f));
		originalDiscretePoints.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	}

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

	// ------------------ interpolate func line objects ------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_Inter);
	glGenBuffers(1, &GL_VBO_Inter);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_Inter);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_Inter);
	glBufferData(GL_ARRAY_BUFFER, estimatePointsForInter.size() * sizeof(glm::vec3) * 2, estimatePointsForInter.data(), GL_STATIC_DRAW);
	glPointSize(8.0f);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	// -------------------------------------------------------------------------------------------------------------------

	// ------------------ discrete point objects ------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_Dis);
	glGenBuffers(1, &GL_VBO_Dis);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_Dis);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_Dis);
	glBufferData(GL_ARRAY_BUFFER, originalDiscretePoints.size() * sizeof(glm::vec3) * 2, originalDiscretePoints.data(), GL_STATIC_DRAW);
	glPointSize(8.0f);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	// -------------------------------------------------------------------------------------------------------------------
	
	// ------------------ fit line objects ------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_Fit);
	glGenBuffers(1, &GL_VBO_Fit);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_Fit);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_Fit);
	glBufferData(GL_ARRAY_BUFFER, estimatePointsForFitting.size() * sizeof(glm::vec3) * 2, estimatePointsForFitting.data(), GL_STATIC_DRAW);
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

		glBindVertexArray(GL_VAO_Inter);
		glDrawArrays(GL_POINTS, 0, 3);
		glDrawArrays(GL_LINE_STRIP, 0, 3);

		glBindVertexArray(GL_VAO_Dis);
		glDrawArrays(GL_POINTS, 0, 12);

		glBindVertexArray(GL_VAO_Fit);
		glDrawArrays(GL_LINE_STRIP, 0, 12);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		
		glfwPollEvents();
		glfwSwapBuffers(GL_window.get());
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &GL_VAO);
	glDeleteBuffers(1, &GL_VBO);
	glDeleteVertexArrays(1, &GL_VAO_Func);
	glDeleteBuffers(1, &GL_VBO_Func);
	glDeleteVertexArrays(1, &GL_VAO_Inter);
	glDeleteBuffers(1, &GL_VBO_Inter);
	glDeleteVertexArrays(1, &GL_VAO_Dis);
	glDeleteBuffers(1, &GL_VBO_Dis);
	glDeleteVertexArrays(1, &GL_VAO_Fit);
	glDeleteBuffers(1, &GL_VBO_Fit);

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

