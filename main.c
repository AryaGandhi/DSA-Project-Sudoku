// Libraries :
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

// Header files of other functions :
#include "structures.h"
#include "verifications.h"
#include "solve.h"
#include "files.h"
#include "generate.h"
#include "help.h"
#include "printing.h"

// Variable declaration :
static int level = 1;
static int modeEdition =0;
static int grid[9][9];
static int gridInitial[9][9];
static int gridTemp[9][9];

// Declaration of the main widgets of the program :
static GtkWidget* window;
static GtkWidget* gridScreen[9][9];

// Function management interface :
static void menuEvent(GtkWidget* widget, gpointer data);//Menu management
static void buttonVerify(GtkWidget* widget, gpointer data);

//function to synchronize the graphic part and the algorithmic part
void synchroniserGridDisplay(int direction, int blocking);

int main (int argc, char *argv[]){
    /* Creating the different widgets needed */

    GtkWidget *verticalBox;//boxes that will contain the different widgets
    GtkWidget *horizontalBox;
    GtkWidget *separator;
    GtkWidget *button; //solve button
    GtkWidget *menuFile, *menuHelp, *menuLevel; //menu widgets
    GtkWidget *toolbar;
    GtkWidget *menuItem;

    /*Creating and initializing labels on the menu bar*/

    const char* file[] = {"New game", "Empty the grid", "Open saved game", "Save current game", "Print current game to file", "Exit"};
    const char* help[] = {"Solve the current game", "Hint", "Rules of the game", "How to play", "About the developer"};
    const char* level[] = {"Easy", "Medium", "Difficult"};

    int i, j;

     gtk_init(&argc, &argv); // initialise gtk

    /*Creation of the window*/

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);//initialize the main window of the program
    gtk_window_set_title(GTK_WINDOW(window), "SUDOKU");
    gtk_window_set_icon_from_file(GTK_WINDOW(window),"icon.ico",NULL); // insert an icon for the program
    gtk_window_set_resizable (GTK_WINDOW(window), FALSE); // restrict the resizing by the user

    verticalBox = gtk_vbox_new(0,15); // initialize the vertical box widget

    /*Creating the menu bar and menus that compose it*/

    toolbar=gtk_menu_bar_new(); //initialise the menu bar
    menuFile=gtk_menu_new(); // and the menus
    menuHelp=gtk_menu_new();
    menuLevel=gtk_menu_new();

    //menu file
    menuItem=gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menuFile);//define menuFile as submenu

    gtk_menu_shell_append(GTK_MENU_SHELL(toolbar), menuItem);//Add items from top to bottom
    
    //loop to define the elements
	for( i=0; i <6 ; i++){
		menuItem=gtk_menu_item_new_with_label(file[i]); //initialize the menu item and we assign it a label
		gtk_menu_shell_append(GTK_MENU_SHELL(menuFile), menuItem); // add the items in the menu from top to bottom
		g_signal_connect(menuItem,"activate",G_CALLBACK(menuEvent), NULL);//connect the signal to the callback function menuEvent
	}

    //menu level
	menuItem=gtk_menu_item_new_with_label("Level");
     gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menuLevel);
     gtk_menu_shell_append(GTK_MENU_SHELL(toolbar), menuItem);
	    	for ( i=0; i <3 ; i++){
            	menuItem=gtk_menu_item_new_with_label(level[i]);
            	gtk_menu_shell_append(GTK_MENU_SHELL(menuLevel), menuItem);
            	g_signal_connect(menuItem,"activate",G_CALLBACK(menuEvent), NULL);
        	}

    //menu help
	menuItem=gtk_menu_item_new_with_label("Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menuHelp);
	gtk_menu_shell_append(GTK_MENU_SHELL(toolbar), menuItem);
	for ( i=0; i <5 ; i++){
           menuItem=gtk_menu_item_new_with_label(help[i]);
           gtk_menu_shell_append(GTK_MENU_SHELL(menuHelp), menuItem);
           g_signal_connect(menuItem,"activate",G_CALLBACK(menuEvent), NULL);
	}

	gtk_box_pack_start(GTK_BOX(verticalBox), toolbar, 0,0,0); //insert the menu bar in the vertical box at the top

    /*Creating the grid */
    for (i =0; i<9; i++){
        horizontalBox = gtk_hbox_new(0,15); // initialize the horizontal box widget
        for (j=0;j<9;j++){
			gridScreen[i][j]=gtk_entry_new();//initialise the "boxes"
               gtk_entry_set_max_length(GTK_ENTRY(gridScreen[i][j]),1); //limit the size of gtk_entry to one element
               gtk_widget_set_size_request(gridScreen[i][j],35,35); // setting the size of gtk_entry

                /*create the regions by artificially separating the boxes*/
                gtk_box_pack_start(GTK_BOX(horizontalBox),gridScreen[i][j],0,0,0);
                if((j+1)%3==0 && j < 8){ //for not having a 3rd separator
                        separator = gtk_vseparator_new();//vertical separator
                        gtk_box_pack_start(GTK_BOX(horizontalBox),separator,0,0,0);
                }
        }
        gtk_box_pack_start(GTK_BOX(verticalBox),horizontalBox,0,0,0);
        if((i+1)%3==0 && i < 8){
                separator = gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(verticalBox),separator,0,0,0);}
    }

    /*Creating the check button */

    	button=gtk_button_new_with_label("Verify my game"); //initialize the button and assign it a label
    	g_signal_connect(button, "clicked", G_CALLBACK(buttonVerify), NULL);//connect the signal to the function that manages the button
   	gtk_box_pack_start(GTK_BOX(verticalBox),button,0,0,0);
    	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);// connect the signal to the function gtk_main_quit to allow the program to stop 
    
	modeEdition=0;
     generate(grid, 1); // call generate grid by sending default level 1(easy)
     copy_grid(grid,gridInitial);//save the initial grid
     synchroniserGridDisplay(0,1);//block the generated boxes	
	
    	gtk_container_add(GTK_CONTAINER(window), verticalBox);//add the vertical box in the window
    	gtk_widget_show_all(window); // display the window
    	gtk_main();//gtk operating loop
     
    	return EXIT_SUCCESS;
}

