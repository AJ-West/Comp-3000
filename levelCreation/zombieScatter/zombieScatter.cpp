// zombieScatter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../../Depot delta/dependencies/tinyxml2.h"

using namespace tinyxml2;

int main()
{
    system("pause");

    srand(time(0));

    XMLDocument doc;
    doc.LoadFile("scatter.xml");

    XMLElement* root = doc.RootElement();

    XMLElement* oldEntity = root->FirstChildElement("entities");
    while (oldEntity) { // delete all previous zombies
        XMLElement* deleteEntity = oldEntity;
        oldEntity = oldEntity->NextSiblingElement("entities");
        if (deleteEntity) {
            root->DeleteChild(deleteEntity);
        }
    }


    int quantity = 100; // quantity of zombies to spawn
    int ID = 3; // id to start from

    // bounds for zombies to be spawned between
    int xStart = 0;
    int yStart = 0;
    int xEnd = 3968;
    int yEnd = 2176;
    int xDiff = xEnd - xStart;
    int yDiff = yEnd - yStart;

    //bounds to avoid depot
    int xIgnoreStart = 1300;
    int yIgnoreStart = 460;
    int xIgnoreEnd = 2400;
    int yIgnoreEnd = 1460;

    float sightDistance = 500.0f;

    int maxHealth = 100;

    for (int i = 0; i < quantity; i++) {
        XMLElement* entity = doc.NewElement("entities");
        XMLElement* name = doc.NewElement("name");
        XMLElement* id = doc.NewElement("id");
        XMLElement* health = doc.NewElement("health");
        XMLElement* x = doc.NewElement("x");
        XMLElement* y = doc.NewElement("y");
        XMLElement* sight = doc.NewElement("sight");

        name->SetText("Basic Zombie");
        entity->InsertEndChild(name);

        id->SetText(ID);
        ID++;
        entity->InsertEndChild(id);

        health->SetText(maxHealth);
        entity->InsertEndChild(health);

        int xPos;
        int yPos;
        bool found = false;
        while (!found) {
            std::cout << "id: " << ID << '\n';
            xPos = xStart + rand() % xDiff;
            yPos = yStart + rand() % yDiff;
            found = true;
            if (xPos < xIgnoreEnd && xPos > xIgnoreStart) {
                std::cout << "1" << '\n';
                if (yPos < yIgnoreEnd && yPos > yIgnoreStart) {
                    std::cout << "2" << '\n';
                    found = false;
                }
            }
            //if (found) {
              //  std::cout << "x:" << xIgnoreStart << "  " << xPos << "  " << xIgnoreEnd << '\n';
                //std::cout << "y:" << yIgnoreStart << "  " << yPos << "  " << yIgnoreEnd << '\n';
            //}
        }

        x->SetText(xPos); 
        y->SetText(yPos);
        entity->InsertEndChild(x);
        entity->InsertEndChild(y);

        sight->SetText(sightDistance);
        entity->InsertEndChild(sight);

        root->InsertEndChild(entity);
    }

    doc.SaveFile("scatter.xml");
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
