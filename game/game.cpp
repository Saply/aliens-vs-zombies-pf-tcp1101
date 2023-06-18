#include "game.h"
#include <utility>
#include <algorithm>
#include <random>
#include <ctime>
#include <climits>
#include <string>

// Clear the old data if exists
void Game::cleanOldData()
{
    this->board.clear();
    this->zombies.clear();
    while (!this->turnQueue.empty()) this->turnQueue.pop();
}

// Get the ID of the zombie at specified coordinates
int Game::getZombieIDAt(pair<int,int> coordinates)
{
    if (this->checkCoordinate(coordinates, 4) != 9) return -1;
    return this->board[coordinates.first][coordinates.second]-'0';
}

// init rows, columns (new game), number of zombies
// player will set board game dimensions and number of zombies
void Game::newGame(int rows, int cols, int numofZombies, int difficulty)
{
    this->cleanOldData();

    // Get rows and cols set by player
    this->rows = rows;
    this->cols = cols;
    this->gameOver = false;

    // Set alien to middle of board
    std::pair<int, int> alienCoordinates;
    alienCoordinates.first = this->rows/2;
    alienCoordinates.second = this->cols/2;
    this->alien.init(alienCoordinates, 100, 5);

    this->numOfZombies = numofZombies;
    this->healthLost = 0;
    this->attackGained = 0;
    this->numOfZombiesAlive = numOfZombies;

    // Generate new board
    generateNewBoard();

    
    // Switch case sets boundaries depending on difficulty picked
    int healthLowerBound, healthUpperBound, attackLowerBound, attackUpperBound, rangeLowerBound, rangeUpperBound;
    switch(difficulty)
    {   
        // easy
        case 1:
            healthLowerBound = 40;
            healthUpperBound = 60;
            attackLowerBound = 10;
            attackUpperBound = 20; 
            rangeLowerBound = 1;
            rangeUpperBound = 2;
            break;

        // medium
        case 2:
            healthLowerBound = 60;
            healthUpperBound = 80;
            attackLowerBound = 15;
            attackUpperBound = 25; 
            rangeLowerBound = 2;
            rangeUpperBound = 3;
            break;
            
        // hard
        case 3:
            healthLowerBound = 80;
            healthUpperBound = 100;
            attackLowerBound = 25;
            attackUpperBound = 35; 
            rangeLowerBound = 3;
            rangeUpperBound = 5;
            break;
    }

    // Seed randomness for these two as they can't be used interchangeably
    srand(time(NULL));
    auto rng = std::default_random_engine(time(NULL));;


    // Loop to determine random stats for each zombie and create Zombie objects
    std::pair<int, int> newZombieCoordinates;
    int health;
    for(int i = 1; i <= numofZombies; i++)
    {
        // Define stats to randomize
        std::uniform_int_distribution<> attack(attackLowerBound, attackUpperBound);
        std::uniform_int_distribution<> range(rangeLowerBound, rangeUpperBound);
        
        // Randomly assign zombies with one of two options for health
        if(rand() % 2)
            health = healthLowerBound;
        else
            health = healthUpperBound;
        
        // Get coordinates for each zombie ID in generated board
        newZombieCoordinates = getZombieCoordinatesFromBoard(i);
        
        // Create Zombie objects
        Zombie zombie;
        zombie.init(health, attack(rng), range(rng), newZombieCoordinates);

        // Assign an ID for each zombie, starting from 1
        zombie.setID(i);
        this->zombies.push_back(zombie);
    }

    // Initialize turns
    for(int i = 0; i <= numOfZombies; i++)
    {
        this->turnQueue.push(i);
    }
}

