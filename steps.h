#ifndef STEPS_H
#define STEPS_H

typedef struct {
    char *data;
    int length;
    int capacity;
} CommandLog;

void concatenate_to_string(char c, int num1, int num2, char *result);
CommandLog* init_command_log();
void add_command_to_log(CommandLog *log, const char *command);
void save_command_log(CommandLog *log, const char *filename);
void free_command_log(CommandLog *log);

#endif