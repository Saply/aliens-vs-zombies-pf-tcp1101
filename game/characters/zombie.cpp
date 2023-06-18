#include "zombie.h"
#include <algorithm>

// Initialize Zombie stats
void Zombie::init(int health, int attack, int range, pair<int,int> coordinates)
{
    this->health = health;
    this->attack = attack;
    this->range = range;

    // (row, column)
    this->coordinates = coordinates;
}

void Zombie::receiveAttack(int damage)
{
    this->health = std::max(this->health-damage, 0);
}

void Zombie::move(int rowDelta, int colDelta)
{
    this->coordinates.first += rowDelta;
    this->coordinates.second += colDelta;
}

bool Zombie::isAlive()
{
    return this->health > 0;
}

// Check if Alien is within the range of zombie
bool Zombie::isInRange(pair<int,int>& coordinates)
{
    // Pythagorean theorem for distance
    // sqrt((x2-x1)^2 - (y2-y1)^2) = d
    // Alien is in range if d <= range
    // Instead of dealing with floating point, we deal with integer
    // ((x2-x1)^2 - (y2-y1)^2) <= range * range
    int diffRow = (coordinates.first - this->coordinates.first);
    int diffCol = (coordinates.second - this->coordinates.second);

    return 
    (
        diffRow * diffRow + diffCol * diffCol <= this->range * this->range
    );
}

// Setter
void Zombie::setID(int ID)
{
    this->ID = ID;
}

// Getters
pair<int,int> Zombie::getCoordinates()
{
    return this->coordinates;
}

int Zombie::getHealth()
{
    return this->health;
}

int Zombie::getAttack()
{
    return this->attack;
}

int Zombie::getRange()
{
    return this->range;
}

int Zombie::getID()
{
    return this->ID;
}