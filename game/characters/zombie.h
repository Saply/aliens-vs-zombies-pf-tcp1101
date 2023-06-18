#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <utility>
using std::pair;

class Zombie
{
    private : 
        pair<int,int> coordinates;
        int health, attack, range, ID;

    public:
        void init(int health, int attack, int range, pair<int,int> coordinates);
        void receiveAttack(int damage);
        void move(int rowDelta, int colDelta);
        bool isAlive();
        bool isInRange(pair<int,int>& coordinates);
        void setID(int ID);
        pair<int,int> getCoordinates();
        int getHealth();
        int getAttack();
        int getRange();
        int getID();
};

#endif