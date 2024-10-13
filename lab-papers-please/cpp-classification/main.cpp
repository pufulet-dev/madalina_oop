#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

using namespace std;

class JsonFileReader {
    string filename;

    void printJsonObject(const rapidjson::Value& obj) {
        cout << "Object " << obj["id"].GetInt() << ":\n";
        cout << "  Is Humanoid: " << (obj.HasMember("isHumanoid") ? (obj["isHumanoid"].GetBool() ? "Yes" : "No") : "N/A") << "\n";
        cout << "  Planet: " << (obj.HasMember("planet") ? obj["planet"].GetString() : "N/A") << "\n";
        cout << "  Age: " << (obj.HasMember("age") ? to_string(obj["age"].GetInt()) : "N/A") << "\n";

        if (obj.HasMember("traits") && obj["traits"].IsArray()) {
            cout << "  Traits: ";
            for (rapidjson::SizeType j = 0; j < obj["traits"].Size(); j++) {
                cout << obj["traits"][j].GetString();
                if (j < obj["traits"].Size() - 1) cout << ", ";
            }
            cout << "\n";
        }
        cout << endl;
    }

public:
    JsonFileReader(const string& filename) : filename(filename) {}

    void readAndPrint() {
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
            cout << "\nParsed JSON Objects:\n";
            for (rapidjson::SizeType i = 0; i < inputArray.Size(); i++) {
                const rapidjson::Value& obj = inputArray[i];
                printJsonObject(obj);
            }
        }
    }
};

int main() {
    JsonFileReader reader("../input.json");
    reader.readAndPrint();
    return 0;
}