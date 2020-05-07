#include "Application.h"

Application::Application(float w, float h, const std::string& windowName) : width(w), height(h), aspect(w / h) {

	if (!glfwInit())
		throw std::exception("glfw init failed");

	window = glfwCreateWindow(w, h, windowName.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw std::exception("glfw window init failed");
	}


	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::exception("glad init failed");
	}

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}

bool Application::isRunning() {
	return !glfwWindowShouldClose(window);
}

void Application::clear(const glm::vec4& clearColor) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

	ImGui_ImplOpenGL3_NewFrame();

	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Application::swapBuffers() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
	glfwPollEvents();
}

Application::~Application() {
	//Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
	glfwTerminate();
	glfwTerminate();
}
