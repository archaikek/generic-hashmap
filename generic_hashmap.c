#include <stdio.h>
#include "generic_hashmap.h"

// basic list functions
// creates an empty item with no connections
list_t *create_item()
{
    list_t *item = (list_t*)malloc(sizeof(list_t));
    item->prev = item->next = NULL;
    item->data = NULL;
    return item;
}
// returns a pointer to the data stored in the list item and unlinks it from the list
void *unlink_item(list_t *item)
{
    if (item->prev != NULL) item->prev->next = item->next;
    if (item->next != NULL) item->next->prev = item->prev;
    return item->data;
}
// block list functions
// prints list's contents
void print_list(list_t *head, void (*print)(const void*))
{
    list_t *current = head->next;
    while (current != NULL)
    {
        printf("(");
        print(current->data);
        printf("); ");
        current = current->next;
    }
}
// remove the entire list
void remove_list(list_t *head, void (*remove)(void*))
{
    list_t *next;
    while (head != NULL)
    {
        next = head->next;
        void *data = unlink_item(head);
        if (data != NULL)
        {
            remove(data);
        }
        head = next;
    }
}

// basic hashmap functions
// create an new hash with no connections and a single (empty) list item as its head
hashmap_t *create_hash(const int hashnum)
{
    hashmap_t *hash = (hashmap_t*)malloc(sizeof(hashmap_t));
    hash->prev = hash->next = NULL;
    hash->head = create_item();
    hash->hash = hashnum;
    return hash;
}
// print the list bound to the hash specified
void print_hash(hashmap_t *hash, void (*print)(const void*))
{
    printf("%d: ", hash->hash);
    print_list(hash->head, print);
    printf("\n");
}
// remove the list bound to the hash and the hash itself
void remove_hash(hashmap_t *hash, void (*remove)(void*))
{
    if (hash->prev != NULL) hash->prev->next = hash->next;
    if (hash->next != NULL) hash->next->prev = hash->prev;
    if (hash->head != NULL)
    {
        remove_list(hash->head, remove);
    }
    free(hash);
}

// block hashmap functions
// create a new hashmap with a single (unusable and empty) hash and provided functions to operate on the data it will store
hashmap_header_t *create_hashmap(int (*hash_fun)(const void*), void (*print)(const void*), int (*is_equal)(const void*, const void*), void (*remove)(void*))
{
    hashmap_header_t *hashmap = (hashmap_header_t*)malloc(sizeof(hashmap_header_t));
    hashmap->map = create_hash(-1);
    hashmap->hash_fun = hash_fun;
    hashmap->print = print;
    hashmap->is_equal = is_equal;
    hashmap->remove = remove;
    return hashmap;
}
// print all contents from the hashmap
void print_hashmap(hashmap_header_t *hashmap)
{
    hashmap_t *current = hashmap->map;
    printf("{\n");
    while (current != NULL)
    {
        print_hash(current, hashmap->print);
        current = current->next;
    }
    printf("}\n\n");
}
// remove all contents of the hashmap and the hashmap itself
void remove_hashmap(hashmap_header_t *hashmap)
{
    hashmap_t *current = hashmap->map, *next;
    while (current != NULL)
    {
        next = current->next;
        remove_hash(current, hashmap->remove);
        current = next;
    }
    free(hashmap);
}

// data manipulation functions
// return the hash to which the current item should be inserted into
hashmap_t *find_hash_to_insert(hashmap_header_t *hashmap, const int hash)
{
    hashmap_t *current = hashmap->map;
    while (current != NULL) // break on list end
    {
        if (current->hash == hash) // matching hash found
        {
            return current;
        }
        else if (current->hash < hash) // target hash is further in the list
        {
            if (current->next != NULL) current = current->next;
            else break; // don't move past the list
        }
        else // target hash skipped
        {
            current = current->prev;
            break; // new hash needs to be created and inserted as current->next
        }
    }
    hashmap_t *new_hash = create_hash(hash);
    new_hash->next = current->next;
    new_hash->prev = current;
    current->next = new_hash;
    return new_hash;
}
// return the hash containing the item to be removed
hashmap_t *find_hash_to_remove(hashmap_header_t *hashmap, const int hash)
{
    hashmap_t *current = hashmap->map;
    while (current != NULL) // break on last existing hash
    {
        if (current->hash == hash) // matching hash found
        {
            return current;
        }
        else if (current->hash < hash) // target hash is further in the list
        {
            current = current->next;
        }
        else // target hash skipped
        {
            return NULL; // no matching hash found
        }
    }
    return NULL;
}
// insert the item into the hashmap
void insert(hashmap_header_t *hashmap, void *data)
{
    const int hash = hashmap->hash_fun(data);
    hashmap_t *current_hash = find_hash_to_insert(hashmap, hash);
    list_t *item = current_hash->head;
    while (item->next != NULL)
    {
        item = item->next;
        if (hashmap->is_equal(item->data, data))
        {
            return;
        }
    }
    list_t *new_item = create_item();
    new_item->data = data;
    new_item->prev = item;
    item->next = new_item;
}
// remove the given item from the hashmap (do nothing if the item is not there)
void *find(hashmap_header_t *hashmap, void *data)
{
    void *result = NULL;
    const int hash = hashmap->hash_fun(data);
    hashmap_t *current_hash = find_hash_to_remove(hashmap, hash);
    if (current_hash == NULL)
    {
        return result;
    }
    list_t *item = current_hash->head;
    while (item->next != NULL)
    {
        item = item->next;
        if (hashmap->is_equal(item->data, data))
        {
            result = unlink_item(item);
            break;
        }
    }
    if (current_hash->head->next == NULL) // no more items in this hash
    {
        remove_hash(current_hash, hashmap->remove);
    }
    return result;
}