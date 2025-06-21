#include <iostream>
#include <time.h>
#include <cmath>
#include <vector>
#include <map>
#include <memory>
using namespace std;



struct position{
    char x;
    int y;
};


float rangefind(position a, position b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
float move_rangefind(position a, position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}



class Vehicle {
    public:
        position pos;

        double fuel;
        double fuel_each_grid_move;
        time_t deploy_time;
        double fuel_consumed_by_time;

        Vehicle(char x, int y) {  // Modified constructor
            pos.x = x;
            pos.y = y;
            deploy_time = time(0);
            fuel = 100;
            fuel_each_grid_move = 1;
            fuel_consumed_by_time = 1;
        }

        virtual void printout(){
            cout <<  " Vehicle at position (" << pos.x << ", " << pos.y << ")" << endl;
        }
        virtual void refill(){fuel = 100;}

        void destructor_for_no_fuel_vehicle() {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                cout << "Vehicle at position (" << pos.x << ", " << pos.y << ") has run out of stock and was towed back to base." << endl;
                delete this; // Destroy the vehicle object
            }
        }

        virtual void move(position target) {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            }
            else {
                float distance = move_rangefind(pos, target);
                if (fuel >= distance * fuel_each_grid_move) {
                    pos = target;
                    fuel -= distance * fuel_each_grid_move;
                    cout << "Moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
                } else {
                    cout << "Not enough fuel to move!" << endl;
                }
            }
        }
};

class Tank : public Vehicle{
    public:
        int ammo;

        Tank(char x, int y, int ammo_count) : Vehicle(x, y) {  // Modified constructor
            deploy_time = time(0); // Set the deployment time to now
            ammo = ammo_count;
            fuel_each_grid_move = 3;
            fuel_consumed_by_time = (2/60.0); // Fuel consumed per second
            fuel = 100; // Initial fuel
        }
        void refill() override {fuel = 100; cout << ">>> Tank has been refueled to 100.\n";}
        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
            } else {
                cout <<" Tank at position (" << pos.x << ", " << pos.y
                     << ") with " << ammo << " ammo and " << fuel << " fuel left." << endl;
            }
        }

        void fire(position target) {
            float distance = rangefind(pos, target);
            if (ammo > 0 && distance <= 3) { // Assuming a range of 3 units for firing
                cout << "Firing at position (" << target.x << ", " << target.y << "). Ammo left: " << --ammo << endl;
            } else {
                cout << "Out of ammo!" << endl;
            }
        }

        // Overridden move method for Tank
        void move(position target) override {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel -= (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            }
            float distance = move_rangefind(pos, target);
            if (fuel >= distance * fuel_each_grid_move) {
                pos = target;
                fuel -= distance * fuel_each_grid_move;
                cout << "Tank moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
            } else {
                cout << "Not enough fuel to move the tank!" << endl;
            }
        }
};

class APC : public Vehicle{
    public:
        int ammo;

        APC(char x, int y, int ammo_count) : Vehicle(x, y) {
            fuel_each_grid_move = 1;
            fuel_consumed_by_time = (1/60.0); // Fuel consumed per second
            deploy_time = time(0); // Set the deployment time to now
            fuel = 100;
            ammo = ammo_count;
        }
        void refill() override{fuel = 100; cout << ">>> APC has been refueled to 100.\n";}
        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                cout << " APC at position (" << pos.x << ", " << pos.y
                     << ") with " << ammo << " ammo and " << fuel << " fuel left." << endl;
            }
        }

        void fire(position target) {
            float distance = rangefind(pos, target);
            if (ammo > 0 && distance <= 2) { // Assuming a range of 2 units for firing
                cout << "Firing at position (" << target.x << ", " << target.y << "). Ammo left: " << --ammo << endl;
            } else {
                cout << "Out of ammo!" << endl;
            }
        }

        // Overridden move method for APC
        void move(position target) override {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel -= (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            }
            float distance = move_rangefind(pos, target);
            if (fuel >= distance * fuel_each_grid_move) {
                pos = target;
                fuel -= distance * fuel_each_grid_move;
                cout << "APC moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
            } else {
                cout << "Not enough fuel to move the APC!" << endl;
            }
        }
};

