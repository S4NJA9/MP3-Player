#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>

// Check whether the file extension is .mp3
Status validate_mp3_extension(char *filename);

// Check whether the file contains ID3 header
Status validate_id3(FILE *fp);

// Read and display all MP3 tags
Status view_tags(FILE *fp);

// Convert 4 bytes into an integer
int convert_big_endian_to_little(unsigned char size[]);

#endif