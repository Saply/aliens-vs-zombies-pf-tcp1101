#ifndef ALIEN_H
#define ALIEN_H

#include <utility>
using std::pair;

class Alien
{
    private : 
        pair<int,int> coordinates;
        int health, attack, energy;
        const int maxHealth = 200, maxEnergy = 5;

    public:
        void init(pair<int,int> coordinates, int health, int energy);
        void receiveAttack(int damage);
        void incHealth(int amount);
        void incAttack(int amount);
        void incEnergy();
        void decEnergy();
        void move(int rowDelta, int colDelta);   
        bool isAlive();
        pair<int,int> getCoordinates();
        int getHealth();
        int getAttack();
        int getEnergy();
};

#endif