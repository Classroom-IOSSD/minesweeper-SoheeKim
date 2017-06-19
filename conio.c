#include "conio.h"

static struct termios old_term_ios, new_term_ios;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
    tcgetattr(0, &old_term_ios); /* grab old terminal i/o settings */
    new_term_ios = old_term_ios; /* make new settings same as old settings */
    new_term_ios.c_lflag &= ~ICANON; /* disable buffered i/o */
    new_term_ios.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new_term_ios); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old_term_ios);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
    char echo_mode;
    initTermios(echo);
    echo_mode = getchar();
    if(echo_mode == '\x1b') {
        if(getchar()=='[') {
            switch(getchar()) {
            case 'A':
                echo_mode = '8';
                break;
            case 'B':
                echo_mode = '2';
                break;
            case 'C':
                echo_mode = '6';
                break;
            case 'D':
                echo_mode = '4';
                break;
            }
        }
    }
    resetTermios();
    return echo_mode;
}

/* Read 1 character without echo */
char getch(void) {
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void) {
    return getch_(1);
}

