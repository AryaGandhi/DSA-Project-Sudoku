// Constants for difficulty levels (number of non-direct boxes to find)
#define EASY_TERMINAL 25
#define MEDIUM_TERMINAL 35
#define DIFFICULT_TERMINAL 45


// Structure to keep in mind the possibilities of each box
typedef struct nb
{
    int number[9];
}nb;

// Doubly linked list structure for solving (thus also generating) grids with guess
typedef struct grid_supp
{
    struct grid_supp *prev;
    int grid[9][9]; // make a backup of the grid with each assumption to be able to go back if the assumption is not good
    int n, i, j; // To know which assumption is made on this grid

    struct grid_supp *next;
}grid_supp;

