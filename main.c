#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>

#include "minefild.h"
#include "scorebord.h"
#include "filegame.h"
#include "steps.h"

#define BUFFERSIZE 256

// tylko dla nie pustego stdin
void clear_stdin(){
    while(getchar()!='\n'){}
}

minefild* setup(){
    printf("podaj poziom trudnosci\n");
    printf("1-latwy; 2-sredni; 3-trudny; 4-wlasny\n");
    int dificulty;
    while(1){
        if(scanf("%d", &dificulty) == 0){
            printf("podaj licze\n");
            clear_stdin();
            continue;
        };
        if(dificulty < 0 || dificulty > 4){
            printf("podaj z przedzialu <1;4>\n");
            continue;
        }
        break;
    }
    clear_stdin();

    if(dificulty < 4){
        int std_seting[][4] = {
            {9,9,10, 1},
            {16,16,40, 2},
            {16,30,99, 3}
        };
        dificulty--;
        return minefild_innit(std_seting[dificulty][0],std_seting[dificulty][1],std_seting[dificulty][2],std_seting[dificulty][3]);
    }

    int custom_minefild_data[3];
    printf("podaj wymiary i ilosc min\nw formacie: x y mine\n");
    while(1){
        if(scanf(
            "%d %d %d", &custom_minefild_data[0], &custom_minefild_data[1], &custom_minefild_data[2]
        ) < 3){
            printf("zly format\n");
            clear_stdin();
            continue;
        }
        if(custom_minefild_data[0]*custom_minefild_data[1]/2 < custom_minefild_data[2]){
            printf("za duzo min\n");
            continue;
        }
        break;
    }
    clear_stdin();
    return minefild_innit(custom_minefild_data[0], custom_minefild_data[1], custom_minefild_data[2], 0);

}

int move(minefild* game, CommandLog* log){
    printf("podaj ruch\n");
    char comand;
    char file[256];
    int x,y;
    while(1){
        if(scanf("%c", &comand) < 1){
            printf("zla komenda %c\n", comand);
            clear_stdin();
            continue;
        }
        if(comand == 's'){
            if(scanf("%255s", file) < 1){
                printf("zly zapis %255s\n", file);
                clear_stdin();
                continue;
            }
        }
        if(comand == 'f' || comand == 'r'){
            if(scanf("%d %d", &x, &y) < 2){
                printf("zly zapis %c\n", comand);
                clear_stdin();
                continue;
            }
        }
        clear_stdin();
        if( comand != 'f' && comand != 'r' && comand != 's'){
            printf("nieznana akcja\n");
            continue;
        }
        if(comand != 's' && (x <= 0 || y <= 0 || x > game->x || y > game->y)){
            printf("zle kordynaty\n");
            continue;
        }
        break;
    }
    if(comand != 's'){
        char command_s[256];
        concatenate_to_string(comand, x, y, command_s);
        add_command_to_log(log, command_s);
    }
    x--;
    y--;
    if(comand == 'r'){
        return minefild_open(game, x, y);
    }else if(comand == 'f'){
        minefild_flag(game, x, y);
        return 0;
    }else if(comand == 's'){
        FILE* save = fopen(file, "w");
        minefild_to_file(game, log, save);
        fclose(save);
        return 1;
    }
    return 1;
}

void start_move(minefild* game, CommandLog* log){
    printf("podaj startowe kordynaty\nr ");
    int x,y;
    while(1){
        if(scanf("%d %d", &x, &y) < 2){
            printf("podaj liczby\n");
            clear_stdin();
            continue;
        }
        if( x <= 0 || y <= 0 || x > game->x || y > game->y){
            printf("zle kordynaty %d %d\n", x, y);
            continue;
        }
        break;
    }
    clear_stdin();
    char command_s[256];
    concatenate_to_string('r', x, y, command_s);
    add_command_to_log(log, command_s);
    x--;
    y--;
    minefild_sopen(game, x, y);
}

void kont_game(minefild* game){
    CommandLog* log = init_command_log();
    minefild_print(game);

    int end_type= 0;
    do{
        move(game, log);
        minefild_print(game);
        end_type = minefild_check_board(game);
    } while (0 == end_type);
    
    if( end_type == 1){
        printf("mina wybuchla :'(\n");
    }else if( end_type == 2){
        printf("udalo ci sie :)\n");
    }

    char player_name[NAME_LEN];
    printf("podaj imie (do 10 liter):\n");
    if(load_name(stdin, player_name) == 1){
        clear_stdin();
    }
}

void hand_game(){
    srand(time(NULL));
    minefild* game;
    CommandLog* log = init_command_log();
    game = setup();
    minefild_print(game);
    start_move(game, log);
    minefild_print(game);

    int end_type= 0;
    int x = 0;
    do{
        x = move(game, log);
        minefild_print(game);
        end_type = minefild_check_board(game);
    } while (0 == end_type && x == 0);
    
    if( end_type == 1){
        printf("mina wybuchla :'(\n");
    }else if( end_type == 2){
        printf("udalo ci sie :)\n");
    }else if(x == 1){
        printf("plik pomyslinie zapisany\n");
        return;
    }

    char player_name[NAME_LEN];
    printf("podaj imie (do 10 liter):\n");
    if(load_name(stdin, player_name) == 1){
        clear_stdin();
    }

    scorebord* table = scorebord_load();
    scorebord_add(table, game->score, player_name);
    printf("najlepsi gracze\n");
    scorebord_print(table);
    scorebord_save(table);
}

int main(int argc, char ** argv){
    char opt;
    opt = getopt(argc, argv, "f:");
    if(opt == 'f'){
        FILE* og_file = fopen(optarg, "r");
        if(og_file == NULL){
            printf("zly plik\n");
            return 0;
        }
        minefild* game = file_game(og_file);
        if(minefild_check_board(game) == 0){
            kont_game(game);
        }
        
    }else{
        hand_game();
    }
    return 0;
}