class Plane : public Vehicle{
    public:
        int aim, agm, bomb, ammo;

        Plane(char x, int y, int aim_count, int agm_count,
              int bomb_count, int ammo_count) : Vehicle(x, y) {
            fuel_each_grid_move = 5;
            fuel_consumed_by_time = (4/60.0); // Fuel consumed per second
            deploy_time = time(0); // Set the deployment time to now
            fuel = 100;
            aim = aim_count;
            agm = agm_count;
            bomb = bomb_count;
            ammo = ammo_count;
        }
        void refill()override {fuel = 100; cout << ">>> Plane has been refueled to 100.\n";}
        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                cout << " Plane at position (" << pos.x << ", " << pos.y
                     << ") with " << ammo << " ammo, " << aim << " aim, "
                     << agm << " agm, " << bomb << " bomb and " << fuel << " fuel left." << endl;
            }
        }

        void move(position target) {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                float distance = rangefind(pos, target);
                if (fuel >= distance * fuel_each_grid_move) {
                    pos = target;
                    fuel -= distance * fuel_each_grid_move;
                    cout << "Plane moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
                } else {
                    cout << "Not enough fuel to move!" << endl;
                }
            }
        }

        void fire(position target, int type) {
            float distance = rangefind(pos, target);
            if (type == 1 && aim > 0 && distance <= 5) { // Range for AIMS
                cout << "Firing AIM at position (" << target.x << ", " << target.y << "). AIM left: " << --aim << endl;
            } else if (type == 2 && agm > 0 && distance <= 6) { // Range for AGMs
                cout << "Firing AGM at position (" << target.x << ", " << target.y << "). AGM left: " << --agm << endl;
            } else if (type == 3 && bomb > 0 && distance <= 1) { // Range for bombs
                cout << "Dropping bomb at position (" << target.x << ", " << target.y << "). Bomb left: " << --bomb << endl;
            } else if (type == 4 && ammo > 0 && distance <= 2) { // Range for ammo
                cout << "Firing ammo at position (" << target.x << ", " << target.y << "). Ammo left: " << --ammo << endl;
            } else {
                cout << "Out of ammo or out of range!" << endl;
            }
        }
};

class Helicopter : public Vehicle{
    public:
        int aim, agm, bomb, ammo, rocket;

        Helicopter(char x, int y, int aim_count, int agm_count,
                  int bomb_count, int ammo_count, int rocket_count) : Vehicle(x, y) {
            fuel_each_grid_move = 5.5;
            fuel_consumed_by_time = (4.5/60.0); // Fuel consumed per second
            deploy_time = time(0); // Set the deployment time to now
            fuel = 100;
            aim = aim_count;
            agm = agm_count;
            bomb = bomb_count;
            ammo = ammo_count;
            rocket = rocket_count;
        }
        void refill()override{fuel = 100; cout << ">>> Helicopter has been refueled to 100.\n";}
        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                cout <<  " Helicopter at position (" << pos.x << ", " << pos.y
                     << ") with " << ammo << " ammo, " << aim << " aim, "
                     << agm << " agm, " << bomb << " bomb, " << rocket
                     << " rocket and " << fuel << " fuel left." << endl;
            }
        }

        void move(position target) {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                float distance = rangefind(pos, target);
                if (fuel >= distance * fuel_each_grid_move) {
                    pos = target;
                    fuel -= distance * fuel_each_grid_move;
                    cout << "Helicopter moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
                } else {
                    cout << "Not enough fuel to move!" << endl;
                }
            }
        }

        void fire(position target, int type) {
            float distance = rangefind(pos, target);
            if (type == 1 && aim > 0 && distance <= 5) {
                cout << "Firing AIM at position (" << target.x << ", " << target.y << "). AIM left: " << --aim << endl;
            } else if (type == 2 && agm > 0 && distance <= 6) {
                cout << "Firing AGM at position (" << target.x << ", " << target.y << "). AGM left: " << --agm << endl;
            } else if (type == 3 && bomb > 0 && distance <= 1) {
                cout << "Dropping bomb at position (" << target.x << ", " << target.y << "). Bomb left: " << --bomb << endl;
            } else if (type == 4 && ammo > 0 && distance <= 2) {
                cout << "Firing ammo at position (" << target.x << ", " << target.y << "). Ammo left: " << --ammo << endl;
            } else if (type == 5 && rocket > 0 && distance <= 3) {
                cout << "Firing rocket at position (" << target.x << ", " << target.y << "). Rocket left: " << --rocket << endl;
            } else {
                cout << "Out of ammo or out of range!" << endl;
            }
        }
};

