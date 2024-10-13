#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <vector>
#include <algorithm>

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

    void classify() {

        // my debugging prints 
        cout << "Classifying Entity ID: " << id << endl;
        cout << "isHumanoid: " << isHumanoid << ", Planet: " << planet << ", Age: " << age << ", Traits: ";
        for (const auto& trait : traits) {
            cout << trait << " ";
        }
        cout << "\n";

        // my classifications
        if (!isHumanoid) {
            if (planet == "Kashyyyk" && age <= 400 && hasTrait("HAIRY") && hasTrait("TALL")) {
                cout << "Entity ID " << id << ": Wookie\n";
            } else if (planet == "Endor" && age <= 60 && hasTrait("SHORT") && hasTrait("HAIRY")) {
                cout << "Entity ID " << id << ": Ewok\n";
            } else if (planet == "Asgard" && age <= 5000 && hasTrait("BLONDE") && hasTrait("TALL")) {
                cout << "Entity ID " << id << ": Asgardian\n";
            } else if (planet == "Betelgeuse" && age <= 100 && hasTrait("EXTRA_ARMS") && hasTrait("EXTRA_HEAD")) {
                cout << "Entity ID " << id << ": Betelgeusian\n";
            } else if (planet == "Vogsphere" && age <= 200 && hasTrait("GREEN") && hasTrait("BULKY")) {
                cout << "Entity ID " << id << ": Vogon\n";
            }
        } else {
            if (planet == "Earth") {
                if (hasTrait("BLONDE") && hasTrait("POINTY_EARS")) {
                    cout << "Entity ID " << id << ": Elf\n";
                } else if (age <= 200 && hasTrait("SHORT") && hasTrait("BULKY")) {
                    cout << "Entity ID " << id << ": Dwarf\n";
                }
            } else if (planet == "Asgard" && age <= 5000 && hasTrait("BLONDE") && hasTrait("TALL")) {
                cout << "Entity ID " << id << ": Asgardian\n";
            } else if (planet == "Betelgeuse" && age <= 100 && hasTrait("EXTRA_ARMS") && hasTrait("EXTRA_HEAD")) {
                cout << "Entity ID " << id << ": Betelgeusian\n";
            }
        }
    }


private:
    bool hasTrait(const string& trait) {
        return find(traits.begin(), traits.end(), trait) != traits.end();
    }
};

class JsonFileReader {
    string filename;
    vector<Entity> entities;

    void printEntity(const Entity& entity) {
        cout << "Entity ID: " << entity.id << "\n";
    }

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
        cout << "\nClassified Entities with IDs:\n";
        for (auto& entity : entities) {
            entity.classify();
        }
    }
};

int main() {
    JsonFileReader reader("../input.json");
    reader.readAndMap();

    reader.classifyEntities();

    return 0;
}