/*Menu management functions*/
/*Recover the signal sent by the menu item and compare its label with different characters to know on which item the user has clicked and thus launch the correct function*/
static void menuEvent(GtkWidget *widget, gpointer data){
    /* Generate a grid */

    	if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"New game")==0){
   		int i,j;
          for (i=0;i<9;i++){
          	for (j=0;j<9;j++){
  	          	grid[i][j]=0;
                    gtk_entry_set_editable (GTK_ENTRY(gridScreen[i][j]),TRUE);//make sure that the boxes are editable
                    gtk_entry_set_text(GTK_ENTRY(gridScreen[i][j]), "");
          	}
		}
          copy_grid(grid,gridInitial);
    
        	modeEdition=0;
        	generate(grid, level); // call generate grid by sending the desired level (from 1 to 3)
        	copy_grid(grid,gridInitial);//save the initial grid
        	synchroniserGridDisplay(0,1);//block the generated boxes
    	}

    /*Empty grid*/
    /*makes the grid blank so that the user can create his own sudoku*/

    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Empty the grid") == 0 ){
                    int i,j;
                    for (i=0;i<9;i++){
                        for (j=0;j<9;j++){
                            grid[i][j]=0;
                            gtk_entry_set_editable (GTK_ENTRY(gridScreen[i][j]),TRUE);//make sure that the boxes are editable
                            gtk_entry_set_text(GTK_ENTRY(gridScreen[i][j]), "");
                        }
                    }
                    copy_grid(grid,gridInitial);
                    modeEdition=1;
    }

     /*Open a grid*/
     /*Allows the user to open a file containing a previously saved game and display it on the screen*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Open saved game")==0){
       modeEdition=0;
       GtkWidget *dialogBox=NULL;//create a widget that will be initialized in dialog box of choice of file
       dialogBox = gtk_file_chooser_dialog_new("Open saved game", GTK_WINDOW(window),GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_OK, GTK_RESPONSE_OK,GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,NULL);
       gtk_widget_show_all(dialogBox);//display the widget dialog box

       //test the response of the user, if he clicks ok, we open the file asked and we display it on the screen
       gint reponse=gtk_dialog_run(GTK_DIALOG(dialogBox));
       if(reponse == GTK_RESPONSE_OK){
                char *filename;//use this pointer to store the file name that the user has chosen
                filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialogBox)); // recover this name

                reading(grid, filename);//send the function read the grid and the name. The file is read and the grid filled
                synchroniserGridDisplay(0,1);//the display (screen grid) is synchronized with the grid.
            }

       gtk_widget_destroy(dialogBox);//destroy the dialogue box
       copy_grid(grid, gridInitial);
    }

     /*Save a grid*/
     /*Save allows the user to save the current game displayed on the screen as a file*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Save current game")==0){
        modeEdition=0;
        synchroniserGridDisplay(1,0);//synchronize the grid with the display
        if (verify_valid(grid)==1){  //check that the grid is valid before saving
            GtkWidget *dialogBox=NULL;//the widget will be initialized in dialog box of choice of file to save
            dialogBox = gtk_file_chooser_dialog_new ("Save game", NULL,GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
            gtk_widget_show_all(dialogBox);
            gint reponse=gtk_dialog_run(GTK_DIALOG(dialogBox));
            if(reponse == GTK_RESPONSE_ACCEPT){
                char *filename;
                filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialogBox));
                synchroniserGridDisplay(1,0); //synchronize the grid with the display
                save(grid, filename);//send the name of the file and the grid to the function that saves the content of the grid in a file
            }
            gtk_widget_destroy(dialogBox);
       }
       else{ //otherwise display a widget that informs the user of the problem in the grid
            GtkWidget *fail;//create a widget that will be initialized in an information dialog box
            fail= gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"\nThere is an error in the grid.\nYou should not save a wrong grid! " );
            gtk_widget_show_all(fail);
            gtk_dialog_run(GTK_DIALOG(fail));//launch the dialog box
            gtk_widget_destroy(fail);
        }
    }

    /*Print to file*/
    /*Allows the user to save a game displayed on the screen in a file in a format suitable for printing. The differences with the previous function are call printing instead of saving and adding the .txt extension to improve compatibility with Windows-based text editors.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Print current game to file")==0){
       modeEdition=0;
       GtkWidget *dialogBox=NULL;
       dialogBox = gtk_file_chooser_dialog_new ("Save  file", NULL,GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
       gtk_widget_show_all(dialogBox);
       gint reponse=gtk_dialog_run(GTK_DIALOG(dialogBox));
       if(reponse == GTK_RESPONSE_ACCEPT){
             /*The grid must be printable in a text editor, for windows a .txt extension is needed*/
                char *filename, extension[]=".txt";
                filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialogBox));
                strcat(filename, extension);// concatenating the name chosen by the user with the extension .txt
                synchroniserGridDisplay(1,0);
                printing(grid, filename); //send the name of the file and the grid to the function that saves the content of the grid in a file in a printable format
        }
       gtk_widget_destroy(dialogBox);
    }

    /*Solve*/
    /*Allows the user to ge the current game solved if there is no fault or indicate if there is some fault.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Solve the current game")==0){
            synchroniserGridDisplay(1,0);//synchronize the grid with the display
            if(modeEdition ==1 ) 
            	copy_grid(grid, gridInitial);//not to prevent the solving of an empty grid
            copy_grid(grid, gridTemp);
            solve(gridTemp);
            solve(gridInitial);
            if (verify_valid(grid)==1 && compare_grid(gridInitial, gridTemp) == 1){
                solve(grid);// if the grid has no user error and is solvable, it is solved
                synchroniserGridDisplay(0,1);//then in the other direction to display the solved grid
            }
            else if (verify_valid(grid)==1 && compare_grid(gridInitial, gridTemp) ==0){
                GtkWidget *fail;//create a widget that will be initialized in an information dialog box
                fail = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"\nThe grid respects the rules of sudoku \n but it does not correspond to the solution, it is therefore not solved.");
                gtk_widget_show_all(fail);
                gtk_dialog_run(GTK_DIALOG(fail));//launch the dialog box
                gtk_widget_destroy(fail);
            }
            else{ //otherwise display a widget that informs the user of the problem so that they correct their problem
                GtkWidget *fail;//create a widget that will be initialized in an information dialog box
                fail = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"\nThere is an error in the grid. \nWith these entries it is unsolvable.");
                gtk_widget_show_all(fail);
                gtk_dialog_run(GTK_DIALOG(fail));//launch the dialog box
                gtk_widget_destroy(fail);
            }

    }

    /*hint*/
    /*Help the user when he is unable to solve any further.*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Hint")==0){

         modeEdition=0;
        synchroniserGridDisplay(1,0);
        help(grid); // call the help function that returns a grid with a random box in addition
        synchroniserGridDisplay(0,0);//synchronize with the display
    }
	
	/*Rules of the game*/
	else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Rules of the game")==0){
        GtkWidget *rules;
        rules = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK, "RULES OF THE GAME:\n\nThis Sudoku game involves a grid of 81 squares. The grid is divided into nine blocks, each containing nine squares.\n The rules of the game are simple: each of the nine blocks has to contain all the numbers 1-9 within its squares. Each number can only appear once in a row, column or box.\n" );
        gtk_widget_show_all(rules);
        gtk_dialog_run(GTK_DIALOG(rules));//launch the dialog box
        gtk_widget_destroy(rules);
    }
	
	/*How to play*/
	else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"How to play")==0){
        GtkWidget *howtoplay;
        howtoplay = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK, "HOW TO PLAY:\n\nBy default, when the game is started, its difficulty level is easy. Start filling the unfilled boxes according to the rules of the game.\nTo verify the boxes filled by you, click on 'Check my grid'.\nTo change the difficulty level, click on the Level tab and select difficulty level(difficulty level implies number of boxes of the grid already filled i.e more boxes are already filled for easy level).\nTo discard the current game and start a new game, click on File tab and select 'New game'.\nIf you want to save your current game, go to File tab and click on 'Save current game'.\n To print your current game in proper presentable format into a text file, go to File tab and click on 'Print current game to file'.\nTo empty the grid(so that you can enter your own grid of sudoku for solving or verifying), go to File tab and click on 'Empty the grid'.\nTo open a previously saved game, go to File tab and click on 'Open saved game'.\n If you are stuck at any point of the game, go to Help tab and click on 'Hint'. You will get one random empty box filled by the machine.\nIf you want the entire grid to be solved by the machine, go to Help tab and click on 'Solve the current game'\nTo exit the game, go to File tab and click on 'Exit'\n" );
        gtk_widget_show_all(howtoplay);
        gtk_dialog_run(GTK_DIALOG(howtoplay));//launch the dialog box
        gtk_widget_destroy(howtoplay);
    }	
	
    /*About developer*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"About the developer")==0){
        GtkWidget *about;
        about = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK, "Sudoku done by : Arya Gandhi\nMIS : 111803151\nDiv : 2" );
        gtk_widget_show_all(about);
        gtk_dialog_run(GTK_DIALOG(about));
        gtk_widget_destroy(about);
    }
    /* recover the choice of the level of the user vary the level value accordingly and start a new game with the selected level*/
    	else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Easy")==0) {
		int i,j;
          for (i=0;i<9;i++){
              	for (j=0;j<9;j++){
                  	grid[i][j]=0;
                    gtk_entry_set_editable (GTK_ENTRY(gridScreen[i][j]),TRUE);
                    gtk_entry_set_text(GTK_ENTRY(gridScreen[i][j]), "");
              	}
        	}
          copy_grid(grid,gridInitial);
    
    	   	level=1;
    	   	modeEdition=0;
        	generate(grid, level); // call generate grid by sending the level 1
        	copy_grid(grid,gridInitial);
        	synchroniserGridDisplay(0,1);
    	}
    	else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Medium")==0){ 
        	int i,j;
          for (i=0;i<9;i++){
          	for (j=0;j<9;j++){
               	grid[i][j]=0;
                    gtk_entry_set_editable (GTK_ENTRY(gridScreen[i][j]),TRUE);
                    gtk_entry_set_text(GTK_ENTRY(gridScreen[i][j]), "");
              	}
        	}
          copy_grid(grid,gridInitial);
            
    	   	level=2;
    	   	modeEdition=0;
        	generate(grid, level); // call generate grid by sending the level 2
        	copy_grid(grid,gridInitial);
        	synchroniserGridDisplay(0,1);	
		}
    	else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Difficult")==0) { 
     	int i,j;
          for (i=0;i<9;i++){
		     for (j=0;j<9;j++){
     	          grid[i][j]=0;
                    gtk_entry_set_editable (GTK_ENTRY(gridScreen[i][j]),TRUE);
                    gtk_entry_set_text(GTK_ENTRY(gridScreen[i][j]), "");
               }
          }
          copy_grid(grid,gridInitial);
    
		level=3;
    	   	modeEdition=0;
        	generate(grid, level); // call generate grid by sending level 3
        	copy_grid(grid,gridInitial);
        	synchroniserGridDisplay(0,1);	
	}
    /*Exit the program*/
    else if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"Exit")==0) 
    		gtk_main_quit();
}

