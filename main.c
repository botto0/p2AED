#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define hash_size 1009u

/*  file_data é a estrutura que contém as 
    informações relativas a cada palavra do ficheiro*/
typedef struct file_data
{
    // public data
    long word_pos;
    long word_num;
    char word[64];
    FILE *fp;
    long current_pos; // zero-based }
    struct file_data *next;
} file_data_t;
/*----------------------------------------------*/

/*Esta é a estrutura que um elemento da Hash Table*/
typedef struct table_element
{
    char word[64];
    file_data_t word_info;
} table_element_t;
/*----------------------------------------------*/

/* cria hashTable de tamanho 37 (porque 37 é um número primo, comvém ser primo)*/
typedef struct table_element_t *hashTable[37];
/*----------------------------------------------*/

/*Função que trata as informações de cada palavra*/
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
/*----------------------------------------------*/

/*Função que abre o ficheiro*/
int open_text_file(char *file_name, file_data_t *fd)
{
    fd->fp = fopen(file_name, "rb");
    if (fd->fp == NULL)
        return -1;
    fd->word_pos = -1;
    fd->word_num = -1;
    fd->word[0] = '\0';
    fd->current_pos = -1;
    /* Inicializa o primeiro node da linked list*/
    file_data_t *head = NULL;
    head = malloc(sizeof(file_data_t));
    if (head == NULL)
    {
        return 1;
    }
    head->word_pos = -1;
    head->word_num = -1;
    head->word[0] = '\0';
    head->current_pos = -1;
    head->next = NULL;
    /*----------------------------------------------*/
    return 0;
}
/*----------------------------------------------*/

/*Função que fecha o ficheiro*/
void close_text_file(file_data_t *fd)
{
    fclose(fd->fp);
    fd->fp = NULL;
}
/*----------------------------------------------*/

/*Hash Fucntion utilizada para a hash table*/
unsigned int hash_function(const char *str, unsigned int s)
{
    static unsigned int table[256];
    unsigned int crc, i, j;
    if (table[1] == 0u) // do we need to initialize the table[] array?
    {
        for (i = 0u; i < 256u; i++)
            for (table[i] = i, j = 0u; j < 8u; j++)
                if (table[i] & 1u)
                    table[i] = (table[i] >> 1) ^ 0xAED00022u; // "magic" constant
                else
                    table[i] >>= 1;
    }
    crc = 0xAED02019u; // initial value (chosen arbitrarily)
    while (*str != '\0')
        crc = (crc >> 8) ^ table[crc & 0xFFu] ^ ((unsigned int)*str++ << 24);
    return crc % s;
}
/*----------------------------------------------*/

/*Função que dá print da linked list*/
void print_linked_list(file_data_t *head)
{
    printf("PRINT LINKED LIST\n");
    file_data_t *current = head;
    while (current != NULL)
    {
        // comando de print ------
        current = current->next;
    }
}
/*----------------------------------------------*/

/*Funçao que adiciona valores ao final da linked list*/
void push(file_data_t *head, int word_pos, int word_num)
{
    file_data_t *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    /* agora podemos adicionar informações */
    current->next = malloc(sizeof(file_data_t));
    current->next->word_pos = word_pos;
    current->next->word_num = word_num;
    current->next->next = NULL;
}
/*----------------------------------------------*/

/*Insere elemento na hash Table*/
void insert(char word[64], file_data_t word_info)
{
    table_element_t *element = (struct table_element_t *)malloc(sizeof(table_element_t));
    element->word_info = word_info;
    strcpy(element->word, word);

    int hcode = hash_function(element->word, 100);
    while (hashTable[hcode] != NULL)
    {
        printf("Hello");
    }
}
/*----------------------------------------------*/

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
    /* Print das palavras e info das palavras*/
    printf("word: (word_pos, word_num)\n");
    while (read_word(&fd) == 0)
    {
        printf("%s: (%ld, %ld)\n", fd.word, fd.word_pos, fd.word_num);
    }
    printf("Current Pos: %ld\n", fd.current_pos);
    /*----------------------------------------------*/
    // print_linked_list(head);

    close_text_file(&fd);

    return 0;
}
