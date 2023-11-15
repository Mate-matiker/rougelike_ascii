#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum DEBUG_MODE{
    NO_DEBUG = 0,
    GAME_DEBUG = 1,
    PLAYER_DEBUG = 2
}debug_mode;

int term_height, term_width;                    // Terminal Spalten, Linien

const int game_height = 10, game_width = 25;    // Groesse der angezeigten Spielwelt
int game_off_y = 5, game_off_x = 30;            // Offset von angezeigter Spielwelt relativ zum Terminal
bool game_printable = FALSE;
int game[game_height][game_width];              // 

int player_y = 0, player_y = 0;

/* Spielwelt auf der Konsole ausgeben */
void print_game();

int main(void){

    // NCURSES Einrichten
    initscr();              // initialisierung von ncurses
    start_color();
    cbreak();               //raw(); Tastatureingabe ungepuffert lesen
    keypad(stdscr, TRUE);   // Aktiviert Funktions- und Pfeiltasten
    noecho();               // keine Ausgabe der Tateneingabe 
    curs_set(0);            // curser unsichtbar machen
    nodelay(stdscr, TRUE);  // getch() liefert -1 bei keiner Tasteneingabe

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    int key_utf8;  // Tastatureingabe
    bool end_prog = FALSE, req_clear = FALSE;
    debug_mode = NO_DEBUG;

    // CLEAN
    // Datei lesen einrichten 
    FILE *fp;
    if((fp = fopen("/home/emle/Dokumente/Programms/C/rougeLike/level.txt","r")) == NULL){
        printf("Fehler beim oeffnen von level.txt!\n");
        return -1;
    }
    int c = 

    char myStr[100];
    char myc[2] = {' ', '\0'};
    int i = 0;
    myc[0] = fgetc(fp);
    while(myc[0] != EOF){   
        if(myc[0] != '\n'){
            strcat(myStr, myc);
        }else{
            mvprintw(i++,0,myStr);
            myStr[0] = '\0';
        }
        myc[0] = fgetc(fp);
    }
    mvprintw(i++,0,myStr);
    fclose(fp);
    // UP

    // Main-loop
    while(!end_prog){
        // Tastaturabfrage
        key_utf8 = getch();
        switch(key_utf8){
            case ERR:       // ERR > keine Tastatureingabe... 10ms warten um CPU zu entlasten.
                napms(10);
                break;
            case 'q':
                end_prog = TRUE;
                break;
            case KEY_LEFT:
                game_off_x = (game_off_x > 0) ? game_off_x - 1 : game_off_x;
                req_clear = TRUE;
                break;
            case KEY_RIGHT:
                ++game_off_x;
                req_clear = TRUE;
                break;
            case KEY_DOWN:
                ++game_off_y;
                req_clear = TRUE;
                break;
            case KEY_UP:
                game_off_y = (game_off_y > 0) ? game_off_y - 1 : game_off_y;
                req_clear = TRUE;
                break;
            case 'a':
                player_y = (player_y > 0) ? player_y - 1 : player_y;
                break;
            case 'd':
                player_y = (player_y < game_width - 1) ? player_y + 1 : player_y;
                break;
            case 'w':
                player_y = (player_y > 0) ? player_y - 1 : player_y;
                break;
            case 's':
                player_y = (player_y < game_height - 1) ? player_y + 1 : player_y;
                break;
            case 0xC3:
                debug_mode = (debug_mode + 1) % 3;
                req_clear = TRUE;
                break;
        }

        // Programm beenden
        if(end_prog) break;

        // Konsoleninformationen Abfragen
        getmaxyx(stdscr, term_height, term_width);
        game_printable = (term_height >= (game_height + game_off_y)) && (term_width >= game_width + game_off_x);

        // Konsole Säubern
        if(req_clear){
            clear();
            req_clear = FALSE;
        }

        // screen Zeichnen
        if(game_printable){
            print_game();
        }

        // Debuging Zeichnen
        switch(debug_mode){
            case GAME_DEBUG:
                mvprintw(0,0, "term_height[%d]", term_height);
                mvprintw(1,0, "term_width[%d]", term_width);
                mvprintw(2,0, "game_off_y[%d]", game_off_y);
                mvprintw(3,0, "game_off_x[%d]", game_off_x);
                mvprintw(4,0, "game_printable[%d]", game_printable);
                break;
            case PLAYER_DEBUG:
                mvprintw(0,0, "game_off_y[%d]", game_off_y);
                mvprintw(1,0, "game_off_x[%d]", game_off_x);
                mvprintw(2,0, "player_y[%d]", player_y);
                mvprintw(3,0, "player_y[%d]", player_y);
                break;
        }

    
    }
    getch();

    return 0; 
}

void print_game(){
    int glob_y, glob_x;
    int utf8_char;
    for(int x = 0; x < game_width; ++x){
        glob_x = game_off_x + x;
        for(int y = 0; y < game_height; ++y){
            glob_y = game_off_y + y;
            move(glob_y, glob_x);
            if((x == player_y) && (y == player_y)){
                utf8_char = '@';
            }else{
                utf8_char = '#';
            }
            addch(utf8_char);
        }
    }
}