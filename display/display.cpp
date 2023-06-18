#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <thread>
#include "display.h"
#include "themes/theme.h"

using std::cout;
using std::cin;
using std::vector;
using std::string;


// input validation functions
bool Display::isInteger(string text)
{
    for (int i = 0; i < size(text); i++)
    {
        if (!isdigit(text[i]))
        {
            return false;
        }
    }
    return true;
}

// check if input is valid, return the correct choice if it's valid, return -1 if invalid
int Display::isChoiceValid(string input, int min, int max)
{
    
    if (!isInteger(input))
    {
        return -1;
    }
    int choice = stoi(input);
    if (choice < min || choice > max)
    {
        return -1;
    }
    return choice;
}

// checks if arrow coordinates inputted by user are valid
bool Display::isArrowCoordinatesValid(string input)
{
    if (input.size() != 2)
    {
        return false;
    }
    if (input[0] > 'z' || input[0] < 'a')
    {
        return false;
    }
    if (input[1] > '9' || input[1] < '1')
    {
        return false;
    }
    return true;
}

// press enter to continue
int Display::pressEnterToContinue()
{
    #if defined(_WIN32)
        return std::system("pause");
    #elif defined(__linux__) || defined(__APPLE__)
        return std::system(R"(read -p "Press any key to continue . . . " dummy)");
    #endif
}


// clears the screen
void Display::clearScreen()
{
    #if defined(_WIN32)
        std::system("cls");
    #elif defined(__linux__) || defined(__APPLE__)
        std::system("clear");
    #endif
}

// Pauses the game for a set amount of time. 1s = 1000ms
void Display::pauseForMilliseconds(int timeInMilliseconds)
{

    std::this_thread::sleep_for(std::chrono::milliseconds(timeInMilliseconds));
}

// displays error message for invalid commands
void Display::displayErrorMessage()
{
    cout << "Invalid Command\n";
    pressEnterToContinue();
}

// displays error message for invalid save/load file
void Display::displayInvalidSaveLoadFile(char saveOrLoad)
{
    string error;
    if (saveOrLoad == 's')
        error = "Invalid Save File choice!";
    else if (saveOrLoad == 'l')
        error = "Invalid Load File choice!";
    cout << error << '\n';
    pressEnterToContinue();
}

// display main menu options
void Display::displayMainMenuOptions()
{
    cout << "   .: Alien vs Zombies :.   \n";
    cout << "+==========================+\n";
    cout << "|     1. Start Game        |\n";
    cout << "+--------------------------+\n";
    cout << "|     2. Load Game         |\n";
    cout << "+--------------------------+\n";
    cout << "|     3. Change Theme      |\n";
    cout << "+--------------------------+\n";
    cout << "|     4. View Stats        |\n";
    cout << "+--------------------------+\n";
    cout << "|     5. Quit Game         |\n";
    cout << "+==========================+\n\n";

    cout << "Choose an option (1-5) : "; 
}

// prompts the user to select an option from main menu
// -1: invalid
// 1: start game
// 2: load
// 3: change theme
// 4: stats
// 5: quit
int Display::getMainMenuOption()
{
    clearScreen();
    string input;
    displayMainMenuOptions();
    cin >> input;

    return isChoiceValid(input, 1, 5);
}


// -1: invalid
// 1: Object theme
// 2: Border theme
// 3: return
int Display::getChangeThemeMenu()
{
    clearScreen();
    string input;
    cout << "    .: Change Theme :.    \n";
    cout << "+========================+\n";
    cout << "|  1.  Object Theme      |\n";
    cout << "+------------------------+\n";
    cout << "|  2.  Border Theme      |\n";
    cout << "+========================+\n\n";

    cout << "Choose a theme to change (1-2) or press 3 to return : ";
    cin >> input;

    return isChoiceValid(input, 1, 3);
}

