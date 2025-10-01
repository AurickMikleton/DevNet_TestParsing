#ifndef HASHTABLE_C
#define HASHTABLE_C

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 64 // longest english word is 45 letters, this is sufficient for reasonable cases
#define HASH_TABLE_SIZE 2048 // bigger number = faster look ups and more memmory use
#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET_BASIS 0xcbf29ce484222325

int wordCount = 0;
int uniqueWords = 0; // currently unused
                     //
typedef struct word {
    char text[BUFFER_SIZE];
    int instances; // use for word count
    struct word *next;
} word;

word *hashTable[HASH_TABLE_SIZE];

word *makeWord(char* text) {
    word *output = (word*) malloc(sizeof(word));
    strcpy(output->text, text);
    uniqueWords += 1;
    return output;
}

// FNV - 1 hash
uint64_t hash(char* text) {
    int length = strnlen(text, BUFFER_SIZE);
    uint64_t hashValue = FNV_OFFSET_BASIS;
    for (int i = 0; i < length; i++) {
        hashValue *= FNV_PRIME;
        hashValue ^= text[i];
    }
    return hashValue % HASH_TABLE_SIZE;
}

void hashTableInsert(word *w, int index) {
    if (w == NULL) return; // likely redundant, to be removed later
    w->next = hashTable[index];
    hashTable[index] = w;
}

void hashTableAddWord(char *text) {
    wordCount += 1;
    int index = hash(text);
    word *tmp = hashTable[index];
    while (tmp != NULL && strncmp(text, tmp->text, BUFFER_SIZE) != 0) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        word *newWord = makeWord(text);
        newWord->instances = 0;
        newWord->next = NULL;
        hashTableInsert(newWord, index);
        tmp = newWord;
    }
    tmp->instances += 1;
}

void printWord(word *w) {
    if (w == NULL) return;
    printf("('%s', %d)\n", w->text, w->instances);
}

void printTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i] == NULL) continue;
        word *tmp = hashTable[i];
        while (tmp != NULL) {
            printWord(tmp);
            tmp = tmp->next;
        }
    }
}

#endif
