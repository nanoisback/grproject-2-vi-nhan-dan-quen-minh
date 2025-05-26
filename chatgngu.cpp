// Warfare Management System (WMS)
// Author: Group for ELEC2030 - VinUniversity
// Date: Spring 2024
// Description: Full implementation of a command-line warfare management system

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <cmath>
#include <ctime>
#include <fstream>
using namespace std;

// Utility structures and functions
struct Coordinate {
    double x, y;

    double distanceTo(const Coordinate &other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }
};

// Base class for all units
class MilitaryUnit {
protected:
    string id;
    Coordinate location;
    string status;

public:
    MilitaryUnit(string id_, Coordinate loc_) : id(id_), location(loc_), status("idle") {}
    virtual void move(Coordinate dest, double speed) = 0;
    virtual void attack() = 0;
    string getStatus() const { return status; }
    Coordinate getLocation() const { return location; }
    virtual void printInfo() const = 0;
    virtual string getType() const = 0;
    virtual ~MilitaryUnit() {}
};

// Tank class
class Tank : public MilitaryUnit {
    double fuel;
    int apfsds, atgm, he;

public:
    Tank(string id_, Coordinate loc_, double fuel_, int apfsds_, int atgm_, int he_)
        : MilitaryUnit(id_, loc_), fuel(fuel_), apfsds(apfsds_), atgm(atgm_), he(he_) {}

    void move(Coordinate dest, double speed) override {
        double dist = location.distanceTo(dest);
        double timeNeeded = dist / speed;
        fuel -= dist * 0.1;
        location = dest;
        status = "moved";
    }

    void attack() override {
        if (apfsds > 0) {
            apfsds--;
            status = "attacked";
        } else {
            status = "no ammo";
        }
    }

    void printInfo() const override {
        cout << "Tank ID: " << id << ", Location: (" << location.x << ", " << location.y << "), Status: " << status << endl;
    }

    string getType() const override { return "Tank"; }
};

// APC class
class APC : public Tank {
public:
    using Tank::Tank;
    void printInfo() const override {
        cout << "APC ID: " << id << ", Location: (" << location.x << ", " << location.y << "), Status: " << status << endl;
    }
    string getType() const override { return "APC"; }
};

// Aircraft class
class Aircraft : public MilitaryUnit {
    double fuel;
    int a2a, a2g;

public:
    Aircraft(string id_, Coordinate loc_, double fuel_, int a2a_, int a2g_)
        : MilitaryUnit(id_, loc_), fuel(fuel_), a2a(a2a_), a2g(a2g_) {}

    void move(Coordinate dest, double speed) override {
        double dist = location.distanceTo(dest);
        double timeNeeded = dist / speed;
        fuel -= dist * 0.2;
        location = dest;
        status = "cruising";
    }

    void attack() override {
        if (a2g > 0) {
            a2g--;
            status = "attacked ground";
        } else {
            status = "no ammo";
        }
    }

    void printInfo() const override {
        cout << "Aircraft ID: " << id << ", Location: (" << location.x << ", " << location.y << "), Status: " << status << endl;
    }

    string getType() const override { return "Aircraft"; }
};

// Infantry class
class Infantry : public MilitaryUnit {
    int soldiers;
    int ammo;
    int foodDays;

public:
    Infantry(string id_, Coordinate loc_, int soldiers_, int ammo_, int food_)
        : MilitaryUnit(id_, loc_), soldiers(soldiers_), ammo(ammo_), foodDays(food_) {}

    void move(Coordinate dest, double speed) override {
        double dist = location.distanceTo(dest);
        location = dest;
        status = "marching";
    }

    void attack() override {
        if (ammo > 0) {
            ammo--;
            status = "attacked";
        } else {
            status = "no ammo";
        }
    }

    void printInfo() const override {
        cout << "Infantry ID: " << id << ", Soldiers: " << soldiers << ", Location: ("
             << location.x << ", " << location.y << "), Status: " << status << endl;
    }

    string getType() const override { return "Infantry"; }
};

// Military Base class
class MilitaryBase {
    string name;
    list<MilitaryUnit*> units;
    Coordinate location;

public:
    MilitaryBase(string name_, Coordinate loc_) : name(name_), location(loc_) {}

    void addUnit(MilitaryUnit* unit) {
        units.push_back(unit);
    }

    void moveUnitTo(MilitaryBase &other, const string &unitId) {
        for (auto it = units.begin(); it != units.end(); ++it) {
            if ((*it)->getStatus() != "destroyed") {
                (*it)->move(other.getLocation(), 40.0);
                other.addUnit(*it);
                units.erase(it);
                return;
            }
        }
    }

    void bombardBase() {
        for (auto unit : units) {
            delete unit;
        }
        units.clear();
    }

    void printBaseStatus() const {
        cout << "\nBase: " << name << " at (" << location.x << ", " << location.y << ")\n";
        for (const auto &u : units) u->printInfo();
    }

    const list<MilitaryUnit*>& getUnits() const { return units; }
    string getName() const { return name; }
    Coordinate getLocation() const { return location; }

    ~MilitaryBase() {
        for (auto unit : units) delete unit;
    }
};

// Command Line Interface main function
int main() {
    Coordinate baseA{0, 0}, baseB{100, 100};
    MilitaryBase allyBase("Alpha", baseA);
    MilitaryBase enemyBase("Bravo", baseB);

    allyBase.addUnit(new Tank("T001", baseA, 100.0, 5, 3, 2));
    allyBase.addUnit(new Aircraft("A001", baseA, 200.0, 4, 2));
    allyBase.addUnit(new Infantry("I001", baseA, 30, 10, 5));

    enemyBase.addUnit(new Tank("ET001", baseB, 80.0, 3, 2, 1));
    enemyBase.addUnit(new Aircraft("EA001", baseB, 150.0, 2, 1));

    int choice;
    do {
        cout << "\n========= Warfare Management System =========\n";
        cout << "1. Print out map (bases and coordinates)\n";
        cout << "2. Print all ally units and vehicles\n";
        cout << "3. Print all enemy units and vehicles\n";
        cout << "0. Exit\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nMap Overview:\n";
                cout << "Ally Base: " << allyBase.getName() << " at (" << allyBase.getLocation().x << ", " << allyBase.getLocation().y << ")\n";
                cout << "Enemy Base: " << enemyBase.getName() << " at (" << enemyBase.getLocation().x << ", " << enemyBase.getLocation().y << ")\n";
                break;
            case 2:
                cout << "\nAlly Units Status:\n";
                allyBase.printBaseStatus();
                break;
            case 3:
                cout << "\nEnemy Units Status:\n";
                enemyBase.printBaseStatus();
                break;
            case 0:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
