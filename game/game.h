#ifndef GAME_H
#define GAME_H
#include "game_state.h"
#include "characters/zombie.h"
#include "characters/alien.h"
#include <vector>
#include <queue>
#include <utility>

using std::vector;
using std::pair;
using std::queue;

class Game
{
    private:
        vector<vector<char>> board;
        // ID as an element of queue, 0 = Alien, >= 1 = Zombie's ID
        queue<int> turnQueue;
        int numOfZombies;
        int numOfZombiesAlive, healthLost, attackGained;

        void cleanOldData();
        void generateNewBoard();
        vector<int> getAlienStats();
        vector<vector<int>> getZombieStats();
        void searchGoodMove(pair<int,int> coord, vector<int>& moveDetails, vector<int>& zombiesHealth, int energyLeft, int initDirection, int curDirection, vector<vector<int>>& arrowsData, vector<int>& goodMoveDetails, int& goodDirection, vector<vector<int>>& goodArrowsData);
        vector<int> getClosestZombiesFromState(pair<int,int>& coord, vector<int>& zombiesHealth);
        vector<int> getClosestZombies();

    public:
        bool gameOver;
        int rows, cols;
        Alien alien;
        vector<Zombie> zombies;
        
        int getZombieIDAt(pair<int,int> coordinates);
        pair<int, int> getZombieCoordinatesFromBoard(int zombieID);
        int getCurrentTurn() ;
        void newGame(int rows, int cols, int numofZombies, int difficulty);
        void loadGame(GameState& save);
        void nextTurn();
        GameState getGameState();
        int checkCoordinate(pair<int,int>& coord, int direction);
        void changeArrowDirection(pair<int,int> arrowCoordinates, int newDirection);
        void moveZombie(int id, int direction);
        void moveAlien(int direction);
        bool attackZombie(pair<int,int> zombieCoordinates);
        bool attackAlien(int zombieID);
        void clearTrail(pair<int,int> coordinates);
        int encounterPod();
        void encounterRock(pair<int,int> rockCoordinates);
        void encounterHealth();
        void encounterArrow();
        int getNumOfZombiesAlive();
        pair<int,vector<vector<int>>> getHint();
};

#endif