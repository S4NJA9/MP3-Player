#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "type.h"
#include "view.h"

// Check whether the given file is an MP3 file
Status validate_mp3_extension(char *filename)
{
    // Find file extension
    char *ext = strrchr(filename, '.');

    // Check if extension exists
    if (ext == NULL)
    {
        printf("ERROR : File has no extension.\n");
        return e_failure;
    }

    // Compare extension with .mp3
    if (strcmp(ext, ".mp3") == 0)
    {
        return e_success;
    }

    printf("ERROR : Invalid MP3 file.\n");
    return e_failure;
}

// Check whether the file contains ID3 header
Status validate_id3(FILE *fp)
{
    char buffer[4];

    // Read first 3 bytes
    if (fread(buffer, 1, 3, fp) != 3)
    {
        printf("ERROR : Unable to read ID3 header.\n");
        return e_failure;
    }

    // Make it a string
    buffer[3] = '\0';

    // Compare with ID3
    if (strcmp(buffer, "ID3") == 0)
        return e_success;

    printf("ERROR : This is not a valid ID3 MP3 file.\n");
    return e_failure;
}

// Read and display all MP3 tags
Status view_tags(FILE *fp)
{
    // Check ID3 header
    if (validate_id3(fp) == e_failure)
        return e_failure;

    // Read version, flag and header size
    char version[2];
    char flag;
    unsigned char header_size[4];

    fread(version, 1, 2, fp);
    fread(&flag, 1, 1, fp);
    fread(header_size, 1, 4, fp);

    // Read six frames
    for (int i = 0; i < 6; i++)
    {
        char frame_id[5];
        unsigned char frame_size[4];

        // Read frame ID
        if (fread(frame_id, 1, 4, fp) != 4)
        {
            printf("ERROR : Unable to read frame ID.\n");
            return e_failure;
        }

        // Make it a string
        frame_id[4] = '\0';

        // Read frame size
        if (fread(frame_size, 1, 4, fp) != 4)
        {
            printf("ERROR : Unable to read frame size.\n");
            return e_failure;
        }

        // Convert frame size into integer
        int tag_size = convert_big_endian_to_little(frame_size);

        if (tag_size <= 0)
        {
            printf("ERROR : Invalid tag size.\n");
            return e_failure;
        }

        // Skip flag bytes and encoding byte
        fseek(fp, 3, SEEK_CUR);

        // Allocate memory for tag data
        char *tag = malloc(tag_size);

        if (tag == NULL)
        {
            printf("ERROR : Memory allocation failed.\n");
            return e_failure;
        }

        // Number of bytes to read
        size_t bytes_to_read = (size_t)(tag_size - 1);

        // Read tag data
        if (fread(tag, 1, bytes_to_read, fp) != bytes_to_read)
        {
            printf("ERROR : Unable to read tag data.\n");
            free(tag);
            return e_failure;
        }

        // Add null character
        tag[tag_size - 1] = '\0';

        // Display tag based on frame ID
        if (strcmp(frame_id, "TIT2") == 0)
            printf("Title   : %s\n", tag);

        else if (strcmp(frame_id, "TPE1") == 0)
            printf("Artist  : %s\n", tag);

        else if (strcmp(frame_id, "TALB") == 0)
            printf("Album   : %s\n", tag);

        else if (strcmp(frame_id, "TYER") == 0)
            printf("Year    : %s\n", tag);

        else if (strcmp(frame_id, "TCON") == 0)
            printf("Genre   : %s\n", tag);

        else if (strcmp(frame_id, "COMM") == 0)
            printf("Comment : %s\n", tag);

        // Release allocated memory
        free(tag);
    }

    return e_success;
}

// Convert big-endian bytes into integer
int convert_big_endian_to_little(unsigned char size[])
{
    int value = 0;

    value = size[0];
    value = (value << 8) | size[1];
    value = (value << 8) | size[2];
    value = (value << 8) | size[3];

    return value;
}