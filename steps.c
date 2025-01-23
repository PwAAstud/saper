#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "steps.h"

void concatenate_to_string(char c, int num1, int num2, char *result) {
    char temp1[10], temp2[10]; // Bufory na liczby przekonwertowane na stringi

    // Konwersja liczb na stringi
    itoa(num1, temp1, 10);
    itoa(num2, temp2, 10);

    // Łączenie wszystkich elementów w jeden string
    sprintf(result, "%c %s %s", c, temp1, temp2);
}

// Funkcja inicjalizująca log komend
CommandLog* init_command_log() {
    CommandLog *log = malloc(sizeof(CommandLog));
    log->length = 0;
    log->capacity = 256; // Początkowy rozmiar
    log->data = malloc(log->capacity);
    log->data[0] = '\0'; // Pusty ciąg znaków
    return log;
}

// Funkcja dodająca komendę do logu
void add_command_to_log(CommandLog *log, const char *command) {
    int command_length = strlen(command) + 1; // Długość komendy + spacja
    if (log->length + command_length >= log->capacity) {
        // Zwiększ pojemność dwukrotnie
        log->capacity *= 2;
        log->data = realloc(log->data, log->capacity);
    }
    strcat(log->data, command);
    strcat(log->data, " "); // Dodaj spację jako separator
    log->length += command_length;
}

// Funkcja zapisująca log do pliku
void save_command_log(CommandLog *log, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Nie można otworzyć pliku");
        return;
    }
    fprintf(file, "%s\n", log->data);
    fclose(file);
}

// Funkcja czyszcząca pamięć logu
void free_command_log(CommandLog *log) {
    free(log->data);
    free(log);
}