// Load game from GameState
void Game::loadGame(GameState& state)
{
    this->cleanOldData();

    // Load the board and other data
    this->rows = state.rows;
    this->cols = state.cols;

    this->healthLost = state.healthLost;
    this->attackGained = state.attackGained;
    this->gameOver = false;

    this->board.resize(this->rows, vector<char> (this->cols));
    for(int i = 0; i < this->rows; i++) 
    {
        for(int j = 0; j < this->cols; j++)
        {
            this->board[i][j] = state.board[i][j];
        }
    }

    // Initialize alien  {row, col, hp, atk, energy}
    std::pair<int, int> alienCoordinates;
    alienCoordinates.first = state.alienAttributes[0];
    alienCoordinates.second = state.alienAttributes[1];
    this->alien.init(alienCoordinates, state.alienAttributes[2], state.alienAttributes[4]);

    // {{id, row, col, health, attack, range}}
    this->numOfZombies = state.numberOfZombies;
    // Initialize number of zombies alive as 0
    this->numOfZombiesAlive = 0;
    this->zombies.resize(this->numOfZombies);
    // Initialize zombies with constructor, ID, then add it to vector<Zombies>
    std::pair<int, int> currentZombieCoordinates;
    for(int i = 0; i < state.numberOfZombies; i++) 
    {
        currentZombieCoordinates.first = state.zombieAttributes[i][1];
        currentZombieCoordinates.second = state.zombieAttributes[i][2];
        Zombie zombie;
        
        zombie.init(
            state.zombieAttributes[i][3], 
            state.zombieAttributes[i][4], 
            state.zombieAttributes[i][5], 
            currentZombieCoordinates
        );

        zombie.setID(state.zombieAttributes[i][0]);
        this->zombies[i] = zombie;
        this->numOfZombiesAlive += this->zombies[i].isAlive();
    }

    // Initialize the current turn Queue by ID
    // 0 for Alien, the rest for zombies
    this->turnQueue.push(0);
    for(int i = 0; i < this->numOfZombies; i++)
    {   
        if (this->zombies[i].isAlive())
            this->turnQueue.push(zombies[i].getID());
    }
}

vector<int> Game::getAlienStats()
{
    // vector {row, column, health, attack, energy}
    vector<int> alienStats;
    alienStats.resize(5);
    alienStats[0] = this->alien.getCoordinates().first;
    alienStats[1] = this->alien.getCoordinates().second;
    alienStats[2] = this->alien.getHealth();
    alienStats[3] = this->alien.getAttack();
    alienStats[4] = this->alien.getEnergy();

    return alienStats;
}

vector<vector<int>> Game::getZombieStats()
{
    vector<vector<int>> zombieStats;
    zombieStats.resize(this->numOfZombies, vector<int> (6));
    // vector<Zombie> zombies;
    // {{ID, row, col, health, attack, range}}
    for(int i = 0; i < this->numOfZombies; i++) 
    {
        zombieStats[i][0] = this->zombies[i].getID();
        zombieStats[i][1] = this->zombies[i].getCoordinates().first;
        zombieStats[i][2] = this->zombies[i].getCoordinates().second;
        zombieStats[i][3] = this->zombies[i].getHealth();
        zombieStats[i][4] = this->zombies[i].getAttack();
        zombieStats[i][5] = this->zombies[i].getRange();
    }
    
    return zombieStats;
}

GameState Game::getGameState() 
{
    GameState state;

    state.rows = this->rows;
    state.cols = this->cols;
    state.board.resize(this->rows, vector<char> (this->cols));
    state.board = this->board;

    state.alienAttributes.resize(5);
    state.alienAttributes = this->getAlienStats();

    state.numberOfZombies = this->numOfZombies;
    state.zombieAttributes.resize(this->numOfZombies, vector<int> (6));
    state.zombieAttributes = this->getZombieStats();

    // To be used by Controller for tracking player stats
    state.numberOfPlayerTurns = 0; // Set/Tracked by Controller
    state.healthLost = this->healthLost;
    state.attackGained = this->attackGained;

    return state;
}

