# Part 1

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

1. Portal Game Object (Additional Feature)
2. Player Stats Tracker (Additional Feature)
3. Alien Energy Bar (Additional Feature)

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

## Contributions

### [Saply](https://github.com/Saply)

1. Create most of the saving and loading logic
2. Determine parsing of txt files to save/load the game saves
3. Create logic for getting saves directory and deleting save files
4. Create randomization of Zombie attributes when starting a new game
5. Obtain Zombie coordinates from randomized board generation when creating a new game
6. Create GameState struct used in storing attributes for various parts of the game
7. Store current game data inside GameState struct
8. Suggest overall file/folder/class structure

### [wyhong3103](https://github.com/wyhong3103)

1. Create random board generation algorithm & turn taking algorithm.
2. Suggest pythagorean theorem without square rooting
3. Suggest MVC Software Architecture
4. Create Alien and Zombie class.
5. Create game loop that controls the game flow.
6. Create functions that trigger the game objects.
7. Create logic for Alien's & Zombie's movement.
8. Create most of the Game's logic.
9. Create most of the Controller's logic (Control between the Game and Display).

### [EpsilonTea](https://github.com/EpsilonTea)

1. Create multiple themes for user to customize board
2. Input validation from user
3. Create helper move functions for alien and zombie
4. Create display board functions
5. Create winning and losing game over screens
6. Create chaining command feature
7. Create all menus and display help
8. Create most of the display logic
9. Edit progress video
10. Suggest pythagorean theorem to calculate distance

## Problems Encountered & Solutions

> Problem : We want to make our code as organized as possible, but it's hard.

**Solution :** We therefore took inspirations from the MVC Software Architecture to ease our development.

<br>

> Problem : The member variable gameOver from the Game class would initialize with garbage values, resulting in the game being over before even starting. 

**Solution :** We fixed this by setting the variable to False instead of the compiler setting the value for us.

<br>

> Problem : Naive board random generation is not so efficient. As the random generated coordinates could already been occupied by other game objects, OR, some objects might not be guaranteed to be generated.

**Solution :** We fixed this by having the below board generation algorithm. First, we generate the total number for each game objects, while the number of alien and zombies are fixed, the number of other game objects are based on percentage, i.e there will be x percents of the board be game object y, for each game object. Then, we generate an array of coordinates and then shuffle it. Finally, we use 2 pointers to assign all the game objects to each of the coordinates in the shuffled array. By doing this, we ensure we don't use coordinates that has already been occupied and a guaranteed percentage of the board to be game objects y.

<br>

> Problem : Naive turn taking logic is not so efficient, as we need to check whether the current character is alive to determine whose turn it is. There are unnecessary computations to this.

**Solution :** We optimized turn taking logic by using a queue. After each turn, we pop the character in the front of the queue and push it to the back of the queue. If current character is not alive, we just pop it off and never push it back to the queue again. This implied we will only have to do at most one time of checking whether a dead zombie is dead or alive.

<br>

> Problem : Naive pythagorean theorem formula to determine distance between two objects in a coordinate plane requires dealing with floating point value, because of the square root, which will cause an inaccurate precision among the values.

**Solution :** We realized that the square root doesn't actually matter, since we do not need the actual distance but the minimal distance among all. Also since the function sqrt(x) is monotonic, i.e the bigger the x, the bigger the sqrt(x), so we can omit the square root and use the value inside to determine the minimal distance.

<br>

> Problem : Saving empty board spaces inside the game and converting them to a .txt save file cannot be done feasibly due to the difficulty of parsing blank characters.

**Solution :** The save files don't actually store blank characters inside them. Instead, we converted empty spaces inside the game board into an underscore character (_), which represents empty spaces inside the game board. Once the game loads from these save files, we convert these underscore characters back into empty spaces in the board.
