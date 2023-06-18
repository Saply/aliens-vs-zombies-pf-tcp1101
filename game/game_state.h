#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
using std::vector;

struct GameState
{
	int rows, cols;
    vector<vector<char>> board;
	vector<int> alienAttributes;
	int numberOfZombies;
	vector<vector<int>> zombieAttributes;
	int numberOfPlayerTurns, healthLost, attackGained;
};

#endif