// -1: invalid
// 0-2: theme number (index of theme in theme.cpp)
// 3: return
int Display::getObjectTheme(int currentObjectTheme)
{
    clearScreen();
    string input;
    cout << "    .: Choose a theme :.    \n";
    cout << "+==========================+\n";
    cout << "|    1. Theme 1 "; cout << ((currentObjectTheme == 0) ? "(current)" : "         "); cout << "  |\n";
    cout << "+--------------------------+\n";
    cout << "|    2. Theme 2 "; cout << ((currentObjectTheme == 1) ? "(current)" : "         "); cout << "  |\n";
    cout << "+--------------------------+\n";
    cout << "|    3. Theme 3 "; cout << ((currentObjectTheme == 2) ? "(current)" : "         "); cout << "  |\n";
    cout << "+==========================+\n\n";

    cout << "Choose a theme (1-3) or press 4 to return : ";

    cin >> input;
    int choice = isChoiceValid(input, 1, 4);
    if (choice == -1)
    {
        return -1;
    }
    return (choice - 1);
}

// -1: invalid
// 0-2: theme number (index of theme in theme.cpp)
// 3: return
int Display::getBorderTheme(int currentBorderTheme)
{
    clearScreen();
    string input;
    cout << "    .: Choose a theme :.    \n";
    cout << "+==========================+\n";
    cout << "|    1. Theme 1 "; cout << ((currentBorderTheme == 0) ? "(current)" : "         "); cout << "  |\n";
    cout << "+--------------------------+\n";
    cout << "|    2. Theme 2 "; cout << ((currentBorderTheme == 1) ? "(current)" : "         "); cout << "  |\n";
    cout << "+--------------------------+\n";
    cout << "|    3. Theme 3 "; cout << ((currentBorderTheme == 2) ? "(current)" : "         "); cout << "  |\n";
    cout << "+==========================+\n\n";
    
    cout << "Choose a theme (1-3) or press 4 to return : ";

    cin >> input;
    int choice = isChoiceValid(input, 1, 4);
    if (choice == -1)
    {
        return -1;
    }
    return (choice - 1);
}

// (row, column); dimensions.first and dimensions.second will be -1 if input is invalid
pair<int, int> Display::getBoardDimensionOption()
{
    clearScreen();
    string numberOfRows, numberOfColumns;
    cout << "     .: Board Dimensions :.    \n";
    cout << "+=============================+\n";
    cout << "|  1. Select board row count  |\n";
    cout << "+=============================+\n\n";

    cout << "Number of Rows (odd number from 3-9) : ";
    cin >> numberOfRows;

    clearScreen();
    cout << "      .: Board Dimensions :.     \n";
    cout << "+================================+\n";
    cout << "|  2. Select board column count  |\n";
    cout << "+================================+\n\n";

    cout << "Number of Columns (odd number from 9-25) : ";
    cin >> numberOfColumns;

    int rowChoice = isChoiceValid(numberOfRows, 3, 9);
    int columnChoice = isChoiceValid(numberOfColumns, 9, 25);

    // set to invalid if numberOfRows or numberOfColumns is even
    if (rowChoice % 2 == 0)
    {
        rowChoice = -1;
    }
    if (columnChoice % 2 == 0)
    {
        columnChoice = -1;
    }

    // if column choice or row choice is invalid, set both to invalid
    if (rowChoice == -1 || columnChoice == -1)
    {
        rowChoice = -1;
        columnChoice = -1;
    }


    pair<int, int> dimensions;
    dimensions.first = rowChoice; dimensions.second = columnChoice;

    return dimensions;
}

// displays difficulty options
void Display::displayDifficultyOptions()
{
    cout << "   .: Difficulty :.   \n";
    cout << "+====================+\n";
    cout << "|     1. Easy        |\n";
    cout << "+--------------------+\n";
    cout << "|     2. Medium      |\n";
    cout << "+--------------------+\n";
    cout << "|     3. Hard        |\n";
    cout << "+====================+\n\n";

    cout << "Choose an option (1-3) or press 4 to return to Main Menu : ";
}

// -1: invalid
// 1-3: easy, medium, hard
// 4: return
int Display::getDifficultyOptions()
{
    clearScreen();
    string input;
    displayDifficultyOptions();
    cin >> input;

    return isChoiceValid(input, 1, 4);
}