/*Audit button management function*/
/*Operation: fill the grid with the elements displayed on the screen and then test if this new grid
is consistent with the sudoku rules by calling the function verify_valid. Then open a dialogue box depending on the result.*/
static void buttonVerify(GtkWidget* widget, gpointer data){
    modeEdition=0;
    GtkWidget *end;
    int finished, valid;

    synchroniserGridDisplay(1,0);//synchonize the grid with the display
    valid = verify_valid(grid); //test if the input is valid with the sudoku rules
    verify_finished(grid, &finished); //check if the grid is complete

    /*Depending on the situation, three messages are possible*/
    if (finished == 1 && valid == 1) 
    		end = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"\nCongratulations, the grid is completely solved!");
    else if ( valid == 1 ) 
    		end = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"\nThe sudoku is correct till here, continue");
    else 
    		end = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK, "\nThe current grid is wrong, try again!" );

    gtk_widget_show_all(end);
    gtk_dialog_run(GTK_DIALOG(end));
    gtk_widget_destroy(end);
}

/*This function allows the transfer of the numbers entered by the user in the widget * gridScreen[][] the grid[][] of integers that will be used in the algorithms
as well as the possibility of blocking the cells of the grid*/
void synchroniserGridDisplay(int direction, int blocking){  //it takes as parameter the direction of the conversion and the blocking of the boxes when it is necessary
    if(direction==1){ //send from display to grid
        int i,j;
        gchar *p;//on screen only characters can be displayed, so conversion is necessary
        for (i=0;i<9;i++){
            for (j=0;j<9;j++){
                int n;
                p = (char*) gtk_entry_get_text(GTK_ENTRY(gridScreen[i][j])); // retrieve in p the character string displayed
              	 n = atoi(p);//use atoi to convert it to an integer
                grid[i][j]=n; // insert the integer in the grid
            }
        }
    }
    else if (direction == 0){  //send from grid to display
        int i,j;
        char numchar[2];//on screen only characters can be displayed, so conversion is necessary
        for (i=0;i<9;i++){
            for (j=0;j<9;j++){
                gtk_entry_set_editable (GTK_ENTRY(gridScreen[i][j]),TRUE);//unlock
                snprintf(numchar, 2, "%i", grid[i][j]);//convert integers to characters
                if(grid[i][j]!=0)
                	gtk_entry_set_text(GTK_ENTRY(gridScreen[i][j]), numchar);//display all characters other than 0
                if((blocking==1) && (grid[i][j]!=0 ))
                	gtk_entry_set_editable (GTK_ENTRY(gridScreen[i][j]),FALSE);//all the filled boxes are blocked
            }
        }
    }

}
