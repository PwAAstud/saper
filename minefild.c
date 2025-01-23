#include<stdlib.h>
#include<stdio.h>
#include "minefild.h"
#include "steps.h"

#define MINE 9

minefild * minefild_innit(int size_x, int size_y, int mines_amount, int multi){
    minefild* retVal = malloc(sizeof(minefild));
    retVal->x = size_x;
    retVal->y = size_y;
    retVal->multi = multi;
    retVal->score = 0;

    retVal->cover = malloc(sizeof(int)*size_x*size_y);
    retVal->mines = malloc(sizeof(int)*size_x*size_y);
    for(int i=0; i<size_x*size_y; i++){
        retVal->cover[i] = 1;
        retVal->mines[i] = 0;
    }

    int rand_pos;
    for(int m=0; m<mines_amount;m++){
        rand_pos = rand()%(size_x*size_y);
        if(retVal->mines[rand_pos] == MINE){
            m--;
        }
        retVal->mines[rand_pos] = MINE;
    }

    return retVal;
}

minefild* minefild_for_file(FILE* sorse){
    int x,y;
    if( fscanf(sorse, "%d %d", &x, &y) < 2 ){
        return NULL;
    }
    minefild* retVal = minefild_innit(x,y,0,1);
    
    char c;
    for(int i=0;i<x*y;i++){
        c = fgetc(sorse);
        if(c == EOF){
            return NULL;
        }
        if(c=='\n'){
            i--;
            continue;
        }
        if(c=='m'){
            retVal->mines[i] = MINE;
        }
    }
    return retVal;
}

void minefild_free(minefild* to_free){
    free(to_free->cover);
    free(to_free->mines);
    free(to_free);
}

int minefild_cord_to_ind(minefild* base, int x, int y){
    return base->y*x + y;
};

void minefild_print(minefild* to_print){
    int pos;
    int x_copy = to_print->x;
    int y_copy = to_print->y;
    int x_with = 1;
    while (x_copy/=10){
        x_with++;
    }
    int y_with = 1;
    while (y_copy/=10){
        y_with++;
    }

    printf("score: %d\n", to_print->score);
    printf("\x1b[90m");
    printf("%*c", x_with, ' ');
    for(int i=1; i<=to_print->y;i++){
        printf("%*d", y_with+1, i);
    }
    printf("\x1b[97m\n");

    for(int x=0; x<to_print->x;x++){
        printf("\x1b[90m%*d\x1b[97m", x_with, x+1);
        for(int y=0; y<to_print->y;y++){
            printf("%*c", y_with, ' ');
            pos = minefild_cord_to_ind(to_print, x, y);
            // if (to_print->mines[pos]==MINE){
            //     printf("M");}else
            if (to_print->cover[pos]==1){
                printf("#");
            }else if (to_print->cover[pos]==2){
                printf("\x1b[91mF\x1b[97m");
            }else if (to_print->mines[pos]==MINE){
                printf("M");
            }else if ( to_print->mines[pos]==0 ){
                printf(".");
            }else{
                printf("\x1b[32m%d\x1b[97m",to_print->mines[pos]);
            }
        }
        printf("\n");
    }
}

void minefild_end_print(minefild* to_print){
    int pos;
    int x_copy = to_print->x;
    int y_copy = to_print->y;
    int x_with = 1;
    while (x_copy/=10){
        x_with++;
    }
    int y_with = 1;
    while (y_copy/=10){
        y_with++;
    }

    printf("score: %d\n", to_print->score);
    printf("\x1b[90m");
    printf("%*c", x_with, ' ');
    for(int i=1; i<=to_print->y;i++){
        printf("%*d", y_with+1, i);
    }
    printf("\x1b[97m\n");

    for(int x=0; x<to_print->x;x++){
        printf("\x1b[90m%*d\x1b[97m", x_with, x+1);
        for(int y=0; y<to_print->y;y++){
            printf("%*c", y_with, ' ');
            pos = minefild_cord_to_ind(to_print, x, y);
            if (to_print->mines[pos]==MINE){
                if(to_print->cover[pos] == 0){
                    printf("\x1b[101m");
                }
                printf("M");
                printf("\x1b[40m");
            }else if (to_print->cover[pos]==2){
                printf("\x1b[91mF\x1b[97m");
            }else if (to_print->cover[pos]==1){
                printf("#");
            }else if ( to_print->mines[pos]==0 ){
                printf(".");
            }else{
                printf("\x1b[32m%d\x1b[97m",to_print->mines[pos]);
            }
        }
        printf("\n");
    }
}

// 0 - safe; 1 - mines
int minefild_open(minefild* play, int x, int y){
    int pos = minefild_cord_to_ind(play, x, y);
    if(play->cover[pos] != 1){
        return 0;
    }
    play->cover[pos] = 0;
    if(play->mines[pos] == MINE){
        return 1;
    }

    play->score += play->multi;
    
    int pola_around[8][2];
    int pola_n = 0;
    for(int move_x= -1; move_x < 2; move_x++){
        for(int move_y= -1; move_y < 2; move_y++){
            if(move_x==0 && move_y==0){
                continue;
            }
            if(x+move_x < 0 || x+move_x >= play->x){
                continue;
            }
            if(y+move_y < 0 || y+move_y >= play->y){
                continue;
            }
            pola_around[pola_n][0] = x+move_x;
            pola_around[pola_n][1] = y+move_y;
            pola_n++;
        }
    }

    //zliczanie min
    for(int i=0; i<pola_n; i++){
        if( play->mines[
            minefild_cord_to_ind(play, pola_around[i][0], pola_around[i][1])
        ] == MINE){
            play->mines[pos]++;
        }
    }

    if(play->mines[pos] != 0){
        return 0;
    }

    for(int i=0; i<pola_n; i++){
        if( play->cover[
            minefild_cord_to_ind(play, pola_around[i][0], pola_around[i][1])
        ] == 0){
            continue;
        }
        minefild_open(play, pola_around[i][0], pola_around[i][1]);
    }

    return 0;
}

// tylko startowy ruch
void minefild_sopen(minefild* play, int x, int y){
    int pos = minefild_cord_to_ind(play, x, y);
    if(play->mines[pos] == MINE){
        int new_pos;
        do{
            new_pos = rand()%(play->x*play->y);
            // printf("%d\n", new_pos);
        } while (play->mines[new_pos] == MINE);
        play->mines[pos] = 0;
        play->mines[new_pos] = MINE;
    }
    minefild_open(play, x, y);
};

void minefild_flag(minefild* play, int x, int y){
    int pos = minefild_cord_to_ind(play, x, y);
    if(play->cover[pos]==0){
        return;
    }
    int switch_to[2] = {2,1};
    play->cover[pos] = switch_to[play->cover[pos]-1];
}

void minefild_to_file(minefild* game, CommandLog* log, FILE* save){
    printf("plik otwarty\n");
    fprintf(save, "%d %d\n", game->x, game->y);
    for(int i=0; i<game->x*game->y; i++){
        if(game->mines[i] == MINE){
            fprintf(save, "m");
        }else{
            fprintf(save, " ");
        }
    }
    fprintf(save, "%s\n", log->data);
}

// 0-nic, 1-przegrana, 2-wygrana
int minefild_check_board(minefild* to_check){
    for(int i=0; i< to_check->x*to_check->y;i++){
        if(to_check->cover[i]==0 && to_check->mines[i]==MINE){
            return 1;
        }
    }

    for(int i=0; i< to_check->x*to_check->y;i++){
        if(to_check->cover[i]!=0 && to_check->mines[i]!=MINE){
            return 0;
        }
    }

    return 2;
}