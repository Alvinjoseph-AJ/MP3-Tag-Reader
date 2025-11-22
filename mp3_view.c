/*
    Name: Alvin Joseph
    Description : MP3 Tag Reader
    Date : 30/04/2025 
*/
#include "mp3.h"

Status open_files(MP3_data *mp3)
{
    mp3->audio_file = fopen(mp3->audio_filename, "rb");
    if (mp3->audio_file == NULL)
    {
        perror("fopen");
        return e_failure;
    }
    return e_success;
}

Status view(MP3_data *mp3)
{
    if (open_files(mp3) == e_failure)
        return e_failure;

    fseek(mp3->audio_file, 10, SEEK_SET);

    if (check_tag(mp3) == e_success)
    {
        printf(GREEN"\n---------------- AUDIO INFORMATION ----------------\n"RESET);
        printf("Title   : %s\n", mp3->title);
        printf("Artist  : %s\n", mp3->artist);
        printf("Album   : %s\n", mp3->album);
        printf("Year    : %s\n", mp3->year);
        printf("Genre   : %s\n", mp3->genre);
        printf(GREEN"---------------------------------------------------\n"RESET);
    }
    else
    {
        printf(RED"Unable to read tags properly.\n"RESET);
    }

    fclose(mp3->audio_file);
    return e_success;
}

Status check_tag(MP3_data *mp3)
{
    char tag[5];
    unsigned char buffer[4];

    while (fread(tag, 1, 4, mp3->audio_file) == 4)
    {
        tag[4] = '\0';
        fread(buffer, 1, 4, mp3->audio_file);

        unsigned int size = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];

        fseek(mp3->audio_file, 2, SEEK_CUR); // Skip flags

        if (size == 0 || size > 1024)
            break;

        char *data = malloc(size + 1);
        fread(data, 1, size, mp3->audio_file);
        data[size] = '\0';

        int offset = (data[0] == '\0') ? 1 : 0;

        if (strcmp(tag, "TIT2") == 0)
            strcpy(mp3->title, data + offset);
        else if (strcmp(tag, "TPE1") == 0)
            strcpy(mp3->artist, data + offset);
        else if (strcmp(tag, "TALB") == 0)
            strcpy(mp3->album, data + offset);
        else if (strcmp(tag, "TYER") == 0)
            strcpy(mp3->year, data + offset);
        else if (strcmp(tag, "TCON") == 0)
            strcpy(mp3->genre, data + offset);

        free(data);
    }

    return e_success;
}
