#include "mapSaver.h"

MapSaver::MapSaver(const char* file)
{
	filename = file;
    doc.LoadFile(filename);
}

MapSaver::~MapSaver()
{
}

void MapSaver::saveFile(vector<GameObject*> unitConvoys, DepotObj* depot, vector<ZombieObj*> zombies)
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
        XMLElement* deleteEntity = nullptr;
        vector<int> IDInUse;
		while (entity) { // identify each entity and save its data
            string name = string(entity->FirstChildElement("name")->GetText());
            if (name == "Basic unit" || name == "Basic Convoy") {
                if (!saveUnit(entity, unitConvoys)) {
                    deleteEntity = entity;
                }
                else {
                    IDInUse.emplace_back(atoi(entity->FirstChildElement("id")->GetText()));
                }
            }
            else if (name == "depot") {
                saveDepot(entity, depot);
            }
            else if (name == "Basic Zombie") {
                if (!saveZombie(entity, zombies)) { 
                    deleteEntity = entity; 
                }
            }
            entity = entity->NextSiblingElement("entities");
            if (deleteEntity) {
                layer->DeleteChild(deleteEntity);
                deleteEntity = nullptr;
            }
        }
        entity = layer->FirstChildElement("entities");
        for (auto unit : unitConvoys) {
            bool found = false;
            auto it = find(IDInUse.begin(), IDInUse.end(), unit->getID());
            if (it != IDInUse.end()) {
                found = true;
            }
            if (!found) {
                saveNewUnit(layer, unit);
            }
        }
        doc.SaveFile(filename);
    }
    else {
        cerr << "Unable to save file! " << endl;
    }
}

bool MapSaver::saveUnit(XMLElement* entity, vector<GameObject*> units)
{
    for (auto& unit : units) {
        if (unit->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, unit);
			saveMovement(entity, unit);
            saveResources(entity, unit);
            return true;
        }
    }
    return false;
}

void MapSaver::saveNewUnit(XMLElement* layer, GameObject* unit) {
    XMLElement* entity = doc.NewElement("entities");
    XMLElement* name = doc.NewElement("name");
    XMLElement* id = doc.NewElement("id");
    XMLElement* health = doc.NewElement("health");
    XMLElement* x = doc.NewElement("x");
    XMLElement* y = doc.NewElement("y");

    if (typeid(*unit).name() == typeid(UnitObj).name()) {
        name->SetText("Basic unit");
    }
    else {
        name->SetText("Basic Convoy");
    }
    entity->InsertEndChild(name);
    id->SetText(unit->getID());
    entity->InsertEndChild(id);
    entity->InsertEndChild(health);
    entity->InsertEndChild(x);
    entity->InsertEndChild(y);

    saveHealth(entity, unit);
    saveMovement(entity, unit);
    saveResources(entity, unit);

    layer->InsertEndChild(entity);
}

bool MapSaver::saveConvoy(XMLElement* entity, vector<ConvoyObj*> convoys)
{
    for (auto& convoy : convoys) {
        if (convoy->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, convoy);
			saveMovement(entity, convoy);
            saveResources(entity, convoy);
            return true;
        }
    }
    return false;
}

bool MapSaver::saveZombie(XMLElement* entity, vector<ZombieObj*> zombies) {
    for (auto& zombie : zombies) {
        if (zombie->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, zombie);
            saveMovement(entity, zombie);
            return true;
        }
    }
    return false;
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
        entity->InsertAfterChild(entity->FirstChildElement("y"), target);
        target = doc.NewElement("target_y");
        target->SetText(obj->getTargetPos().y);
        entity->InsertAfterChild(entity->FirstChildElement("target_x"), target);
    }
}