#include "ImGuiApp.h"

ImGuiApp::ImGuiApp(const std::string& GuiName, const std::string& ShaderVersion)
	:GL_ImGui_Name(GuiName),GL_Shader_Version(ShaderVersion)
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

void ImGuiApp::SetUIDetail(glm::mat4& projection)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(GL_ImGui_Name.c_str()); 
	// Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / GL_ImGuiIO.get()->Framerate, GL_ImGuiIO.get()->Framerate);
	ImGui::Text("Set Projection Matrix Use Parameters Below:");
	ImGui::SliderFloat("Horizontal Scale:", &projection[0][0], 0.0f, 10.0f);
	ImGui::SliderFloat("Vertical Scale:", &projection[1][1], 0.0f, 10.0f);
	ImGui::SliderFloat("Bottom:", &projection[2][2], 0.0f, 10.0f);
	ImGui::SliderFloat("Uniform Scale:", &projection[3][3], 0.0f, 10.0f);

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