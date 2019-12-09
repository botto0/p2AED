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

int read_word(file_data_t *fd)
{
    int i, c;
    do
    {
        c = fgetc(fd->fp);
        if (c == EOF)
            return -1;
        fd->current_pos++;
    } while (c <= 32);
    // record word

    fd->word_pos = fd->current_pos;
    fd->word_num++;
    fd->word[0] = (char)c;

    for (i = 1; i < (int)sizeof(fd->word) - 1; i++)
    {
        c = fgetc(fd->fp);
        if (c == EOF)
            break; // end of file
        fd->current_pos++;
        if (c <= 32)
            break; // terminate word
        fd->word[i] = (char)c;
    }
    fd->word[i] = '\0';
    return 0;
}

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

int main(int argc, char const *argv[])
{
    file_data_t fd;
    // meter antes a ler o ficheiro do terminal -- Nao esquecer da verificacao de argumentos
    int r = open_text_file("/Users/botto/Desktop/p2AED/teste.txt" , &fd);
    if (r != 0)
    {
        fprintf(stderr, "Unable to open file");
        return 2;
    }

    while (read_word(&fd) == 0)
    {
        printf("%s: %ld %ld\n",fd.word,fd.word_pos,fd.word_num);  
    } 

    printf("%ld", fd.current_pos);

    close_text_file(&fd);
    return 0;
}
