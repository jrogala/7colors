/* Template of the 7 wonders of the world of the 7 colors assigment. */

/* Cf. http://people.irisa.fr/Anne-Cecile.Orgerie/teaching2015.html  */

#include <stdio.h>  /* printf */
#include <stdlib.h> /* random */
#include <time.h> /* time */

/* We want a 30x30 board game by default */
#define BOARD_SIZE 5
#define NUMBER_OF_COLOR 7

/* fonctions auxilliaires */
void assert(int check, int line){
    if (!check){
        printf("error at line %i\n", line);
        exit(1);
    }
}

/** Represent the actual current board game
 *
 * NOTE: global variables are usually discouraged (plus encapsulation in
 *  an appropriate data structure would also be preferred), but don't worry.
 *  For this first assignment, no dinosaure will get you if you do that.
 */
char board[BOARD_SIZE * BOARD_SIZE] = { 0 }; // Filled with zeros

/** Retrieves the color of a given board cell */
char get_cell(int x, int y) // O(1)
{
   return board[y*BOARD_SIZE + x];
}

/** Changes the color of a given board cell */
void set_cell(int x, int y, char color) // O(1)
{
   board[y*BOARD_SIZE + x] = color;
}

/** Prints the board in terminal*/
void print_board() // O(BOARD_SIZE²)
{
   int i, j;
   system("clear");
   for (i=0; i<BOARD_SIZE; i++) {
      for (j=0; j<BOARD_SIZE; j++)
	    printf("%i ", get_cell(i, j)); // à modifier pour rendre compatible avec des chars
          printf("\n");
   }
}

int random_color(){ // Choose a random color != than 0 or 1
  return ((rand()%NUMBER_OF_COLOR)+2);
}

/** Creates the board and fills it with random colors*/
void initgame(){// Create board, O(BOARD_SIZE²)
  int buff; // Random buffer
  int i,j;
  for (i=0;i<BOARD_SIZE;i++){
    for (j=0;j<BOARD_SIZE;j++){
      buff = random_color();
      set_cell(i,j,buff);
    }
  }
  set_cell(0,BOARD_SIZE-1,0);
  set_cell(BOARD_SIZE-1,0,1);
}

/**allows a player to make a move*/
void make_move(int joueur, int color){// Make a shot, good complexity
    assert(joueur <= 1, __LINE__); // Never suppose to have this error
    assert(color >= 2 && (color <= NUMBER_OF_COLOR + 2), __LINE__); // this one too
    char board_temp[BOARD_SIZE * BOARD_SIZE] = { 0 }; // this made a o(BOARD_SIZE²) spacial complexity
    void move_step(int x, int y){
        if (x >= 0 && x <= BOARD_SIZE-1 && y>=0 && y <= BOARD_SIZE-1 && (get_cell(x,y) == joueur || get_cell(x,y) == color) && board_temp[y*BOARD_SIZE + x] == 0){
            set_cell(x,y,joueur);
            board_temp[y*BOARD_SIZE + x] = 1; // Marker to avoid infinite loop
            move_step(x-1,y);
            move_step(x,y-1);
            move_step(x+1,y);
            move_step(x,y+1);
        }
    }
    if (joueur == 0)
        move_step(0,BOARD_SIZE-1);
    else
        move_step(BOARD_SIZE-1,0);
}

/** implements the strategy of a human player by taking their input*/
char player(){ // Have to check that return is correct
    int color;
    do { // loop to get the color
    	printf("Choisi ta couleur entre 2 et 9\n"); // Hard Coded
    	scanf("%d",&color);
    } while (color < 2 || color > NUMBER_OF_COLOR +2);
    return color;
}

/** Implements a random strategy for the computer*/
char ia_verybad(){ // Simple random choice (very stupid)
    return random_color();
}

/** Implements a greedy strategy for the computer*/
char ia_average(int player){
	int colors_best[NUMBER_OF_COLOR +2] = { 0 };
	char board_temp[BOARD_SIZE * BOARD_SIZE] = { 0 };
  unsigned i;
	void move_step(int x, int y){
		void check_color(char color,int x,int y){
			if (x>= 0 && x <= BOARD_SIZE-1 && y>= 0 && y<= BOARD_SIZE-1 && board_temp[y*BOARD_SIZE+x] == 0 && get_cell(x,y) == color){
				board_temp[y*BOARD_SIZE +x] = 1;
				colors_best[(int)(get_cell(x,y))] +=1;
				check_color(color,x-1,y);
				check_color(color,x,y-1);
				check_color(color,x+1,y);
				check_color(color,x,y+1);
			}
		}
		if ( x >= 0 && x <= BOARD_SIZE -1 && y >= 0 && y <= BOARD_SIZE-1 && board_temp[y*BOARD_SIZE + x] == 0){
			if (get_cell(x,y) == player){
				board_temp[y*BOARD_SIZE+x] = 1;
				move_step(x-1,y);
				move_step(x,y-1);
				move_step(x+1,y);
				move_step(x,y+1);
			}
			else {
				check_color(get_cell(x,y),x,y);
			}
		}
	}
	if (player == 0){
		move_step(0,BOARD_SIZE -1);
	}
	else {
		move_step(BOARD_SIZE -1,0);
	}
	char color = 2;
    	int m = 0;
    	for (i=2;i<NUMBER_OF_COLOR +2;i++){
        	if (colors_best[i] > m){
            color = i;
            m = colors_best[i];
        	}
    	}
    return color;
}

/** checks if the situation meets the conditions to end the game*/
char gamenotend(){ // check if the game is over, O(BOARD_SIZE²)
    int nombre_de_case = BOARD_SIZE * BOARD_SIZE;
    int case_joueur0 = 0;
    int case_joueur1 = 0;
    int i,j;
    for (i=0;i<BOARD_SIZE;i++){
        for(j=0;j<BOARD_SIZE;j++){
            if (get_cell(i,j) == 0)
                case_joueur0 += 1;
            if (get_cell(i,j) == 1)
                case_joueur1 += 1;
        }
    }
    if (case_joueur0>= nombre_de_case/2){
        printf("joueur 0 a gagné");
        return 0;
    }
    if (case_joueur1>= nombre_de_case/2){
        printf("joueur 1 a gagné");
        return 0;
    }
    return 1;

}

/** Program entry point */
int main()
{
    srand(time(NULL)); // seed initialisation
    char col = 0;
    char turn_player;
    initgame();
    printf("\n\n  Welcome to the 7 wonders of the world of the 7 colors\n"
	      "  *****************************************************\n\n"
	"Current board state:\n");
    print_board();
    printf("Joueur 0 commence\n"); // hard coded player 0 start (don't blame me please)
    for(turn_player = 0;gamenotend(); turn_player = 1-turn_player){ // While game is not over
        if (turn_player == 0)
            col = player(); // first player
        else
            col = ia_average(1); // other player
        make_move(turn_player,col); // make the play
        print_board();
    }
    return 0; // Everything went well
}
