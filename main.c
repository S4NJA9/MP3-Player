/*
-------------------------------------------------------------
Project Name : MP3 Tag Reader & Editor
Author       : Sanjay Hibare
Date         : 30/07/2026

Description:
This program is used to read and edit ID3 tags of an MP3 file.
It supports viewing and editing the following tags:
Title, Artist, Album, Year, Genre and Comment.
-------------------------------------------------------------
*/

#include <stdio.h>      // Standard input/output functions
#include <string.h>     // String handling functions
#include "type.h"       // User-defined data types and enums
#include "view.h"       // Functions related to viewing MP3 tags
#include "edit.h"       // Functions related to editing MP3 tags

// Check which operation user selected
OperationType check_operation_type(char *symbol);

int main(int argc, char *argv[])
{
    OperationType operation;

    // Check whether enough arguments are passed
    if (argc < 3)
    {
        printf("=========================================\n");
        printf("ERROR : Invalid number of arguments\n");
        printf("=========================================\n");

        printf("\nUsage:\n");
        printf("./a.out -v <song.mp3>\n");
        printf("./a.out -e <-t/-a/-A/-y/-g/-m> <new_data> <song.mp3>\n");

        return e_failure;
    }

    // Find the operation selected by user
    operation = check_operation_type(argv[1]);

    switch (operation)
    {
        /*---------------- View Module ----------------*/

        case v_view:
        {
            // View operation needs only 3 arguments
            if (argc != 3)
            {
                printf("ERROR : Invalid arguments for View\n");
                return e_failure;
            }

            printf("=========================================\n");
            printf("            MP3 TAG READER\n");
            printf("=========================================\n\n");

            // Check whether the file is an MP3 file
            if (validate_mp3_extension(argv[2]) == e_failure)
            {
                printf("ERROR : Invalid MP3 file\n");
                return e_failure;
            }

            // Open MP3 file
            FILE *fp = fopen(argv[2], "r");

            if (fp == NULL)
            {
                printf("ERROR : Unable to open file\n");
                return e_failure;
            }

            // Display all MP3 tags
            if (view_tags(fp) == e_failure)
            {
                fclose(fp);
                return e_failure;
            }

            // Close the file
            fclose(fp);

            printf("\n=========================================\n");
            printf("View Operation Successful\n");
            printf("=========================================\n");

            break;
        }

        /*---------------- Edit Module ----------------*/

        case e_edit:
        {
            // Edit operation needs 5 arguments
            if (argc != 5)
            {
                printf("ERROR : Invalid arguments for Edit\n");

                printf("\nUsage:\n");
                printf("./a.out -e -t <title> song.mp3\n");
                printf("./a.out -e -a <artist> song.mp3\n");
                printf("./a.out -e -A <album> song.mp3\n");
                printf("./a.out -e -y <year> song.mp3\n");
                printf("./a.out -e -g <genre> song.mp3\n");
                printf("./a.out -e -m <comment> song.mp3\n");

                return e_failure;
            }

            printf("=========================================\n");
            printf("            MP3 TAG EDITOR\n");
            printf("=========================================\n\n");

            // Edit Title
            if (strcmp(argv[2], "-t") == 0)
            {
                printf("Editing Title...\n\n");

                if (edit_tag(argv[4], "TIT2", argv[3]) == e_success)
                    printf("\nTitle Updated Successfully\n");
            }

            // Edit Artist
            else if (strcmp(argv[2], "-a") == 0)
            {
                printf("Editing Artist...\n\n");

                if (edit_tag(argv[4], "TPE1", argv[3]) == e_success)
                    printf("\nArtist Updated Successfully\n");
            }

            // Edit Album
            else if (strcmp(argv[2], "-A") == 0)
            {
                printf("Editing Album...\n\n");

                if (edit_tag(argv[4], "TALB", argv[3]) == e_success)
                    printf("\nAlbum Updated Successfully\n");
            }

            // Edit Year
            else if (strcmp(argv[2], "-y") == 0)
            {
                printf("Editing Year...\n\n");

                if (edit_tag(argv[4], "TYER", argv[3]) == e_success)
                    printf("\nYear Updated Successfully\n");
            }

            // Edit Genre
            else if (strcmp(argv[2], "-g") == 0)
            {
                printf("Editing Genre...\n\n");

                if (edit_tag(argv[4], "TCON", argv[3]) == e_success)
                    printf("\nGenre Updated Successfully\n");
            }

            // Edit Comment
            else if (strcmp(argv[2], "-m") == 0)
            {
                printf("Editing Comment...\n\n");

                if (edit_tag(argv[4], "COMM", argv[3]) == e_success)
                    printf("\nComment Updated Successfully\n");
            }

            // Invalid edit option
            else
            {
                printf("ERROR : Invalid Edit Option\n\n");

                printf("Valid Options:\n");
                printf("-t : Edit Title\n");
                printf("-a : Edit Artist\n");
                printf("-A : Edit Album\n");
                printf("-y : Edit Year\n");
                printf("-g : Edit Genre\n");
                printf("-m : Edit Comment\n");

                return e_failure;
            }

            printf("\n=========================================\n");
            printf("Edit Operation Successful\n");
            printf("=========================================\n");

            break;
        }

        /*---------------- Invalid Operation ----------------*/

        default:
        {
            printf("=========================================\n");
            printf("ERROR : Unsupported Operation\n");
            printf("=========================================\n");

            printf("\nSupported Operations:\n");
            printf("./a.out -v <song.mp3>\n");
            printf("./a.out -e <-t/-a/-A/-y/-g/-m> <new_data> <song.mp3>\n");

            return u_unsupported;
        }
    }

    return e_success;
}

// Check whether user selected View or Edit
OperationType check_operation_type(char *symbol)
{
    if (strcmp(symbol, "-v") == 0)
        return v_view;

    if (strcmp(symbol, "-e") == 0)
        return e_edit;

    return u_unsupported;
}