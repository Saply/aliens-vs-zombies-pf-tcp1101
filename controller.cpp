#include "controller.h"
#include "display/display.h"
#include "display/themes/theme.h"
#include "game/game.h"
#include "game/game_state.h"
#include "save/save.h"

Controller::Controller()
{
    // set default theme
    this->borderThemeID = 0;
    this->objectThemeID = 0;
}

// Create a new game
void Controller::newGame()
{
    pair<int, int> boardDimensions;
    int difficulty, numOfZombies;

    // Default values
    this->saveNumberChoice = 0;
    this->numberOfPlayerTurns = 0;
    this->alienHasMoved = false;

    // Get the input from user

    boardDimensions = Display::getBoardDimensionOption();
    while (boardDimensions.first == -1)
    {
        Display::displayErrorMessage();
        boardDimensions = Display::getBoardDimensionOption();
    }
    this->numOfRows = boardDimensions.first;
    this->numOfCols = boardDimensions.second;
    difficulty = Display::getDifficultyOptions();

    while (difficulty == -1)
    {
        Display::displayErrorMessage();
        difficulty = Display::getDifficultyOptions();
    }

    if (difficulty == 4){
        return;
    }

    numOfZombies = Display::getNumberOfZombies();

    // Start new game with settings
    game.newGame(boardDimensions.first, boardDimensions.second, numOfZombies, difficulty);
    startGame();
}

// Load game from save file
void Controller::loadGame()
{ 
    // Get list of save files
    vector<int> saveFiles;
    saveFiles = Saves::getSaveFilesDirectory(); // {1, 2, 0}
    // Get choice from user on which file to pick
    int saveFileNumber = Display::getLoadFileChoice(saveFiles);

    // Check for invalid input or invalid file choice
    while(saveFileNumber == -1 || saveFiles[saveFileNumber-1] == 0)
    {
        Display::displayInvalidSaveLoadFile('l');
        saveFileNumber = Display::getLoadFileChoice(saveFiles);
    }

    
    // Return to the previous screen
    if(saveFileNumber == 4)
    {
        return;
    }
    this->saveNumberChoice = saveFileNumber;

    
    // Retrieve GameState struct from save files
    GameState state;
    state = Saves::saveFileToGameState(saveFileNumber);

    // Set rows cols for Controller class
    this->numOfRows = state.rows;
    this->numOfCols = state.cols;

    // Load values from save file
    this->numberOfPlayerTurns = state.numberOfPlayerTurns;
    this->alienHasMoved = false;

    // Load and start game from GameState
    game.loadGame(state);
    startGame(); 
}

