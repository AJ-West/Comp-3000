#include "mapSaver.h"

MapSaver::MapSaver(const char* file)
{
	filename = file;
    doc.LoadFile(filename);
}

MapSaver::~MapSaver()
{
}

void MapSaver::saveFile(vector<UnitObj*> units, DepotObj* depot, vector<ConvoyObj*> convoys)
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
        while (entity) {
            string name = string(entity->FirstChildElement("name")->GetText());
            if (name == "Basic unit") {
                save_unit(entity, units);
            }
            else if (name == "depot") {
                save_depot(entity, depot);
            }
			else if (name == "Basic Convoy") {
				save_convoy(entity, convoys);
			}
            entity = entity->NextSiblingElement("entities");
        }
        doc.SaveFile(filename);
    }
    else {
        cerr << "Unable to save file! " << endl;
    }
}

void MapSaver::save_unit(XMLElement* entity, vector<UnitObj*> units)
{
    for (auto& unit : units) {
        if (unit->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            entity->FirstChildElement("x")->SetText(static_cast<int>(unit->getDimensions().x));
            entity->FirstChildElement("y")->SetText(static_cast<int>(unit->getDimensions().y));
            if (entity->FirstChildElement("target_x")) {
                entity->FirstChildElement("target_x")->SetText(unit->getTargetPos().first);
                entity->FirstChildElement("target_y")->SetText(unit->getTargetPos().second);
            }
            else {
                XMLElement* target = doc.NewElement("target_x");
                target->SetText(unit->getTargetPos().first);
                entity->InsertAfterChild(entity->FirstChildElement("originY"), target);
                target = doc.NewElement("target_y");
                target->SetText(unit->getTargetPos().second);
                entity->InsertAfterChild(entity->FirstChildElement("target_x"), target);
            }
            save_resources(entity, unit);
            break;
        }
    }
}

void MapSaver::save_convoy(XMLElement* entity, vector<ConvoyObj*> convoys)
{
    for (auto& convoy : convoys) {
        if (convoy->getID() == atoi(entity->FirstChildElement("id")->GetText())) {
            entity->FirstChildElement("x")->SetText(static_cast<int>(convoy->getDimensions().x));
            entity->FirstChildElement("y")->SetText(static_cast<int>(convoy->getDimensions().y));
            if (entity->FirstChildElement("target_x")) {
                entity->FirstChildElement("target_x")->SetText(convoy->getTargetPos().first);
                entity->FirstChildElement("target_y")->SetText(convoy->getTargetPos().second);
            }
            else {
                XMLElement* target = doc.NewElement("target_x");
                target->SetText(convoy->getTargetPos().first);
                entity->InsertAfterChild(entity->FirstChildElement("originY"), target);
                target = doc.NewElement("target_y");
                target->SetText(convoy->getTargetPos().second);
                entity->InsertAfterChild(entity->FirstChildElement("target_x"), target);
            }
            save_resources(entity, convoy);
            break;
        }
    }
}

void MapSaver::save_depot(XMLElement* entity, DepotObj* depot)
{
    save_resources(entity, depot);
}

void MapSaver::save_resources(XMLElement* entity, GameObject* obj) {
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