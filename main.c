#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} file_data_t;
/*----------------------------------------------*/

/*  file_data é a estrutura que contém as 
    informações relativas a cada palavra do ficheiro*/
typedef struct node
{
    file_data_t node_data;
    struct node_t *next;
} node_t;
/*----------------------------------------------*/

/*Esta é a estrutura que um elemento da Hash Table*/
typedef struct table_element
{
    char key[64];
    node_t *linked_list[37];
} table_element_t;
/*----------------------------------------------*/

/* cria hashTable de tamanho 37 (porque 37 é um número primo, comvém ser primo)*/
table_element_t *hashTable[37] = {NULL};
/*----------------------------------------------*/

/*Hash Fucntion utilizada para a hash table*/
unsigned int hash_function(const char *str, unsigned int s)
{
    printf("hash fucntion");

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
void print_linked_list(node_t *head)
{
    printf("PRINT LINKED LIST\n");
    node_t *current = head;
    while (current != NULL)
    {
        printf("(%ld,%ld)\n", head->node_data.word_num, head->node_data.word_pos);
        current = current->next;
    }
}
/*----------------------------------------------*/

/*Insere elemento na hash Table*/
void insert(char word[64], long word_pos, long word_num)
{
    printf("INSERT");
    int hcode = hash_function(word, 37);
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->node_data.word_num = word_num;
    node->node_data.word_pos = word_pos;

    if (hashTable[hcode] == NULL)
    {
        printf("INSIDE IF");
        table_element_t *element = (table_element_t *)malloc(sizeof(table_element_t));
        strcpy(element->key, word);
        printf("hashTable[hcode]->linked_list[0] = node;");
        hashTable[hcode]->linked_list[0] = node;
        printf("return");
        return;
    }
    printf("PASS if\n");
    node_t *current = (node_t *)malloc(sizeof(node_t));

    hashTable[hcode]->linked_list[0] = current;
    while (current != NULL)
    {
        current = current->next;
    }

    current->node_data.word_num = word_num;
    current->node_data.word_pos = word_pos;
    printf("PUSH DONE\n");
    return;
}
/*----------------------------------------------*/

int main(int argc, char const *argv[])
{
    printf("MAIN");
    insert("abc", 1, 2);
    printf("insert1");
    insert("def", 3, 4);
    insert("qwe", 5, 6);
    insert("rty", 7, 8);
    insert("uio", 9, 10);

    for (int i = 0; i < 37; i++)
    {
        if (hashTable[i] != NULL)
        {
            print_linked_list(hashTable[i]->linked_list[0]);
        }
    }

    return 0;
}
