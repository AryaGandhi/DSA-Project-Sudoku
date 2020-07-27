#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Headers files of other functions :
#include "structures.h"
#include "verifications.h"
#include "solve.h"
#include "files.h"
#include "generate.h"

/*
This function will save the grid in a txt file which can then be printed.
A grid is drawn to make everything more readable.
Boxes and regions are not delimited in the same way.
*/
void printing (int grid[9][9], char* filename){
    int i, j, n, k, l;
    FILE *fp;
    fp = fopen (filename, "w+");

    l = 1;

    for(i = 0; i < 9; i++){
        k = 0;
        for(j = 0; j < 9; j++){
            n = grid[i][j];
            k++;
            if(n == 0) 
            	fprintf(fp, "   "); // replace the 0 with spaces to not print them
            else 
            	fprintf(fp, " %d ", n);

            if(k == 3 || k == 6) 
            	fprintf(fp, " * ");
            else if (k != 9) 
            	fprintf(fp, "|");
        }
        if(l == 3 || l == 6) 
        	  fprintf(fp, "\n* * * * * * * * * * * * * * * * * * *\n");
        else if(l == 9) 
        	  fprintf(fp, "\n");
        else 
        	  fprintf(fp, "\n----------- * ----------- * ----------\n");
        l++;
    }
    fclose(fp);
}
