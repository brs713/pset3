/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 *
 * For testing purposes, a solution to a 3 x 3 board is as follows:
 * 1 2   5 4 3   1 2 3   7 6   1 2 3   7 4 5 7 4 5   8 6   1 2 3 4 5
 * 8 6   1 2 3 4 5   8 6   7 8 6   4 5 6   8 7   4 5 6
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);
void swap(int *value1, int *value2);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(250000);
        }

        // sleep for animation's sake
        usleep(250000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    // esc [2J = erase display; goto 0,0
    printf("\033[2J");
    // esc [#;#H = cursor position - [line;columnH
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(250000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // make a set of values & store them – d*d -1,
    int populator = d * d - 1;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = populator;
            populator--;
        }
    }

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 2)
            {
                if ((d * d) % 2 == 0)
                {
                    swap(&board[i][j], &board[i][j + 1]);
                    break;
                }
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf(" _   ");
            }
            else
            {
                if (board[i][j] >= 10)
                {
                    printf("%i   ", board[i][j]);
                }
                else
                {
                    printf(" %i   ", board[i][j]);
                }
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
// Check values to find the board[][] value that matches the tile entered
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                if (board[i + 1][j] == 0)
                {
                    if ((i + 1) <= (d - 1))
                    {
                        board[i + 1][j] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                }
                if (board[i - 1][j] == 0)
                {
                    if ((i - 1) >= 0)
                    {
                        board[i - 1][j] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                }
                if (board[i][j + 1] == 0)
                {
                    if ((j + 1) <= (d - 1))
                    {
                        board[i][j + 1] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                }
                if (board[i][j - 1] == 0)
                {
                    if ((j - 1) >= 0)
                    {
                        board[i][j - 1] = tile;
                        board[i][j] = 0;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int mismatch = 0;
    int populator = 1;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (populator <= (d * d - 1))
            {
                if (board[i][j] == populator)
                {
                }
                else
                {
                    mismatch++;
                }
                if (mismatch != 0)
                {
                    return false;
                    break;
                }
            }
            populator++;
        }
    }
    if (mismatch == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Swaps two numbers
 */
void swap(int *value1, int *value2)
{
    int temp;
    temp = *value1;
    *value1 = *value2;
    *value2 = temp;
    return;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
