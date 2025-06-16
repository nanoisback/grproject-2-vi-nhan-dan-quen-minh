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

class Vehicle{
    public:
        position pos;
        double fuel;
        double fuel_each_grid_move;
        time_t deploy_time;
        double fuel_consumed_by_time;

        virtual void printout(){
            cout << "Vehicle at position (" << pos.x << ", " << pos.y << ")" << endl;
        }

        Vehicle(char x, int y) {
            pos.x = x;
            pos.y = y;
            deploy_time = time(0);
            fuel = 100;
            fuel_each_grid_move = 1;
            fuel_consumed_by_time = 1;
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
        
        Tank(char x, int y, int ammo_count) : Vehicle(x, y){
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
                cout << "Tank at position (" << pos.x << ", " << pos.y << ") with " 
                     << ammo << " ammo and " << fuel << " fuel left." << endl;
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

        APC(char x, int y, int ammo_count) : Vehicle(x, y) {
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
                cout << "APC at position (" << pos.x << ", " << pos.y << ") with " << fuel << " fuel left." << endl;
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

        Plane(char x, int y, int aim_count, int agm_count, int bomb_count, int ammo_count) : Vehicle(x, y) {
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
                cout << "Plane at position (" << pos.x << ", " << pos.y << ") with " << ammo << " ammo, " 
                     << aim << " aim, " << agm << " agm, " << bomb << " bomb and " << fuel << " fuel left." << endl;
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

        Helicopter(char x, int y, int aim_count, int agm_count, int bomb_count, int ammo_count, int rocket_count) : Vehicle(x, y) {
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
                cout << "Helicopter at position (" << pos.x << ", " << pos.y << ") with " 
                     << ammo << " ammo, " << aim << " aim, " << agm << " agm, " 
                     << bomb << " bomb, " << rocket << " rocket and " << fuel << " fuel left." << endl;
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
        cout << "1. Deploy vehicles or infantry" << endl;
        cout << "2. Move vehicles or infantry" << endl;
        cout << "3. Fire weapons" << endl;
        cout << "4. Check status" << endl;
        cout << "5. Exit" << endl;

        int choice;
        cin >> choice;

        if (choice == 5) {
            exit(0);
        }

        char x;
        int y, ammo_count, aim_count, agm_count, bomb_count, rocket_count;

        switch (choice){
            case 1: {
                cout << "1. Tank" << endl;
                cout << "2. APC" << endl;
                cout << "3. Plane" << endl;
                cout << "4. Helicopter" << endl;
                cout << "5. Infantry" << endl;
                int vehicle_choice;
                cin >> vehicle_choice;
                cout << "Enter position (x y): ";
                cin >> x >> y;
                
                switch (vehicle_choice) {
                    case 1: {
                        cout << "Enter ammo count: ";
                        cin >> ammo_count;
                        Vehicle* tank = new Tank(x, y, ammo_count);
                        tank->printout();
                        vehicles.push_back(tank);  // Add this line after creating tank
                        break;
                    }
                    case 2: {
                        cout << "Enter ammo count: ";
                        cin >> ammo_count;
                        Vehicle* apc = new APC(x, y, ammo_count);
                        apc->printout();
                        vehicles.push_back(apc);   // Add this line after creating APC
                        break;
                    }
                    case 3: {
                        cout << "Enter AIM count, AGM count, bomb count, and ammo count: ";
                        cin >> aim_count >> agm_count >> bomb_count >> ammo_count;
                        Vehicle* plane = new Plane(x, y, aim_count, agm_count, bomb_count, ammo_count);
                        
                        plane->printout();
                        vehicles.push_back(plane); // Add this line after creating plane
                        break;
                    }
                    case 4: {
                        cout << "Enter AIM count, AGM count, bomb count, ammo count, and rocket count: ";
                        cin >> aim_count >> agm_count >> bomb_count >> ammo_count >> rocket_count;
                        Vehicle* helicopter = new Helicopter(x, y, aim_count, agm_count, bomb_count, ammo_count, rocket_count);
                        
                        helicopter->printout();
                        vehicles.push_back(helicopter); // Add this line after creating helicopter
                        break;
                    }
                    case 5: {
                        cout << "Enter ammo count: ";
                        cin >> ammo_count;
                        Infantry* infantry = new Infantry(x, y, ammo_count);
                        
                        // Optionally print infantry status
                        infantries.push_back(infantry);   // Add this line after creating infantry
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
                cout << "Feature not implemented yet." << endl;
                break;
            case 4:
                cout << "Feature not implemented yet." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}