// -1: invalid
// 1-9: number of zombies
int Display::getNumberOfZombies()
{
    clearScreen();
    string input;
    cout << "       .: Number of zombies :.       \n";
    cout << "+===================================+\n";
    cout << "|  4. Select the number of zombies  |\n";
    cout << "+===================================+\n\n";

    cout << "Choose the number of zombies (1-9) : ";
    cin >> input;

    int choice = isChoiceValid(input, 1, 9);
    return choice;
}


// -1: invalid
// 0: help
// 1-4: up, down, left, right
// 5: arrow
// 6: save
// 7: quit
// 8: hint
int Display::getCommand()
{
    string input;
    cout << "Enter a command (press h for help) : ";
    cin >> input;

    if (input.compare("h") == 0) return 0;
    if (input.compare("up") == 0) return 1;
    if (input.compare("down") == 0) return 2;
    if (input.compare("left") == 0) return 3;
    if (input.compare("right") == 0) return 4;
    if (input.compare("arrow") == 0) return 5;
    if (input.compare("save") == 0) return 6;
    if (input.compare("quit") == 0) return 7;
    if (input.compare("hint") == 0) return 8;

    return -1;
}

// displays help menu
void Display::displayHelp()
{
    clearScreen();
    char input;
    cout << "        .: List of Commands :.        " << "\n";
    cout << "+====================================+" << "\n";
    cout << "|   1. up (move the Alien up)        |" << "\n";
    cout << "+------------------------------------+" << "\n";
    cout << "|   2. down (move the Alien Down     |" << "\n";
    cout << "+------------------------------------+" << "\n";
    cout << "|   3. left (move the Alien left)    |" << "\n";
    cout << "+------------------------------------+" << "\n";
    cout << "|   4. right (move the alien right)  |" << "\n";
    cout << "+------------------------------------+" << "\n";
    cout << "|   5. arrow (rotate an arrow)       |" << "\n";
    cout << "+------------------------------------+" << "\n";
    cout << "|   6. save (save the game)          |" << "\n";
    cout << "+------------------------------------+" << "\n";
    cout << "|   7. quit (quit the game)          |" << "\n";
    cout << "+------------------------------------+" << "\n";
    cout << "|   8. hint (get a useful hint)      |" << "\n";
    cout << "+====================================+" << "\n\n";

    pressEnterToContinue();
}

// displays player stats
// {games won, games lost, total player turns, 
// total zombies spawned, total zombies defeated,
// total health lost, total attack gained, 
// last border theme, last object theme}
void Display::displayStats(vector<int> stats)
{
    clearScreen();
    string gamesWon = std::to_string(stats[0]);
    string gamesLost = std::to_string(stats[1]);
    string turnsPlayed = std::to_string(stats[2]);
    string zombiesSpawned = std::to_string(stats[3]);
    string zombiesDefeated = std::to_string(stats[4]);
    string healthLost = std::to_string(stats[5]);
    string attackGained = std::to_string(stats[6]);

    cout << "       .: Player Stats :.       \n";
    cout << "+==============================+\n";
    cout << "|  1. Games Won : " << gamesWon; 
    for (int i = 0; i < 13 - gamesWon.size(); i++) cout << " "; cout << "|\n";
    cout << "+------------------------------+\n";
    cout << "|  2. Games Lost : " << gamesLost; 
    for (int i = 0; i < 12 - gamesLost.size(); i++) cout << " "; cout << "|\n";
    cout << "+------------------------------+\n";
    cout << "|  3. Turns Played : " << turnsPlayed;
    for (int i = 0; i < 10 - turnsPlayed.size(); i++) cout << " "; cout << "|\n";
    cout << "+------------------------------+\n";
    cout << "|  4. Zombies Spawned : " << zombiesSpawned;
    for (int i = 0; i < 7 - zombiesSpawned.size(); i++) cout << " "; cout << "|\n";
    cout << "+------------------------------+\n";
    cout << "|  5. Zombies Defeated : " << zombiesDefeated;
    for (int i = 0; i < 6 - zombiesDefeated.size(); i++) cout << " "; cout << "|\n";
    cout << "+------------------------------+\n";
    cout << "|  6. Health Lost : " << healthLost;
    for (int i = 0; i < 11 - healthLost.size(); i++) cout << " "; cout << "|\n";
    cout << "+------------------------------+\n";
    cout << "|  7. Attack Gained : " << attackGained;
    for (int i = 0; i < 9 - attackGained.size(); i++) cout << " "; cout << "|\n";
    cout << "+==============================+\n\n";

    pressEnterToContinue();
}

