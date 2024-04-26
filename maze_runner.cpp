#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <chrono>
#include <map>
#include <string>

using namespace std;
using namespace chrono;

// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define BLACK "\033[30m"   /* Black */

unsigned int level = 0;

// Set the time limit based on the level (in seconds)
unsigned int timeLimit = 0; // 5 minutes for example

struct Cell
{
    int row, col;
    bool visited;
    bool obstacle;
    int distance;   // Distance from start for Dijkstra's algorithm.
    Cell *previous; // Previous cell in the shortest path for Dijkstra's algorithm.
};

map<pair<int, int>, string> shortestPath;

// Function prototypes
void generateMaze(Cell **&maze, int rows, int cols);
void printMaze(Cell **maze, int rows, int cols, int playerRow, int playerCol);
bool isValid(int row, int col, int rows, int cols);
void movePlayer(Cell **&maze, int &playerRow, int &playerCol, int rows, int cols);
void disktraAlgo(Cell **maze, int playerRow, int playerCol, int rows, int cols);
int calculateDistance(int startX, int startY, int targetX, int targetY);
void freeMaze(Cell **maze, int rows);

int main()
{

    cout << endl;
    cout << endl;
    cout << BLUE << "................Welcome to maze runner...........\n"
         << RESET
         << endl;

    cout << "Please choose your level:\n"
         << endl;
    cout << YELLOW << "1. Level 1: 5 * 5 maze" << RESET << endl;
    cout << BLUE << "2. Level 2: 10 * 10  maze" << RESET << endl;
    cout << RED << "3. Level 3: 20 * 20 maze" << RESET << endl;
    cout << RED << "4. Level 3: 50 * 50 maze" << RESET << endl;
    cout << "Q : Quit" << endl;
    cin >> level;

    srand(time(nullptr));

    int rows, cols;
    switch (level)
    {
    case 1:
        rows = cols = 5;
        timeLimit = 10;
        break;
    case 2:
        rows = cols = 10;
        timeLimit = 20;
        break;
    case 3:
        rows = cols = 20;
        timeLimit = 40;
        break;
    case 4:
        rows = cols = 50;
        timeLimit = 40;
        break;
    case 'Q':
    case 'q':
        cout << "Quitting game..." << endl;
        return 0; // Exit program
    default:
        cout << "Invalid input. Please choose a valid level." << endl;
        return 1; // Exit program with error
    }

    // Get the starting time
    auto startTime = high_resolution_clock::now();

    // maze initialization and generation.
    Cell **maze;
    generateMaze(maze, rows, cols);

    int playerRow = 0, playerCol = 0;
    maze[playerRow][playerCol].visited = true;

    // Game loop
    bool gameOver = false;
    while (!gameOver)
    {
        // Print maze and player position
        printMaze(maze, rows, cols, playerRow, playerCol);

        // Print the remaining time
        auto currentTime = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(currentTime - startTime);
        cout << MAGENTA << "Time remaining: " << timeLimit - duration.count() << " seconds" << RESET << endl;

        // Move player
        movePlayer(maze, playerRow, playerCol, rows, cols);

        disktraAlgo(maze, playerRow, playerCol, rows, cols);

        // Checks for game over conditions (reached exit, hit obstacle, etc.).
        if (playerRow == rows - 1 && playerCol == cols - 1)
        {
            cout << YELLOW << "Congratulations! You reached the exit!" << RESET << endl;
            cout << "You have taken\t" << calculateDistance(0, 0, playerRow, playerCol) << "steps to reach the exit." << endl;
            gameOver = true;
            // Free allocated memory.
            freeMaze(maze, rows);
        }

        // Check if time is up.
        if (duration.count() >= timeLimit)
        {
            cout << BLACK << "Time's up! You failed to complete the maze within the time limit." << RESET << endl;
            cout << "You have taken\t" << calculateDistance(0, 0, playerRow, playerCol) << "steps." << endl;
            gameOver = true;
            // Free allocated memory.
            freeMaze(maze, rows);
        }
    }

    freeMaze(maze, rows);
    return 0;
}

// Generates maze using Depth-First Search algorithm.
void generateMaze(Cell **&maze, int rows, int cols)
{
    maze = new Cell *[rows]; // Allocate memory for rows.

    for (int i = 0; i < rows; ++i)
    {
        maze[i] = new Cell[cols]; // Allocate memory for each column.
        for (int j = 0; j < cols; ++j)
        {
            maze[i][j].row = i;
            maze[i][j].col = j;
            maze[i][j].visited = false;
            maze[i][j].obstacle = false; // Initializing all cells as non-obstacles.

            // 25% chance of being an obstacle.
            maze[i][j].obstacle = rand() % 4 == 0;
            maze[i][j].distance = INT_MAX;
            maze[i][j].previous = nullptr;

            // Mark cells that are not obstacles as paths
            if (!maze[i][j].obstacle)
            {
                shortestPath.insert({{i, j}, "."});
            }
        }
    }
}

