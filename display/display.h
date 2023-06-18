#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <utility>
#include <string>
#include "../game/game_state.h"

using std::string;
using std::vector;
using std::pair;

namespace Display
{
    // input validation functions
    bool isInteger(string text);
    int isChoiceValid(string input, int min, int max);
    bool isArrowCoordinatesValid(string input);

    int pressEnterToContinue();
    void clearScreen();
    void pauseForMilliseconds(int timeInSeconds);
    void displayErrorMessage();
    void displayInvalidSaveLoadFile(char saveOrLoad);

    void displayMainMenuOptions();
    int getMainMenuOption();
    int getChangeThemeMenu();
    int getObjectTheme(int currentObjectTheme);
    int getBorderTheme(int currentBorderTheme);
    pair<int, int> getBoardDimensionOption();
    void displayDifficultyOptions();
    int getDifficultyOptions();
    int getNumberOfZombies();
    int getCommand();
    void displayHelp();
    void displayStats(vector<int> stats);
    void displayNoEnergy();
    int getSaveFileChoice(vector<int> saveFiles);
    int getLoadFileChoice(vector<int> saveFiles);
    int deleteSaveFile();
    int quitGame();
    pair<int, int> getArrowCoordinates(int numberOfRows);
    int getNewArrowDirection();

    void displayGameState(int borderThemeID, int objectThemeID, GameState gameState, int currentTurn);
    void displayCharacterStats(vector<int> alienAttributes, int numberOfZombies, vector<vector<int>> zombieAttributes, int turn);
    void displayBoard(vector<vector<char>> board, vector<char> objectTheme, vector<char> borderTheme);
    void displayGameAction(vector<int> actions);
    void displayHint(int maxRows, pair<int, vector<vector<int>>> hints);
    void addLetter(bool outcome, string& s1, string& s2, string& s3, string& s4, string& s5);
    string cycleString(string& s);
    void displayWinLose(bool outcome, int duration);
    void displayMessage(string message);
}

#endif
