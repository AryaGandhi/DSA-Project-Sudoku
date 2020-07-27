#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "files.h"


void copy_grid(int grid[9][9], int copy[9][9]){ // Copy grid in copy
    int i, j;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++)
            copy[i][j] = grid[i][j];
    }
}

int compare_grid(int grid[9][9], int otherGrid[9][9]){
    int i, j;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++)
            if (grid[i][j]!=otherGrid[i][j]) 
            	return 0;
    }

    return 1;
}

int save (int grid[9][9], char* filename){ // Returns 1 if it works, 0 if there is an error
    int i, j;
    FILE *fp = NULL;

        fp = fopen(filename, "a"); // Open the file

    if(fp == NULL) // check if it opened
        return 0;
    else{
        for(i = 0; i < 9; i++){
            for(j = 0; j < 9; j++)
                fprintf(fp, "%d", grid[i][j]);
        }
        fprintf(fp, "\n"); // return to the line when all the grid is returned

        fclose(fp);
        return 1;
    }

}

int reading (int grid[9][9], char* filename){
    int i, j, n, k, nb = 0;
    FILE *fp = NULL;
    char c;

    // open the right level file
    fp = fopen(filename, "r");

    if(fp == NULL) 
    		return 0;

    else{
        while((c = fgetc(fp)) != EOF)
            if(c == '\n') 
            	nb++; // count the number of lines
        n = (int)((rand() / RAND_MAX) * nb); // draw a lot to know which grid to open
        k = 1;
        fseek(fp, 0, SEEK_SET);
        while(k < n){
            c = fgetc(fp);
            if(c == '\n') 
            	k++; // to go to the right line
        }
        for(i = 0; i < 9; i++){
            for(j = 0; j < 9; j++)
                grid[i][j] = fgetc(fp) - 48; // -48 to switch from ASCII to decimal
        }
        fclose(fp);
        return 1;
    }
}
