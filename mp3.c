/*
    Name: Alvin Joseph
    Description : MP3 Tag Reader
    Date : 30/04/2025 
*/

#include "mp3.h"

int main(int argc, char *argv[])
{
    MP3_data mp3;
    
    Op operation = check_operation(argc, argv, &mp3);

    switch (operation)
    {
        case mp3_view:
            view(&mp3);
            break;

        case mp3_edit:
            edit(&mp3,argv);
            printf(GREEN"\nEDITED SUCCESSFULLY\n\n"RESET);
            break;

        case mp3_help:
            for (int i = 0; i < 60; i++) printf(GREEN"-"RESET);
            printf("\n"RED"%30s\n"RESET, "HELP MENU");
            for (int i = 0; i < 60; i++) printf(GREEN"-"RESET);
            printf(YELLOW"\n1. ./a.out -v <filename>.mp3  → View MP3 metadata\n");
            printf("2. ./a.out -e <tag(TIT2: -t,TPE1: -a,TALB: -A,TYER: -y,TCON: -c)> <new_value> <file> → Edit metadata (to be implemented)\n");
            printf("3. ./a.out --help → Show help menu\n"RESET);
            for (int i = 0; i < 60; i++) printf(GREEN"-"RESET);
            printf("\n");
            break;

        case mp3_invalid:
        default:
            display_error();
            return 1;
    }

    return 0;
}
