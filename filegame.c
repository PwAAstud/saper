#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "minefild.h"
#include "scorebord.h"

int f_move(minefild* game, FILE* source){
    char comand;
    int x,y;
    while(1){
        if(fscanf(source, " %c", &comand) == EOF){
            return 1;
        }
        if(fscanf(source, " %d %d", &x, &y) < 2){
            printf("plik zawiera zly zapis %c; %d; %d; \n", comand, x, y);
            continue;
        }
        if( comand != 'f' && comand != 'r'){
            printf("plik zawiera nieznana akcje\n");
            continue;
        }
        if( x <= 0 || y <= 0 || x > game->x || y > game->y){
            printf("plik zawiera zle koordynaty x:%d, y:%d\n", x, y);
            continue;
        }
        break;
    }
    x--;
    y--;
    if(comand == 'r'){
        return minefild_open(game, x, y);
    }else if(comand == 'f'){
        minefild_flag(game, x, y);
        return 0;
    }
    return 1;
}

minefild* file_game(FILE* source){
    int poprawne_kroki = 0;
    minefild* game = minefild_for_file(source);

    int end_type= 0;
    int czy_k = 0;
    do{
        czy_k = f_move(game, source);
        end_type = minefild_check_board(game);
        if(end_type != 1){
            poprawne_kroki++;
        }
    } while (0 == end_type && czy_k == 0);

    if( end_type == 1){
        printf("liczba poprawnych krokow: %d\n liczba uzyskanych punktow: %d\n 0 - gra zakonczona niepwodzeniem\n", poprawne_kroki, game->score);
    }else if( end_type == 2){
        printf("liczba poprawnych krokow: %d\n liczba uzyskanych punktow: %d\n 1 - gra zakonczona pwodzeniem\n", poprawne_kroki, game->score);
    }
    return game;
}
