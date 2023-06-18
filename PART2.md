# Part 2

## Minimum Requirements

### Completed

1. Game Board
2. Alien Attributes
3. Zombie Attributes
4. Alien Movement
5. Zombie Movement
6. Alien Attack
7. Zombie Attack
8. Multiple Zombies Generation
9. Arrow, Health, Pod, Rock, Empty and Trail Game Objects
10. Game Controls / Logic
11. Saving Game File
12. Loading Game File
13. Random Game Objects Generation
14. Allow user to select number of zombies
15. Custom board size
16. Display (Main Menu & Game)


### To Do

1. *None*

## Additional Features

1. Border/Object themes
2. Multiple Save Files
3. Rotating Win/Lose message
4. Larger Board Size
5. Smooth Transition for Alien Movement
6. Column Numbering with Letters
7. Command Chaining (feature of the future)
8. Smooth Trail Reset
9. 3 Different Difficulty Settings
10. Hint
11. Player Stats
12. Alien Energy Bar

## Contributions

### [Saply](https://github.com/Saply)

1. Create Player Stats feature
2. Ensure player saves and GameState works with new energy feature 
3. Add input validations for saving/loading invalid save files 

### [wyhong3103](https://github.com/wyhong3103)

1. Create a backtracking algorithm to find the most secure move then feed it to user through hint
2. Create logic that connects the hint and command

### [EpsilonTea](https://github.com/EpsilonTea)

1. Create energy system for Alien
2. Redesign UI to be more consistent
3. Add a few extra menu/prompts/hint display etc.
4. Edit progress video

## Problems Encountered & Solutions

> Problem : We need an algorithm to find the most secure move to hint it to user.

**Solution :** We came up with a backtracking algorithm to find the move that guarantee the most net health. The idea is to recursively find every possible move (i.e, bruteforce) and mark the move that ends with the most net health as the most secure move. Well actually, this is not how we prioritize a move. The actual priority goes like, game result (win or lose), net health, total zombies slained, attack dealt, attack gained, and whether we break a rock at the end of the move. The final move is going to be the move that has the greatest priority list in lexicographical order.

<br>

> Problem : It's difficult for Alien to attack Zombies when the attack resets to 0 each turn, and too many rocks prevents Alien movement.

**Solution :** During balancing, we increased the spawn rate for arrows to compensate for the fact that its difficult to attack Zombies, and also reduced the rock spawn rate. 

<br>

> Problem : The Alien could infinitely rotate arrows without any repercussions.

**Solution :** Taking inspiration from the original Alien Path game, we implemented an Energy system to limit the amount of arrow rotations the Alien can make. This forces the player to plan ahead and think strategically, rather than resorting to bruteforcing attacks.

<br>

> Problem : I want to see my 0% win rate, and other miscellaneous info based off how much I've played the game.

**Solution :** We implemented a Player Stats feature so now you can check miscellaneous info based off how much you played the game. The stats are tracked at the end of each game. The player stats include the number of games won, games lost, total player turns, total zombies spawned, total zombies defeated, total health lost and the total attack gained. 

<br>

> Problem : The portal game object felt like an unnecessary feature.

**Solution :** We've decided to divert our focus on making things that are helpful to the user (eg. game hint), rather than adding game object bloat that may complicate the game even further. 