// Start the game. Most of the logic for the game itself is contained within this function
void Controller::startGame()
{
    bool quit = false;
    bool displayHint = false;
    while (!game.gameOver && !quit)
    {
        int currentTurn = game.getCurrentTurn();

        // increase alien's energy by 2 every turn if have moved
        if (this->alienHasMoved == true && currentTurn == 0)
        {
            game.alien.incEnergy();
            this->alienHasMoved = false;
        }
        
        Display::displayGameState(borderThemeID, objectThemeID, game.getGameState(), game.getCurrentTurn());

        // Display hint once between Game State and Input for command if user has asked for hint
        if (displayHint)
        {
            Display::displayHint(this->numOfRows, game.getHint());
            displayHint = false;
        }

        // current turn = 0 means its alien (i.e user) turn
		if (currentTurn == 0)
        {
            pair<int,int> currentCoordinates = game.alien.getCoordinates();

            int command = -1;

            while (command == -1)
            {        
                command = Display::getCommand();
                // Invalid Command
                if (command == -1)
                {
                    Display::displayErrorMessage();
                } 
                // Display Help
                else if (command == 0)
                {
                    Display::displayHelp();
                }
                // Move Direction {1, 2, 3, 4} (U, D, L, R)
                else if (command <= 4)
                {
                    int coordinateCode = game.checkCoordinate(currentCoordinates, command-1);
                    command = (coordinateCode != -1 ? command : -1);

                    if (command == -1)
                    {
                        Display::displayErrorMessage();
                        continue;
                    }

                    this->numberOfPlayerTurns++;
                    this->keepMovingAlien(command-1);
                    this->alienHasMoved = true;
                    game.nextTurn();
                }
                // Change certain arrow direction
                else if (command == 5)
                {
                    pair<int,int> arrowCoordinates = Display::getArrowCoordinates(this->numOfRows);
                    int coordinateCode = game.checkCoordinate(arrowCoordinates, command-1);
                    command = (coordinateCode >= 0 && coordinateCode <= 3 ? command : -1);

                    // if coordinates is valid
                    if (command == -1)
                    {
                        Display::displayGameState(borderThemeID, objectThemeID, game.getGameState(), game.getCurrentTurn());
                        Display::displayErrorMessage();
                        Display::displayGameState(borderThemeID, objectThemeID, game.getGameState(), game.getCurrentTurn());
                        continue;
                    }

                    // if no energy left
                    if (game.alien.getEnergy() == 0)
                    {
                        // will change to no energy message
                        Display::displayGameState(borderThemeID, objectThemeID, game.getGameState(), game.getCurrentTurn());
                        Display::displayNoEnergy();
                        continue;
                    }

                    // get new arrow direction from user
                    int newDirection = Display::getNewArrowDirection();
                    if (newDirection) 
                    {
                        game.changeArrowDirection(arrowCoordinates, newDirection-1);
                        game.alien.decEnergy();
                        
                    }
                }
                // Save game
                else if (command == 6)
                {
                    int saveNum = Display::getSaveFileChoice(Saves::getSaveFilesDirectory());
                    while(saveNum == -1)
                    {
                        Display::displayInvalidSaveLoadFile('s');
                        saveNum = Display::getSaveFileChoice(Saves::getSaveFilesDirectory());
                    }

                    // Return to game
                    if(saveNum == 4)
                    {
                        continue;
                    }
                    this->saveNumberChoice = saveNum;
                    GameState state = game.getGameState();
                    state.numberOfPlayerTurns = this->numberOfPlayerTurns;
                    Saves::gameStateToSaveFile(saveNum, state);
                }
                // Quit game
                else if (command == 7)
                {
                    quit = (Display::quitGame() == 1) ? true : false;
                    break;
                }
                // Display hint
                else if (command == 8)
                {
                    displayHint = true;
                }
            }
        } 
        else 
        {
            // Zombie's turn, randomly move zombie in a valid direction
            int newDirection = rand() % 4;
            pair<int,int> currentCoordinate = game.zombies[currentTurn-1].getCoordinates();
            int coordinatesCode = game.checkCoordinate(currentCoordinate, newDirection);
            while (coordinatesCode == -1 || coordinatesCode == 8 || coordinatesCode == 9)
            {
                newDirection = rand() % 4;
                coordinatesCode = game.checkCoordinate(currentCoordinate, newDirection);
            }
            this->keepMovingZombie(currentTurn, newDirection);
            game.nextTurn();
        }
    }
    
    // When game ends
    if (game.gameOver)
    {
        // alien win
        if (game.alien.isAlive())
        {
            Display::displayWinLose(true, 100);
        }
        // alien lose
        else
        {
            Display::displayWinLose(false, 100);
        }

        // Adjust player stats once the game ends
        GameState finalState = game.getGameState();
        finalState.numberOfPlayerTurns = this->numberOfPlayerTurns;
        
        Saves::addToPlayerStats(
            finalState,
            this->borderThemeID,
            this->objectThemeID,
            finalState.numberOfZombies - game.getNumOfZombiesAlive(),
            game.alien.isAlive()
        );
        
        // Delete save file prompt
        if(this->saveNumberChoice != 0)
        {   
            int saveDeletionChoice = Display::deleteSaveFile();
            while(saveDeletionChoice == -1)
            {
                Display::displayErrorMessage();
                saveDeletionChoice = Display::deleteSaveFile();
            }
            if (saveDeletionChoice == 1)
                Saves::deleteSaveFile(this->saveNumberChoice);
        }
        
    }
}


/*
    {0} = h
    {1} = rock
    {2, x} = pod, dealt 10 damage to zombie x
    {3, x, y} = zombie x dealth y damage to alien
*/
// Displays when something specific happens (eg. encounter pod, zombie attack etc)
void Controller::displayWithAction(vector<int> a)
{
    Display::displayGameState(borderThemeID, objectThemeID, game.getGameState(), game.getCurrentTurn());
    Display::displayGameAction(a);
}

// Displays when normal things happen (eg. alien moving on an empty cell)
void Controller::displayWithoutAction(int millisecond)
{
    Display::displayGameState(borderThemeID, objectThemeID, game.getGameState(), game.getCurrentTurn());
    Display::pauseForMilliseconds(millisecond);
}


