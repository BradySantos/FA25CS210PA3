#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0}; // Up, right, down, left
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// Add arguments, return type, and logic
// ----------------------------------------------------------
bool dfs(int r, int c, const vector<vector<int>>& maze, vector<vector<bool>>& visited,
         vector<vector<int>>& parent_r, vector<vector<int>>& parent_c, int exit_r, int exit_c) {

    int numRows = maze.size(); // Store number of rows to not go out of bounds horizontally
    int numCols = maze[0].size(); // Store number of columns to not go out of bounds vertically
    // Format of coords will be (r, c)
    // To get value of maze location, maze[r][c]. 1 cannot pass, 0 can go through
    // Visited[r][c] stores true/false
    // Base: nothing found, return false. If E is found, return true
    // Neighbors using dr and dc
    // dr[4] = {-1, 0, 1, 0}; // Up, right, down, left
    // dc[4] = {0, 1, 0, -1};
    // 0: Up, 1: Right, 2: Down, 3: Left

    // go all the way down first?
    cout << "Starting traversal from: " << "(" << r << ", " << c << ")\n";
    if ((r == exit_r && c == exit_c)) { // If exit is found, return true
        return true;
    }
    if ((maze[r + dr[2]][c + dc[2]] == 0) && (visited[r + dr[2]][c + dc[2]] == false)) { // If r + 1 is open&&unvisited, move down one row
        // Parent?
        r += dr[2]; // Row changes
        c += dc[2];
    }
    else if ((maze[r + dc[1]][c + dc[1]] == 0) && (visited[r + dc[1]][c + dc[1]] == false)) { // Else if c + 1 is open&&unvisited, move right

        c += dc[1]; // Col changes
        r += dr[1];
    }
    else if ((maze[r + dr[3]][c + dc[3]]) && (visited[r + dr[3]][c + dc[3]] == false)) { // Else if c - 1 is open&&unvisited, move left

        c += dr[3];
        r += dc[3];
    }
    else { // If cannot go down, left, or right then go back up
        r += dr[4];
        c += dc[4];
    }



    // Different cases with different recursive calls depending on where wall is and what has been visited
    // Assign parent before each recurse


}


// ----------------------------------------------------------
// MAIN PROGRAM (students add DFS calls and logic)
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // Pick entrance and exit
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    while (exitcell == entrance) {
        exitcell = chooseBoundaryCell(maze);
    }

    int ent_r = entrance.first;
    int ent_c = entrance.second;
    int exit_r = exitcell.first;
    int exit_c = exitcell.second;

    // Display the maze
    printMaze(maze, ent_r, ent_c, exit_r, exit_c);

    // Students must use these
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // ------------------------------------------------------
    // STUDENT WORK:
    // Call your DFS, track visited, and fill parent_r and parent_c
    // ------------------------------------------------------
    bool found = dfs(ent_r, ent_c, maze, visited, parent_r, parent_c, exit_r, exit_c);

    // ------------------------------------------------------
    // STUDENT WORK:
    // If found, print the path
    // ------------------------------------------------------
    if (found) {
        printPath(exitcell, parent_r, parent_c, ent_r, ent_c);
    } else {
        cout << "\nNo path exists.\n";
    }

    return 0;
}