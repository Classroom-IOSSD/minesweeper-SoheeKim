#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
#define MAX 10

// background color
#define BGNRM  "\x1B[0m"
#define BGRED  "\x1B[41m"
#define BGGRN  "\x1B[42m"
#define BGYEL  "\x1B[43m"
#define BGBLU  "\x1B[44m"
#define BGMAG  "\x1B[45m"
#define BGCYN  "\x1B[46m"
#define BGWHT  "\x1B[47m"
// text color
#define TEXTRED  "\x1B[31m"
#define TEXTGRN  "\x1B[32m"
#define TEXTYEL  "\x1B[33m"
#define TEXTBLU  "\x1B[34m"
#define TEXTMAG  "\x1B[35m"
#define TEXTCYN  "\x1B[36m"
#define TEXTWHT  "\x1B[37m"


// global variables
// game table
unsigned char table_array[MAX][MAX];
// location of cursor
int xcoord=0, ycoord=0;
// flag: input mode = 0, flag mode = 1, check mode = 2
int game_mode=0;

// to prevent negative index and out of bounds
bool isValid(int index) {
	bool valid = (rows[index] >= 0 && rows[index] < MAX) && (columns[index] >= 0 && columns[index] < MAX);
	return valid;
}

/*This is a recursive function which uncovers blank cells while they are adjacent*/
int uncover_blank_cell(int row, int col) {
    int value, rows[8], columns[8], i;

    bool isError = table_array[row][col] != 0;
    if(isError)
        return 0; // error

    table_array[row][col] += 10; // uncover current cell

    // Get position of adjacent cells of current cell
    rows[0] = row - 1;
    columns[0] = col + 1;
    rows[1] = row;
    columns[1] = col + 1;
    rows[2] = row + 1;
    columns[2] = col + 1;
    rows[3] = row - 1;
    columns[3] = col;
    rows[4] = row + 1;
    columns[4] = col;
    rows[5] = row - 1;
    columns[5] = col - 1;
    rows[6] = row;
    columns[6] = col - 1;
    rows[7] = row + 1;
    columns[7] = col - 1;

    for(i = 0; i < 8; i++) {
        value = table_array[rows[i]][columns[i]];

        if( isValid(i) ) {	// to prevent negative index and out of bounds
            if(value > 0 && value <= 8)
                table_array[rows[i]][columns[i]] += 10;										// it is a cell with 1-8 number so we need to uncover
            else if(value == 0)
                uncover_blank_cell(rows[i], columns[i]);
        }

    }

    return 1; // success!
}

void print_table() {
    // clear screen
    system("clear");

    int i, j, value;
    for(i = 0; i < MAX; i++) {
        for(j = 0; j < MAX; j++) {
            if(xcoord == j && ycoord == i) {
                if(game_mode == 1) {
                    printf("|%sF%s",BGMAG,BGNRM);
                    continue;
                } else if(game_mode == 2) {
                    printf("|%sC%s",BGMAG,BGNRM);
                    continue;
                }

            }
            value = table_array[i][j];

            if((value >= 0 && value <= 8) || value == 0 || value == 99)
                printf("|X");
            else if(value == 10) // clean area
                printf("|%s%d%s",TEXTCYN, value - 10,BGNRM);
            else if(value == 11) // the number of near mine is 1
                printf("|%s%d%s",TEXTYEL, value - 10,BGNRM);
            else if(value > 11 && value <= 18) // the number of near mine is greater than 1
                printf("|%s%d%s",TEXTRED, value - 10,BGNRM);
            else if((value >= 20 && value <= 28) || value == 100)
                printf("|%sF%s",TEXTGRN,BGNRM);
            else
                printf("ERROR"); // test purposes

        }
        printf("|\n");
    }

    printf("cell values: 'X' unknown, '%s0%s' no mines close, '1-8' number of near mines, '%sF%s' flag in cell\n",TEXTCYN,BGNRM,TEXTGRN,BGNRM);
    if(game_mode == 0) {
        printf("f (put/remove Flag in cell), c (Check cell), n (New game), q (Exit game): ");
    } else if(game_mode == 1) {
        printf("Enter (select to put/remove Flag in cell), q (Exit selection): ");
    } else if(game_mode == 2) {
        printf("Enter (select to check cell), q (Exit selection): ");
    }


}


