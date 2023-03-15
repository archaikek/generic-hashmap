#include <stdlib.h>

typedef struct list_t list_t;
typedef struct hashmap_t hashmap_t;
typedef struct hashmap_header_t hashmap_header_t;

struct list_t
{
    list_t *prev, *next;
    void *data;
};
struct hashmap_t
{
    hashmap_t *prev, *next;
    list_t *head;
    int hash;
};
struct hashmap_header_t
{
    hashmap_t *map;
    int (*hash_fun)(const void*);
    void (*print)(const void*);
    int (*is_equal)(const void*, const void*);
    void (*remove)(void*);
};

// basic list functions
// creates an empty item with no connections
list_t *create_item();
// returns a pointer to the data stored in the list item and unlinks it from the list
void *unlink_item(list_t *item);

// block list functions
// prints list's contents
void print_list(list_t *head, void (*print)(const void*));
// remove the entire list
void remove_list(list_t *head, void (*remove)(void*));

// basic hashmap functions
// create an new hash with no connections and a single (empty) list item as its head
hashmap_t *create_hash(const int hashnum);
// print the list bound to the hash specified
void print_hash(hashmap_t *hash, void (*print)(const void*));
// remove the list bound to the hash and the hash itself
void remove_hash(hashmap_t *hash, void (*remove)(void*));

// block hashmap functions
// create a new hashmap with a single (unusable and empty) hash and provided functions to operate on the data it will store
hashmap_header_t *create_hashmap(int (*hash_fun)(const void*), void (*print)(const void*), int (*is_equal)(const void*, const void*), void (*remove)(void*));
// print all contents from the hashmap
void print_hashmap(hashmap_header_t *hashmap);
// remove all contents of the hashmap and the hashmap itself
void remove_hashmap(hashmap_header_t *hashmap);

// data manipulation functions
// return the hash to which the current item should be inserted into
hashmap_t *find_hash_to_insert(hashmap_header_t *hashmap, const int hash);
// return the hash containing the item to be removed
hashmap_t *find_hash_to_remove(hashmap_header_t *hashmap, const int hash);
// insert the item into the hashmap
void insert(hashmap_header_t *hashmap, void *data);
// remove the given item from the hashmap (do nothing if the item is not there)
void *find(hashmap_header_t *hashmap, void *data);