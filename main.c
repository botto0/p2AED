#include <stdio.h>
#include <stdlib.h>
#define hash_size 1009u

typedef struct file_data
{
    // public data
    long word_pos;
    long word_num;
    char word[64];
    FILE *fp;
    long current_pos; // zero-based }
} file_data_t;

typedef struct DataItem
{
    int data;
    int key;
} DataItem;

struct DataItem *hashArray[hash_size];
struct DataItem *dummyItem;
struct DataItem *item;

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

int hashCode(int key)
{
    return key % hash_size;
}

struct DataItem *search(int key)
{
    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while (hashArray[hashIndex] != NULL)
    {

        if (hashArray[hashIndex]->key == key)
            return hashArray[hashIndex];

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= hash_size;
    }

    return NULL;
}

void insert(int key, int data)
{

    struct DataItem *item = (struct DataItem *)malloc(sizeof(struct DataItem));
    item->data = data;
    item->key = key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty or deleted cell
    while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1)
    {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= hash_size;
    }

    hashArray[hashIndex] = item;
}

struct DataItem *delete (struct DataItem *item)
{
    int key = item->key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while (hashArray[hashIndex] != NULL)
    {
        if (hashArray[hashIndex]->key == key)
        {
            struct DataItem *temp = hashArray[hashIndex];
            //assign a dummy item at deleted position
            hashArray[hashIndex] = dummyItem;
            return temp;
        }
        //go to next cell
        ++hashIndex;
        //wrap around the table
        hashIndex %= hash_size;
    }

    return NULL;
}

void display()
{
    int i = 0;

    for (i = 0; i < hash_size; i++)
    {
        if (hashArray[i] != NULL)
            printf(" (%d,%d)", hashArray[i]->key, hashArray[i]->data);
        else
            printf(" (NULL)");
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    file_data_t fd;
    // meter antes a ler o ficheiro do terminal -- Nao esquecer da verificacao de argumentos
    int r = open_text_file("/Users/botto/Desktop/p2AED/teste.txt", &fd);
    if (r != 0)
    {
        fprintf(stderr, "Unable to open file");
        return 2;
    }

    while (read_word(&fd) == 0)
    {
        printf("%s: (word_pos) - %ld /// (word_num) - %ld\n", fd.word, fd.word_pos, fd.word_num);
    }

    printf("Current Pos: %ld\n", fd.current_pos);

    close_text_file(&fd);
    insert(1, 20);
    insert(2, 70);
    insert(42, 80);
    insert(4, 25);
    insert(12, 44);
    insert(14, 32);
    insert(17, 11);
    insert(13, 78);
    insert(37, 97);
    display();
    return 0;
}
