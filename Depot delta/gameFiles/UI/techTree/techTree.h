#pragma once
#include "gameFiles/UI/techTree/tech.h"
#include "vector"

using namespace std;

class techTree {
public:
	techTree(){
		readTechFile();
	}
	~techTree(){
		saveTechFile();
	}

	void readTechFile() {

	}

	void saveTechFile() {

	}

private:
	vector<tech*> allTechs;
};