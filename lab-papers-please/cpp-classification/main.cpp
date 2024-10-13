#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <vector>
#include <algorithm>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

class Entity {
public:
    int id;
    bool isHumanoid;
    string planet;
    int age;
    vector<string> traits;

    Entity(int id, bool isHumanoid, const string& planet, int age, const vector<string>& traits)
        : id(id), isHumanoid(isHumanoid), planet(planet), age(age), traits(traits) {}

    string classify() {
        if (!isHumanoid) {
            if (planet == "Kashyyyk" && age <= 400 && hasTrait("HAIRY") && hasTrait("TALL")) {
                return "Wookie";
            } else if (planet == "Endor" && age <= 60 && hasTrait("SHORT") && hasTrait("HAIRY")) {
                return "Ewok";
            } else if (planet == "Vogsphere" && age <= 200 && hasTrait("GREEN") && hasTrait("BULKY")) {
                return "Vogon";
            }
        } else {
            if (planet == "Earth") {
                if (hasTrait("BLONDE") && hasTrait("POINTY_EARS")) {
                    return "Elf";
                } else if (age <= 200 && hasTrait("SHORT") && hasTrait("BULKY")) {
                    return "Dwarf";
                }
            } else if (planet == "Asgard" && age <= 5000 && hasTrait("BLONDE") && hasTrait("TALL")) {
                return "Asgardian";
            } else if (planet == "Betelgeuse" && age <= 100 && hasTrait("EXTRA_ARMS") && hasTrait("EXTRA_HEAD")) {
                return "Betelgeusian";
            }
        }
        return "Unknown"; 
    }

private:
    bool hasTrait(const string& trait) {
        return find(traits.begin(), traits.end(), trait) != traits.end();
    }
};

class JsonFileReader {
    string filename;
    vector<Entity> entities;

public:
    JsonFileReader(const string& filename) : filename(filename) {}

    void readAndMap() {
        ifstream ifs(filename);
        if (!ifs) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string jsonContent((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
        rapidjson::Document document;
        if (document.Parse(jsonContent.c_str()).HasParseError()) {
            cerr << "JSON Parse Error: " << rapidjson::GetParseError_En(document.GetParseError()) << endl;
            return;
        }

        if (document.HasMember("input") && document["input"].IsArray()) {
            const rapidjson::Value& inputArray = document["input"];
            for (rapidjson::SizeType i = 0; i < inputArray.Size(); i++) {
                const rapidjson::Value& obj = inputArray[i];
                int id = obj["id"].GetInt();
                bool isHumanoid = obj.HasMember("isHumanoid") ? obj["isHumanoid"].GetBool() : false;
                string planet = obj.HasMember("planet") ? obj["planet"].GetString() : "N/A";
                int age = obj.HasMember("age") ? obj["age"].GetInt() : 0;
                vector<string> traits;

                if (obj.HasMember("traits") && obj["traits"].IsArray()) {
                    for (rapidjson::SizeType j = 0; j < obj["traits"].Size(); j++) {
                        traits.push_back(obj["traits"][j].GetString());
                    }
                }

                entities.emplace_back(id, isHumanoid, planet, age, traits);
            }
        }
    }

    void classifyEntities() {
        vector<Entity> starWars, marvel, hitchhikers, lordOfTheRings;

        for (auto& entity : entities) {
            string classification = entity.classify();
            if (classification == "Wookie" || classification == "Ewok") {
                starWars.push_back(entity);
            } else if (classification == "Asgardian") {
                marvel.push_back(entity);
            } else if (classification == "Betelgeusian" || classification == "Vogon") {
                hitchhikers.push_back(entity);
            } else if (classification == "Elf" || classification == "Dwarf") {
                lordOfTheRings.push_back(entity);
            }
        }

        writeToFile("star_wars.json", starWars);
        writeToFile("marvel.json", marvel);
        writeToFile("hitchhikers.json", hitchhikers);
        writeToFile("lord_of_the_rings.json", lordOfTheRings);
    }

    void writeToFile(const string& universe, const vector<Entity>& entities) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        
        writer.StartObject();
        writer.Key("name");
        writer.String(universe.c_str()); 
        writer.Key("individuals");
        writer.StartArray();
        for (const auto& entity : entities) {
            writer.StartObject();
            writer.Key("id");
            writer.Int(entity.id);
            writer.Key("isHumanoid");
            writer.Bool(entity.isHumanoid);
            writer.Key("planet");
            writer.String(entity.planet.c_str());
            writer.Key("age");
            writer.Int(entity.age);
            writer.Key("traits");
            writer.StartArray();
            for (const auto& trait : entity.traits) {
                writer.String(trait.c_str());
            }
            writer.EndArray();
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();

        ofstream ofs(universe);
        ofs << buffer.GetString();
        ofs.close();
    }

};

int main() {
    JsonFileReader reader("../input.json");
    reader.readAndMap();

    reader.classifyEntities();
    
    return 0;
}