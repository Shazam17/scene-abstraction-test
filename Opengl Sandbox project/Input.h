#pragma once
#include "Application.h"
class Input {


	Input() {

	}

	Application* app;

	
public:
	static Input* getInstance() {
		static Input* inp = nullptr;
		if (inp == nullptr) {
			inp = new Input();
		}
		return inp;
	}

	void InitInput(Application* App) {
		this->app = App;
	}

	

	static bool GetMouseDown(Application::MOUSE_KEY_STATE mouseCode) {
		Input* inp = getInstance();
		return (inp->app->mouseKeyTypeState == mouseCode &&
			inp->app->mouseKeyPressState == Application::KEY_PRESS_STATE::KEY_PRESSED);
	}

	static bool GetMouseUp(Application::MOUSE_KEY_STATE mouseCode) {
		Input* inp = getInstance();
		return (inp->app->mouseKeyTypeState == mouseCode &&
			inp->app->mouseKeyPressState == Application::KEY_PRESS_STATE::KEY_RELEASED);
	}

	static bool GetKeyDown(int keyCode) {

	}

	static bool GetKeyUp(int keyCode) {

	}




};