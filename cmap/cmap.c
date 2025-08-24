#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h> // Required for WebAssembly exports

// The core node structure
typedef struct MapNode
{
    int key;
    int value;
    struct MapNode *next_bucket;
} MapNode;

// The main Map structure
typedef struct
{
    MapNode **buckets;
    int num_buckets;
} Map;

// Simple hash function for integers
unsigned long hash_function(int key, int num_buckets)
{
    // A simple modulo hash is sufficient for this example
    return (unsigned long)key % num_buckets;
}

// --- Functions Exported to JavaScript ---

// Creates a map and returns a pointer to it
EMSCRIPTEN_KEEPALIVE
Map *map_create(int num_buckets)
{
    Map *map = (Map *)malloc(sizeof(Map));
    if (!map)
        return NULL;

    map->num_buckets = num_buckets;
    map->buckets = (MapNode **)calloc(num_buckets, sizeof(MapNode *));
    if (!map->buckets)
    {
        free(map);
        return NULL;
    }
    return map;
}

// Sets a key-value pair
EMSCRIPTEN_KEEPALIVE
void map_set(Map *map, int key, int value)
{
    if (!map)
        return;
    unsigned long index = hash_function(key, map->num_buckets);
    MapNode *current = map->buckets[index];

    // Check if key already exists
    while (current)
    {
        if (current->key == key)
        {
            current->value = value; // Update
            return;
        }
        current = current->next_bucket;
    }

    // Key is new, create a node and add to the front of the bucket's chain
    MapNode *newNode = (MapNode *)malloc(sizeof(MapNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next_bucket = map->buckets[index];
    map->buckets[index] = newNode;
}

// Gets a value by key. Returns -1 if not found.
EMSCRIPTEN_KEEPALIVE
int map_get(Map *map, int key)
{
    if (!map)
        return -1;
    unsigned long index = hash_function(key, map->num_buckets);
    MapNode *current = map->buckets[index];

    while (current)
    {
        if (current->key == key)
        {
            return current->value;
        }
        current = current->next_bucket;
    }
    return -1; // A simple way to signal "not found"
}

// Frees the map's memory
EMSCRIPTEN_KEEPALIVE
void map_free(Map *map)
{
    if (!map)
        return;
    for (int i = 0; i < map->num_buckets; i++)
    {
        MapNode *current = map->buckets[i];
        while (current)
        {
            MapNode *temp = current;
            current = current->next_bucket;
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}