/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep;
 */
 
#define _XOPEN_SOURCE 500

#include "cs50.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }
    //if dimension == even...
    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        //fflush forces a write of all user-space buffered data for the
        //given output or update stream.
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    for(int i = 0 ; i < d; i++){
        for(int j = 0; j < d; j++){
            board[i][j] = d*d - (1+d*i + j);
            if(i == (d-1) && j == (d-1)){
                board[i][j] = d * d;
            }
        }
    }
    //if board is even, switch 1 and 2.
    if((d % 2) == 0){
        int temp =  board[d-1][d-2];
        board[d-1][d-2] = board[d-1][d-3];
        board[d-1][d-3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //Set background to red.
    printf("\033[31m");
    for(int i = 0 ; i < d; i++){
        for(int j = 0; j < d; j++){
            if(board[i][j]!=d*d){
                printf("%d\t", board[i][j]);
            } else {
//Control Sequence Introducer. in oct, \033, in hex, \0x1B..+[
                printf("_\t");
            }
        }
        printf("\n");
    }
    //Set to normal display.
    printf("\033[00m");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    //If move is not even in range, return false;
    int whereTile = 0;
    int whereSpace = 0;
    if(tile<0 || tile > (d*d - 1)){
        return false;
    } else {
        //find the tile
        for(int i = 0; i < d; i++){
            for(int j = 0; j < d; j++){
                if(board[i][j]==tile){
                    whereTile = i * d + j;
                }
                if(board[i][j]==d*d){
                    whereSpace = i * d + j;
                }
            }
        }
        //if the tile is adjacent to space, move.
        if(whereTile==(whereSpace-d) || whereTile==(whereSpace+d) 
            || whereTile==(whereSpace-1) || whereTile==(whereSpace+1)){
            int temp = board[whereTile/d][whereTile%d];
            board[whereTile/d][whereTile%d] = board[whereSpace/d][whereSpace%d];
            board[whereSpace/d][whereSpace%d] = temp;
            return true;
        } else {
            return false;
        }
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            if(board[i][j]!=(i*d + 1)){
                return false;
            }
        }
    }
    return true;
}
