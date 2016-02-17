/* Template of the 7 wonders of the world of the 7 colors assigment. */

/* Cf. http://people.irisa.fr/Anne-Cecile.Orgerie/teaching2015.html  */

#include <stdio.h>  /* printf */
#include <stdlib.h> /* random */
#include <time.h> /* time */

/* We want a 30x30 board game by default */
#define BOARD_SIZE 30
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
char get_cell(int x, int y) 
{
   return board[y*BOARD_SIZE + x];
}

/** Changes the color of a given board cell */
void set_cell(int x, int y, char color) 
{
   board[y*BOARD_SIZE + x] = color;
}
   
/** Prints the current state of the board on screen
 * 
 * Implementation note: It would be nicer to do this with ncurse or even 
 * SDL/allegro, but this is not really the purpose of this assignment.
 */
void print_board() 
{
   int i, j;
   for (i=0; i<BOARD_SIZE; i++) {
      for (j=0; j<BOARD_SIZE; j++) 
	    printf("%i ", get_cell(i, j)); // à modifier pour rendre compatible avec des chars
          printf("\n");
   }
}

int random_color(){
  return ((rand()%NUMBER_OF_COLOR)+2);
}

void initgame(){
  int buff; // On crée un buffer pour l'aléatoire
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

void coup(int joueur, int color){
    assert(joueur <= 1, __LINE__);
    assert(color >= 2 && (color <= NUMBER_OF_COLOR + 2), __LINE__);
    char board_temp[BOARD_SIZE * BOARD_SIZE] = { 0 };
    void step_coup(int x, int y){
        if (x >= 0 && x <= BOARD_SIZE-1 && y>=0 && y <= BOARD_SIZE-1 && (get_cell(x,y) == joueur || get_cell(x,y) == color) && board_temp[y*BOARD_SIZE + x] == 0){
            set_cell(x,y,joueur);
            board_temp[y*BOARD_SIZE + x] = 1; // on indique qu'on a deja visité la case
            step_coup(x-1,y);
            step_coup(x,y-1);
            step_coup(x+1,y);
            step_coup(x,y+1);
        }
    }
    if (joueur == 0)
        step_coup(0,BOARD_SIZE-1);
    else
        step_coup(BOARD_SIZE-1,0);
}

char player(){ // doit verifier la couleur
    int color;
    printf("Choisi ta couleur entre 2 et 9\n");
    scanf("%d",&color);
    assert(color >=2 && color <= 9,__LINE__); // assert temporaire, à modifier avec une boucle qui attend une vrai couleur
    return color;
}

char ia_aleatoire(){
    return random_color();
}
   

char gamenotend(){
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
    srand(time(NULL)); // initialise le seed
    char col = 0;
    char turn_player;
    initgame();
    printf("\n\n  Welcome to the 7 wonders of the world of the 7 colors\n"
	      "  *****************************************************\n\n"
	"Current board state:\n");
   
    print_board();
    printf("Joueur 0 commence\n");
    for(turn_player = 0;gamenotend(); turn_player = 1-turn_player){
        if (turn_player == 0)
            col = ia_aleatoire(); // on sait que coup est une couleur valide
        else
            col = ia_aleatoire();
        coup(turn_player,col);
        system("clear");
        print_board();
    }
    return 0; // Everything went well
}
