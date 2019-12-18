#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 37

/*  file_data é a estrutura que contém as 
    informações relativas a cada palavra do ficheiro*/
typedef struct node
{
    long word_pos;
    long word_num;
    char word[64];
    long first_pos;
    long current_pos;
    long max_distance;
    long min_distance;
    long med_distance;
    FILE *fp;
    struct node_t *next;
} node_t;
/*----------------------------------------------*/

/* cria hashTable de tamanho 37 (porque 37 é um número primo, comvém ser primo)*/
node_t *hashTable[SIZE] = {NULL};
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
void print_linked_list()
{
    node_t *temp = malloc(sizeof(node_t));
    for (int i = 0; i < SIZE; i++)
    {
         node_t *current = malloc(sizeof(node_t));
        current = hashTable[i];
        if (current==NULL){
            continue;
        }
        for(;;)
        {
            printf("%s - (%ld,%ld) -> ", current->word, current->word_num, current->word_pos);
            if (current->next == NULL)
                break;
            temp =current;
            current = temp->next;
        }
        printf("\n");
    }

<<<<<<< HEAD
=======
      struct node *temp;

    // Return if list is empty 
    if(head == NULL)
    {
        printf("List is empty.");
        return;
    }
    
    temp = head;
    while(temp != NULL)
    {
        printf("Data = %d\n", temp->data); // Print data of current node
        temp = temp->next;                 // Move to next node
    }
>>>>>>> 37cfdf6c7ade2db2b49f6d88dc9858d673454df5
}

/*----------------------------------------------*/
int open_text_file(char *file_name, node_t *fd)
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

void close_text_file(node_t *fd)
{
    fclose(fd->fp);
    fd->fp = NULL;
}

int read_word(node_t *fd)
{
    int i, c;
    // skip white spaces
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
/*Insere elemento na hash Table*/
void insert(char word[64], node_t *word_info)
{

    long current_distance;

    node_t *node = malloc(sizeof(node_t));
    node = word_info;
    strcpy(node->word, word);
    int hcode = hash_function(word, 37);

    if (hashTable[hcode] == NULL)
    {
        hashTable[hcode] = node;
        return;
    }
    else
    {
        node_t *current = malloc(sizeof(node_t));
        current = hashTable[hcode];

        // se as palavras forem as mesmas (e hashcode igual)
        // neste caso apenas temos de atualizar as informacoes do node
        if (strcmp(current->word, word) == 0)
        {
            /*pos, num, cuurentpos, depois-> distances*/
            current_distance = node->word_pos - current->word_pos;

            current->word_num = node->word_num;
            current->word_pos = node->word_pos;
            current->current_pos = node->current_pos;

            if (current_distance > current->max_distance)
            {
                current->max_distance = current_distance;
            }
            if (current_distance < current->min_distance)
            {
                current->min_distance = current_distance;
            }
            current->med_distance = current->med_distance + (current_distance - current->med_distance) / (current->word_num + 1);
            //media = media + (valoratual - media)/(num valores metidos +1)
            return;
        }
        // se as palavras forem diferentes (e hashcode igual)
        // neste caso temos de adicionar um novo node com a nova palavra
        while (current->next)
        {
            current = current->next;
        }
        current = node;
        return;
    }
}
/*----------------------------------------------*/

int main(int argc, char const *argv[])
{
    node_t *nd = malloc(sizeof(node_t));
    open_text_file("/home/lucas/Desktop/p2AED/teste.txt", nd);
    while (read_word(nd) == 0)
    {
        printf("%s->    Current Position %ld\n\t First Postion %ld\n\t Word Num %ld\n\t Word Pos %ld\n", nd->word, nd->current_pos,nd->first_pos,nd->word_num,nd->word_pos);
        insert(nd->word, nd);
    }
    close_text_file(nd);
    print_linked_list();


    return 0;
}
