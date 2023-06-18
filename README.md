# Alien vs. Zombies

Alien vs Zombies is a terminal-based video game where the user plays as the Alien to fight a horde of Zombie(s). This is a group assignment made together with 2 other groupmates (credited below)
<p align = "center">
    <img src = "https://user-images.githubusercontent.com/88439700/217570758-b4070e72-b73f-4c7c-b27c-47593c6120df.gif">
</p>


## Table Of Contents
- [Video Demo](#video-demo)
- [Compilation Instructions](#compilation-instructions)
    - [For Windows users:](#for-windows-users)
    - [For Linux users:](#for-linux-users)
- [User Manual](#user-manual)
- [Game Objects](#game-objects)
    - [Arrow (^, v, \<, \>)](#arrow--v--)
    - [Health (h)](#health-h)
    - [Pod (p)](#pod-p)
    - [Rock (r)](#rock-r)
    - [How To Win/Lose The Game](#how-to-winlose-the-game)
- [Characters](#characters)
    - [Alien (A)](#alien-a)
    - [Zombie (numbered from 1-9)](#zombie-numbered-from-1-9)
    - [The player](#the-player)
- [Progress Log](#progress-log)
- [Contributors](#contributors)
- [File Structure](#file-structure)


## Video Demo
[Click Me](https://www.youtube.com/watch?v=ncITUPT7XI0).

## Compilation Instructions

1. Run the following command in a terminal.

```
g++ main.cpp controller.cpp save/save.cpp game/*.cpp game/characters/*.cpp display/display.cpp display/themes/theme.cpp -o alien_vs_zombies -std=c++17
```

2. Run the generated executable file in the terminal.
#### For Windows users:
```
.\alien_vs_zombies
```
#### For Linux users:
```
chmod +x alien_vs_zombies
./alien_vs_zombies
```


## User Manual

Before the game starts, the user can set the game board dimensions, number of zombies, and difficulty. If the user chooses to save from a previous Save File, then it will load the game from where the user last left off.

Other than the Alien and Zombies, the game has several other game objects as well that can be interacted with by the alien and zombies. 

### Game Objects

#### Arrow (^, v, <, >)
When the Alien passes through an **arrow**, it will change directions according to the direction of the arrow and increase its attack by 20. 

When the Alien hits a Zombie, it will deal damage to it based off its current attack stat. If the attack does not defeat the zombie, it will stop the alien and end the alien's turn, otherwise, the alien will keep moving until it is stopped.

#### Health (h)
When the alien picks up a health pack, it heals itself for 20 health. The alien has a max health of 200, meaning that any health pack picked up will not increase the alien's health past 200.

#### Pod (p)
When the alien picks up a pod, it deals 10 damage to the closest zombie. If there are multiple closest zombies, it will attack 1 of them at random.

#### Rock (r)
When the alien/zombie moves and hits a rock, it stop the alien/zombie from moving, ending the alien/zombie's turn. This will also break the rock with a chance of a random game object spawning under it.

#### How To Win/Lose The Game
If the Alien defeats all the Zombies, then the Alien wins! If the Alien's health drops to 0, then the Zombies win. Do NOT let the Zombies win. Otherwise, the player will be subject to looking at an annoying losing screen for quite a while.

### Characters

#### Alien (A)
During the game, the user can control the Alien to do a number of actions.
1. Movement (up, down, left, right)
2. Rotation of arrow game object
3. Attack Zombie if it encounters them during movement
4. Get a hint for move that allows you to survive the longest, in case you're in a pinch

#### Zombie (numbered from 1-9)
1. Move in a random direction
2. Hurt the Alien if its within range
3. Destroy game objects and move to that tile, with the exceptions of Alien, another Zombie, and Rock

#### The player
1. Start the game
2. Save the current game to a Save File
3. Load the current game from a pre-existing Save File
4. View their overall stats from all games played
5. Quit the game

### Energy
The Alien has a max energy of 5, and will regain 2 energy at the beginning of each turn. At the beginning of the game, the alien will spawn with maximum energy. Everytime an **arrow** is rotated, 1 energy is consumed. Once the alien has no energy left, it cannot rotate any arrows.

## Progress Log

- [Part 1](PART1.md)
- [Part 2](PART2.md)

## Contributors

- [wyhong3103](https://github.com/wyhong3103)
- [EpsilonTea](https://github.com/EpsilonTea)
- [Saply](https://github.com/Saply)





## File Structure

```
/
├─ display/
  ├─ themes/
     ├─ theme.cpp 
     ├─ theme.h
   ├─ display.cpp
   ├─ display.h
├─ save/
   ├─ save_files/
     ├─ 1.txt
     ├─ 2.txt
     ├─ 3.txt
   ├─ save.cpp
   ├─ save.h
├─ game/
   ├─ characters/
     ├─ alien.cpp 
     ├─ alien.h
     ├─ zombie.cpp 
     ├─ zombie.h
  ├─ game.cpp
  ├─ game.h
  ├─ game_state.h
├─ controller.cpp
├─ controller.h
├─ main.cpp
├─ .gitignore
├─ PART1.md
├─ PART2.md
├─ README.md
```
