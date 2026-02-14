#include "gameFiles/fileHandling/mapSaver.h"

#include "gameFiles/components/resourceTransferComponent.h"

MapSaver::MapSaver(const char* file)
{
	filename = file;
    doc.LoadFile(filename);
}

MapSaver::~MapSaver()
{
}

void MapSaver::saveFile(vector<shared_ptr<HumanObj>> unitConvoys, shared_ptr < DepotObj> depot, vector< shared_ptr<ZombieObj>> zombies)
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

bool MapSaver::saveUnit(XMLElement* entity, vector< shared_ptr<HumanObj>> units)
{
    for (auto& unit : units) {
        if (unit->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, unit);
			saveMovement(entity, unit);
            saveResources(entity, unit);
            saveResourceTransfer(entity, unit);
            return true;
        }
    }
    return false;
}

void MapSaver::saveNewUnit(XMLElement* layer, shared_ptr < HumanObj> unit) {
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

bool MapSaver::saveConvoy(XMLElement* entity, vector< shared_ptr<ConvoyObj>> convoys)
{
    for (auto& convoy : convoys) {
        if (convoy->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, convoy);
			saveMovement(entity, convoy);
            saveResources(entity, convoy);
            saveResourceTransfer(entity, convoy);
            return true;
        }
    }
    return false;
}

bool MapSaver::saveZombie(XMLElement* entity, vector< shared_ptr<ZombieObj>> zombies) {
    for (auto& zombie : zombies) {
        if (zombie->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            saveHealth(entity, zombie);
            saveMovement(entity, zombie);
            return true;
        }
    }
    return false;
}

void MapSaver::saveDepot(XMLElement* entity, shared_ptr < DepotObj> depot)
{
    saveHealth(entity, depot);
    saveResources(entity, depot);
}

void MapSaver::saveHealth(XMLElement* entity, shared_ptr < GameObject> obj) {
    entity->FirstChildElement("health")->SetText(static_cast<int>(obj->getHealth()));
}

void MapSaver::saveResources(XMLElement* entity, shared_ptr<GameObject> obj) {
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

void MapSaver::saveResourceTransfer(XMLElement* entity, shared_ptr < GameObject> obj) {
    shared_ptr<resourceTransferComponent> rTC;
    if (rTC = obj->getComponent<resourceTransferComponent>()) {
        if (!entity->FirstChildElement("Transfering")) {
            XMLElement* transfering = doc.NewElement("Transfering");
            entity->InsertEndChild(transfering);
        }
        int targetID = -1;
        if (rTC->getTarget()) {
            targetID = rTC->getTarget()->getID();
        }
        entity->FirstChildElement("Transfering")->SetText(targetID);
        if (entity->FirstChildElement("ResourceTransfer")) {
            XMLElement* resources = entity->FirstChildElement("ResourceTransfer");
            resources->FirstChildElement("PersonnelChange")->SetText(rTC->getTransferAmount(PERSONNEL));
            resources->FirstChildElement("AmmunitionChange")->SetText(rTC->getTransferAmount(AMMUNITION));
            resources->FirstChildElement("DoSChange")->SetText(rTC->getTransferAmount(DOS));
            resources->FirstChildElement("FuelChange")->SetText(rTC->getTransferAmount(FUEL));
            resources->FirstChildElement("ScrapChange")->SetText(rTC->getTransferAmount(SCRAP));
        }
        else {
            XMLElement* resources = doc.NewElement("ResourceTransfer");
            XMLElement* personnel = doc.NewElement("PersonnelChange");
            XMLElement* ammunition = doc.NewElement("AmmunitionChange");
            XMLElement* dos = doc.NewElement("DoSChange");
            XMLElement* fuel = doc.NewElement("FuelChange");
            XMLElement* scrap = doc.NewElement("ScrapChange");
            personnel->SetText(rTC->getTransferAmount(PERSONNEL));
            ammunition->SetText(rTC->getTransferAmount(AMMUNITION));
            dos->SetText(rTC->getTransferAmount(DOS));
            fuel->SetText(rTC->getTransferAmount(FUEL));
            scrap->SetText(rTC->getTransferAmount(SCRAP));
            resources->InsertEndChild(personnel);
            resources->InsertEndChild(ammunition);
            resources->InsertEndChild(dos);
            resources->InsertEndChild(fuel);
            resources->InsertEndChild(scrap);
            entity->InsertEndChild(resources);
        }
    }
}

bool MapSaver::saveTransfering(XMLElement* entity, shared_ptr<GameObject> target, bool transfering){
    int targetID = -1;
    if (transfering) {
        targetID = target->getID();
    }
    entity->FirstChildElement("Transfering")->SetText(targetID);
    if (targetID != -1) {
        return true;
    }
    return false;
    //return targetID != -1;
}

void MapSaver::saveMovement(XMLElement* entity, shared_ptr<GameObject> obj) {
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