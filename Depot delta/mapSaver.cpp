#include "mapSaver.h"

MapSaver::MapSaver(const char* file)
{
	filename = file;
    doc.LoadFile(filename);
}

MapSaver::~MapSaver()
{
}

void MapSaver::saveFile(vector<UnitObj*> units, DepotObj* depot, vector<ConvoyObj*> convoys, vector<ZombieObj*> zombies)
{
    XMLElement* root = doc.RootElement();
    XMLElement* layer = root->FirstChildElement("layers");
    bool found = false;
    while (!found && layer) {
        if (string(layer->FirstChildElement("name")->GetText()) == "Entities") {
            found = true;
            break;
        }
        layer = layer->NextSiblingElement("layers");
    }
    if (found) {
        XMLElement* entity = layer->FirstChildElement("entities");
		while (entity) { // identify each entity and save its data
            string name = string(entity->FirstChildElement("name")->GetText());
            if (name == "Basic unit") {
                saveUnit(entity, units);
            }
            else if (name == "depot") {
                saveDepot(entity, depot);
            }
			else if (name == "Basic Convoy") {
				saveConvoy(entity, convoys);
			}
            else if (name == "Basic Zombie") {
                saveZombie(entity, zombies);
            }
            entity = entity->NextSiblingElement("entities");
        }
        doc.SaveFile(filename);
    }
    else {
        cerr << "Unable to save file! " << endl;
    }
}

void MapSaver::saveUnit(XMLElement* entity, vector<UnitObj*> units)
{
    for (auto& unit : units) {
        if (unit->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, unit);
			saveMovement(entity, unit);
            saveResources(entity, unit);
            break;
        }
    }
}

void MapSaver::saveConvoy(XMLElement* entity, vector<ConvoyObj*> convoys)
{
    for (auto& convoy : convoys) {
        if (convoy->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, convoy);
			saveMovement(entity, convoy);
            saveResources(entity, convoy);
            break;
        }
    }
}

void MapSaver::saveZombie(XMLElement* entity, vector<ZombieObj*> zombies) {
    for (auto& zombie : zombies) {
        if (zombie->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, zombie);
            saveMovement(entity, zombie);
            break;
        }
    }
}

void MapSaver::saveDepot(XMLElement* entity, DepotObj* depot)
{
    saveHealth(entity, depot);
    saveResources(entity, depot);
}

void MapSaver::saveHealth(XMLElement* entity, GameObject* obj) {
    entity->FirstChildElement("health")->SetText(static_cast<int>(obj->getHealth()));
}

void MapSaver::saveResources(XMLElement* entity, GameObject* obj) {
    shared_ptr<resourceComponent> rC = obj->getComponent<resourceComponent>();
    if (entity->FirstChildElement("Resources")) {
        XMLElement* resources = entity->FirstChildElement("Resources");
        resources->FirstChildElement("Personnel")->SetText(rC->getResourcesCount(PERSONNEL));
        resources->FirstChildElement("Ammunition")->SetText(rC->getResourcesCount(AMMUNITION));
        resources->FirstChildElement("DoS")->SetText(rC->getResourcesCount(DOS));
        resources->FirstChildElement("Fuel")->SetText(rC->getResourcesCount(FUEL));
        resources->FirstChildElement("Scrap")->SetText(rC->getResourcesCount(SCRAP));
    }
    else {
        XMLElement* resources = doc.NewElement("Resources");
        XMLElement* personnel = doc.NewElement("Personnel");
        XMLElement* ammunition = doc.NewElement("Ammunition");
        XMLElement* dos = doc.NewElement("DoS");
        XMLElement* fuel = doc.NewElement("Fuel");
        XMLElement* scrap = doc.NewElement("Scrap");
        personnel->SetText(rC->getResourcesCount(PERSONNEL));
        ammunition->SetText(rC->getResourcesCount(AMMUNITION));
        dos->SetText(rC->getResourcesCount(DOS));
        fuel->SetText(rC->getResourcesCount(FUEL));
        scrap->SetText(rC->getResourcesCount(SCRAP));
        resources->InsertEndChild(personnel);
        resources->InsertEndChild(ammunition);
        resources->InsertEndChild(dos);
        resources->InsertEndChild(fuel);
        resources->InsertEndChild(scrap);
        entity->InsertEndChild(resources);
    }
}

void MapSaver::saveMovement(XMLElement* entity, GameObject* obj) {
    entity->FirstChildElement("x")->SetText(static_cast<int>(obj->getDimensions().x));
    entity->FirstChildElement("y")->SetText(static_cast<int>(obj->getDimensions().y));
    if (entity->FirstChildElement("target_x")) {
        entity->FirstChildElement("target_x")->SetText(obj->getTargetPos().x);
        entity->FirstChildElement("target_y")->SetText(obj->getTargetPos().y);
    }
    else {
        XMLElement* target = doc.NewElement("target_x");
        target->SetText(obj->getTargetPos().x);
        entity->InsertAfterChild(entity->FirstChildElement("originY"), target);
        target = doc.NewElement("target_y");
        target->SetText(obj->getTargetPos().y);
        entity->InsertAfterChild(entity->FirstChildElement("target_x"), target);
    }
}