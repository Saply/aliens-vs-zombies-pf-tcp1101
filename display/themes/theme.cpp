#include <string>
#include <vector>
#include "theme.h"

using std::vector;
using std::string;

// Object themes
// alien, health, rock, pod, trail, arrows (up, down, left, right), space;	
vector<vector<char>> Theme::objectThemes = {{'A', 'h', 'r', 'p', '.', '^', 'v', '<', '>', ' '},
                                        {'A', '+', 'r', 'p', '*', '^', 'v', '<', '>', ' '},
                                        {'@', '+', 'r', 'p', '*', '^', 'v', '<', '>', ' '}};


// Border themes
// verticalBorder, horizontalBorder1, horizontalBorder2
vector<vector<char>> Theme::borderThemes = {{'|', '+', '-'},
                                        {'|', '~', '-'},
                                        {'|', '=', '='}};