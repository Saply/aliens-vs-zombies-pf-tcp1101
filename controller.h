#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game/game.h"

class Controller
{
    private:

        int borderThemeID, objectThemeID;
        int maxHealth, minHealth, maxDamage, minDamage, minZombieRange, maxZombieRange;
        int numOfRows, numOfCols;
        int saveNumberChoice;
        int numberOfPlayerTurns;
        bool alienHasMoved;
        Game game;

        void themeMenu();
        void changeObjectThemeMenu();
        void changeBorderThemeMenu();
        void newGame();
        void loadGame();
        void startGame();
        void keepMovingAlien(int direction);
        void keepMovingZombie(int id, int direction);
        void displayWithAction(vector<int> action);
        void displayWithoutAction(int millisecond);


    public:

        Controller();
        void mainMenu();
};

#endif