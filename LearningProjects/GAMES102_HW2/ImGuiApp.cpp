#include "ImGuiApp.h"

ImGuiApp::ImGuiApp(const std::string& GuiName, const std::string& ShaderVersion)
	:GL_ImGui_Name(GuiName),
	GL_Shader_Version(ShaderVersion),
	GL_MouseDragSpeed(0.001f),
	GL_MouseRollSpeed(0.01f)
{
}

ImGuiApp::~ImGuiApp()
{
	
}

bool ImGuiApp::Init(GLFWwindow* windowPtr)
{
	if (windowPtr == nullptr)
		return false;
	// set up imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	GL_ImGuiIO.reset(&io);
	// Style
	ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
	ImGui_ImplOpenGL3_Init(GL_Shader_Version.c_str());

	return true;
}

void ImGuiApp::SetDebugTransform(glm::mat4& viewing, glm::mat4& projection)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// get mouse move when dragging
	float yaw = 0.0f;float pitch = 0.0f;
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		yaw += GL_ImGuiIO.get()->MouseDelta.x * GL_MouseDragSpeed;
		pitch += GL_ImGuiIO.get()->MouseDelta.y * GL_MouseDragSpeed;
	}
	glm::vec3 translateVec = glm::vec3(-yaw, pitch, 0.0f);
	viewing = glm::translate(viewing, -translateVec);
	// get mouse middle change when middle key is down 
	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		projection[3][3] -= GL_ImGuiIO.get()->MouseWheel * GL_MouseRollSpeed;
	}

	ImGui::Begin(GL_ImGui_Name.c_str()); 
	// Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / GL_ImGuiIO.get()->Framerate, GL_ImGuiIO.get()->Framerate);
	ImGui::Text("Set Projection Matrix Use Parameters Below:");
	
	ImGui::SliderFloat("Mouse Drag Sensitivity", &GL_MouseDragSpeed, 0.0f, 1.0f);
	ImGui::SliderFloat("Mouse Roll Sensitivity", &GL_MouseRollSpeed, 0.0f, 1.0f);
	ImGui::SliderFloat("Horizontal Scale", &projection[0][0], 0.0f, 10.0f);
	ImGui::SliderFloat("Vertical Scale", &projection[1][1], 0.0f, 10.0f);
	ImGui::SliderFloat("Bottom", &projection[2][2], 0.0f, 10.0f);
	ImGui::SliderFloat("Uniform Scale", &projection[3][3], 0.0f, 10.0f);
	
	if (ImGui::Button("Reset Position"))
	{
		viewing = glm::mat4(1.0f);
		projection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, -1.0f, 1.0f);
	}

	ImGui::End();
	
}

void ImGuiApp::RenderOnWindow(GLFWwindow* windowPtr)
{
	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(windowPtr, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiApp::Destroy()
{	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}