// Generates new board with randomized game object and zombie placements 
void Game::generateNewBoard()
{
    // Total number of empty cells (w/o Alien and Zombies)
    int totalEmptyCell = (this->rows * this->cols) - 1 - this->numOfZombies; 
    /*
    alien        - A
    zombie       - 1, 2, ..., 9

    rock (10%)   - r
    pod (10%)    - p
    health (5%)  - h
    arrows (20%) - ^v><
    the rest is empty space 
    */

    // {numberOFZombies, numberOfRocks, numberOfPods, numberOfHealth, numberOfArrows}
    vector<pair<char,int>> totalEntities = {
        {'Z', this->numOfZombies},
        {'r', (totalEmptyCell*10) / 100},
        {'p', (totalEmptyCell*10) / 100},
        {'h', (totalEmptyCell*5) / 100},
        {'*',(totalEmptyCell*20) / 100}
    };

    // Store all the coordinates
    vector<pair<int,int>> coordinates;

    for(int row = 0; row < this->rows; row++) 
    {
        for(int col = 0; col < this->cols; col++)
        {
            // make sure it's not the reserved cell for alien
            if (row != this->rows/2 || col != this->cols/2)
            {
                coordinates.push_back({row, col});
            }
        }
    }

    // Size the board and initialize every cell as ' '
    this->board.resize(this->rows, vector<char> (this->cols, ' '));

    // Set Alien at the center of the board
    board[this->rows/2][this->cols/2] = 'A';

    // Shuffle the coordinates
    auto rng = std::default_random_engine(time(NULL));
    std::shuffle(coordinates.begin(), coordinates.end(), rng);

    
    // Two pointer to assign every objects to shuffled coordinates
    int pointer1 = 0, pointer2 = 0;
    while (pointer1 < totalEntities.size() && pointer2 < coordinates.size())
    {
        char& selectedCoordinate = this->board[coordinates[pointer2].first][coordinates[pointer2].second]; 
        if (totalEntities[pointer1].second)
        {
            selectedCoordinate = totalEntities[pointer1].first; 

            // If it is an arrow cell we random generate a direction
            if (selectedCoordinate == 'Z')
            {
                selectedCoordinate = totalEntities[pointer1].second + '0';
            }
            if (selectedCoordinate == '*')
            {
                vector<char> arrows = {'^', 'v', '>', '<'};
                selectedCoordinate = arrows[rand() % 4];
            }

            totalEntities[pointer1].second--;
            pointer2++;
        }
        else
        {
            pointer1++;
        }
    }
}

// Get Zombie coordinates based off its ID from randomly generated board
std::pair<int, int> Game::getZombieCoordinatesFromBoard(int zombieID)
{
    // Convert zombie ID (eg. 1) to char ('1')
    char zombieInBoard = zombieID + '0';

    // Iterate through board to find coordinates of each zombie on the randomly generated board
    std::pair<int, int> zombieCoordinates;
    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            // If the zombie corresponding to the ID is found on the board
            if (this->board[i][j] == zombieInBoard)
            {
                zombieCoordinates.first = i;
                zombieCoordinates.second = j;
                return zombieCoordinates;
            }
        }
    }

    // Default
    return zombieCoordinates;
}


int Game::getCurrentTurn()
{
    while (this->turnQueue.front() != 0 && !this->zombies[this->turnQueue.front()-1].isAlive()){
        this->turnQueue.pop();
    }
    return this->turnQueue.front();
}


void Game::nextTurn()
{
    int currentID = this->turnQueue.front();
    this->turnQueue.pop();
    if (currentID == 0)
    {
        this->turnQueue.push(currentID);
    }
    else
    {
        if (this->zombies[currentID-1].isAlive()) this->turnQueue.push(currentID);
    }
}


void Game::moveZombie(int id, int direction) 
{
    // (row, column)
    pair<int, int> zombieCoordinates = this->zombies[id-1].getCoordinates();
    int row = zombieCoordinates.first;
    int column = zombieCoordinates.second;

    // change zombie to space on the board
    this->board[row][column] = ' ';

    // direction: 0 up, 1 down, 2 left, 3 right
    vector<int> moveColumn = {0, 0, -1, 1};
    vector<int> moveRow = {-1, 1, 0, 0};
    this->zombies[id-1].move(moveRow[direction], moveColumn[direction]);

    // put zombie on the new coordinate
    this->board[row + moveRow[direction]][column + moveColumn[direction]] = id + '0';
}


