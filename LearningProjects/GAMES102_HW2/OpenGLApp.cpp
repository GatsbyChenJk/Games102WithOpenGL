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
		 glm::vec3(-5.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)
	};

	// generate func points which want to be interpolated
	std::vector<glm::vec3> functionPoints;
	for (int i = 0;i < 10;i++)
	{
		for (float j = static_cast<float>(i);j < static_cast<float>(i + 1);j += 0.1f)
		{
			functionPoints.push_back(glm::vec3(j, static_cast<float>(exp(sin(j) + log(j))), 0.0f));  // position
			functionPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));												  // color : dark
		}
	}
	vectorSize = static_cast<int>(functionPoints.size());

	// debug point vector
	vector<glm::vec2> debugPoints;
	debugPoints.push_back(glm::vec2(functionPoints[0].x, functionPoints[0].y));
	debugPoints.push_back(glm::vec2(functionPoints[14].x, functionPoints[14].y));
	debugPoints.push_back(glm::vec2(functionPoints[20].x, functionPoints[20].y));
	debugPoints.push_back(glm::vec2(functionPoints[28].x, functionPoints[28].y));
	debugPoints.push_back(glm::vec2(functionPoints[46].x, functionPoints[46].y));
	debugPoints.push_back(glm::vec2(functionPoints[60].x, functionPoints[60].y));
	debugPoints.push_back(glm::vec2(functionPoints[74].x, functionPoints[74].y));
	debugPoints.push_back(glm::vec2(functionPoints[88].x, functionPoints[88].y));
	debugPoints = PointAdjuster_2D(debugPoints);

	std::vector<glm::vec3> debugPointsForRender;
	for (auto it = debugPoints.begin();it != debugPoints.end();it++)
	{
		debugPointsForRender.push_back(glm::vec3(it->x, it->y, 0.0f));
		debugPointsForRender.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	}
	// -------------------------------------------------------------------------------------------------------------------

	// create dataset for training
	vector<vector<double>> input, output;
	input.push_back({ functionPoints[0].x });
	input.push_back({ functionPoints[14].x });
	input.push_back({ functionPoints[20].x });
	input.push_back({ functionPoints[28].x });
	input.push_back({ functionPoints[46].x });
	input.push_back({ functionPoints[60].x });
	input.push_back({ functionPoints[74].x });
	output.push_back({ functionPoints[88].x });

	output.push_back({ functionPoints[0].y });
	output.push_back({ functionPoints[14].y });
	output.push_back({ functionPoints[20].y });
	output.push_back({ functionPoints[28].y });
	output.push_back({ functionPoints[46].y });
	output.push_back({ functionPoints[60].y });
	output.push_back({ functionPoints[74].y });
	output.push_back({ functionPoints[88].y });

	// -------------------------------------------------------------------------------------------------------------------
	// create a nerual network with one-dimension input and output along with 5 node hidden layer
	RBFNetwork RBFNetFunc(1, 5, 1);
	RBFNetFunc.train(input,output,0.01);
	// -------------------------------------------------------------------------------------------------------------------
	
	// generate Netsfunc points which want to be interpolated
	std::vector<glm::vec3> NetfunctionPoints;
	for (int i = 0;i < 10;i++)
	{
		for (float j = static_cast<float>(i);j < static_cast<float>(i + 1);j += 0.1f)
		{
			vector<double> testInput = { j };
			vector<double> estimateValue;
			// get estimate value by trained network
			RBFNetFunc.feedForward(testInput, estimateValue);
			NetfunctionPoints.push_back(glm::vec3(testInput[0], estimateValue[0], 0.0f));		// estimate position
			NetfunctionPoints.push_back(glm::vec3(0.0f, 1.0f, 0.0f));									// color : green
		}
	}
	vectorSize = static_cast<int>(functionPoints.size());
	// position adjustment
	functionPoints = PointAdjuster_3D(functionPoints);
	NetfunctionPoints = PointAdjuster_3D(NetfunctionPoints);
	//-------------------------------------------------------------------------------------------------------------------

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

	// ------------------ discrete point objects ------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_RBF);
	glGenBuffers(1, &GL_VBO_RBF);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_RBF);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_RBF);
	glBufferData(GL_ARRAY_BUFFER, debugPointsForRender.size() * sizeof(glm::vec3) * 2, debugPointsForRender.data(), GL_STATIC_DRAW);
	glPointSize(8.0f);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);
	// -------------------------------------------------------------------------------------------------------------------

	// ------------------Net func line objects -------------------------------------------------------------------------------
	glGenVertexArrays(1, &GL_VAO_Estimate);
	glGenBuffers(1, &GL_VBO_Estimate);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(GL_VAO_Estimate);

	glBindBuffer(GL_ARRAY_BUFFER, GL_VBO_Estimate);
	glBufferData(GL_ARRAY_BUFFER, NetfunctionPoints.size() * sizeof(glm::vec3) * 2, NetfunctionPoints.data(), GL_STATIC_DRAW);
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

		glBindVertexArray(GL_VAO_Estimate);
		glDrawArrays(GL_LINE_STRIP, 0, vectorSize / 2);

		glBindVertexArray(GL_VAO_RBF);
		glDrawArrays(GL_POINTS, 0, 8);
		
		glfwPollEvents();
		glfwSwapBuffers(GL_window.get());
	}
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &GL_VAO);
	glDeleteBuffers(1, &GL_VBO);
	glDeleteVertexArrays(1, &GL_VAO_Func);
	glDeleteBuffers(1, &GL_VBO_Func);
	glDeleteVertexArrays(1, &GL_VAO_RBF);
	glDeleteBuffers(1, &GL_VBO_RBF);
	glDeleteVertexArrays(1, &GL_VAO_Estimate);
	glDeleteBuffers(1, &GL_VBO_Estimate);

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

