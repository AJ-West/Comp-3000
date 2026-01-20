#include "transferBox.h"
#include "UnitStats.h"

transferBox::transferBox(LevelManager* lManager, SDL_FRect rSize, GameObject* sUnit, GameObject* sConvoy) : ResourceBox(lManager, rSize), unit(sUnit), convoy(sConvoy) {
	//add convoy->unit button
	elements.push_back(new transferDirectionButton({ size.x + 415.0f * camera.xScale, size.y + 788.0f * camera.yScale, 170.0f * camera.xScale , 65.0f * camera.yScale }, true));
	//add unit->convoy button
	//elements.push_back(new transferDirectionButton({ size.x + 394.0f * camera.xScale, size.y + 140.0f * camera.yScale, 192.0f * camera.xScale , 48.0f * camera.yScale }, false));
	addUnitInfo(unit, false);
	addUnitInfo(convoy, true);

	addArrows();
}
transferBox::~transferBox() {}

void transferBox::addUnitInfo(GameObject* unit, bool second) {
	if (second) {
		SDL_FRect pos{ size.x + 539 * camera.xScale, size.y + 222 * camera.yScale, 303 * camera.xScale, 562 * camera.yScale };
		elements.push_back(new UnitStats(unit, pos));
		return;
	}
	SDL_FRect pos{ size.x + 157 * camera.xScale, size.y + 222 * camera.yScale, 318 * camera.xScale, 562 * camera.yScale };
	elements.push_back(new UnitStats(unit, pos));
}

void transferBox::addArrows() {
	//Load transfer box texture - needs replacing in the future with specific unit make box
	SDL_Surface* surface = IMG_Load("art/UI/level/arrow.png");
	if (!surface) {
		cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	SDL_DestroySurface(surface); // Free the surface after creating the texture
	if (!texture) {
		cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << endl;
		return;
	}

	surface = IMG_Load("art/UI/level/flippedArrow.png");
	if (!surface) {
		cerr << "Unable to load image! IMG_Error: " << SDL_GetError() << endl;
		return;
	}
	SDL_Texture* flippedTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(flippedTexture, SDL_SCALEMODE_PIXELART);
	SDL_DestroySurface(surface); // Free the surface after creating the texture
	if (!flippedTexture) {
		cerr << "Unable to create flipped texture! SDL_Error: " << SDL_GetError() << endl;
		return;
	}

	arrowButton* arrow = new arrowButton({ size.x + 478.0f * camera.xScale, size.y + 541.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, texture, flippedTexture);
	elements.push_back(arrow);
	arrows.push_back(arrow);

	arrow = new arrowButton({ size.x + 478.0f * camera.xScale, size.y + 591.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, texture, flippedTexture);
	elements.push_back(arrow);
	arrows.push_back(arrow);

	arrow = new arrowButton({ size.x + 478.0f * camera.xScale, size.y + 641.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, texture, flippedTexture);
	elements.push_back(arrow);
	arrows.push_back(arrow);

	arrow = new arrowButton({ size.x + 478.0f * camera.xScale, size.y + 691.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, texture, flippedTexture);
	elements.push_back(arrow);
	arrows.push_back(arrow);

	arrow = new arrowButton({ size.x + 478.0f * camera.xScale, size.y + 741.0f * camera.yScale, 60.0f * camera.xScale , 25.0f * camera.yScale }, texture, flippedTexture);
	elements.push_back(arrow);
	arrows.push_back(arrow);
}

void transferBox::transferDirectionButtonClicked(UIElement* elem) {
	vector<int> amounts;
	int i = 0;
	for (auto elem : elements) { // get value of each resource input
		if (typeid(*elem).name() == typeid(textInput).name()) {
			if (elem->getText() != "") {
				if (arrows[i]->getToSecond()) {
					amounts.push_back(stoi(elem->getText()));
				}
				else{
					amounts.push_back(stoi(elem->getText())*-1);
				}
			}
			else {
				amounts.push_back(0);
			}
			i++;
		}
	}
	unit->getComponent<resourceTransferComponent>()->initiateTransfer(convoy, amounts);
	unPause();
	//should remove transfer box on beginning of transfer
	toDelete = true;
}