void Game::moveAlien(int direction) 
{
    // (row, column)
    pair<int, int> alienCoordinates = alien.getCoordinates();
    

    int row = alienCoordinates.first;
    int column = alienCoordinates.second;

    // direction: 0 up, 1 down, 2 left, 3 right
    vector<int> moveColumn = {0, 0, -1, 1};
    vector<int> moveRow = {-1, 1, 0, 0};

    alien.move(moveRow[direction], moveColumn[direction]);

    // change alien to trail on the board
    board[row][column] = '.';

    // put alien on the new coordinate
    board[row + moveRow[direction]][column + moveColumn[direction]] = 'A';
}

/*
    Return what is on the coordinate

    invalid = -1
    arrows = 0-3
    health = 4
    pod = 5
    rock = 6
    empty space = 7
    alien = 8
    zombie = 9
*/
int Game::checkCoordinate(pair<int,int>& coord, int direction)
{
    // U D L R S(Stay)
    vector<int> moveRow = {-1, 1, 0, 0, 0};
    vector<int> moveColumn = {0, 0, -1, 1, 0};
    int row = coord.first + moveRow[direction];
    int column = coord.second + moveColumn[direction];

    int numberOfRows = this->rows;
    int numberOfColumns = this->cols;


    if (row < 0 || row >= numberOfRows || column < 0 || column >= numberOfColumns)
    {
        return -1;
    }

    switch(this->board[row][column])
    {
        case '^':
            return 0;
        case 'v':
            return 1;
        case '<':
            return 2;
        case '>':
            return 3;
        case 'h':
            return 4;
        case 'p':
            return 5;
        case 'r':
            return 6;
        case ' ':
            return 7;
        case 'A':
            return 8;
        case '.':
            return 10;
        default:
            // can only be zombie
            return 9;
    }
}

void Game::changeArrowDirection(pair<int,int> arrowCoordinates, int newDirection)
{
    vector<char> arrows = {'^', 'v', '<', '>'};
    this->board[arrowCoordinates.first][arrowCoordinates.second] = arrows[newDirection];
}


// return if the zombie is alive
bool Game::attackZombie(pair<int,int> zombieCoordinates)
{
    int zombieID = this->board[zombieCoordinates.first][zombieCoordinates.second]-'0';

    this->zombies[zombieID-1].receiveAttack(this->alien.getAttack());

    if (!this->zombies[zombieID-1].isAlive())
    {
        this->numOfZombiesAlive--;
        gameOver = !numOfZombiesAlive;
        return true;
    }
    else
    {
        return false;
    }
}

// return true if zombie attacked alien
bool Game::attackAlien(int zombieID)
{
    pair<int,int> alienCoordinates = this->alien.getCoordinates();
    if (this->zombies[zombieID-1].isInRange(alienCoordinates))
    {
        this->alien.receiveAttack(this->zombies[zombieID-1].getAttack());
        this->healthLost += this->zombies[zombieID-1].getAttack();
        this->gameOver = !this->alien.isAlive();
        return true;
    }
    return false;
}

// Randomize game objects that spawn from Alien trail
void Game::clearTrail(pair<int,int> coordinates)
{
    vector<char> arrows = {'^', 'v', '<', '>'};
    int rng = rand() % 100;
    char& cell = this->board[coordinates.first][coordinates.second];
    if (rng < 55)
    {
       cell = ' '; 
    }
    else if (rng < 75)
    {
        cell = 'a';
    }
    else if (rng < 85)
    {
        cell = 'p';
    }
    else if (rng < 90)
    {
        cell = 'h';
    }
    else
    {
        cell = 'r';
    }

    if (cell == 'a')
    {
        cell = arrows[rand() % 4];
    }
}

