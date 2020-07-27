// Libraries :
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Header files of other functions :
#include "structures.h"
#include "verifications.h"
#include "solve.h"
#include "files.h"
#include "generate.h"


/*
Function that helps the player by giving a box.
Returns 1 if all goes well, 0 if the grid is not solvable.
*/
int help(int grid[9][9])
{
    int k, l, copy[9][9], possible, finished;


    // solve a copy of the grid

    copy_grid(grid, copy);
    possible = solve(copy);

    verify_finished(grid, &finished);
    if(finished)
    		return 0;
    if(!possible) 
    		return 0;

    else{
        // give a random box
        do{
            k = (int)((rand()/(double)RAND_MAX)*8);
            l = (int)((rand()/(double)RAND_MAX)*8);
        } while(grid[k][l] != 0);

        grid[k][l] = copy[k][l]; // place the value
        return 1;
    }
}