int main(int argc, char *argv[]) {

    char echo_mode;
    int Num_RemainMines; // the number of the remaining mines
    int i,j,r,c,value, rows[8], columns[8];

new_game:
    // the number of mines
    Num_RemainMines = 10;
    if(argc == 2) {
        Num_RemainMines = atoi(argv[1]);
    }
    srand (time(NULL));						// random seed
    // setting cursor
    xcoord = 0;
    ycoord = 0;
    // set all cells to 0
    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
            table_array[i][j] = 0;

    for(i = 0; i < Num_RemainMines; i++) {
        /* initialize random seed: */

        r = rand() % 10;					// it generates a integer in the range 0 to 9
        c = rand() % 10;

        // put mines
        if(table_array[r][c] != 99) {
            table_array[r][c] = 99;

            // Get position of adjacent cells of current cell
            rows[0] = r - 1;
            columns[0] = c + 1;
            rows[1] = r;
            columns[1] = c + 1;
            rows[2] = r + 1;
            columns[2] = c + 1;
            rows[3] = r - 1;
            columns[3] = c;
            rows[4] = r + 1;
            columns[4] = c;
            rows[5] = r - 1;
            columns[5] = c - 1;
            rows[6] = r;
            columns[6] = c - 1;
            rows[7] = r + 1;
            columns[7] = c - 1;

            for(j = 0; j < 8; j++) {
                value = table_array[rows[j]][columns[j]];
                if(isValid(j)) {	// to prevent negative index and out of bounds
                    if(value != 99)																// to prevent remove mines
                        table_array[rows[j]][columns[j]] += 1;									// sums 1 to each adjacent cell
                }
            }

        } else {							// to make sure that there are the properly number of mines in table
            i--;
            continue;
        }
    }

    //
    while(Num_RemainMines != 0) {			// when nMines becomes 0 you will win the game
        print_table();

        echo_mode = getch();
        // cursor direction
        char CursorDirection;
        switch (echo_mode) {

        // flag
        case 'f':
        case 'F':


flag_mode:
            game_mode = 1;
            do {
                print_table();
                CursorDirection = getch();
                // arrow direction
                if(CursorDirection == '8') {
                    // up
                    ycoord = (MAX + --ycoord) % MAX;
                } else if(CursorDirection == '2') {
                    // down
                    ycoord = ++ycoord % MAX;
                } else if(CursorDirection == '4') {
                    xcoord = (MAX + --xcoord) % MAX;
                } else if(CursorDirection == '6') {
                    xcoord = ++xcoord % MAX;
                } else if(CursorDirection == 'c' || CursorDirection == 'C') {
                    goto check_mode;
                } else if(CursorDirection == '\n') {
                    value = table_array[ycoord][xcoord];

                    if (value == 99) {				// mine case
                        table_array[ycoord][xcoord] += 1;
                        Num_RemainMines -= 1;				// mine found
                    } else if(value >= 0 && value <= 8) {	// number of mines case (the next cell is a mine)
                        table_array[ycoord][xcoord] += 20;
                    } else if(value >= 20 && value <= 28) {
                        table_array[ycoord][xcoord] -= 20;
                    }

                    if(Num_RemianMines == 0)
                        break;
                }
            } while (CursorDirection != 'q' && CursorDirection != 'Q');
            game_mode = 0;

            break;

        // check cell
        case 'c':
        case 'C':

check_mode:
            game_mode = 2;
            do {
                print_table();
                CursorDirection = getch();

                // arrow direction
                if(direction == '8') {
                    // up
                    ycoord = (MAX + --ycoord) % MAX;
                } else if(CursorDirection == '2') {
                    // down
                    ycoord = ++ycoord % MAX;
                } else if(CursorDirection == '4') {
                    xcoord = (MAX + --xcoord) % MAX;
                } else if(CursorDirection == '6') {
                    xcoord = ++xcoord % MAX;
                } else if(CursorDirection == 'f' || CursorDirection == 'F') {
                    goto flag_mode;
                }

                else if(CursorDirection == '\n') {
                    value = table_array[ycoord][xcoord];
                    if(value == 0)						// blank case
                        uncover_blank_cell(ycoord, xcoord);
                    else if(value == 99)				// mine case
                        goto end_of_game;
                    else if(value > 0 && value <= 8)	// number case (the next cell is a mine)
                        table_array[ycoord][xcoord] += 10;

                    //	break;
                }
            } while (CursorDirection != 'q' && CursorDirection != 'Q');
            game_mode = 0;

            break;



        // jump to a new game
        case 'n':
        case 'N':
            goto new_game;
            break;

        // exit
        case 'q':
        case 'Q':
            goto end_of_game;

        default:
            break;
        }
    }

end_of_game:
    game_mode = 0;
    print_table();
    printf("\nGAME OVER\n");

    if(Num_RemainMines == 0)
        printf("you won!!!!\n");

    else
        printf("BOOM! you LOOSE!\n");

    do {
        printf("Are you sure to exit? (y or n)? ");
        echo_mode = getch();
        putchar('\n');
        if(echo_mode == 'y' || echo_mode == 'Y') {
            break;
        } else if(echo_mode == 'n' || echo_mode == 'N') {
            goto new_game;
        }
        printf("Please answer y or n\n");
    } while(1);
    printf("See you next time!\n");

    fflush(stdin);

    return 0;
}