void Game::encounterRock(pair<int,int> rockCoordinates)
{
    vector<char> arrows = {'^', 'v', '<', '>'};
    int rng = rand() % 90;
    char& cell = this->board[rockCoordinates.first][rockCoordinates.second];

    if (rng < 55)
    {
       cell = ' '; 
    }
    else if (rng < 65)
    {
        cell = 'p';
    }
    else if (rng < 70)
    {
        cell = 'h';
    }
    else
    {
        cell = 'a';
    }

    if (cell == 'a')
    {
        cell = arrows[rand() % 4];
    }
}

void Game::encounterHealth()
{
    this->alien.incHealth(20);
}

void Game::encounterArrow()
{
    this->alien.incAttack(20);
    this->attackGained += alien.getAttack();
}

// Get vector of closest zombies to be used by Pod
vector<int> Game::getClosestZombies()
{
    pair<int,int> alienCoordinates = this->alien.getCoordinates();
    int minimalDistance = INT_MAX;
    vector<int> closestZombies;

    for(int i = 0; i < this->numOfZombies; i++)
    {
        if (!this->zombies[i].isAlive()) continue;

        pair<int,int> zombieCoordinates = this->zombies[i].getCoordinates();
        
        // Pythagorean theorem for distance
        // Without square rooting, to avoid dealing with floating point value
        int distanceBetween = 
            (alienCoordinates.first-zombieCoordinates.first) * (alienCoordinates.first-zombieCoordinates.first)
            +
            (alienCoordinates.second-zombieCoordinates.second) * (alienCoordinates.second-zombieCoordinates.second)
        ;

        if (distanceBetween < minimalDistance)
        {
            minimalDistance = distanceBetween;
            closestZombies.clear();           
        }

        if (distanceBetween <= minimalDistance)
        {
            closestZombies.push_back(i);
        }
    }

    return closestZombies;
}

// Checks for closest zombie based off conditions, used by hint
vector<int> Game::getClosestZombiesFromState(pair<int,int>& coord, vector<int>& zombiesHealth)
{
    int minimalDistance = INT_MAX;
    vector<int> closestZombies;

    for(int i = 0; i < this->numOfZombies; i++)
    {
        if (zombiesHealth[i] <= 0) continue;

        pair<int,int> zombieCoordinates = this->zombies[i].getCoordinates();
        
        // Pythagorean theorem for distance
        // Without square rooting, to avoid dealing with floating point value
        int distanceBetween = 
            (coord.first-zombieCoordinates.first) * (coord.first-zombieCoordinates.first)
            +
            (coord.second-zombieCoordinates.second) * (coord.second-zombieCoordinates.second)
        ;

        if (distanceBetween < minimalDistance)
        {
            minimalDistance = distanceBetween;
            closestZombies.clear();           
        }

        if (distanceBetween <= minimalDistance)
        {
            closestZombies.push_back(i);
        }
    }

    return closestZombies;
}

int Game::encounterPod()
{
    vector<int> closestZombies = this->getClosestZombies();
    int randomZombieSelected = closestZombies[rand() % closestZombies.size()];
    this->zombies[randomZombieSelected].receiveAttack(10);

    if (!this->zombies[randomZombieSelected].isAlive())
    {
        pair<int,int> zombieCoordinates = this->zombies[randomZombieSelected].getCoordinates();
        this->board[zombieCoordinates.first][zombieCoordinates.second] = ' ';
        this->numOfZombiesAlive--;
        this->gameOver = !this->numOfZombiesAlive;
    }

    return randomZombieSelected+1;
}



