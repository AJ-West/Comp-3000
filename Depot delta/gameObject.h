#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <queue>
#include <cmath>
#include <unordered_map>

#include <SDL3/SDL.h>

#include "usefulFunctions.h"
#include "variables.h"


using namespace std;

class GameObject;

//GameObject class that holds multiple components

class Component {
public:
	virtual void update(GameObject* owner) = 0;//to be implemented by derived components

	template <typename T> Component(T* obj) : owner(obj) {};
	virtual ~Component() = default;
protected:
	GameObject* owner;
};

class GameObject {
public:
	GameObject(int x, int y){
		size.x = x;
		size.y = y;
	}

	//add a component to the GameObject
	template <typename T> void AddComponent(shared_ptr<T> component) {
		components[typeid(T).name()] = component;
	}

	//get a component from GameObject
	template <typename T> shared_ptr<T> AddComponent() {
		return static_pointer_cast<T>(components[typeid(T).name()]);
	}

	//update all components
	virtual void Update() {
		for (auto& pair : components) {
			pair.second->update(this);
		}
	}

	//getters
	SDL_Texture getTexture() { return texture; }
	SDL_FRect getDimensions() { return size; }

	//setters
	void setX(float x) { size.x = x; }
	void setY(float y) { size.y = y; }
	void setDimensions(SDL_FRect newSize) { size = newSize; }

	//virtual getters
	virtual bool getSelected() { return false; }

	//virtual setters


	//pure virtual getters
	virtual pair<float, float> getTargetPos() = 0;
	virtual bool getHovering() = 0;
	
	
	//pure virtual setters
	virtual void setHovering(bool hover) = 0;
	virtual void setTarget(float x, float y) = 0;

private:
	unordered_map<string, shared_ptr<Component>> components;// Store components

	SDL_Texture texture;

	SDL_FRect size{0,0,40,40};
};