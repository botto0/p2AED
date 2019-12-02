
class file_data_t
{
    long word_pos;
    long word_num;
    char word[64];
    long current_pos;
};

int open_text_file(char *file_name, file_data_t *fd)
{
    fd->fp = fopen(file_name, "rb");
    
    return 0;
}

void close_text_file(file_data_t *fd)
{
    fclose(fd->fp);

        fd->fp = NULL;
}
