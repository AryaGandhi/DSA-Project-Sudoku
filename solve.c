#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "verifications.h"
#include "solve.h"
#include "files.h"
#include "generate.h"


int solve_without_assumption(int grid[9][9]){
    int i = 0, j = 0, n = 0, posi = 0, posj = 0, posn = 0, nb_poss = 0, posi2 = 0, posj2 = 0, posn2 = 0, nb_poss2 = 0, finished = 0, k;
    nb grid_poss[9][9];

    // By default put all the possibilities to 1 (= true), then do the tests and we set to 0 if it's impossible
    for (i=0; i < 9; i++){
        for (j = 0;j < 9 ; j++){
            for (n = 0;  n < 9; n++)
            	grid_poss[i][j].number[n] = 1;
        }
    }


    // call the function verify_possibilites to know which figures can be put in each box
    verify_possibilites(grid, grid_poss);

   k = 1;

    while(!finished && k){
        k = 0; // k is used to find out if a box is found or if we are stuck
        for(i = 0; i < 9; i++){
            for(j = 0; j < 9; j++){
                nb_poss = 0;
                for(n = 0; n < 9; n++){ //look if for a given box a single shot is possible
                if(grid_poss[i][j].number[n]){
                        nb_poss++;
                        posn = n;
                        posi = i;
                        posj = j;
                    }
                }
                if (nb_poss == 1){
                    grid[posi][posj] = posn + 1;
                    verify_possibilites(grid, grid_poss);
                    k = 1;
                }
            }
        }

        // check if an n can only go to one cell of the row / column / region
        for(n = 0; n < 9; n++){
            for(i = 0; i < 9; i++){
                nb_poss = 0;
                nb_poss2 = 0;
                for(j = 0; j < 9; j++){
                    if(grid_poss[i][j].number[n]){ // traverse the line
                        nb_poss++;
                        posn = n;
                        posi = i;
                        posj = j;
                    }
                    if(grid_poss[j][i].number[n]){ // traverse the column
                        nb_poss2++;
                        posn2 = n;
                        posi2 = j;
                        posj2 = i;
                    }
                }
                if(nb_poss == 1){ // If n can only go to one place on the line, place it
                    grid[posi][posj] = posn + 1;
                    verify_possibilites(grid, grid_poss);
                    k = 1;
                }
                if(nb_poss2 == 1){ // If n can only go in one box of the column, place it
                    grid[posi2][posj2] = posn2 + 1;
                    verify_possibilites(grid, grid_poss);
                    k = 1;
                }
            }
        }

    verify_finished(grid, &finished);
    }

    if(finished)
		return 1; // returns 1 if the grid is resolved, and 0 if it can not be resolved completely logically
    else 
    		return 0;
}




int solve_with_assumption(struct grid_supp *last_element, int impossible){
    int i, j, n, p;
    grid_supp supp;

    supp.next = last_element; // stand between the penultimate and the last element
    supp.prev = supp.next->prev;

    i = supp.next->i; // recover the coordinates if an assumption needs to be changed
    j = supp.next->j;
    n = supp.next->n;

    if(impossible){ // If impossible, change an assumption
        copy_grid(supp.next->prev->grid, supp.next->grid); // reset the grid

        if(n < 9){ // test another n (if possible)
            n++;
            p = verify_case(supp.next->grid, n, i, j); //looking for an n that can go in the box
            while(n < 9 && !p){
                n++;
                p = verify_case(supp.next->grid, n, i, j);
            }
            if(p){ // If a possible shot is found, test it
                supp.next->grid[i][j] = n; // change the n in the grid
                supp.next->n = n; // save the assumption (to be able to return)
            }
            else{
                impossible = 1;
                supp.next->n = 9; // No n is suitable, so move on to the next part
                solve_with_assumption(supp.next, impossible); // call the function to go back in the list (so execute the next part))
            }
        }


        else{ // Or after testing all go back to change an old assumption
            if(supp.prev != NULL){ // do not delete the first box
                free(supp.next); // go back, so the next box is no longer needed
                supp.next = supp.next; // go back
                supp.prev = supp.prev->prev;
                supp.next->next = NULL; // There is nothing after
                impossible = 1;
                solve_with_assumption(supp.next, impossible); // call the function to change the guess (to execute the part just above)
            }
            else // That means we're at the first link, so it should not be impossible, so there's a mistake
                return 0;
        }
    }


    else{ // If no, we make a new assumption and we continue
        supp.next->next = malloc(sizeof(grid_supp)); // Add a new item to the list
        supp.next->next->prev = supp.next; //  connect in both directions
        supp.prev = supp.next; // move supp
        supp.next = supp.next->next;
        supp.next->next = NULL;
        copy_grid(supp.prev->grid, supp.next->grid); // initialize the grid

        i=0;
        j=0;
        p = 0; // p = 1 if the box is empty, 0 if not empty.

        if(supp.next->grid[i][j] == 0) 
        		p = 1;

        while(i <= 8 && !p){ // search for the first empty box of the grid
            while(j < 8 && !p){
                j++;
                if(supp.next->grid[i][j] == 0) 
                	p = 1;
            }
            if(!p){ // If the box is not suitable test another line
                j = 0;
                i++;
            }
        }
        // Once out of the loop the coordinates i, j correspond to an empty box

        n=1;
        p = verify_case(supp.next->grid, n, i, j);

        while(!p && n < 9){ // look for the first n that can go in the box (there is necessarily one, if not impossible = 1 and function does not execute till here)
            n++;
            p = verify_case(supp.next->grid, n, i, j);
        }

        if(n <= 9){ // check all the same
            supp.next->grid[i][j] = n; // put n in the grid
            supp.next->i = i; // record the assumption to be able to change it if necessary
            supp.next->j = j;
            supp.next->n = n;
        }
    }
    return 1;
}




int solve(int grid[9][9]){
    int finished, impossible, prov = 1;
    grid_supp first, aux;

    finished = solve_without_assumption(grid); // try to solve without assumption

    impossible = verify_impossible(grid);
    if(impossible) 
    		return 0; // The grid is not solvable

    if(!finished){
        first.next = NULL; // Initialization of all the variables of first
        first.prev = NULL;
        first.i = -1; // No boxes were changed
        first.j = -1;
        first.n = 0;
        copy_grid(grid, first.grid); // copy the grid

        aux.next = &first; // Aux (which allows to browse the list) points to the first item (then it will be moved if necessary)
        do{
            impossible = verify_impossible(aux.next->grid);
            prov = solve_with_assumption(aux.next, impossible);
            aux.next = &first; // go back to the beginning of the list
            while(aux.next->next != NULL) 
            	aux.next = aux.next->next; // go to the last link (it can be changed)
            finished = solve_without_assumption(aux.next->grid); // After each guess look at whether it allows to find other boxes
        } while(!finished && prov); // continue until the grid is complete

        copy_grid(aux.next->grid, grid); // copy the grid

        // free the whole chain
        aux.next = first.next;
        while(aux.next->next != NULL){
            aux.prev = aux.next;
            aux.next = aux.next->next;
            free(aux.prev);
        }
        free(aux.next); // free the last element of the chain
    }
    return 1;
}
