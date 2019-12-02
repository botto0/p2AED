#ifndef _OPEN_TEXT_FILE_
#define _OPEN_TEXT_FILE_

#include <stdio.h>

typedef struct file_data
{
    // public data
    long word_pos;
    long word_num;
    char word[64];
    FILE *fp;
    long current_pos; // zero-based }
} file_data_t;

int open_text_file(char *file_name, file_data_t *fd);
void close_text_file(file_data_t *fd);
#endif 