// displays when the alien has no energy
void Display::displayNoEnergy()
{
    cout << "Alien is too tired to turn any arrows right now :(\n";
    pressEnterToContinue();
}


// get choice of user on where to save game
// -1: invalid
// 1: save file 1
// 2: save file 2
// 3: save file 3
// 4: return
int Display::getSaveFileChoice(vector<int> saveFiles)
{
    clearScreen();
    // saveFiles = {1, 0, 3}; file 1 and 3 exists
    string input;
    cout << "     .: Choose a save file :.     " << "\n";
    cout << "+================================+\n";
    cout << "|     1. Save File 1 ";
    cout << ((saveFiles[0] != 0) ? "(taken)" : "(empty)"); cout << "     |\n";
    cout << "+--------------------------------+\n";
    cout << "|     2. Save File 2 ";
    cout << ((saveFiles[1] != 0) ? "(taken)" : "(empty)"); cout << "     |\n";
    cout << "+--------------------------------+\n";
    cout << "|     3. Save File 3 ";
    cout << ((saveFiles[2] != 0) ? "(taken)" : "(empty)"); cout << "     |\n";
    cout << "+================================+\n\n";
    cout << "Choose a save file (1-3) or press 4 to return back : ";

    cin >> input;
    if (isChoiceValid(input, 1, 4) == -1)
    {
        return -1;
    }
    int choice = stoi(input); 

    if (choice == 4)
    {
        return 4;
    }


    clearScreen();
    cout << "Are you sure you want to save the file in slot " << choice << " ? This will override any saved game existing in that slot." << "\n";
    cout << "Press 1 to confirm, press 2 to return : ";
    cin >> input;

    if (stoi(input) == 2)
    {
        return 4;
    }

    if (isChoiceValid(input, 1, 1) == -1)
    {
        return -1;
    }

    return choice;
}


// return save file choice (does not check if save file has a save game inside)
// -1: invalid
// 1: save file 1
// 2: save file 2
// 3: save file 3
// 4: return
int Display::getLoadFileChoice(vector<int> saveFiles)
{
    clearScreen();
    // saveFiles = {1, 0, 3}; file 1 and 3 exists
    string input;
    cout << "      .: Load a save file :.     " << "\n";
    cout << "+===============================+\n";
    cout << "|     1. Save File 1 ";
    cout << ((saveFiles[0] != 0) ? "(taken)" : "(empty)"); cout << "    |\n";
    cout << "+-------------------------------+\n";
    cout << "|     2. Save File 2 ";
    cout << ((saveFiles[1] != 0) ? "(taken)" : "(empty)"); cout << "    |\n";
    cout << "+-------------------------------+\n";
    cout << "|     3. Save File 3 ";
    cout << ((saveFiles[2] != 0) ? "(taken)" : "(empty)"); cout << "    |\n";
    cout << "+===============================+\n\n";
    cout << "Choose a save file (1-3) or press 4 to return back : ";

    cin >> input;
    if (isChoiceValid(input, 1, 4) == -1)
    {
        return -1;
    }
    int choice = stoi(input); 

    if (choice == 4)
    {
        return 4;
    }

    clearScreen();
    cout << "Are you sure you want to load the file in slot " << choice << " ?\n";
    cout << "Press 1 to confirm, press 2 to return : ";
    cin >> input;

    if (stoi(input) == 2)
    {
        return 4;
    }

    if (isChoiceValid(input, 1, 1) == -1)
    {
        return -1;
    }

    return choice;
}

