/**
 *   life.c
 *   2014, 2017 (c) Aaron Ceross
 *
 *  This is an implementation of Conway's Game of Life,
 *  https://en.wikipedia.org/wiki/Conway's_Game_of_Life.  I completed this as my
 *  first assignment in C programming, about 3 or 4 weeks from being a total
 *  novice. I would not necessarily go about designing the program like this now
 *  but am nontheless proud of my first attempts in programming.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>

#define BOARD_LIMIT 50
#define BOARD_MINUMUM 2
#define RANDOM 0.20 /* for 20% of the board to be filled with random inhabited cells */
#define MAX_ITERATION 5000
#define BIRTH_CONDITION 3
#define SURVIVE_CONDITION_1 3 /* includes the cell itself, hence +1 to the rule condition */
#define SURVIVE_CONDITION_2 4 /* includes the cell itself, hence +1 to the rule condition */
#define ANIMATE 1

int create_board (int user_size);
void set_board (int board[][BOARD_LIMIT], int board_size);
void randomise_board (int board[][BOARD_LIMIT], int board_size);
void print_board (int board[][BOARD_LIMIT], int board_size);
void life_or_death (int board[][BOARD_LIMIT], int next_board[][BOARD_LIMIT], int k, int j, int board_size); 
int count_neighbours (int board[][BOARD_LIMIT], int i, int j, int board_size);
void update_board (int board[][BOARD_LIMIT], int next_board[][BOARD_LIMIT], int k, int j, int board_size);
int check_if_equal(int board[][BOARD_LIMIT], int next_board[][BOARD_LIMIT], int board_size); 

/* main program */
int
main(void)
{
        int i, user_size, k=0, j=0, count=1;
        int *board_size = &user_size;
        int board[BOARD_LIMIT][BOARD_LIMIT];
        int next_board[BOARD_LIMIT][BOARD_LIMIT];

        user_size = create_board(user_size); /* updates board size based on user input */
        set_board(board, *board_size); /* initialise current board */
        randomise_board(board, *board_size);
        set_board(next_board, *board_size); /* initialise next board to zero */
        printf("\nIntial board:\n");
        print_board(board, *board_size);
        printf("\nThe Game of Life begins:\n");

        for (i = 0; i < MAX_ITERATION; i++) {
                life_or_death (board, next_board, k, j, *board_size); /* update next board */
                if (check_if_equal(board, next_board, *board_size) == 1) {
                        i = MAX_ITERATION;
                } else {
                        printf("\nIteration number: %d\n", count);
                        print_board(next_board, *board_size);
                        update_board(board, next_board, k, j, *board_size);     /* updates current board */
                        count++;
                }
        }

        printf("The Game of Life has ended.\n");

        return 0;
}

/* checks if the boards are equal */
int
check_if_equal(int board[][BOARD_LIMIT], int next_board[][BOARD_LIMIT], int board_size)
{
        int k, j;
  
        for (k = 0; k < board_size; k++) {
                for (j = 0; j < board_size; j++) {
                        if (board[k][j] != next_board[k][j]) {
                                return 0;
                        }
                }
        }
        return (1);
}

/* user-defined board */
int
create_board(int board_size)
{
        printf("Welcome to my implementation of Conway's Game of Life\n");
        printf("Please ensure you are running this terminal at full screen for best results!\n");
        printf("You can have any size from 2 to 50. How large would you like the board to be? \n");

        if (scanf("%d", &board_size) != 1) {
                printf("There has been an error with your input. Please insert an integer. Try again.\n");
        } else if (board_size > BOARD_LIMIT || board_size < BOARD_MINUMUM) {
                printf("That is out of bounds for our purposes.\n");
        } else {
                printf("The board size is now %d\n", board_size);
        }

        return board_size;
}

/* sets the values of the board to zero - clears junk values */
void
set_board (int board[][BOARD_LIMIT], int board_size)
{
        int k, j;

        for (k = 0; k < board_size; k++) {
                for (j = 0; j < board_size; j++) {
                        board[k][j] = 0;
                  }
        }
}

/* randomise board */
void
randomise_board(int board[][BOARD_LIMIT], int board_size)
{
        int k, i, j;
  
        srand((unsigned)time(NULL)); /* set the random seed */

        for (k = 0; k < (board_size * board_size * RANDOM); k++) {
                i = rand() % (board_size);
                j = rand() % (board_size);
                board[i][j] = 1;
        }
}

/* prints the board */
void
print_board(int board[][BOARD_LIMIT], int board_size)
{
        int i, j;

        for (i = 0; i < board_size; i++) {
                for (j = 0; j < board_size; j++) {
                        if (board[i][j] == 1) {
                                putchar ('@');
                        } else {
                                putchar(' ');
                        }
                }
                printf("\n");
        }

#if ANIMATE == 1
        sleep(1);
#endif
}

/* counts the neighbours of a given cell */
/* see Note at Line 237 for diagram of index table */
int
count_neighbours(int board[][BOARD_LIMIT], int k, int j, int board_size)
{
        int row_indx, column_indx, value, neighbour_count;
        value = 0;
        neighbour_count = 0; /* reset count for new cell */

        for (row_indx = -1; row_indx <= 1; row_indx++) {
                for (column_indx = -1; column_indx <= 1; column_indx++) {
                        if ((k + row_indx >= 0) &&
                            (j + column_indx >= 0) &&
                            (k + row_indx < board_size) &&
                            (j + column_indx < board_size)) {
                                /* assign the cell value (0 or 1) to variable value */
                                value = board[k + row_indx][j + column_indx];
                                  /* value is determined base on the cell.
                                     if there is a neighbour cell,
                                     value =1, else value =0
                                  */
                                if (value == 1) {
                                        neighbour_count++;
                                }
                        }
                }
        }

        return neighbour_count;
}

/* perform life or death rules */
/* If the cell is inhabited and has exactly 2 or 3 neighbours - Survive (cell remains inhabited)
 * If the cell is inhabited and has fewer than 2 or more than 3 neighbours - Death (cell becomes uninhabited)
 * If the cell is uninhabited has has exactly 3 neighbours - Birth (cell becomes inhabited) */

void
life_or_death(int board[][BOARD_LIMIT], int next_board[][BOARD_LIMIT], int k, int j, int board_size)
{
        int neighbour_count;

        set_board(next_board, board_size);

        for (k = 0; k < board_size; k++) {
                for (j = 0; j < board_size; j++) {
                        neighbour_count = count_neighbours(board, k, j, board_size);
                        if (board[k][j] == 0) {
                                /* cell is uninhabited, check if conditions
                                   for birth are met
                                */
                                if (neighbour_count == BIRTH_CONDITION) {
                                        next_board[k][j] = 1;   /* birth */
                                } else {
                                        next_board[k][j] =0;
                                }
                        } else if (board[k][j] == 1) {
                                /* cell exists, check if it survies or dies */
                                if (neighbour_count == SURVIVE_CONDITION_1 ||
                                    neighbour_count == SURVIVE_CONDITION_2) {
                                        /* survival */
                                        next_board[k][j] = board[k][j];
                                } else {
                                        /* death */
                                        next_board[k][j] = 0;
                                }
                        }
                }
        }
        printf("\n");
}

/* updates the board for the next iteration */
void
update_board(int board[][BOARD_LIMIT], int next_board[][BOARD_LIMIT], int k, int j, int board_size)
{
        for (k = 0; k < board_size; k++) {
                for (j = 0; j < board_size; j++) {
                        board[k][j] = next_board[k][j];
                }
        }
}
