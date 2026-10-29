#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type.h"
#include "edit.h"
#include "view.h"

// Function to edit the required MP3 tag
Status edit_tag(char *filename, char *search_frame, char *new_data)
{
    // Open file in read and write mode
    FILE *fp = fopen(filename, "r+");

    if (fp == NULL)
    {
        printf("ERROR : Unable to open MP3 file.\n");
        return e_failure;
    }

    // Read first 10 bytes (ID3 header)
    unsigned char header[10];

    if (fread(header, 10, 1, fp) != 1)
    {
        printf("ERROR : Failed to read ID3 header.\n");
        fclose(fp);
        return e_failure;
    }

    // Read frame one by one
    while (1)
    {
        char frame_id[5];
        unsigned char size_buffer[4];
        int frame_size;

        // Read frame ID
        if (fread(frame_id, 4, 1, fp) != 1)
        {
            printf("ERROR : Required tag not found.\n");
            fclose(fp);
            return e_failure;
        }

        // Make it a string
        frame_id[4] = '\0';

        // Read frame size
        if (fread(size_buffer, 4, 1, fp) != 1)
        {
            printf("ERROR : Failed to read frame size.\n");
            fclose(fp);
            return e_failure;
        }

        // Convert 4 bytes into integer
        frame_size = convert_big_endian_to_little(size_buffer);

        if (frame_size <= 0)
        {
            printf("ERROR : Invalid frame size.\n");
            fclose(fp);
            return e_failure;
        }

        // Skip 2 flag bytes
        fseek(fp, 2, SEEK_CUR);

        // Check whether this is the frame we want
        if (strcmp(frame_id, search_frame) == 0)
        {
            // Allocate memory to store old tag
            char *old_data = malloc(frame_size + 1);

            if (old_data == NULL)
            {
                printf("ERROR : Memory allocation failed.\n");
                fclose(fp);
                return e_failure;
            }

            // Read old tag data
            if (fread(old_data, frame_size, 1, fp) != 1)
            {
                printf("ERROR : Failed to read tag data.\n");
                free(old_data);
                fclose(fp);
                return e_failure;
            }

            // Add string terminator
            old_data[frame_size] = '\0';

            // Print old and new values
            printf("Old Data : %s\n", old_data + 1);
            printf("New Data : %s\n", new_data);

            // Move pointer back to beginning of tag data
            fseek(fp, -frame_size, SEEK_CUR);

            // Write encoding byte
            unsigned char encoding = 0;
            fwrite(&encoding, 1, 1, fp);

            // Write new tag data
            fwrite(new_data, strlen(new_data), 1, fp);

            // Fill remaining bytes with NULL
            int remaining = frame_size - (strlen(new_data) + 1);

            while (remaining > 0)
            {
                fputc('\0', fp);
                remaining--;
            }

            // Release allocated memory
            free(old_data);

            // Close file
            fclose(fp);

            return e_success;
        }
        else
        {
            // Skip this frame and move to next one
            fseek(fp, frame_size, SEEK_CUR);
        }
    }
}