// 1: delete file
// 2: don't delete file
// -1: invalid command
int Display::deleteSaveFile()
{
    clearScreen();
    cout << "   .: Delete Save File :.   \n";
    cout << "+==========================+\n";
    cout << "|          1. Yes          |\n";
    cout << "+--------------------------+\n";
    cout << "|          2. No           |\n";
    cout << "+==========================+\n\n";

    cout << "Do you want to delete this save file? (1-2) : ";

    string input;
    cin >> input;
    return isChoiceValid(input, 1, 2);
}

// (row, column); Does not check if arrow actually exists there
pair<int, int> Display::getArrowCoordinates(int numberOfRows)
{
    pair<int, int> coordinates;
    string input;
    cout << "Which arrow do you want to rotate? (ex: a2, c4, g5) : ";
    cin >> input;

    // return (-1, -1) as coordinates if invalid coordinates is given
    if (!isArrowCoordinatesValid(input))
    {
        coordinates.first = -1; coordinates.second = -1;
        return coordinates;
    }

    coordinates.first = numberOfRows - int(input[1] - '0');
    coordinates.second = int(input[0] - 'a');

    return coordinates;
}

// asks if the user wants to quit the game
int Display::quitGame()
{
    clearScreen();
    cout << "   .: Quit Game :.   \n";
    cout << "+===================+\n";
    cout << "|      1. Yes       |\n";
    cout << "+-------------------+\n";
    cout << "|      2. No        |\n";
    cout << "+===================+\n\n";

    cout << "Do you want to quit the game? (1-2) : ";

    string input;
    cin >> input;
    return isChoiceValid(input, 1, 1);
}



// -1: invalid
// 0: back
// 1-4: up, down, left, right
int Display::getNewArrowDirection()
{
    string input; 
    cout << "Choose the direction of the arrow (up, down, left, right) or press b to return : ";
    cin >> input;

    if (input.compare("b") == 0) {return 0;};
    if (input.compare("up") == 0) {return 1;};
    if (input.compare("down") == 0) {return 2;};
    if (input.compare("left") == 0) {return 3;};
    if (input.compare("right") == 0) {return 4;};

    return -1;
}



// display board, stats, etc.
void Display::displayGameState(int borderThemeID, int objectThemeID, GameState gameState, int currentTurn)
{
    clearScreen();
    vector<char> borderTheme = Theme::borderThemes[borderThemeID];
    vector<char> objectTheme = Theme::objectThemes[objectThemeID];

    // display board
    displayBoard(gameState.board, objectTheme, borderTheme);

    // display stats and turns
    cout << "\n";
    displayCharacterStats(gameState.alienAttributes,gameState.numberOfZombies ,gameState.zombieAttributes, currentTurn);

    cout << "\n";
}

