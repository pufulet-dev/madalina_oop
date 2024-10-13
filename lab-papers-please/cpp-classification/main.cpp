#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
class Entity {
public:
    bool isHumanoid;
    string planet;
    pair<int,int> ageRange;
    vector<string> traits;
    string isHuman() {
        return isHumanoid ? "yes" : "no";
    }
};
int main() {
    cout << "hey everybody!!!\n";
    Entity alien;
    alien.isHumanoid = false;
    alien.planet = "Mars";
    alien.ageRange = {100, 200};
    alien.traits = {"SHORT"};
    cout << "My new alien is a humanoid: " << alien.isHuman();
    return 0;
}