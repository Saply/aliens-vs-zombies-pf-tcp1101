#include "alien.h"
#include <algorithm>

// Initialize Alien stats
void Alien::init(pair<int,int> coordinates, int health, int energy)
{
    this->health = health;
    this->attack = 0;
    this->coordinates = coordinates;
    this->energy = energy;
}

void Alien::receiveAttack(int damage)
{
    this->health = std::max(this->health-damage, 0);
}

void Alien::incHealth(int amount)
{
    this->health = std::min(maxHealth, this->health + amount);
}

void Alien::incAttack(int amount)
{
    this->attack += amount;
}

void Alien::incEnergy()
{
    this->energy += 2;
    if (this->energy > 5)
    {
        this->energy = 5;
    }
}

void Alien::decEnergy()
{
    --this->energy;
}

void Alien::move(int rowDelta, int colDelta)
{
    this->coordinates.first += rowDelta;
    this->coordinates.second += colDelta;
}

bool Alien::isAlive()
{
    return this->health > 0;
}

// Getters
pair<int,int> Alien::getCoordinates()
{
    return this->coordinates;
}

int Alien::getHealth()
{
    return this->health;
}

int Alien::getAttack()
{
    return this->attack;
}

int Alien::getEnergy()
{
    return this->energy;
}