void Display::displayBoard(vector<vector<char>> board, vector<char> objectTheme, vector<char> borderTheme)
{
    // Object themes
    // alien, health, rock, pod, trail, arrows (up, down, left, right), space;

    // Border themes
    // verticalBorder, horizontalBorder1, horizontalBorder2

    char objects[256];
    objects['A'] = objectTheme[0]; objects['h'] = objectTheme[1];
    objects['r'] = objectTheme[2]; objects['p'] = objectTheme[3];
    objects['.'] = objectTheme[4]; objects['^'] = objectTheme[5];
    objects['v'] = objectTheme[6]; objects['<'] = objectTheme[7];
    objects['>'] = objectTheme[8]; objects[' '] = objectTheme[9];

    objects['Z'] = 'Z'; objects['1'] = '1'; objects['2'] = '2'; objects['3'] = '3'; objects['4'] = '4'; objects['5'] = '5'; objects['6'] = '6'; objects['7'] = '7'; objects['8'] = '8'; objects['9'] = '9';
    

    int numberOfRows = board.size();
    int numberOfColumns = board[0].size();


    int boardspan = numberOfColumns * 4 + 1;
    int titleLength = 22; // length of ".: Alien vs Zombies :."

    // print spaces before title
    cout << "\n";
    for (int i = 0; i < (boardspan - titleLength) / 2 + 4; i++)
    {
        cout << " ";
    }

    cout << ".: Alien vs Zombies :." << "\n";
    cout << "   ";
    for (int i = 0; i < boardspan / 2; i++)
    {
        cout << "=-";
    }
    cout << "=";
    cout << "\n";


    // print out each row
    for (int row = 0; row < numberOfRows; row++)
    {
        
        // print out each column
        cout << "   ";
        for (int col = 0; col < numberOfColumns; col++)
        {
            cout << borderTheme[1] << borderTheme[2] << borderTheme[2] << borderTheme[2];
        }
        cout << borderTheme[1] << "\n";

        // print out row number
        cout << " " << numberOfRows - row << " ";

        for (int col = 0; col < numberOfColumns; col++)
        {
            cout << borderTheme[0] << " " << objects[board[row][col]] << " ";
        }
        cout << borderTheme[0] << "\n";
    }

    cout << "   ";
    for (int col = 0; col < numberOfColumns; col++)
    {
        cout << borderTheme[1] << borderTheme[2] << borderTheme[2] << borderTheme[2];
    }
    cout << borderTheme[1] << "\n";

    // print column number
    cout << "    ";
    for (int col = 0; col < numberOfColumns; col++)
    {
        cout << " " << char('a' + col) << "  ";
    }
    cout << "\n";
}


void Display::displayCharacterStats(vector<int> alienAttributes, int numberOfZombies,vector<vector<int>> zombieAttributes, int turn)
{
    // display alien stats

    // AlienAttributes: {x, y, hp, attack, energy}
    int alienHealth = alienAttributes[2];
    int alienAttack = alienAttributes[3];
    int alienEnergy = alienAttributes[4];

    cout << (turn == 0 ? "-> " : "   ");
    cout << "Alien    : Life ";
    cout.width(3); cout << alienHealth;
    cout << ", Attack ";
    cout.width(3); cout << alienAttack;
    cout << ", Energy (" << alienEnergy << "/5)";
    cout << "\n";


    // display zombie stats
    for (int i = 0; i < numberOfZombies; i++)
    {
        int zombieHealth = zombieAttributes[i][3];
        int zombieAttack = zombieAttributes[i][4];
        int zombieRange = zombieAttributes[i][5];

        cout << (turn == i + 1 ? "-> " : "   ");
        cout << "Zombie " << i + 1 << " : Life ";
        cout.width(3); cout << zombieHealth;
        cout << ", Attack ";
        cout.width(3); cout << zombieAttack;
        cout << ", Range ";
        cout.width(2); cout << zombieRange;
        cout << "\n";
    }
}

/*
    actions:
    {0} = h
    {1, x} = rock. x = 0: Alien. x = 1: Zombie
    {2, x} = pod, dealt 10 damage to zombie x
    {3, x} = Alien attacked zombie x
    {4, x, y} = zombie x dealth y damage to alien
    {5, x} = zombie x was slain
    {6} = alien hit a wall
    {7, x, y} = zombie x moved. y = 0: no alien in range. y = 1: alien is in range.
*/
void Display::displayGameAction(vector<int> actions)
{
    int action = actions[0];

    // Alien gained 20 health
    if (action == 0)
    {
        cout << "Alien found a health pack. Alien gained 20 health.";
    }
    else if (action == 1)
    {
        string character = (actions[1] == 0 ? "Alien" : "Zombie ");
        if (actions[1] == 1) character += actions[2] + '0';
        cout << character <<" hit a rock. " << character << " has stopped moving.";
    }
    else if (action == 2)
    {
        cout << "Alien found a pod. The pod dealt 10 damage to zombie " << actions[1] << ".";
    }
    else if (action == 3)
    {
        cout << "Alien attacked zombie " << actions[1] << ".";
    }
    else if (action == 4)
    {
        cout << "Zombie " << actions[1] << " attacked Alien. Alien took " << actions[2] << " damage.";
    }
    else if (action == 5)
    {
        cout << "Zombie " << actions[1] << " was slain.";
    }
    else if (action == 6)
    {
        cout << "Alien hit a wall. Alien has stopped moving.";
    }
    else if (action == 7)
    {
        cout << "Zombie " << actions[1] << " moved. ";
        cout << ((actions[2] == 0) ? "Alien not in range." : "Alien is in range.");
    }
    cout << "\n";

    pressEnterToContinue();
}

