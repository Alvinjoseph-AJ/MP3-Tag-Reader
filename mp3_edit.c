// /*
//     Name: Alvin Joseph
//     Description : MP3 Tag Reader
//     Date : 30/04/2025 
// */

#include "mp3.h"

Status edit(MP3_data *mp3, char **argv)
{
    int first = 0, last = 0;
    
    // Open the original MP3 file
    mp3->audio_file = fopen(mp3->audio_filename, "rb");
    if (mp3->audio_file == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    // Get file size
    fseek(mp3->audio_file, 0, SEEK_END);
    int filesize = ftell(mp3->audio_file);
    fseek(mp3->audio_file, 10, SEEK_SET); // Skip ID3v2 header

    // Determine tag type from argv[2]
    char Tag[5] = {0};
    if (!strcmp(argv[2], "-t"))
        strcpy(Tag, "TIT2");
    else if (!strcmp(argv[2], "-a"))
        strcpy(Tag, "TPE1");
    else if (!strcmp(argv[2], "-A"))
        strcpy(Tag, "TALB");
    else if (!strcmp(argv[2], "-y"))
        strcpy(Tag, "TYER");
    else if (!strcmp(argv[2], "-c"))
        strcpy(Tag, "TCON");
    else
    {
        printf(RED "\nInvalid Tag Option\n" RESET);
        fclose(mp3->audio_file);
        return e_failure;
    }

    // Prepare new content
    char *new_data = argv[3];
    unsigned int new_size = strlen(new_data);
    unsigned char size_bytes[4] = {
        (new_size >> 24) & 0xFF,
        (new_size >> 16) & 0xFF,
        (new_size >> 8) & 0xFF,
        new_size & 0xFF
    };

    // Find the position of the tag in the file
    char current_tag[5] = {0};
    unsigned char frame_size[4];
    int found = 0;

    while (fread(current_tag, 1, 4, mp3->audio_file) == 4)
    {
        current_tag[4] = '\0';

        fread(frame_size, 1, 4, mp3->audio_file);
        unsigned int curr_size = (frame_size[0] << 24) | (frame_size[1] << 16) | (frame_size[2] << 8) | frame_size[3];

        fseek(mp3->audio_file, 2, SEEK_CUR); // Skip flags

        if (strcmp(Tag, current_tag) == 0)
        {
            first = ftell(mp3->audio_file) - 10; // Go back to frame header
            fseek(mp3->audio_file, curr_size, SEEK_CUR);
            last = ftell(mp3->audio_file);
            found = 1;
            break;
        }

        fseek(mp3->audio_file, curr_size, SEEK_CUR);
    }

    if (!found)
    {
        printf(RED "Tag not found in file.\n" RESET);
        fclose(mp3->audio_file);
        return e_failure;
    }

    // Go back to start
    fseek(mp3->audio_file, 0, SEEK_SET);

    // Open temporary file
    FILE *temp = fopen("edit.txt", "wb");
    if (temp == NULL)
    {
        perror("fopen");
        fclose(mp3->audio_file);
        return e_failure;
    }

    // Copy content before tag
    char *buffer = malloc(first);
    fread(buffer, 1, first, mp3->audio_file);
    fwrite(buffer, 1, first, temp);
    free(buffer);

    // Write new frame
    fwrite(Tag, 1, 4, temp);            // Frame ID
    fwrite(size_bytes, 1, 4, temp);     // Frame size
    unsigned char flags[2] = {0x00, 0x00};
    fwrite(flags, 1, 2, temp);          // Frame flags
    fwrite(new_data, 1, new_size, temp);// Frame data

    // Copy remaining content
    fseek(mp3->audio_file, last, SEEK_SET);
    int remaining = filesize - last;
    char *tail = malloc(remaining);
    fread(tail, 1, remaining, mp3->audio_file);
    fwrite(tail, 1, remaining, temp);
    free(tail);

    fclose(mp3->audio_file);
    fclose(temp);

    // Replace original file with edited content
    FILE *source = fopen("edit.txt", "rb");
    FILE *dest = fopen(mp3->audio_filename, "wb");
    if (source == NULL || dest == NULL)
    {
        perror("fopen");
        if (source) fclose(source);
        if (dest) fclose(dest);
        return e_failure;
    }

    fseek(source, 0, SEEK_END);
    int total = ftell(source);
    fseek(source, 0, SEEK_SET);
    char *final_data = malloc(total);
    fread(final_data, 1, total, source);
    fwrite(final_data, 1, total, dest);
    free(final_data);

    fclose(source);
    fclose(dest);

    // Delete the temporary file
    remove("edit.txt");

    return e_success;
}


// #include "mp3.h"

// Status edit(MP3_data *mp3, char  **argv)
// {
//     int first=0,last=0;
//     mp3->audio_file = fopen(mp3->audio_filename, "rb");
//     if (mp3->audio_file == NULL)
//     {
//         perror("fopen");
//         return e_failure;
//     }
//     fseek(mp3->audio_file,0,SEEK_END);
//     int size= ftell(mp3->audio_file);
//     //printf("size = %d\n",size);
//     fseek(mp3->audio_file,10,SEEK_SET);

//     char Tag[5];

//     char *buffer = argv[3];
//     unsigned int tag_size = strlen(buffer);

//     unsigned int new_size[4] = {(tag_size >> 24) & 0xFF,(tag_size >> 16) & 0xFF,(tag_size >> 8) & 0xFF,tag_size & 0xFF};
//     //printf("new_size = %ls",new_size);

//     if(!strcmp(argv[2],"-t"))
//     {
//         strcpy(Tag, "TIT2");
//     }
//     else if(!strcmp(argv[2],"-a"))
//     {
//         strcpy(Tag, "TPE1");
//     }
//     else if(!strcmp(argv[2],"-A"))
//     {
//         strcpy(Tag, "TALB");
//     }
//     else if(!strcmp(argv[2],"-y"))
//     {
//         strcpy(Tag, "TYER");
//     }
//     else if(!strcmp(argv[2],"-c"))
//     {
//         strcpy(Tag, "TCON");
//     }
//     else
//     {
//         printf(RED"\nInvalid Tag\n\n"RESET);

//     }
//     Tag[4]='\0';
//     //printf("Tag = %s\n",Tag);

//     char Ftag[5];
//     unsigned char Fbuffer[4];
//     int flag=0;

//     while (fread(Ftag, 1, 4, mp3->audio_file) == 4)
//     {
//         Ftag[4] = '\0';

//         if(strcmp(Tag,Ftag)==0)
//         {
//             //printf("FTag = %s\n",Ftag);
//             first = ftell(mp3->audio_file);
//             //printf("first = %d\n",first);
//             flag=1;
//         }

//         fread(Fbuffer, 1, 4, mp3->audio_file);

//         unsigned int size = (Fbuffer[0] << 24) | (Fbuffer[1] << 16) | (Fbuffer[2] << 8) | Fbuffer[3];


//         fseek(mp3->audio_file, 2, SEEK_CUR);
//         fseek(mp3->audio_file, size, SEEK_CUR);        
//         if(flag==1)
//         {
//             last = ftell(mp3->audio_file);
//             //printf("last = %d\n",last);
//             break;
//         }

//     }
//     fseek(mp3->audio_file, 0, SEEK_SET);

//     FILE*fb = fopen("edit.txt","wb");
//     if(fb == NULL)
//     {
//         perror("fopen");
//         return e_failure;
//     }

//     char *store;
//     char *Lstore;
//     Lstore = malloc(size);
//     store = malloc(size);
//     fread(store, 1, first,mp3->audio_file );
//     fwrite(store, 1, first, fb);
//     //printf("buffer = %s\n",buffer);
//     fwrite(new_size,1,4,fb);
//     fseek(fb, 2, SEEK_CUR);
//     fwrite(buffer,1,tag_size,fb);
//     fseek(mp3->audio_file, last, SEEK_SET);
//     fread(Lstore, 1, size-last,mp3->audio_file );
//     fwrite(Lstore, 1, size-last, fb);

//     fclose(mp3->audio_file);
//     fclose(fb);

    
//     FILE*fr = fopen("edit.txt","rb");
//     if(fr == NULL)
//     {
//         perror("fopen");
//         return e_failure;
//     }
//     FILE*fw = fopen(mp3->audio_filename,"wb");
//     if(fw == NULL)
//     {
//         perror("fopen");
//         return e_failure;
//     }
//     fseek(fr,0,SEEK_END);
//     int end = ftell(fr);
//     //printf("end = %d\n",end);
//     char*rfull = malloc(end);
//     fread(rfull,1,end,fr);
//     fwrite(rfull,1,end,fw);
//     fclose(fr);
//     fclose(fw);

//     free(store);
//     free(Lstore); 
//     free(rfull);

// }

// #include "mp3.h"

// Status edit(MP3_data *mp3, char **argv)
// {
//     FILE *src = fopen(mp3->audio_filename, "rb");
//     if (!src)
//     {
//         perror("fopen");
//         return e_failure;
//     }

//     fseek(src, 0, SEEK_END);
//     int total_size = ftell(src);
//     fseek(src, 10, SEEK_SET); // Skip ID3 header

//     char *tag_text = argv[3];
//     unsigned int tag_size = strlen(tag_text);
//     char Tag[5];

//     if (strcmp(argv[2], "-t") == 0) strcpy(Tag, "TIT2");
//     else if (strcmp(argv[2], "-a") == 0) strcpy(Tag, "TPE1");
//     else if (strcmp(argv[2], "-A") == 0) strcpy(Tag, "TALB");
//     else if (strcmp(argv[2], "-y") == 0) strcpy(Tag, "TYER");
//     else if (strcmp(argv[2], "-c") == 0) strcpy(Tag, "TCON");
//     else {
//         printf(RED "\nInvalid Tag\n\n" RESET);
//         fclose(src);
//         return e_failure;
//     }

//     int first = 0, last = 0, found = 0;
//     char frame_id[5];
//     unsigned char size_bytes[4];

//     while (fread(frame_id, 1, 4, src) == 4)
//     {
//         frame_id[4] = '\0';
//         fread(size_bytes, 1, 4, src);

//         unsigned int frame_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];
//         fseek(src, 2, SEEK_CUR); // Skip flags

//         if (strcmp(frame_id, Tag) == 0)
//         {
//             found = 1;
//             first = ftell(src) - 10; // Start of frame (4 ID + 4 size + 2 flags)
//             fseek(src, frame_size, SEEK_CUR);
//             last = ftell(src);
//             break;
//         }
//         else
//         {
//             fseek(src, frame_size, SEEK_CUR);
//         }
//     }

//     if (!found)
//     {
//         printf(RED "Tag not found in file.\n" RESET);
//         fclose(src);
//         return e_failure;
//     }

//     // Prepare new frame
//     unsigned int new_size = tag_size;
//     unsigned char new_size_bytes[4] = {
//         (new_size >> 24) & 0xFF,
//         (new_size >> 16) & 0xFF,
//         (new_size >> 8) & 0xFF,
//         new_size & 0xFF
//     };

//     // Allocate memory for old MP3
//     fseek(src, 0, SEEK_SET);
//     unsigned char *before = malloc(first);
//     unsigned char *after = malloc(total_size - last);
//     if (!before || !after) {
//         perror("malloc");
//         fclose(src);
//         free(before);
//         free(after);
//         return e_failure;
//     }

//     fread(before, 1, first, src);
//     fseek(src, last, SEEK_SET);
//     fread(after, 1, total_size - last, src);
//     fclose(src);

//     // Rewrite updated MP3 file
//     FILE *dest = fopen(mp3->audio_filename, "wb");
//     if (!dest)
//     {
//         perror("fopen");
//         free(before);
//         free(after);
//         return e_failure;
//     }

//     fwrite(before, 1, first, dest);
//     fwrite(Tag, 1, 4, dest);                      // Frame ID
//     fwrite(new_size_bytes, 1, 4, dest);           // Frame size
//     unsigned char flags[2] = {0x00, 0x00};         // Frame flags
//     fwrite(flags, 1, 2, dest);
//     fwrite(tag_text, 1, tag_size, dest);          // New data
//     fwrite(after, 1, total_size - last, dest);    // Remainder of file

//     fclose(dest);
//     free(before);
//     free(after);

//     return e_success;
// }