// Prints maze with player's position.
void printMaze(Cell **maze, int rows, int cols, int playerRow, int playerCol)
{
    // Print maze grid with cell coordinates.
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (i == playerRow && j == playerCol)
            {
                shortestPath.insert({{i, j}, "P"});
                cout << CYAN << "P " << RESET; // Player.
            }
            else if (i == 0 && j == 0)
            {
                shortestPath.insert({{i, j}, "S"});
                cout << "S "; // Start.
            }
            else if (i == rows - 1 && j == cols - 1)
            {
                shortestPath.insert({{i, j}, "E"});
                cout << "E "; // Exit.
            }
            else if (maze[i][j].obstacle)
            {
                if ((i == 0 && j == 1) || (i == 1 && j == 0) || (i == rows - 1 && j == cols - 2) || (i == rows - 2 && j == cols - 1))
                {
                    continue;
                }
                shortestPath.insert({{i, j}, "#"});
                cout << GREEN << "# " << RESET; // Obstacle.
            }
            else
            {
                shortestPath.insert({{i, j}, "."});
                cout << BLACK << ". " << RESET; // Path.
            }
        }
        cout << endl;
    }
}

// Checks if cell coordinates are within bounds.
bool isValid(int row, int col, int rows, int cols)
{
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

// Free allocated memory for the maze.
void freeMaze(Cell **maze, int rows)
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] maze[i];
    }
    delete[] maze;
    cout << "deleted the maze successfully" << endl;
}

// Moves player based on user input.
void movePlayer(Cell **&maze, int &playerRow, int &playerCol, int rows, int cols)
{
    char direction;
    cout << "Enter direction (W/A/S/D): ";
    cin >> direction;
    switch (direction)
    {
    case 'W':
    case 'w':
        if (isValid(playerRow - 1, playerCol, rows, cols) && !maze[playerRow - 1][playerCol].obstacle)
        {
            playerRow--;
        }
        break;
    case 'A':
    case 'a':
        if (isValid(playerRow, playerCol - 1, rows, cols) && !maze[playerRow][playerCol - 1].obstacle)
        {
            playerCol--;
        }
        break;
    case 'S':
    case 's':
        if (isValid(playerRow + 1, playerCol, rows, cols) && !maze[playerRow + 1][playerCol].obstacle)
        {
            playerRow++;
        }
        break;
    case 'D':
    case 'd':
        if (isValid(playerRow, playerCol + 1, rows, cols) && !maze[playerRow][playerCol + 1].obstacle)
        {
            playerCol++;
        }
        break;
    default:
        cout << "Invalid input. Please enter W, A, S, or D." << endl;
    }
}

// Calculates distance between two points.
int calculateDistance(int startX, int startY, int targetX, int targetY)
{
    return abs(startX - targetX) + abs(startY - targetY);
}

void disktraAlgo(Cell **maze, int playerRow, int playerCol, int rows, int cols)
{
    // Initialize distance of all cells to infinity and previous pointer to nullptr.
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            maze[i][j].distance = INT_MAX;
            maze[i][j].previous = nullptr;
        }
    }

    // Create a priority queue to store cells ordered by distance.
    priority_queue<pair<int, Cell *>, vector<pair<int, Cell *>>, greater<pair<int, Cell *>>> pq;
    maze[playerRow][playerCol].distance = 0;   // Start cell has distance 0.
    pq.push({0, &maze[playerRow][playerCol]}); // Push start cell to the priority queue.

    // Dijkstra's algorithm.
    while (!pq.empty())
    {
        // Extract the cell with the smallest distance.
        Cell *current = pq.top().second;
        pq.pop();

        // Explore neighbors of the current cell.
        int row = current->row;
        int col = current->col;
        int distance = current->distance;

        // Neighbor positions.
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; ++i)
        {
            int newRow = row + dx[i];
            int newCol = col + dy[i];

            // Check if the neighbor is within bounds , so that memory does not go out of bound.
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols)
            {
                // Check if the neighbor is not an obstacle.
                if (!maze[newRow][newCol].obstacle)
                {
                    int newDistance = distance + 1; // Distance to neighbor is 1 more than current cell.
                    if (newDistance < maze[newRow][newCol].distance)
                    {
                        // Update distance and previous pointer of the neighbor.
                        maze[newRow][newCol].distance = newDistance;
                        maze[newRow][newCol].previous = current;
                        // Push the neighbor to the priority queue with updated distance.
                        pq.push({newDistance, &maze[newRow][newCol]});
                    }
                }
            }
        }
    }

    // Once Dijkstra's algorithm is done, backtrack from the exit to the start to find the shortest path.
    Cell *current = &maze[rows - 1][cols - 1]; // Start from the exit.
    vector<pair<int, int>> shortestPath;       // Store the shortest path coordinates.

    while (current != nullptr)
    {
        shortestPath.push_back({current->row, current->col});
        current = current->previous; // Move to the previous cell in the shortest path.
    }

    // Print the shortest path coordinates (in reverse order).
    // cout << "Shortest Path: ";
    // for (int i = shortestPath.size() - 1; i >= 0; --i)
    // {
    //     cout << "(" << shortestPath[i].first << ", " << shortestPath[i].second << ") ";
    // }
    // cout << endl;

    // Suggest the next coordinates to the player based on the shortest path.
    if (!shortestPath.empty())
    {
        pair<int, int> nextCoordinates = shortestPath[shortestPath.size() - 2]; // Get the second last coordinate.
        cout << "Next Coordinates: (" << nextCoordinates.first << ", " << nextCoordinates.second << ")" << endl;
    }
}
