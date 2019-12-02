#include <stddef.h>
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

int open_text_file(char *file_name, file_data_t *fd)
{
    fd->fp = fopen(file_name, "rb");
    if (fd->fp == NULL)
        return -1;
    fd->word_pos = -1;
    fd->word_num = -1;
    fd->word[0] = '\0';
    fd->current_pos = -1;
    return 0;
}

void close_text_file(file_data_t *fd)
{
    fclose(fd->fp);
    fd->fp = NULL;
}