void Controller::keepMovingAlien(int direction)
{
    vector<pair<int,int>> trails;
    bool isMoving = true;	
    int currentDirection = direction;

    while (isMoving)
    {
        /*
            invalid = -1
            arrows = 0-3
            health = 4
            pod = 5
            rock = 6
            empty space = 7
            alien = 8
            zombie = 9
        */
        
        pair<int,int> currentCoordinates = game.alien.getCoordinates();
        int result = game.checkCoordinate(currentCoordinates,currentDirection);
        if (result >= 0 && result <= 3)
        {
            game.encounterArrow();
            trails.push_back(currentCoordinates);
            game.moveAlien(currentDirection);
            currentDirection = result;
            this->displayWithoutAction(1000);
        }
        else if (result == 4)
        {
            game.encounterHealth();
            trails.push_back(currentCoordinates);
            game.moveAlien(currentDirection);
            this->displayWithAction({0});
        }
        else if (result == 5)
        {
            int zombieAttacked = game.encounterPod();
            trails.push_back(currentCoordinates);
            game.moveAlien(currentDirection);
            this->displayWithAction({2,zombieAttacked});
            if (game.gameOver) break;
        }
        else if (result == 6)
        {
            isMoving = false;
            vector<int> moveRow = {-1, 1, 0, 0};
            vector<int> moveCol = {0, 0, -1, 1};
            pair<int,int> rockCoordinates = currentCoordinates;
            rockCoordinates.first += moveRow[currentDirection];
            rockCoordinates.second += moveCol[currentDirection];
            game.encounterRock(rockCoordinates);
            this->displayWithAction({1, 0});
        }
        else if (result == 7 || result == 10)
        {
            game.moveAlien(currentDirection);
            trails.push_back(currentCoordinates);
            this->displayWithoutAction(1000);
        }
        else if (result == 9)
        {
            vector<int> moveRow = {-1, 1, 0, 0};
            vector<int> moveCol = {0, 0, -1, 1};
            pair<int,int> zombieCoordinates = currentCoordinates;
            zombieCoordinates.first += moveRow[currentDirection];
            zombieCoordinates.second += moveCol[currentDirection];

            this->displayWithAction({3, game.getZombieIDAt(zombieCoordinates)});
            if (game.attackZombie(zombieCoordinates))
            {
                int zombieID = game.getZombieIDAt(zombieCoordinates);
                trails.push_back(currentCoordinates);
                game.moveAlien(currentDirection);
                this->displayWithAction({5, zombieID});
                if (game.gameOver) break;
            }
            else
            {
                isMoving = false;
            }
        }
        else 
        {
            isMoving = false;
            this->displayWithAction({6});
        }
    }
    vector<vector<bool>> hasGenerated(this->numOfRows, vector<bool>(this->numOfCols));
    for(int i = 0; i < trails.size(); i++)
    {
        if (trails[i] == game.alien.getCoordinates() || hasGenerated[trails[i].first][trails[i].second]) continue;
        hasGenerated[trails[i].first][trails[i].second] = 1;
        game.clearTrail(trails[i]);
        displayWithoutAction(500);
    }
    game.alien.incAttack(-game.alien.getAttack());
}

void Controller::keepMovingZombie(int id, int direction)
{

    pair<int,int> currentCoordinates = game.zombies[id-1].getCoordinates();
    int result = game.checkCoordinate(currentCoordinates, direction);

    //if zombie steps on a rock
    if (result == 6)
    {
        vector<int> moveRow = {-1, 1, 0, 0};
        vector<int> moveCol = {0, 0, -1, 1};
        pair<int,int> rockCoordinates = currentCoordinates;
        rockCoordinates.first += moveRow[direction];
        rockCoordinates.second += moveCol[direction];
        displayWithAction({1, 1, id});
        game.encounterRock(rockCoordinates);
    }
    else
    {
        game.moveZombie(id, direction);
    }

    if (game.attackAlien(id))
    {
        displayWithAction({7, id, 1});
        displayWithAction({4, id, game.zombies[id-1].getAttack()});
    }
    else
    {
        displayWithAction({7, id, 0});
    }
}

// Sets the object theme based off user selection
void Controller::changeObjectThemeMenu()
{
    bool quit = false;
    while (!quit)
    {
        int objectThemeMenuOption = Display::getObjectTheme(this->objectThemeID);
        while (objectThemeMenuOption == -1)
        {
            Display::displayErrorMessage();
            objectThemeMenuOption = Display::getObjectTheme(this->objectThemeID);
        }
        
        if (objectThemeMenuOption == 3)
        {
            quit = true;
        }
        else
        {
            this->objectThemeID = objectThemeMenuOption;
        }
    }
}

// Sets the border theme based off user selection
void Controller::changeBorderThemeMenu()
{
    bool quit = false;
    while (!quit)
    {
        int borderThemeMenuOption = Display::getBorderTheme(this->borderThemeID);
        while (borderThemeMenuOption == -1)
        {
            Display::displayErrorMessage();
            borderThemeMenuOption = Display::getBorderTheme(this->borderThemeID);
        }
        
        if (borderThemeMenuOption == 3)
        {
            quit = true;
        }
        else
        {
            this->borderThemeID = borderThemeMenuOption;
        }
    }
}

// Display Theme menu
void Controller::themeMenu()
{
    bool quit = false;
    while (!quit)
    {
        int themeMenuOption = Display::getChangeThemeMenu();
        while (themeMenuOption == -1)
        {
            Display::displayErrorMessage();
            themeMenuOption = Display::getChangeThemeMenu();
        }

        switch (themeMenuOption) 
        {
            case 1:
                this->changeObjectThemeMenu();
                break;
            case 2:
                this->changeBorderThemeMenu();
                break;
            case 3:
                quit = true;
                break;
        }
    }
}

// Displays main menu
void Controller::mainMenu()
{
    bool quit = false;
    while (!quit)
    {
        int mainMenuOption = Display::getMainMenuOption();
        while (mainMenuOption == -1)
        {
            Display::displayErrorMessage();
            mainMenuOption = Display::getMainMenuOption();
        }
        switch (mainMenuOption)
        {
            case 1:
                this->newGame();
                break;
            case 2:
                this->loadGame();
                break;
            case 3:
                this->themeMenu();
                break;
            case 4:
                Display::displayStats(Saves::getPlayerStats());
                break;
            case 5:
                quit = (Display::quitGame() == 1) ? true : false;
                break;
        }
    }
}
