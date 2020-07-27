#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structures.h"
#include "verifications.h"
#include "solve.h"
#include "files.h"
#include "generate.h"


int evaluate_level(int grid[9][9], int nb_cases){
    int lvl, i, j, n, nb_directs = 0, nb_a_find = 0, nbre;
    nb grid_poss[9][9];
    verify_possibilites(grid, grid_poss);

    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            nbre = 0;
            for(n = 0; n < 9; n++){
                if(grid_poss[i][j].number[n] == 1) 
                	nbre++;
            }
            if(nbre == 1) //count the number of boxes where there is only one possibility
            	nb_directs++;
        }
    }
    nb_a_find = nb_cases - nb_directs;//In the number of boxes to find remove the most direct boxes
    
    
    //The level is assigned according to the number of non-direct boxes to find
	if(nb_a_find >= EASY_TERMINAL && nb_a_find < MEDIUM_TERMINAL) 
		lvl = 1;
	else if(nb_a_find >= MEDIUM_TERMINAL && nb_a_find < DIFFICULT_TERMINAL) 
		lvl = 2;
	else if(nb_a_find >= DIFFICULT_TERMINAL) 
		lvl = 3;
	else 
		lvl = 0; //If there are not enough boxes
	return lvl;
}


void generate (int grid[9][9], int lvl){
    int i, j, n, nb, p, continuer = 1, lvl_current, grid_test[9][9];

    srand(time(NULL));//Initialization of rand

    //Initialization of the grid
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++)
            grid[i][j] = 0;
    }
    
/*The 3 regions of the diagonal can be filled randomly without worrying about rows or columns (just being careful not to put the same number twice in the region)*/
    
    for(i = 0; i < 3; i++) {//Filling the region at the top left
        for(j = 0; j < 3; j++){
            n = (int)((rand()/(double)RAND_MAX)*9 +1); //+1 because with casting risks getting 0
            if(n > 9) //check that it does not go beyond the limits
            	n = 9; 
            p = verify_case(grid, n, i, j);
            while(!p){
                if(n==9) 
                	n=1;
                else 
                	n++;
                p = verify_case(grid, n, i, j);
            }
            grid[i][j] = n; //here, the n is valid
        }
    }

    for(i = 3; i < 6; i++){//Filling the region in the middle
        for(j = 3; j < 6; j++){
            n = (int)((rand()/(double)RAND_MAX)*9 +1); 
            if(n > 9) 
            	n = 9;
            p = verify_case(grid, n, i, j);
            while(!p){
                if(n==9) 
                	n=1;
                else
                	n++;
                p = verify_case(grid, n, i, j);
            }
            grid[i][j] = n;
        }
    }

    for(i = 6; i < 9; i++){ //Filling the region at the bottom right
        for(j = 6; j < 9; j++){
            n = (int)((rand()/(double)RAND_MAX)*9 +1); 
            if(n > 9) 
            	n = 9;
            p = verify_case(grid, n, i, j);
            while(!p){
                if(n==9) 
                	n=1;
                else 
                	n++;
                p = verify_case(grid, n, i, j);
            }
            grid[i][j] = n;
        }
    }
    
/*The diagonal is generated, finish completing the grid by calling the solve function and assuming the missing boxes*/
    solve(grid);

/*A complete grid is obtained, some boxes must be removed to make it playable*/
    continuer = 1;
    nb = 0;

    while(continuer){
        i = (int)((rand()/(double)RAND_MAX)*8);
        j = (int)((rand()/(double)RAND_MAX)*8);
        if(grid[i][j] != 0){
            n = grid[i][j];
            grid_test[i][j] = 0;
            p = solve_without_assumption(grid_test);//check that there is only one possible solution
            if(p){
                grid[i][j] = 0;
                nb++;
                lvl_current = evaluate_level(grid, nb);
                if(lvl_current == lvl) 
                	continuer = 0;
            }
            copy_grid(grid, grid_test);
        }
    }
}
