#ifndef SAVE_H
#define SAVE_H

#include "../game/game_state.h"

namespace Saves 
{
    GameState saveFileToGameState(int saveNum);
    void gameStateToSaveFile(int saveNum, GameState& state);
    void deleteSaveFile(int saveNum);
    void addToPlayerStats(GameState& state, int borderThemeID, int objectThemeID, int zombiesDefeated, bool winOrLose);
    vector<int> getSaveFilesDirectory();
    vector<int> getPlayerStats();
};

#endif