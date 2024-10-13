#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include <vector>

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
};

class JsonFileReader {
    string filename;
    vector<Entity> entities;

    void printEntity(const Entity& entity) {
        cout << "Entity ID: " << entity.id << "\n";
        cout << "Is" << (entity.isHumanoid ? " " : " NOT ") << "a humanoid" << "\n";
        cout << "Planet: " << entity.planet << "\n";
        cout << "Age: " << entity.age << "\n";
        cout << "Traits: ";
        for (int i = 0; i < entity.traits.size(); i++) cout << entity.traits[i] << " ";
        entity.traits.size() ? cout << "\n" : cout << "N/A";
        cout << "________________________\n";
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

    void printFilteredEntities() {
        cout << "\nFiltered Entity IDs:\n";
        for (const auto& entity : entities) {
            printEntity(entity);
        }
    }
};

int main() {
    JsonFileReader reader("../input.json");
    reader.readAndMap();
    
    reader.printFilteredEntities();

    return 0;
}