// (direction, {{arrow row, arrow column}})
void Display::displayHint(int maxRows, pair<int, vector<vector<int>>> hints)
{
    vector<string> directions = {"up", "down", "left", "right"};
    int direction = hints.first;
    vector<vector<int>> arrows = hints.second;

    cout << "hint: ";
    for (int i = 0; i < arrows.size(); i++)
    {
        if (i != 0)
        {
            cout << "      ";
        }

        cout << i+1 << ". arrow ";
        cout << char(arrows[i][1] + 'a');
        cout << maxRows - arrows[i][0];
        cout << " " << directions[arrows[i][2]];
        cout << "\n";
    }
    if (arrows.size() != 0)
    {
        cout << "      ";
    }
    cout << arrows.size() + 1 << ". " << directions[direction] << "\n";
}


// Adds letters to the strings via pass by reference
void Display::addLetter(bool outcome, string& s1, string& s2, string& s3, string& s4, string& s5)
{   
    if (outcome)
    {
        s1 = "YY   YY    OOOOO    UU   UU           W     W   IIIIIII   NN    N     !!!                    ";
        s2 = " YY YY    OO   OO   UU   UU           W  W  W     III     N N   N     !!!                    ";
        s3 = "  YYY     OO   OO   UU   UU           W  W  W     III     N  N  N     !!!                    ";
        s4 = "   Y      OO   OO   UU   UU           WW W WW     III     N   N N                            ";
        s5 = "   Y       OOOOO     UUUUU             WW WW    IIIIIII   N    NN     !!!                    ";
        return;     
    }
    else
    {
        s1 = "YY   YY    OOOOO    UU   UU           LL         OOOOO     SSSSSS   EEEEEEE     !!!                    ";
        s2 = " YY YY    OO   OO   UU   UU           LL        OO   OO   SS        EE          !!!                    ";
        s3 = "  YYY     OO   OO   UU   UU           LL        OO   OO    SSSSS    EEEEEEE     !!!                    ";
        s4 = "   Y      OO   OO   UU   UU           LL        OO   OO        SS   EE                                 ";
        s5 = "   Y       OOOOO     UUUUU            LLLLLLL    OOOOO    SSSSSS    EEEEEEE     !!!                    ";
        return;     
    } 
}

// Cycles the string
string Display::cycleString(string& s)
{
    string newString;
    for (int i = 1; i < s.length(); i++)
    {
        newString += s[i];
    }
    newString += s[0];
    return newString;
}


// true: win
// false: lose
// duration (0.1s) duration = 10 -> 1s
void Display::displayWinLose(bool outcome, int duration)
{
    string s1, s2, s3, s4, s5;
    addLetter(outcome, s1, s2, s3, s4, s5);

    for (int i = 0; i < duration; i++)
    {   
        cout << "\n";
        for (int j = 0; j < s1.length() + 10; j++)
        {
            cout << "=";
        }
        cout << "\n";

        cout << "||   "  << s1 << "   ||"  << "\n";
        cout << "||   "  << s2 << "   ||"  << "\n";
        cout << "||   "  << s3 << "   ||"  << "\n";
        cout << "||   "  << s4 << "   ||"  << "\n";
        cout << "||   "  << s5 << "   ||"  << "\n";

        for (int j = 0; j < s1.length() + 10; j++)
        {
            cout << "=";
        }
        cout << "\n";

        s1 = cycleString(s1);
        s2 = cycleString(s2);
        s3 = cycleString(s3);
        s4 = cycleString(s4);
        s5 = cycleString(s5);

        pauseForMilliseconds(100);
        clearScreen();
    }
}

// used for debugging
void Display::displayMessage(string message)
{
    cout << message << "\n";
}