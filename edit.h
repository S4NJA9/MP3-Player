#ifndef EDIT_H
#define EDIT_H

#include <stdio.h>
#include "type.h"

// filename  -> MP3 file name
// frame_id  -> Frame to edit (TIT2, TPE1, TALB, TYER, TCON, COMM)
// new_data  -> New value to store in the tag
Status edit_tag(char *filename, char *frame_id, char *new_data);

#endif