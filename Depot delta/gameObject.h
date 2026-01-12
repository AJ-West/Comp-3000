#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <vector>

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
	GameObject(float x, float y, float width, float height, int vHealth): health(vHealth){
		size.x = x;
		size.y = y;
		size.w = width*tileWidth;
		size.h = height*tileHeight;
	}

	//add a component to the GameObject
	template <typename T> void AddComponent(shared_ptr<T> component) {
		components[typeid(T).name()] = component;
	}

	//get a component from GameObject
	template <typename T> shared_ptr<T> getComponent() {
		return static_pointer_cast<T>(components[typeid(T).name()]);
	}

	//update all components
	virtual void Update() {
		for (auto& pair : components) {
			pair.second->update(this);
		}
	}

	//might be removable to use button component instead
	virtual void onClick() = 0;
	void checkHover(int x, int y) {
		isHover = isWithinRect(x, y);
	}
	bool isWithinRect(int x, int y) {
		if (x < size.x) return false;
		if (x > size.x + size.w) return false;
		if (y < size.y) return false;
		if (y > size.y + size.h) return false;
		return true;
	}

	virtual void takeDamage(int damage) {
		health -= damage;
		if (health <= 0) { alive = false; }
	}

	GameObject* getTargetObject() { return targetObject; }
	virtual void setTargetObject(GameObject* object) { targetObject = object; }

	// Pure virtual functions
	virtual void renderHover(SDL_Renderer* renderer) = 0;
	virtual void clickAway() { cout << "clickAway"; }; // used for when clicking away from the selected object

	//getters
	SDL_Texture getTexture() { return texture; }
	SDL_FRect getDimensions() { return size; }
	Vec2 getPos() { return { size.x + size.w / 2, size.y + size.h / 2 }; }
	bool getSelected() { return selected; }
	bool getHovering() { return hovering; }
	SDL_FRect getSize() { return size; }
	int getHealth() { return health; }
	int getMaxHealth() { return maxHealth; }
	bool getAttacking() { return attacking; }
	bool getAlive() { return alive; }

	//setters
	void setX(float x) { size.x = x; }
	void setY(float y) { size.y = y; }
	void setDimensions(SDL_FRect newSize) { size = newSize; }
	void setHovering(bool hover) { hovering = hover; }
	void setAttacking(bool attack) { attacking = attack; }

	//virtual getters
	virtual Vec2 getTargetPos() { return Vec2{NULL, NULL}; };
	virtual int getID() { return 0; }
	
	//virtual setters
	virtual void setTarget(float x, float y) { cout << "target set"; }
protected:
	bool selected = false;
	bool hovering = false;
	float tx = NULL;
	float ty = NULL;

	int health = 100;
	int maxHealth = 1000;

	bool attacking = false;

	bool alive = true;

	GameObject* targetObject = nullptr;

private:
	bool isHover = false;

	unordered_map<string, shared_ptr<Component>> components;// Store components

	SDL_Texture texture;

	SDL_FRect size{ NULL, NULL, NULL, NULL };

	
};