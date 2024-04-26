Maze Runner Game
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
Welcome to Maze Runner, a console-based maze traversal game implemented in C++. This game offers an exciting challenge where players navigate through randomly generated mazes of varying sizes, aiming to reach the exit within a time limit.


How to Run the Game
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
1. Clone this repository to your local machine.
2. Navigate to the directory containing the maze_runner.cpp file.
   
3. Compile the code using a C++ compiler, such as G++: (if you are in the same directory as maze_runner.cpp)
-----------------------------------
g++ maze_runner.cpp -o maze_runner
-----------------------------------

4. Run the compiled executable:
--------------
./maze_runner
--------------

5. Choose a level by entering the corresponding number (1, 2, 3, or 4) or 'Q' to quit.
6. Use the W, A, S, and D keys to navigate the player through the maze.
   
Features
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
>> Dynamic Maze Generation: The game generates mazes of varying sizes based on the chosen level.
>> Time Limit: Each level has a time limit within which the player must reach the exit.
>> Obstacles: A percentage of maze cells are randomly designated as obstacles, adding challenge to navigation.
>> Dijkstra's Algorithm: The game employs Dijkstra's algorithm to suggest the next move based on the shortest path to the exit.
>> Colorful Interface: ANSI escape codes are used to add colour to the console interface, enhancing the gaming experience.


Look for the PNG for the screenshots of How the game works.

