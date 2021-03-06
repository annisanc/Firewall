#include "hash.h"

#include "speck.h"

#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

// Based off of design from Lab Document
HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        // Il nome della rosa
        ht->salt[0] = 0x85ae998311115ae3;
        ht->salt[1] = 0xb6fac2ae33a40089;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// Lists are checked before freeing them
// so no exxessive freeing occurs
void ht_delete(HashTable **ht) {
    for (int i = (*ht)->size - 1; i >= 0; i--) {
        if ((*ht)->lists[i] != NULL) {
            ll_delete(&(*ht)->lists[i]);
        }
    }
    free((*ht)->lists);
    (*ht)->lists = NULL;
    free(*ht);
    *ht = NULL;
    return;
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
    return ll_lookup(ht->lists[index], oldspeak);
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
    if (ht->lists[index] == NULL) {
        ht->lists[index] = ll_create(ht->mtf);
    }
    ll_insert(ht->lists[index], oldspeak, newspeak);
    return;
}

void ht_print(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->lists[i] != NULL) {
            ll_print(ht->lists[i]);
        }
    }
    return;
}