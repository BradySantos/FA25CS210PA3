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

/* Format of coords is (r, c)
 * To get value of maze location, maze[r][c]. 1 cannot pass, 0 can go through
 * Visited[r][c] stores true/false
 * dr[4] = {-1, 0, 1, 0}; Up, right, down, left
 * dc[4] = {0, 1, 0, -1};
 * 0: Up, 1: Right, 2: Down, 3: Left
 * Check down direction as far as possible; if cannot go down anymore, check right, left, up
 * Parent debug statements are upside down because of stack unwinding
 */


bool dfs(int r, int c, const vector<vector<int>>& maze, vector<vector<bool>>& visited,
         vector<vector<int>>& parent_r, vector<vector<int>>& parent_c, int exit_r, int exit_c) {

    int numRows = maze.size(); // Store number of rows to not go out of bounds horizontally
    int numCols = maze[0].size(); // Store number of columns to not go out of bounds vertically

    visited[r][c] = true; // Set current coord as visited; updates with each recursion automatically
    cout << "Traversing from: " << "(" << r << ", " << c << ")\n";
    if ((r == exit_r && c == exit_c)) { // If exit is found, return true
        cout << "Exiting dfs... path found! :)\n";
        return true;
    }

    // Update for moving down
    int downR = r + dr[2];
    int downC = c + dc[2];

    // If r + 1 is in bounds, open, unvisited, move down one row
    if ((downR < numRows) && (maze[downR][downC] == 0) && (visited[downR][downC] == false)) {
        cout << "Moving down one\n";
        if (dfs(downR, downC, maze, visited, parent_r, parent_c, exit_r, exit_c)) { // Go as far as possible since dfs
            cout << "Updating parent after moving down\n";
            parent_r[downR][downC] = r; // Set parent of the moved coord to the current coord
            parent_c[downR][downC] = c;
            return true; // If exit is found
        }
    }

    // Update for moving right
    int rightR = r + dr[1];
    int rightC = c + dc[1];

    // If c + 1 is in bounds, open, unvisited, move right one col
    if ((rightC < numCols) && (maze[rightR][rightC] == 0) && (visited[rightR][rightC] == false)) {
        cout << "Moving right one\n";
        if (dfs(rightR, rightC, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            cout << "Updating parent after moving right one\n";
            parent_r[rightR][rightC] = r;
            parent_c[rightR][rightC] = c;
            return true;
        }
    }

    // Update for moving left
    int leftR = r + dr[3];
    int leftC = c + dc[3];

    // If c - 1 is in bounds, open, unvisited, move left one col
    if ((leftR >= 0) && (maze[leftR][leftC] == 0) && (visited[leftR][leftC] == false)) {
        cout << "Moving left one\n";
        if (dfs(leftR, leftC, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            cout << "Updating parent after moving left one\n";
            parent_r[leftR][leftC] = r;
            parent_c[leftR][leftC] = c;
            return true;
        }
    }

    // Update for moving up
    int upR = r + dr[0];
    int upC = c + dc[0];

    // Going up is the last condition since dfs should check deepest coords first
    if ((upR >= 0) && (maze[upR][upC] == 0) && (visited[upR][upC] == false)) {
        cout << "Moving up one\n";
        if (dfs(upR, upC, maze, visited, parent_r, parent_c, exit_r, exit_c)) {
            cout << "Updating parent after moving up one\n";
            parent_r[upR][upC] = r;
            parent_c[upR][upC] = c;
            return true;
        }
    }

    return false; // If no path is found, false is returned
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