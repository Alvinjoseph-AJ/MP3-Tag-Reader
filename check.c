#include "mp3.h"

Op check_operation(int argc, char *argv[], MP3_data *mp3)
{
    if (argc >= 3 && strcmp(argv[1], "-v") == 0)
    {
        if (strstr(argv[2], ".mp3") != NULL)
        {
            FILE *fptr = fopen(argv[2], "rb");
            if (!fptr)
            {
                printf(RED "\nERROR: File '%s' does not exist\n" RESET, argv[2]);
                return mp3_invalid;
            }

            char id[4];
            fread(id, 1, 3, fptr);
            id[3] = '\0';

            if (strcmp(id, "ID3") != 0)
            {
                printf(RED "\nERROR: Not an ID3 tagged MP3 file\n" RESET);
                fclose(fptr);
                return mp3_invalid;
            }

            unsigned char version[2];
            fread(version, 1, 2, fptr);
            if (version[0] != 0x03 || version[1] != 0x00)
            {
                printf(RED "\nERROR: Unsupported ID3 version. Only ID3v2.3.0 supported\n" RESET);
                fclose(fptr);
                return mp3_invalid;
            }

            fclose(fptr);
            strcpy(mp3->audio_filename, argv[2]);
            return mp3_view;
        }
    }
    else if (argc >= 5 && strcmp(argv[1], "-e") == 0)
    {
        if (strstr(argv[4], ".mp3") != NULL)
        {
            FILE *fptr = fopen(argv[4], "rb");
            if (!fptr)
            {
                printf(RED "\nERROR: File '%s' does not exist\n" RESET, argv[2]);
                return mp3_invalid;
            }
            fclose(fptr);

            strcpy(mp3->audio_filename, argv[4]);

            return mp3_edit;
        }
    }
    else if (argc >= 2 && strcmp(argv[1], "--help") == 0)
    {
        return mp3_help;
    }

    return mp3_invalid;
}

void display_error()
{
    printf(RED "\nUsage Error:\n" RESET);
    printf(YELLOW "./a.out -v <filename>.mp3\n");
    printf("./a.out -e <tag(TIT2: -t,TPE1: -a,TALB: -A,TYER: -y,TCON: -c)> <new_value> <filename>.mp3\n");
    printf("./a.out --help\n" RESET);
}
