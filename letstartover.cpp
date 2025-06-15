#include <iostream>
#include <time.h>
#include <cmath>

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

        void move(position target) {
        time_t current_time = time(0);
        double elapsed_time = difftime(current_time, deploy_time);
        fuel = fuel - (elapsed_time * fuel_consumed_by_time);
        if (fuel <= 0) {
            Vehicle::destructor_for_no_fuel_vehicle();
            return;
        }
        else{
        float distance = move_rangefind(pos, target);
        if (fuel >= distance * fuel_each_grid_move) {
            pos = target;
            fuel -= distance * fuel_each_grid_move;
            cout << "Moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
        } else {
            cout << "Not enough fuel to move!" << endl;
        }
        }}

    
};

class Tank : public Vehicle{
    public:
        int ammo;
        

        Tank(char x, int y, int ammo_count) : Vehicle(x, y){
            deploy_time = time(0); // Set the deployment time to now
            ammo = ammo_count;
            fuel_each_grid_move = 3;
            fuel_consumed_by_time = (2/60); // Fuel consumed per second
            fuel = 100; // Initial fuel
        }

        void printout(){
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
            } else {
                cout << "Tank at position (" << pos.x << ", " << pos.y << ") with " << ammo << " ammo and " << fuel << " fuel left." << endl;
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

        
};

class APC : public Vehicle{
    public:
        int ammo;

        APC(char x, int y, int ammo_count) : Vehicle(x, y) {
            fuel_each_grid_move = 1;
            fuel_consumed_by_time = (1/60); // Fuel consumed per second
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
            }
            else{
            cout << "APC at position (" << pos.x << ", " << pos.y << ") with " << fuel << " fuel left." << endl;
        }}

        void fire(position target) {
            float distance = rangefind(pos, target);
            if (ammo > 0 && distance <= 2) { // Assuming a range of 2 units for firing
                cout << "Firing at position (" << target.x << ", " << target.y << "). Ammo left: " << --ammo << endl;
            } else {
                cout << "Out of ammo!" << endl;
            }
        }
};

class Plane : public Vehicle{
    public:
        int aim, agm, bomb, ammo;

        Plane(char x, int y, int aim_count, int agm_count, int bomb_count, int ammo_count) : Vehicle(x, y) {
            fuel_each_grid_move = 5;
            fuel_consumed_by_time = (4/60); // Fuel consumed per second
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
            }
            else{
            cout << "Plane at position (" << pos.x << ", " << pos.y << ") with " << ammo << " ammo " << aim << " aim " << agm << " agm " << bomb << " bomb and " << fuel << " fuel left." << endl;
        }}

        void move(position target) {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            }
            else{
            float distance = rangefind(pos, target);
            if (fuel >= distance * fuel_each_grid_move) {
                pos = target;
                fuel -= distance * fuel_each_grid_move;
                cout << "Plane moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
            } else {
                cout << "Not enough fuel to move!" << endl;
            }
        }}

        void fire(position target, int type) {
            float distance = rangefind(pos, target);
            if (type == 1 && aim > 0 && distance <= 5) { // Assuming a range of 5 units for AIMS
                cout << "Firing AIM at position (" << target.x << ", " << target.y << "). AIM left: " << --aim << endl;
            } else if (type == 2 && agm > 0 && distance <= 6) { // Assuming a range of 6 units for AGMs
                cout << "Firing AGM at position (" << target.x << ", " << target.y << "). AGM left: " << --agm << endl;
            } else if (type == 3 && bomb > 0 && distance <= 1) { // Assuming a range of 1 unit for bombs
                cout << "Dropping bomb at position (" << target.x << ", " << target.y << "). Bomb left: " << --bomb << endl;
            } else if (type == 4 && ammo > 0 && distance <= 2) { // Assuming a range of 2 units for ammo
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
            fuel_consumed_by_time = (4.5/60); // Fuel consumed per second
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
            }
            else{
            cout << "Helicopter at position (" << pos.x << ", " << pos.y << ") with " << ammo << " ammo " << aim << " aim " << agm << " agm " << bomb << " bomb " << rocket << " rocket and " << fuel << " fuel left." << endl;
        }}

        void move(position target) {
            time_t current_time = time(0);
            double elapsed_time = difftime(current_time, deploy_time);
            fuel = fuel - (elapsed_time * fuel_consumed_by_time);
            if (fuel <= 0) {
                Vehicle::destructor_for_no_fuel_vehicle();
                return;
            }
            else{
            float distance = rangefind(pos, target);
            if (fuel >= distance * fuel_each_grid_move) {
                pos = target;
                fuel -= distance * fuel_each_grid_move;
                cout << "Helicopter moved to position (" << pos.x << ", " << pos.y << "). Fuel left: " << fuel << endl;
            } else {
                cout << "Not enough fuel to move!" << endl;
            }
        }}

        void fire(position target, int type) {
            float distance = rangefind(pos, target);
            if (type == 1 && aim > 0 && distance <= 5) { // Assuming a range of 5 units for AIMS
                cout << "Firing AIM at position (" << target.x << ", " << target.y << "). AIM left: " << --aim << endl;
            } else if (type == 2 && agm > 0 && distance <= 6) { // Assuming a range of 6 units for AGMs
                cout << "Firing AGM at position (" << target.x << ", " << target.y << "). AGM left: " << --agm << endl;
            } else if (type == 3 && bomb > 0 && distance <= 1) { // Assuming a range of 1 unit for bombs
                cout << "Dropping bomb at position (" << target.x << ", " << target.y << "). Bomb left: " << --bomb << endl;
            } else if (type == 4 && ammo > 0 && distance <= 2) { // Assuming a range of 2 units for ammo
                cout << "Firing ammo at position (" << target.x << ", " << target.y << "). Ammo left: " << --ammo << endl;
            } else if (type == 5 && rocket > 0 && distance <= 3) { // Assuming a range of 3 units for rockets
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
            food_consumed_by_time = (1/60); // Food consumed per second
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