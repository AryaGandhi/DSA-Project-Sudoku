#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "verifications.h"
#include "solve.h"
#include "files.h"
#include "generate.h"


/* This function checks whether a coordinate box (i, j) can take the value n */
int verify_case(int grid[9][9], int n, int i, int j){
    	int i_case, j_case, k, l;

/* We place ourselves in the box of the upper right corner of the square of 3x3 which contains the box (i, j) */
	if (i < 3) 
    		i_case = 0;
	else if (i < 6 && i >= 3) 
		i_case = 3;
	else 
		i_case = 6;

	if (j < 3) 
		j_case = 0;
    	else if (j < 6 && j >= 3) 
    		j_case = 3;
    	else 
    		j_case = 6;



    	if (grid[i][j] != 0) // check if there is already a number in the box, in which case the following steps are not necessary, no move is possible in this box.
		return 0;

    	else{
    /* check if there is already a box containing n in this square */
        	for (k = i_case; k < i_case + 3;  k++){ // There are two boxes in addition to the one in the square (row and column)
            	for (l = j_case; l < j_case + 3;  l++ ){
                	if (grid[k][l] == n)
                    	return 0; // If there is a box in the square that contains n, no need to go further:  stop and return 0
            	}
        	}
    // If no square box contains n, test the row and the column
        	for (k = 0; k < 9; k++){
            	if (grid[k][j] == n) // test the column
                	return 0; // If there is already an n in the column, stop and return 0

            	else if (grid[i][k] == n) // test the row
                	return 0; // If there is already an n in the row, stop and return 0
        	}

    	return 1; // If we get here, there is no n in the square, the column and the line, so the box can contain n
    	}
}

/* check all the boxes of the grid by using the function verify_case */
void verify_possibilites (int grid[9][9], nb grid_poss[9][9]){
    	int i, j, n;
    	for (i = 0; i < 9; i++){
        	for (j = 0;  j < 9;j++){
            	for (n = 0;  n < 9; n++)
                	grid_poss[i][j].number[n] = verify_case(grid, (n+1), i, j); // n + 1 because here n from 0 to 8 (offset due to the array, while testing the true n)
        	}
    	}
}


void verify_finished(int grid[9][9], int *finished){
    	int i, j;
    	*finished = 1; // assign the value 1 to finite then check if there are still white boxes, in which case the value is 0 (= false, not finished)
    	for (i= 0;  i < 9; i++){
        	for (j = 0; j < 9;  j++){
            	if(grid[i][j] == 0)
                	*finished = 0;
        	}
    	}
}


int verify_impossible(int grid[9][9]){ // Check if the grid resolution is possible (no boxes where there are no possibilities)
    	int i, j, n, impossible = 0, shot_possible;
    	nb grid_poss[9][9] ;

    	verify_possibilites(grid, grid_poss);

    	for(i = 0; i < 9; i++){
        	for(j = 0; j < 9; j++){
            	if(grid[i][j] == 0){
                	shot_possible = 0;
                	for(n = 0; n < 9; n++){
                    	if(grid_poss[i][j].number[n] != 0) 
                    		shot_possible = 1; // check if at least one shot is possible
                	}
                	if(shot_possible == 0) 
                		impossible = 1; // If no shot is possible the grid is impossible
            	}
        	}
    	}
    return impossible;
}


int verify_valid(int grid[9][9]){
    	int i, j, n, valid, p;

    	valid = 1;

    	for(i = 0; i < 9; i++){
        	for(j = 0; j < 9; j++){
            	n = grid[i][j]; // Save the number in the box
            	grid[i][j] = 0; // empty this box to know if n can go in
            	p = verify_case(grid, n, i, j); // check if the shot is possible
            	grid[i][j] = n; // replace n
            	if(!p && n != 0) 
            		valid = 0; // If the shot is not possible the grid is not valid
        	}
    	}

    	return valid;
}
