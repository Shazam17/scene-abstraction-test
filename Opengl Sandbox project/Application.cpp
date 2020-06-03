#include "Application.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD


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
	glfwSetWindowUserPointer(window, this);
	glfwSetScrollCallback(window, mouseWheelCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetMouseButtonCallback(window, mouseClickCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::exception("glad init failed");
	}

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Multi-Viewport / Platform Windows
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	bool opt_fullscreen = true;
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	
}

bool Application::isRunning() {
	return !glfwWindowShouldClose(window);
}

void Application::clear(const glm::vec4& clearColor) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

}

void Application::swapBuffers() {
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::mouseWheelCallback(GLFWwindow* window, double xOff, double yOff)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->mouseScrollCalback(xOff, yOff);
}

inline void Application::calcMouseState(const glm::vec2& tMouseCord) {
	static bool first = true;
	static glm::vec2 lastPos;
	if (first) {
		lastPos = tMouseCord;
		first = false;
	}
	else {
		glm::vec2 diff = lastPos - tMouseCord;
		float len = sqrt(diff.x* diff.x + diff.y * diff.y);
		
		if (len < 3) {
			mouseMoveState = MOUSE_MOVE_STATE::Stationary;
		}
		else {
			mouseMoveState = MOUSE_MOVE_STATE::Moving;
		}
		lastPos = tMouseCord;
	}
}

void Application::resizeWindow(int width, int height)
{
	this->width = width;
	this->height = height;
	this->aspect = (float)width / (float)height;
}

void Application::mouseScrollCalback(double xOff, double yOff)
{
	scrollOffset += yOff;
}

void Application::mouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->mouseX = xPos;
	app->mouseY = yPos;
	app->calcMouseState(glm::vec2(app->mouseX, app->mouseY));
}

void Application::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->mouseKeyPressState = (KEY_PRESS_STATE)action;
	app->mouseKeyTypeState = (MOUSE_KEY_STATE)button;
	app->clickListener((MOUSE_KEY_STATE)button,(KEY_PRESS_STATE)action);
}

void Application::resizeCallback(GLFWwindow* window, int width, int height)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->resizeWindow(width, height);
}

void Application::setMouseClickListener(std::function<void(const Application& app,MOUSE_KEY_STATE,KEY_PRESS_STATE)> cb) {
	
	clickListener = [this,cb](MOUSE_KEY_STATE k, KEY_PRESS_STATE s) {
		cb(*this,k ,s);
	};
}

Application::~Application() {
	//Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
	glfwTerminate();
	glfwTerminate();
}
