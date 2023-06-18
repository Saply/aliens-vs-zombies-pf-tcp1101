#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "../game/game_state.h"
#include "save.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::to_string;
using std::vector;

// Getting the GameState from a save file for loading a game
GameState Saves::saveFileToGameState(int saveNum)
{
    GameState state;
    string savePathFile = "save/save_files/" + to_string(saveNum) + ".txt";
    ifstream saveFile(savePathFile);

    saveFile >> state.rows >> state.cols;
    state.board.resize(state.rows, vector<char> (state.cols));
    
    for(int i = 0; i < state.rows; i++) 
    {
        for(int j = 0; j < state.cols; j++) 
        {
            saveFile >> state.board[i][j];
            // Convert '_' from save file to empty spaces on board
            if(state.board[i][j] == '_')
                state.board[i][j] = ' ';
        }
        
    }

    state.alienAttributes.resize(5);
    for(int i = 0; i < 5; i++) 
    {
        saveFile >> state.alienAttributes[i];
    }

    saveFile >> state.numberOfZombies;
    state.zombieAttributes.resize(state.numberOfZombies, vector<int> (6));
    for(int i = 0; i < state.numberOfZombies; i++) 
    {
        for(int j = 0; j < 6; j++) 
        {
            saveFile >> state.zombieAttributes[i][j];
        }
    }

    saveFile >> state.numberOfPlayerTurns >> state.healthLost >> state.attackGained;

    saveFile.close();
    return state;
}

// Saving a game to a specified save file using the GameState
void Saves::gameStateToSaveFile(int saveNum, GameState& state)
{   
    string savePathFile;
    savePathFile = "save/save_files/" + to_string(saveNum) + ".txt";
    ofstream saveFile(savePathFile);

    saveFile << state.rows << ' ' << state.cols << '\n';

    for(int i = 0; i < state.rows; i++) 
    {
        for(int j = 0; j < state.cols; j++) 
        {
            // Convert empty spaces from board to '_' on save file
            if(state.board[i][j] == ' ')
                state.board[i][j] = '_';
            saveFile << state.board[i][j];
        }
        saveFile << '\n';
    }

    for(int i = 0; i < 5; i++)
    {
        saveFile << state.alienAttributes[i] << ' ';
    }
    saveFile << '\n';

    saveFile << state.numberOfZombies << '\n';
    for(int i = 0; i < state.numberOfZombies; i++)
    {     
        for(int j = 0; j < 6; j++) 
        {
            saveFile << state.zombieAttributes[i][j] << ' ';
            
        }
        saveFile << '\n';
    }

    saveFile << state.numberOfPlayerTurns <<  ' ' << state.healthLost << ' ' << state.attackGained << '\n';

    saveFile.close();    
}       

// Returns an int vector that contains number of save file if it exists, if it doesn't then element is 0
vector<int> Saves::getSaveFilesDirectory()
{ 
    vector<int> saveFiles;
    string path;

    for(int i = 1; i <= 3; i++)
    {
        path = "save/save_files/" + to_string(i) + ".txt";
        if(std::filesystem::exists(path)) 
        {
            saveFiles.push_back(i);
        }
        else
        {
            saveFiles.push_back(0);
        }
        path.clear();
    }

    return saveFiles;
}

// Remove save file after player has finished the game
void Saves::deleteSaveFile(int saveNum)
{
    string path;
    path = "save/save_files/" + to_string(saveNum) + ".txt";
    std::filesystem::remove(path);
}

// Add game data to overall player stats once a game is finished
void Saves::addToPlayerStats(GameState& state, int borderThemeID, int objectThemeID, int zombiesDefeated, bool winOrLose)
{
    string path = "save/playerstats.txt";

    // Read and clear old data
    ifstream currentStatsFile(path);
    int gamesWon, gamesLost;
    int totalNumOfPlayerTurns, totalZombiesSpawned, totalZombiesDefeated, totalHealthLost, totalAttackGained;
    int lastBorderThemeID, lastObjectThemeID;

    currentStatsFile >> gamesWon
                     >> gamesLost 
                     >> totalNumOfPlayerTurns
                     >> totalZombiesSpawned
                     >> totalZombiesDefeated 
                     >> totalHealthLost
                     >> totalAttackGained
                     >> lastBorderThemeID 
                     >> lastObjectThemeID;
    currentStatsFile.close();
    std::filesystem::remove(path);

    // Add with new data
    ofstream newStatsFile(path);
    (winOrLose == true) ? gamesWon++ : gamesLost++;
    newStatsFile << gamesWon << ' '
                 << gamesLost << ' '
                 << totalNumOfPlayerTurns + state.numberOfPlayerTurns << ' '
                 << totalZombiesSpawned + state.numberOfZombies << ' '
                 << totalZombiesDefeated + zombiesDefeated << ' '
                 << totalHealthLost + state.healthLost << ' '
                 << totalAttackGained + state.attackGained << ' '                 
                 << borderThemeID << ' '
                 << objectThemeID;
    newStatsFile.close();
}

// Get player stats 
// {games won, games lost, total player turns, total zombies spawned, total zombies defeated, total health lost, total attack gained, last border theme, last object theme}
vector<int> Saves::getPlayerStats()
{
    string path = "save/playerstats.txt";
    vector<int> playerStats;

    ifstream statsFile(path);
    int stats;
    for(int i = 0; i < 9; i++)
    {
        statsFile >> stats;
        playerStats.push_back(stats);
    }
    
    return playerStats;
}