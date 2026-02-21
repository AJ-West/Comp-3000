#pragma once
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "gameFiles/useThroughout/variables.h"
#include "gameFiles/entities/gameObject.h"
#include "gameFiles/UI/textInput.h"
#include "gameFiles/UI/resourceBoxes/transferDirectionButton.h"
#include "gameFiles/UI/resourceBoxes/newUnitButton.h"
#include "gameFiles/components/resourceTransferComponent.h"

#include "gameFiles/levelHandling/levelManager.h"

class textInput;
class TechArrow;

class ResourceBox : public UIElement {
public:
	ResourceBox(LevelManager* lManager, SDL_FRect rSize);
	~ResourceBox() {}

	void addTextInputs();

	bool update(SDL_Keycode key);

	virtual void render(SDL_Renderer* renderer);

	bool findClickedElement(float cx, float cy);

	virtual void transferDirectionButtonClicked(UIElement* elem) { cout << "transfer"; }
	virtual void newUnitButtonClicked(UIElement* elem) { cout << "new unit"; }
	virtual void arrowClicked(TechArrow* elem) { cout << "arrow"; }

	void unPause();

	void deleteBox() {
		toDelete = true;
		unPause();
	}

	//getters
	bool getToDelete() { return toDelete; }
	UIElement* getSelectedElement() { return selectedElement; }

protected:
	vector<UIElement*> elements;
	bool toDelete = false;
	LevelManager* manager;

	UIElement* selectedElement = nullptr;

	SDL_Texture* texture = nullptr;

private:
	SDL_FRect input{ camera.dimen.x + camera.dimen.w / 2 - camera.dimen.w / 20, camera.dimen.y + camera.dimen.h / 2 - camera.dimen.h / 20 , camera.dimen.w / 10 , camera.dimen.h / 10 };

	string text = "";	
};