// moveDetails = {win/lose, netHealth, zombieSlained, attackDealt, attackGain, isARock}
// Every ID of zombies is 0-indexed in this function
void Game::searchGoodMove(pair<int,int> coord, vector<int>& moveDetails, vector<int>& zombiesHealth, int energyLeft, int initDirection, int curDirection, vector<vector<int>>& arrowsData, vector<int>& goodMoveDetails, int& goodDirection, vector<vector<int>>& goodArrowsData)
{
    // U, D, L, R
    vector<int> rowMove = {-1, 1, 0, 0};
    vector<int> colMove = {0, 0, -1, 1};

    // First Move, Go at every direction
    if (initDirection == -1)
    {
        for(int i = 0; i < 4; i++)
        {
            if (this->checkCoordinate(coord, i) == -1) continue;
            searchGoodMove(coord, moveDetails, zombiesHealth, energyLeft, i, i,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);
        }
        return;
    }

    int cellID = this->checkCoordinate(coord, curDirection);

    if (cellID == -1 || cellID == 6)
    {
        int healthLost = 0;
        for(int i = 0; i < this->numOfZombies; i++)
        {
            // pair<int,int> zombieCoordinate = this->zombies[i].getCoordinates();
            // If zombie is alive
            if (zombiesHealth[i] > 0)
            {
                healthLost += this->zombies[i].isInRange(coord) * this->zombies[i].getAttack();
            }
        }
        // Decrease net health
        moveDetails[1] -= healthLost;

        // Search if there is any zombie that is alive
        bool foundAlive = 0;
        for(int i = 0; i < this->numOfZombies; i++)
        {
            if (zombiesHealth[i] > 0) foundAlive = 1;
        }

        // If no zombie is alive, we won
        if (!foundAlive)
        {
            moveDetails[0] = 1;
        }

        moveDetails[5] = cellID == 6;

        if (moveDetails >= goodMoveDetails)
        {
            goodMoveDetails = moveDetails;
            goodDirection = initDirection;
            goodArrowsData = arrowsData; 
        }

        // Revert the change
        if (!foundAlive)
        {
            moveDetails[0] = 0;
        }

        moveDetails[5] = 0;

        moveDetails[1] += healthLost;
    }
    else if (cellID == 9)
    {
        int zombieID = this->board[coord.first+rowMove[curDirection]][coord.second+colMove[curDirection]]-'0'-1;
        if (zombiesHealth[zombieID] > 0)
        {
            int zombieHealth = zombiesHealth[zombieID];
            // If Alien can kill the zombie
            if (this->alien.getAttack() + moveDetails[3] > zombiesHealth[zombieID])
            {
                // Increase attack dealt
                // Set zombie health to 0
                // Increase number of zombies slained
                moveDetails[3] +=  zombieHealth;
                zombiesHealth[zombieID] = 0;
                moveDetails[2]++;
                this->board[coord.first+rowMove[curDirection]][coord.second+colMove[curDirection]] = ' ';

                searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, curDirection,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);

                this->board[coord.first+rowMove[curDirection]][coord.second+colMove[curDirection]] = zombieID + 1 + '0';
                // Revert the change
                moveDetails[3] -=  zombieHealth;
                zombiesHealth[zombieID] = zombieHealth;
                moveDetails[2]--;
            }
            else
            {
                // Increase attack dealth
                // Decrease Zombie Health
                moveDetails[3] +=  this->alien.getAttack() + moveDetails[4];
                zombiesHealth[zombieID] -= this->alien.getAttack() + moveDetails[4];

                int healthLost = 0;
                for(int i = 0; i < this->numOfZombies; i++)
                {
                    // pair<int,int> zombieCoordinate = this->zombies[i].getCoordinates();
                    // If zombie is alive
                    if (zombiesHealth[i] > 0)
                    {
                        healthLost += this->zombies[i].isInRange(coord) * this->zombies[i].getAttack();
                    }
                }
                // Decrease net health
                moveDetails[1] -= healthLost;

                if (moveDetails >= goodMoveDetails)
                {
                    goodMoveDetails = moveDetails;
                    goodDirection = initDirection;
                    goodArrowsData = arrowsData; 
                }

                //Rever the change
                moveDetails[3] -=  this->alien.getAttack() + moveDetails[4];
                moveDetails[1] += healthLost;
                zombiesHealth[zombieID] += this->alien.getAttack() + moveDetails[4];
            }
        }
        else
        {
            searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, curDirection,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);
        }
    }
    else if (cellID >= 0 && cellID <= 3)
    {
        vector<char> arrows = {'^', 'v', '<', '>'};
        // Gain 20 attack
        moveDetails[4] += 20;
        if (energyLeft)
        {
            for(int i = 0; i < 4; i++)
            {
                // Original arrow doesn't need energy to rotate
                if (cellID == i) continue;

                // Add to arrow change 
                arrowsData.push_back({coord.first+rowMove[curDirection], coord.second+colMove[curDirection], i});
                this->board[coord.first+rowMove[curDirection]][coord.second+colMove[curDirection]] = ' ';

                searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft-1, initDirection, i,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);
                // Revert the change
                arrowsData.pop_back();
                this->board[coord.first+rowMove[curDirection]][coord.second+colMove[curDirection]] = arrows[cellID];
            }
        }
        this->board[coord.first+rowMove[curDirection]][coord.second+colMove[curDirection]] = ' ';

        searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, cellID,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);

        this->board[coord.first+rowMove[curDirection]][coord.second+colMove[curDirection]] = arrows[cellID];
        //Revert the change
        moveDetails[4] -= 20;
    }
    else if (cellID == 4)
    {
        int healthGain = std::min(200-(this->alien.getHealth()),20);
        moveDetails[1] += healthGain;
        searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, curDirection,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);
        moveDetails[1] -= healthGain;
    }
    else if (cellID == 5)
    {

        vector<int> closestZombies = this->getClosestZombiesFromState(coord, zombiesHealth);
        
        if (closestZombies.size() == 0)
        {
            searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, curDirection,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);
        }else
        {
            int randomZombieSelected = closestZombies[rand() % closestZombies.size()];
            pair<int,int> zombieCoord = this->getZombieCoordinatesFromBoard(randomZombieSelected+1);
            int zombieHealth = zombiesHealth[randomZombieSelected];

            if (zombieHealth <= 10)
            {
                // Increase attack dealt
                // Set zombie health to 0
                // Replace the cell with a space

                moveDetails[3] += zombieHealth;
                zombiesHealth[randomZombieSelected] = 0;
                this->board[zombieCoord.first][zombieCoord.second] = ' ';

                searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, curDirection,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);

                // Revert the change
                this->board[zombieCoord.first][zombieCoord.second] = randomZombieSelected+1+'0';
                zombiesHealth[randomZombieSelected] = zombieHealth;
                moveDetails[3] -= zombieHealth;

            }
            else
            {
                // Increase attack dealt
                // Decreae zombie health
                moveDetails[3] += 10;
                zombiesHealth[randomZombieSelected] -= 10;

                searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, curDirection,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);

                // Revert the change
                zombiesHealth[randomZombieSelected] += 10;
                moveDetails[3] -= 10;
            }
        }
    }
    else{
        // If empty space, just keep moving
        searchGoodMove({coord.first+rowMove[curDirection], coord.second+colMove[curDirection]}, moveDetails, zombiesHealth, energyLeft, initDirection, curDirection,arrowsData, goodMoveDetails, goodDirection, goodArrowsData);
    }
}

/* 
Return : 
{
    direction to go: 0, 1, 2, 3 (U, D, L, R),
    vector of arrows to change : {arrowRow, arrowCol, direction to change : 0, 1, 2, 3 (U, D, L, R) }
}
*/
pair<int,vector<vector<int>>> Game::getHint()
{
    vector<int> moveDetails(6);
    vector<int> zombiesHealth;
    for(int i = 0; i < this->numOfZombies; i++)
    {
        zombiesHealth.push_back(this->zombies[i].getHealth());
    }
    vector<vector<int>> arrowsData;

    vector<int> goodMoveDetails(6, -INT_MAX);
    vector<vector<int>> goodArrowsData;
    int goodDirection = -1;

    searchGoodMove(this->alien.getCoordinates(), moveDetails, zombiesHealth, this->alien.getEnergy(), -1, -1, arrowsData, goodMoveDetails, goodDirection, goodArrowsData);

    return {goodDirection, goodArrowsData};
}

// Getter
int Game::getNumOfZombiesAlive()
{
    return this->numOfZombiesAlive;
}