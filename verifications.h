// This function checks whether a digit n can go in a coordinate box (i, j).
int verify_case(int grid[9][9], int n, int i, int j);

// This function checks all the possibilities of all the boxes of the game.
void verify_possibilites (int grid[9][9], nb grid_poss[9][9]);

// This function checks if the game is finished, i.e if all the boxes are filled.
void verify_finished(int grid[9][9], int *finished);

// This function checks if a grid is sovable
int verify_impossible(int grid[9][9]);

int verify_valid(int grid[9][9]);
