#pragma once
#include <map>
#include <string>


class Component {
private:
	std::map<std::string,Component*>* components;
protected:
	std::string componentsName;
	bool enabled = true;
	friend class Object;
public:
	Component(std::map<std::string, Component*>* cmp) {
		components = cmp;
	}
	Component() {

	}
	
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* GetComponent() const {
		try {
			auto res = components->find(typeid(T).name())->second;
			return (T*)(res);
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl;
			return nullptr;
		}

	}


	virtual void onStart() = 0;
	virtual void onUpdate() = 0;

};



