#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100003

/*  file_data é a estrutura que contém as 
    informações relativas a cada palavra do ficheiro*/
typedef struct node
{
    long word_pos;
    long word_num;
    long word_count;
    char word[64];
    long first_pos;
    long current_pos;
    long max_distance;
    long min_distance;
    long med_distance;
    long last_pos;
    long prev_pos;
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
void printHashTable()
{
    for (int i = 0; i < SIZE; i++)
    {
        node_t *current = hashTable[i];
        if (current == NULL)
        {
            continue;
        }
        printf(" WORD ::: %s\n WORD_COUNT ::: %ld\n FIRST_POS ::: %ld\n LAST POS ::: %ld\n MAX DISTANCE ::: %ld\n MIN DISTANCE ::: %ld\n AVG DISTANCE ::: %ld\n", current->word,current->word_count, current->first_pos,  current->last_pos, current->max_distance, current->min_distance, current->med_distance);
        printf("\n");
        }
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
void insert(node_t *word_info)
{

    long current_distance;

    int hcode = hash_function(word_info->word, SIZE);

    if (hashTable[hcode] == NULL)
    {
        hashTable[hcode] = word_info;
        hashTable[hcode]->first_pos=word_info->current_pos-strlen(word_info->word);
        hashTable[hcode]->last_pos=hashTable[hcode]->first_pos;
        hashTable[hcode]->word_count=1;
        hashTable[hcode]->max_distance=0;
        hashTable[hcode]->min_distance=999999999;
        hashTable[hcode]->med_distance=0;
        hashTable[hcode]->prev_pos=word_info->current_pos;
    }
    else
    {
        node_t *current = malloc(sizeof(node_t));
        current = hashTable[hcode];
        // se as palavras forem as mesmas (e hashcode igual)
        // neste caso apenas temos de atualizar as informacoes do node
        if (strcmp(current->word, word_info->word) == 0)
        {
            /*pos, num, currentpos, depois-> distances*/
            current->word_count++;
            current->med_distance = (current->med_distance+ (word_info->current_pos - current->prev_pos))/(current->word_count-1);
            current_distance = word_info->word_pos - current->word_pos;
			current->last_pos=word_info->current_pos-strlen(word_info->word);
            current->word_num = word_info->word_num;
            current->word_pos = word_info->word_pos;
            if (current_distance < current->min_distance)
            {
                current->min_distance = current_distance;
            }
            if(current->current_pos> current->last_pos)
            {
				current->last_pos=current->current_pos;
			}
			current->max_distance = current-> last_pos - current->first_pos;
            current->current_pos = word_info->current_pos;
            //media = media + (valoratual - media)/(num valores metidos +1)
        }
        // se as palavras forem diferentes (e hashcode igual)
        // neste caso temos de adicionar um novo node com a nova palavra

    }
}
/*----------------------------------------------*/

int main(int argc, char const *argv[])
{
    node_t *nd = malloc(sizeof(node_t));
    open_text_file("teste.txt", nd);

    while (read_word(nd) == 0)
    {
		node_t *pq = malloc(sizeof(node_t));
		strcpy(pq->word,nd->word);
		pq->word_pos = nd->word_pos;
		pq->word_num = nd->word_num;
		pq->first_pos= nd->first_pos;
		pq->last_pos = nd->last_pos;
		pq->current_pos = nd->current_pos;
		pq->max_distance= nd->max_distance;
		pq->min_distance = nd->min_distance;
		pq->med_distance = nd->med_distance;
        insert(pq);
    }
    close_text_file(nd);
   printHashTable();

    return 0;
}
