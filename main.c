#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct node
{
    int key;
    int value;
    struct node *next;
};

/* For storing a Linked List at each index of Hash Table  */
struct arrayitem
{

    struct node *head;
    /* head pointing the first element of Linked List at an index of Hash Table */

    struct node *tail;
    /* tail pointing the last element of Linked List at an index of Hash Table */
};

struct arrayitem *array;
int size = 0; /* Determines the no. of elements present in Hash Table */
int max = 10; /* Determines the maximum capacity of Hash Table array */

/* This function creates an index corresponding to the every given key */
int hashcode(int key)
{
    return (key % max);
}

struct node *get_element(struct node *list, int find_index);
void remove_element(int key);
void rehash();
void init_array();

void insert(int key, int value)
{
    float n = 0.0;
    /* n => Load Factor, keeps check on whether rehashing is required or not */

    int index = hashcode(key);

    /* Extracting Linked List at a given index */
    struct node *list = (struct node *)array[index].head;

    /* Creating an item to insert in the Hash Table */
    struct node *item = (struct node *)malloc(sizeof(struct node));
    item->key = key;
    item->value = value;
    item->next = NULL;

    if (list == NULL)
    {
        /* Absence of Linked List at a given Index of Hash Table */

        printf("Inserting %d(key) and %d(value) \n", key, value);
        array[index].head = item;
        array[index].tail = item;
        size++;
    }
    else
    {
        /* A Linked List is present at given index of Hash Table */

        int find_index = find(list, key);
        if (find_index == -1)
        {
            /*
			 *Key not found in existing linked list
			 *Adding the key at the end of the linked list
			*/

            array[index].tail->next = item;
            array[index].tail = item;
            size++;
        }
        else
        {
            /*
			 *Key already present in linked list
			 *Updating the value of already existing key
			*/

            struct node *element = get_element(list, find_index);
            element->value = value;
        }
    }

    //Calculating Load factor
    n = (1.0 * size) / max;
    if (n >= 0.75)
    {
        //rehashing

        printf("going to rehash\n");
        rehash();
    }
}

void rehash()
{
    struct arrayitem *temp = array;
    /* temp pointing to the current Hash Table array */

    int i = 0, n = max;
    size = 0;
    max = 2 * max;

    /*
	 *array variable is assigned with newly created Hash Table
	 *with double of previous array size
	*/
    array = (struct arrayitem *)malloc(max * sizeof(struct node));
    init_array();

    for (i = 0; i < n; i++)
    {

        /* Extracting the Linked List at position i of Hash Table array */
        struct node *list = (struct node *)temp[i].head;

        if (list == NULL)
        {

            /* if there is no Linked List, then continue */
            continue;
        }
        else
        {
            /*
			 *Presence of Linked List at i
			 *Keep moving and accessing the Linked List item until the end.
			 *Get one key and value at a time and add it to new Hash Table array.
			*/

            while (list != NULL)
            {
                insert(list->key, list->value);
                list = list->next;
            }
        }
    }
    temp = NULL;
}

/*
 *This function finds the given key in the Linked List
 *Returns it's index
 *Returns -1 in case key is not present
*/
int find(struct node *list, int key)
{
    int retval = 0;
    struct node *temp = list;
    while (temp != NULL)
    {
        if (temp->key == key)
        {
            return retval;
        }
        temp = temp->next;
        retval++;
    }
    return -1;
}

/* Returns the node (Linked List item) located at given find_index  */
struct node *get_element(struct node *list, int find_index)
{
    int i = 0;
    struct node *temp = list;
    while (i != find_index)
    {
        temp = temp->next;
        i++;
    }
    return temp;
}

/* To remove an element from Hash Table */
void remove_element(int key)
{
    int index = hashcode(key);
    struct node *list = (struct node *)array[index].head;

    if (list == NULL)
    {
        printf("This key does not exists\n");
    }
    else
    {
        int find_index = find(list, key);

        if (find_index == -1)
        {
            printf("This key does not exists\n");
        }
        else
        {
            struct node *temp = list;
            if (temp->key == key)
            {

                array[index].head = temp->next;
                printf("This key has been removed\n");
                return;
            }

            while (temp->next->key != key)
            {
                temp = temp->next;
            }

            if (array[index].tail == temp->next)
            {
                temp->next = NULL;
                array[index].tail = temp;
            }
            else
            {
                temp->next = temp->next->next;
            }

            printf("This key has been removed\n");
        }
    }
}

/* To display the contents of Hash Table */
void display()
{
    int i = 0;
    for (i = 0; i < max; i++)
    {
        struct node *temp = array[i].head;
        if (temp == NULL)
        {
            printf("array[%d] has no elements\n", i);
        }
        else
        {
            printf("array[%d] has elements-: ", i);
            while (temp != NULL)
            {
                printf("key= %d  value= %d\t", temp->key, temp->value);
                temp = temp->next;
            }
            printf("\n");
        }
    }
}

/* For initializing the Hash Table */
void init_array()
{
    int i = 0;
    for (i = 0; i < max; i++)
    {
        array[i].head = NULL;
        array[i].tail = NULL;
    }
}

/* Returns size of Hash Table */
int size_of_array()
{
    return size;
}

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

    display();
    return 0;
}
