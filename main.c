#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "generic_hashmap.h"

typedef struct string_t string_t;
typedef struct pair_t pair_t;

// ----------------------
// custom string type
struct string_t
{
    int size, mem_size;
    char *c;
};
void init_string(string_t *s)
{
    s->size = s->mem_size = 0;
    s->c = NULL;
}
string_t *create_string()
{
    string_t *new_string = (string_t*)malloc(sizeof(string_t));
    init_string(new_string);
    return new_string;
}
void push(string_t *s, const char c)
{
    if (s->size >= s->mem_size - 1)
    {
        s->mem_size *= 2;
        s->mem_size += 4;
        s->c = (char*)realloc(s->c, s->mem_size * sizeof(char));
        for (int i = s->size; i < s->mem_size; ++i)
        {
            s->c[i] = '\0';
        }       
        }
    s->c[s->size] = c;
    ++(s->size);
}
void read_string(string_t *s)
{
    char c = getchar();
    while (c <= ' ')
    {
        c = getchar();
        
    }
    while (c > ' ')
    {
        if (c == ' ')
        {
            break;     
        }
        push(s, c);
        c = getchar();     
    }  
}

int hash_string(const void *data)
{
    string_t *s = (string_t*)data;
    const int HASH_BASE = 147;
    const int MAX_HASH = 100007;

    int hash = 0;
    for (int i = 0; i < s->size; ++i)
    {
        hash *= HASH_BASE;
        hash += s->c[i];
        hash %= MAX_HASH;
    }
    return hash;
}
void print_string(const void *data)
{
    string_t *s = (string_t*)data;
    for (int i = 0; i < s->size; ++i)
    {
        putchar(s->c[i]);     
    }
}
int is_equal_string(const void *d1, const void *d2)
{
    string_t *s1 = (string_t*)d1, *s2 = (string_t*)d2;
    if (s1->size != s2->size)
    {
        return 0;
    }
    for (int i = 0; i < s1->size; ++i)
    {
        if(s1->c[i] != s2->c[i])
        {
            return 0;
        }
    }
    return 1;
}
void unload_string(void *data)
{
    string_t *s = (string_t*)data;
    if (s != NULL)
    {
        if (s->c != NULL)
        {
            free(s->c);
        }
        free(s);
    }
}

// regular int
int hash_int(const void *data)
{
    int *num = (int*)data;
    const int MAX_HASH = 100007;

    return (*num)%MAX_HASH;
}
void print_int(const void *data)
{
    int *num = (int*)data;
    printf("%d", *num);
}
int is_equal_int(const void *d1, const void *d2)
{
    int *n1 = (int*)d1, *n2 = (int*)d2;
    return (*n1) == (*n2);
}
void remove_int(void *data)
{
    free(data);
}

// pair of two ints
struct pair_t
{
    int first, second;
};
pair_t *create_pair()
{
    pair_t *new_pair = (pair_t*)malloc(sizeof(pair_t));
    new_pair->first = new_pair->second = 0;
    return new_pair;
}

int hash_pair(const void *data)
{
    pair_t *pair = (pair_t*)data;
    const long long int HASH_BASE = (1LL << 31) + 1;
    const int MAX_HASH = 100007;

    int hash = (HASH_BASE * pair->first + pair->second) % MAX_HASH;
    return hash;
}
void print_pair(const void *data)
{
    pair_t *pair = (pair_t*)data;
    printf("[%d, %d]", pair->first, pair->second);
}
int is_equal_pair(const void *d1, const void *d2)
{
    pair_t *p1 = (pair_t*)d1, *p2 = (pair_t*)d2;
    return (p1->first == p2->first && p1->second == p2->second);
}
void remove_pair(void *data)
{
    free(data);
}


int main()
{
    hashmap_header_t *string_map = create_hashmap(hash_string, print_string, is_equal_string, unload_string);
    hashmap_header_t *int_map = create_hashmap(hash_int, print_int, is_equal_int, remove_int);
    hashmap_header_t *pair_map = create_hashmap(hash_pair, print_pair, is_equal_pair, remove_pair);

    // string hashmap demonstration
    printf("Read 10 strings to insert:\n");
    for (int i = 0; i < 10; ++i)
    {
        string_t *string = create_string();
        read_string(string);
        insert(string_map, string);
    }
    print_hashmap(string_map);
    printf("Read 6 strings to insert:\n");
    for (int i = 0; i < 6; ++i)
    {
        string_t *string = create_string();
        read_string(string);
        insert(string_map, string);
    }
    print_hashmap(string_map);
    printf("Read 2 strings to delete:\n");
    for (int i = 0; i < 2; ++i)
    {
        string_t *string = create_string();
        read_string(string);
        unload_string((string_t*)find(string_map, string));
    }
    print_hashmap(string_map);

    // int hashmap demonstration
    printf("Read 10 ints to insert:\n");
    for (int i = 0; i < 10; ++i)
    {
        int *num = (int*)malloc(sizeof(int));
        scanf("%d", num);
        insert(int_map, num);
    }
    print_hashmap(int_map);
    printf("Read 2 ints to delete:\n");
    for (int i = 0; i < 2; ++i)
    {
        int *num = (int*)malloc(sizeof(int));
        scanf("%d", num);
        remove_int((int*)find(int_map, num));
    }
    print_hashmap(int_map);
    printf("Read 4 ints to insert:\n");
    for (int i = 0; i < 4; ++i)
    {
        int *num = (int*)malloc(sizeof(int));
        scanf("%d", num);
        insert(int_map, num);
    }
    print_hashmap(int_map);

    // pair hashmap demonstration
    printf("Read 5 pairs to insert:\n");
    for (int i = 0; i < 5; ++i)
    {
        pair_t *pair = (pair_t*)malloc(sizeof(pair_t));
        scanf("%d%d", &(pair->first), &(pair->second));
        insert(pair_map, pair);
    }   
    print_hashmap(pair_map);
    printf("Read 2 pairs to delete:\n");
    for (int i = 0; i < 2; ++i)
    {
        pair_t *pair = (pair_t*)malloc(sizeof(pair_t));
        scanf("%d%d", &(pair->first), &(pair->second));
        remove_pair((pair_t*)find(pair_map, pair));
    }   
    print_hashmap(pair_map);
    printf("Read 3 pairs to insert:\n");
    for (int i = 0; i < 3; ++i)
    {
        pair_t *pair = (pair_t*)malloc(sizeof(pair_t));
        scanf("%d%d", &(pair->first), &(pair->second));
        insert(pair_map, pair);
    }   
    print_hashmap(pair_map);

    remove_hashmap(pair_map);
    remove_hashmap(int_map);
    remove_hashmap(string_map);
    return 0;
}
/*
Dobry wieczor cos sie popsulo i nie bylo mnie slychac
wiec powtorze Dobry jeszcze raz wynik
wiec nie

1 2 3 4 5 6 7 100008 100009 100010
4 100011
6 7 100014 200021

1 2 3 4 5 6 7 8 1 100008
1 100009 7 8
1 100009 2 100010 6 5 
 
*/