class Infantry{
    public:
        position pos;
        int ammo;
        time_t deploy_time;
        double food;
        double food_consumed_by_time;

        Infantry(char x, int y, int ammo_count) {
            pos.x = x;
            pos.y = y;
            ammo = ammo_count;
            deploy_time = time(0);
            food = 100;
            food_consumed_by_time = (1/60.0); // Food consumed per second
        }
        void refill(){food = 100; cout << ">>> Infantry has been refueled food to 100.\n";}
        void move(position target){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            food = food - (elapsed_time * food_consumed_by_time);
            float distance = move_rangefind(pos, target);
            if (food >= 0 && ammo > 0) {
                pos = target;
                cout << "Infantry moved to position (" << pos.x << ", " << pos.y << "). Food left: " << food << endl;
            } else {
                cout << "Out of food or ammo, infantry retreated" << endl;
                delete this; // Destroy the infantry object
            }
        }
};
class Base{ //it's the military base, not base class
    public:
    position pos;
    string classify;
    Base(char x, int y, string classify) {
        pos.x = x;
        pos.y = y;
        this->classify = classify;
    }
    //Vehicles storage
    map<string, unique_ptr<Vehicle>> vehicles;
    map<string, Infantry*> infantries;
    //
    void printInfo() const {
    cout << "----- BASE REPORT -----" << endl;
    cout << "Base is located at (" << pos.x << ", " << pos.y << ")" << endl;

    cout << "\n>>> Vehicles Status:\n";
    if (vehicles.empty()) {
        cout << "No vehicles deployed.\n";
    } else {
        for (const auto& [id, vehicle] : vehicles) {
            cout << "[" << id << "] ";
            vehicle->printout();
        }
    }

    cout << "\n>>> Infantry Status:\n";
    if (infantries.empty()) {
        cout << "No infantry deployed.\n";
    } else {
        for (const auto& [id, inf] : infantries) {
            cout << "[" << id << "] Infantry " <<  " at position (" << inf->pos.x << ", " << inf->pos.y << ")\n";
        }
    }

    cout << "------------------------\n";
}
};
map<int, Base*> All_Base;
int main(){
    while(1){
        cout << "-----------------------------------------"<< endl;
        cout << "Welcome to the Vehicle Management System!" << endl;
        cout << "Please choose an option:" << endl;
        cout << "0. Initialize the base" << endl;
        cout << "1. Deploy vehicles or infantry" << endl;
        cout << "2. Move vehicles or infantry" << endl;
        cout << "3. Fire weapons" << endl;
        cout << "4. Check status" << endl;
        cout << "5. Exit" << endl;

        cout << "Enter your choice (0-5): ";
        int choice;
        cin >> choice;

        if (choice == 5) {
            cout << "System exiting..." << endl;
            for (auto& [id, base] : All_Base) {
            for (auto& [infID, inf] : base->infantries) {
            delete inf;
        }
        delete base;
    }
        All_Base.clear();
            exit(0);
        }

        char x;
        int y, ammo_count, aim_count, agm_count, bomb_count, rocket_count;

        switch (choice){
            case 0: {
                cout << "Enter base position (x y): ";
                char temp;
                cin >> x >> temp;
                if (temp < '9' && temp > '0'){
                    y = temp - 48;
                }
                else
                {
                    cout<<"Y must be integer";
                    break;
                }
                cout << "Enter label (enemy/friendly): ";
                string label;
                cin >> label;
                int baseID = All_Base.size(); // e.g., 0, 1, 2,...
                All_Base[baseID] = new Base(x, y,label);
                cout << label <<" base #" << baseID << " initialized at position (" << x << ", " << y << ").\n";
                break;
            }
            case 1: {
                 if (All_Base.empty()) {
        cout << "No bases available. Create one first.\n";
        break;
    }

    cout << "There are " << All_Base.size() << " bases.\n";
    for (const auto& [id, base] : All_Base) {
        cout << "Base #" << id << " at (" << base->pos.x << ", " << base->pos.y << ")\n";
    }

    int chosenBase;
    cout << "Choose a base to deploy to (enter base ID): ";
    cin >> chosenBase;

    if (All_Base.find(chosenBase) == All_Base.end()) {
        cout << "Invalid base ID.\n";
        break;
    }

    Base* base = All_Base[chosenBase];

    cout << "Choose a unit to deploy:\n";
    cout << "1. Tank\n2. APC\n3. Plane\n4. Helicopter\n5. Infantry\n";
    int vehicle_choice;
    cin >> vehicle_choice;

    cout << "Enter unit ID (e.g., tank001): ";
    string unit_id;
    cin >> unit_id;


    cout << "Enter position (x y): ";
    char temp;
    cin >> x >> temp;
    if (temp < '9' && temp > '0'){
        y = temp - 48;
    }
    else
    {
        cout<<"Y must be integer"<<endl;
        break;
    }

    switch (vehicle_choice) {
        case 1: {
            cout << "Enter ammo count: ";
            cin >> ammo_count;
            base->vehicles[unit_id] = make_unique<Tank>(x, y, ammo_count);
            base->vehicles[unit_id]->printout();
            break;
        }
        case 2: {
            cout << "Enter ammo count: ";
            cin >> ammo_count;
            base->vehicles[unit_id] = make_unique<APC>(x, y, ammo_count);
            base->vehicles[unit_id]->printout();
            break;
        }
        case 3: {
            cout << "Enter AIM, AGM, Bomb, and Ammo count: ";
            cin >> aim_count >> agm_count >> bomb_count >> ammo_count;
            base->vehicles[unit_id] = make_unique<Plane>(x, y, aim_count, agm_count, bomb_count, ammo_count);
            base->vehicles[unit_id]->printout();
            break;
        }
        case 4: {
            cout << "Enter AIM, AGM, Bomb, Ammo, and Rocket count: ";
            cin >> aim_count >> agm_count >> bomb_count >> ammo_count >> rocket_count;
            base->vehicles[unit_id] = make_unique<Helicopter>(x, y, aim_count, agm_count, bomb_count, ammo_count, rocket_count);
            base->vehicles[unit_id]->printout();
            break;
        }
        case 5: {
            cout << "Enter ammo count: ";
            cin >> ammo_count;
            base->infantries[unit_id] = new Infantry(x, y, ammo_count);
            cout << "Infantry " << unit_id << " deployed.\n";
            break;
        }
        default:
            cout << "Invalid unit type.\n";
    }
    break;
            }
            case 2: {
                 if (All_Base.empty()) {
        cout << "No bases available. Create one first.\n";
        break;
    }

    cout << "Enter unit ID to move (e.g., tank001 or inf002): ";
    string unitID;
    cin >> unitID;

    cout << "Enter target position (x y): ";
    position target;
    char temp;
    cin >> target.x >> temp;
    if (temp < '9' && temp > '0'){
        target.y = temp - 48;
    }
    else
    {
        cout<<"Y must be integer"<<endl;
        break;
    }

    bool found = false;

    for (const auto& [baseID, base] : All_Base) {
        auto vehIt = base->vehicles.find(unitID);
        if (vehIt != base->vehicles.end()) {
            cout << "Unit \"" << unitID << "\" found in Base #" << baseID
                 << " at (" << base->pos.x << ", " << base->pos.y << ")\n";



            vehIt->second->move(target);

            if (target.x == base->pos.x && target.y == base->pos.y && vehIt -> second -> fuel >= 0) {
                cout << ">>> The unit is moving back to its base.\n";
                vehIt->second->refill();  // Refill before move
            }
            found = true;
            break;
        }

        auto infIt = base->infantries.find(unitID);
        if (infIt != base->infantries.end()) {
            cout << "Unit \"" << unitID << "\" found in Base #" << baseID
                 << " at (" << base->pos.x << ", " << base->pos.y << ")\n";



            infIt->second->move(target);
            if (target.x == base->pos.x && target.y == base->pos.y && infIt->second->food >= 0 ) {
                cout << ">>> The unit is moving back to its base.\n";
               infIt->second->refill();
            }
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No unit with ID \"" << unitID << "\" found in any base.\n";
    }

    break;
            }
            case 3: {
                if (All_Base.empty()) {
        cout << "No bases available.\n";
        break;
    }

    cout << "Enter unit ID to fire (e.g., tank001 or inf002): ";
    string fire_unitID;
    cin >> fire_unitID;
    cout << "Enter target position (x y): ";
    position fire_target;
    char temp;
    cin >> fire_target.x >> temp;
    if (temp < '9' && temp > '0'){
        fire_target.y = temp - 48;
    }
    else
    {
        cout<<"Y must be integer"<<endl;
        break;
    }
        

    // First find the firing unit
    bool found_firing_unit = false;
    Vehicle* firing_vehicle = nullptr;
    Base* firing_base = nullptr;

    // Find target at position
    Vehicle* target_vehicle = nullptr;
    Infantry* target_infantry = nullptr;
    Base* target_base = nullptr;
    string target_id;

    // Find firing unit and target
    for (const auto& [baseID, base] : All_Base) {
        // Look for firing unit
        auto vehIt = base->vehicles.find(fire_unitID);
        if (vehIt != base->vehicles.end()) {
            firing_vehicle = vehIt->second.get();
            firing_base = base;
            found_firing_unit = true;
        }

        // Look for target at position
        for (const auto& [id, veh] : base->vehicles) {
            if (veh->pos.x == fire_target.x && veh->pos.y == fire_target.y) {
                target_vehicle = veh.get();
                target_base = base;
                target_id = id;
                break;
            }
        }
        for (const auto& [id, inf] : base->infantries) {
            if (inf->pos.x == fire_target.x && inf->pos.y == fire_target.y) {
                target_infantry = inf;
                target_base = base;
                target_id = id;
                break;
            }
        }
    }

    if (!found_firing_unit) {
        cout << "Firing unit not found!\n";
        break;
    }

    // Check if target is a base
    Base* target_base_direct = nullptr;
    int target_base_id = -1;
    for (const auto& [baseID, base] : All_Base) {
        if (base->pos.x == fire_target.x && base->pos.y == fire_target.y) {
            target_base_direct = base;
            target_base_id = baseID;
            break;
        }
    }

    if (target_base_direct) {
        // Check friendly fire
        if (firing_base->classify == target_base_direct->classify) {
            cout << "No friendly fire allowed on bases!\n";
            break;
        }

        bool base_destroyed = false;

        if (auto firing_tank = dynamic_cast<Tank*>(firing_vehicle)) {
            float distance = rangefind(firing_tank->pos, fire_target);
            if (distance <= 3 && firing_tank->ammo >= 5) {
                cout << "Tank firing 5 shells at enemy base...\n";
                firing_tank->ammo -= 5;
                base_destroyed = true;
            } else {
                cout << "Not enough ammo or target out of range!\n";
            }
        }
        else if (auto firing_apc = dynamic_cast<APC*>(firing_vehicle)) {
            float distance = rangefind(firing_apc->pos, fire_target);
            if (distance <= 2 && firing_apc->ammo >= 1000) {
                cout << "APC unleashing 1000 rounds at enemy base...\n";
                firing_apc->ammo -= 1000;
                base_destroyed = true;
            } else {
                cout << "Not enough ammo or target out of range!\n";
            }
        }
        else if (auto firing_plane = dynamic_cast<Plane*>(firing_vehicle)) {
            int type;
            cout << "Choose weapon (2: AGM, 3: Bomb): ";
            cin >> type;
            float distance = rangefind(firing_plane->pos, fire_target);
            if ((type == 2 && firing_plane->agm > 0 && distance <= 6) || 
                (type == 3 && firing_plane->bomb > 0 && distance <= 1)) {
                firing_plane->fire(fire_target, type);
                base_destroyed = true;
            } else {
                cout << "Invalid weapon choice, out of ammo, or out of range!\n";
            }
        }
        else if (auto firing_heli = dynamic_cast<Helicopter*>(firing_vehicle)) {
            int type;
            cout << "Choose weapon (2: AGM, 3: Bomb, 5: Rocket): ";
            cin >> type;
            float distance = rangefind(firing_heli->pos, fire_target);
            if (type == 5 && firing_heli->rocket >= 20 && distance <= 3) {
                cout << "Helicopter launching 20 rocket barrage at enemy base...\n";
                firing_heli->rocket -= 20;
                base_destroyed = true;
            } else if ((type == 2 && firing_heli->agm > 0 && distance <= 6) || 
                      (type == 3 && firing_heli->bomb > 0 && distance <= 1)) {
                firing_heli->fire(fire_target, type);
                base_destroyed = true;
            } else {
                cout << "Invalid weapon choice, not enough ammo, or out of range!\n";
            }
        }

        if (base_destroyed) {
            cout << "Enemy base destroyed!\n";
            
            // Find another base of the same team
            Base* transfer_base = nullptr;
            int transfer_base_id = -1;
            for (const auto& [baseID, base] : All_Base) {
                if (baseID != target_base_id && 
                    base->classify == target_base_direct->classify) {
                    transfer_base = base;
                    transfer_base_id = baseID;
                    break;
                }
            }

            if (transfer_base) {
                // Transfer all vehicles and infantry to the other base
                cout << "Transferring surviving units to base #" << transfer_base_id << "...\n";
                
                // Transfer vehicles
                for (auto& [id, veh] : target_base_direct->vehicles) {
                    transfer_base->vehicles[id] = std::move(veh);
                }
                
                // Transfer infantry
                for (auto& [id, inf] : target_base_direct->infantries) {
                    transfer_base->infantries[id] = inf;
                }
                
                target_base_direct->vehicles.clear();
                target_base_direct->infantries.clear();
            } else {
                cout << "No remaining bases for " << target_base_direct->classify 
                     << " team. All units destroyed!\n";
                
                // Clear all units as there's no base to transfer to
                for (auto& [id, inf] : target_base_direct->infantries) {
                    delete inf;
                }
                target_base_direct->vehicles.clear();
                target_base_direct->infantries.clear();
            }
            
            // Delete the destroyed base
            delete target_base_direct;
            All_Base.erase(target_base_id);
        }
        break;
    }

    // Check friendly fire
    if (firing_base->classify == target_base->classify) {
        cout << "No friendly fire allowed!\n";
        break;
    }

    // Handle firing based on target type
    if (target_vehicle) {
        bool is_air_unit = dynamic_cast<Plane*>(target_vehicle) || dynamic_cast<Helicopter*>(target_vehicle);
        
        if (auto firing_tank = dynamic_cast<Tank*>(firing_vehicle)) {
            if (is_air_unit) {
                cout << "Tanks cannot engage air units!\n";
                break;
            }
            firing_tank->fire(fire_target);
            target_base->vehicles.erase(target_id);
            cout << "Enemy vehicle destroyed!\n";
        }
        else if (auto firing_apc = dynamic_cast<APC*>(firing_vehicle)) {
            firing_apc->fire(fire_target);
            target_base->vehicles.erase(target_id);
            cout << "Enemy vehicle destroyed!\n";
        }
        else if (auto firing_plane = dynamic_cast<Plane*>(firing_vehicle)) {
            int type;
            if (is_air_unit) {
                cout << "Choose weapon (1: AIM, 4: Gun): ";
                cin >> type;
                if (type != 1 && type != 4) {
                    cout << "Invalid weapon choice for air targets!\n";
                    break;
                }
            } else {
                cout << "Choose weapon (2: AGM, 3: Bomb, 4: Gun): ";
                cin >> type;
                if (type == 1) {
                    cout << "Cannot use AIM against ground targets!\n";
                    break;
                }
            }
            firing_plane->fire(fire_target, type);
            target_base->vehicles.erase(target_id);
            cout << "Enemy vehicle destroyed!\n";
        }
        else if (auto firing_heli = dynamic_cast<Helicopter*>(firing_vehicle)) {
            int type;
            if (is_air_unit) {
                cout << "Choose weapon (1: AIM, 4: Gun): ";
                cin >> type;
                if (type != 1 && type != 4) {
                    cout << "Invalid weapon choice for air targets!\n";
                    break;
                }
            } else {
                cout << "Choose weapon (2: AGM, 3: Bomb, 4: Gun, 5: Rocket): ";
                cin >> type;
                if (type == 1) {
                    cout << "Cannot use AIM against ground targets!\n";
                    break;
                }
            }
            firing_heli->fire(fire_target, type);
            target_base->vehicles.erase(target_id);
            cout << "Enemy vehicle destroyed!\n";
        }
    }
    else if (target_infantry) {
        // Infantry can be targeted by any weapon
        if (auto firing_tank = dynamic_cast<Tank*>(firing_vehicle)) {
            firing_tank->fire(fire_target);
        }
        else if (auto firing_apc = dynamic_cast<APC*>(firing_vehicle)) {
            firing_apc->fire(fire_target);
        }
        else if (auto firing_plane = dynamic_cast<Plane*>(firing_vehicle)) {
            cout << "Choose weapon (2: AGM, 3: Bomb, 4: Gun): ";
            int type;
            cin >> type;
            if (type == 1) {
                cout << "Cannot use AIM against infantry!\n";
                break;
            }
            firing_plane->fire(fire_target, type);
        }
        else if (auto firing_heli = dynamic_cast<Helicopter*>(firing_vehicle)) {
            cout << "Choose weapon (2: AGM, 3: Bomb, 4: Gun, 5: Rocket): ";
            int type;
            cin >> type;
            if (type == 1) {
                cout << "Cannot use AIM against infantry!\n";
                break;
            }
            firing_heli->fire(fire_target, type);
        }
        delete target_infantry;
        target_base->infantries.erase(target_id);
        cout << "Enemy infantry eliminated!\n";
    }
    break;
}
            case 4:
                if (All_Base.empty()) {
                cout << "No bases to report.\n";
                break;}

                cout << "=== SYSTEM STATUS REPORT ===\n";
                for (const auto& [id, base] : All_Base) {
                cout << "\n>>> Base #" << id << ":\n";
                base->printInfo();}
                cout << "=============================\n";
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
