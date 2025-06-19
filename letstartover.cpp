#include <iostream>
#include <time.h>
#include <cmath>
#include <vector>
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

class Base{ //it's the military base, not base class
    public:
    position pos;
    Base(char x, int y) {
        pos.x = x;
        pos.y = y;
    }
};

class Vehicle {
    public:
        position pos;
        string label;  // Add this line
        double fuel;
        double fuel_each_grid_move;
        time_t deploy_time;
        double fuel_consumed_by_time;

        Vehicle(char x, int y, string l) {  // Modified constructor
            pos.x = x;
            pos.y = y;
            label = l;
            deploy_time = time(0);
            fuel = 100;
            fuel_each_grid_move = 1;
            fuel_consumed_by_time = 1;
        }

        virtual void printout(){
            cout << label << " Vehicle at position (" << pos.x << ", " << pos.y << ")" << endl;
        }

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
        
        Tank(char x, int y, string l, int ammo_count) : Vehicle(x, y, l) {  // Modified constructor
            deploy_time = time(0); // Set the deployment time to now
            ammo = ammo_count;
            fuel_each_grid_move = 3;
            fuel_consumed_by_time = (2/60.0); // Fuel consumed per second
            fuel = 100; // Initial fuel
        }

        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
            } else {
                cout << label << " Tank at position (" << pos.x << ", " << pos.y 
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
        virtual void move(position target) override {
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

        APC(char x, int y, string l, int ammo_count) : Vehicle(x, y, l) {
            fuel_each_grid_move = 1;
            fuel_consumed_by_time = (1/60.0); // Fuel consumed per second
            deploy_time = time(0); // Set the deployment time to now
            fuel = 100;
            ammo = ammo_count;
        }

        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                cout << label << " APC at position (" << pos.x << ", " << pos.y 
                     << ") with " << fuel << " fuel left." << endl;
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
        virtual void move(position target) override {
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

        Plane(char x, int y, string l, int aim_count, int agm_count, 
              int bomb_count, int ammo_count) : Vehicle(x, y, l) {
            fuel_each_grid_move = 5;
            fuel_consumed_by_time = (4/60.0); // Fuel consumed per second
            deploy_time = time(0); // Set the deployment time to now
            fuel = 100;
            aim = aim_count;
            agm = agm_count;
            bomb = bomb_count;
            ammo = ammo_count;
        }

        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                cout << label << " Plane at position (" << pos.x << ", " << pos.y 
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

        Helicopter(char x, int y, string l, int aim_count, int agm_count, 
                  int bomb_count, int ammo_count, int rocket_count) : Vehicle(x, y, l) {
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

        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            } else {
                cout << label << " Helicopter at position (" << pos.x << ", " << pos.y 
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
        string label;  // Add this line
        int ammo;
        time_t deploy_time;
        double food;
        double food_consumed_by_time;

        Infantry(char x, int y, string l, int ammo_count) {
            pos.x = x;
            pos.y = y;
            label = l;
            ammo = ammo_count;
            deploy_time = time(0);
            food = 100;
            food_consumed_by_time = (1/60.0); // Food consumed per second
        }

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

vector<Vehicle*> vehicles;
vector<Infantry*> infantries;

int main(){
    while(1){
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
            exit(0);
        }

        char x;
        int y, ammo_count, aim_count, agm_count, bomb_count, rocket_count;

        switch (choice){
            case 0: {
                cout << "Enter base position (x y): ";
                cin >> x >> y;
                Base base(x, y);
                cout << "Base initialized at position (" << base.pos.x << ", " << base.pos.y << ")." << endl;
                break;
            }
            case 1: {
                cout << "Choose a vehicle or infantry to deploy:" << endl;
                cout << "1. Tank" << endl;
                cout << "2. APC" << endl;
                cout << "3. Plane" << endl;
                cout << "4. Helicopter" << endl;
                cout << "5. Infantry" << endl;
                int vehicle_choice;
                cout << "Enter your choice (1-5): ";
                cin >> vehicle_choice;
                cout << "Enter label (enemy / friendly): ";
                string label;
                cin >> label;
                cout << "Enter position (x y): ";
                cin >> x >> y;
                
                switch (vehicle_choice) {
                    case 1: {
                        cout << "Enter ammo count: ";
                        cin >> ammo_count;
                        Vehicle* tank = new Tank(x, y, label, ammo_count);
                        vehicles.push_back(tank);
                        tank->printout();
                        break;
                    }
                    case 2: {
                        cout << "Enter ammo count: ";
                        cin >> ammo_count;
                        Vehicle* apc = new APC(x, y, label, ammo_count);
                        vehicles.push_back(apc);
                        apc->printout();
                        break;
                    }
                    case 3: {
                        cout << "Enter AIM count, AGM count, bomb count, and ammo count: ";
                        cin >> aim_count >> agm_count >> bomb_count >> ammo_count;
                        Vehicle* plane = new Plane(x, y, label, aim_count, agm_count, 
                                     bomb_count, ammo_count);
                        vehicles.push_back(plane);
                        plane->printout();
                        break;
                    }
                    case 4: {
                        cout << "Enter AIM count, AGM count, bomb count, ammo count, and rocket count: ";
                        cin >> aim_count >> agm_count >> bomb_count >> ammo_count >> rocket_count;
                        Vehicle* helicopter = new Helicopter(x, y, label, aim_count, agm_count, 
                                              bomb_count, ammo_count, rocket_count);
                        vehicles.push_back(helicopter);
                        helicopter->printout();
                        break;
                    }
                    case 5: {
                        cout << "Enter ammo count: ";
                        cin >> ammo_count;
                        Infantry* infantry = new Infantry(x, y, label, ammo_count);
                        infantries.push_back(infantry);
                        break;
                    }
                    default:
                        cout << "Invalid choice! Please try again." << endl;
                }
                break;
            }
            case 2: {
                cout << "Enter current position (x y) of the unit to move: ";
                char current_x;
                int current_y;
                cin >> current_x >> current_y;
                
                cout << "Enter target position (x y): ";
                position target;
                cin >> target.x >> target.y;
                
                bool found = false;
                
                // First check vehicles
                for(auto v : vehicles) {
                    if(v->pos.x == current_x && v->pos.y == current_y) {
                        v->move(target);
                        found = true;
                        break;
                    }
                }
                
                // If not found in vehicles, check infantry
                if(!found) {
                    for(auto inf : infantries) {
                        if(inf->pos.x == current_x && inf->pos.y == current_y) {
                            inf->move(target);
                            found = true;
                            break;
                        }
                    }
                }
                
                if(!found) {
                    cout << "No unit found at position (" << current_x << ", " << current_y << ")" << endl;
                }
                break;
            }
            case 3:
                cout << "Enter current position (x y) of the unit to fire: ";
                char fire_x;
                int fire_y;
                cin >> fire_x >> fire_y;
                cout << "Enter target position (x y): ";
                position fire_target;
                cin >> fire_target.x >> fire_target.y;
                bool found_fire = false;
                // First check vehicles
                for(auto v : vehicles) {
                    if(v->pos.x == fire_x && v->pos.y == fire_y) {
                        if(dynamic_cast<Tank*>(v)) {
                            dynamic_cast<Tank*>(v)->fire(fire_target);
                            found_fire = true;
                        } else if(dynamic_cast<APC*>(v)) {
                            dynamic_cast<APC*>(v)->fire(fire_target);
                            found_fire = true;
                        } else if(dynamic_cast<Plane*>(v)) {
                            cout << "Choose weapon type to fire:" << endl;
                            cout << "1. AIM" << endl;
                            cout << "2. AGM" << endl;
                            cout << "3. Bomb" << endl;
                            cout << "4. Gun" << endl;
                            int weapon_choice;
                            cin >> weapon_choice;
                            dynamic_cast<Plane*>(v)->fire(fire_target, weapon_choice);
                            found_fire = true;
                        } else if(dynamic_cast<Helicopter*>(v)) {
                            cout << "Choose weapon type to fire:" << endl;
                            cout << "1. AIM" << endl;
                            cout << "2. AGM" << endl;
                            cout << "3. Bomb" << endl;
                            cout << "4. Gun" << endl;
                            int weapon_choice;
                            cin >> weapon_choice;
                            dynamic_cast<Helicopter*>(v)->fire(fire_target, weapon_choice);
                            found_fire = true;
                        }
                    }
                    }
                // If not found in vehicles, check infantry
                if(!found_fire) {
                    for(auto inf : infantries) {
                        if(inf->pos.x == fire_x && inf->pos.y == fire_y) {
                            cout << "Infantry at position (" << inf->pos.x << ", " << inf->pos.y 
                                 << ") cannot fire." << endl;
                            found_fire = true;
                        }
                    }
                }
                if(!found_fire) {
                    cout << "No unit found at position (" << fire_x << ", " << fire_y << ")" << endl;
                }
                break;
            case 4:
                cout << "Feature not implemented yet." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}