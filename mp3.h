#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef  MP3_H
#define MP3_H

#define RED        "\033[1;31m"
#define GREEN      "\033[1;32m"
#define YELLOW     "\033[1;33m"
#define BLUE       "\033[1;34m"
#define RESET      "\033[1;37m"

typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    mp3_view = 2,
    mp3_edit = 3,
    mp3_help = 4,
    mp3_invalid
}Op;

typedef struct {
    char audio_filename[20];
    FILE *audio_file;
    char title[50];
    char artist[50];
    char album[50];
    char year[50];
    char genre[50];
}MP3_data;

void display_error();
Op check_operation(int argc, char *argv[], MP3_data *mp3);
Status open_files(MP3_data *mp3);
Status view(MP3_data *mp3);
Status edit(MP3_data *mp3,char**argv);
Status check_tag(MP3_data *mp3);
// Status Match_tag(MP3_data *mp3,char tag[5]);

#endif