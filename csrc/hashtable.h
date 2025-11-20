#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 64 // longest english word is 45 letters, this is sufficient for reasonable cases
#define HASH_TABLE_SIZE 2048 // bigger number = faster look ups and more memmory use
#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET_BASIS 0xcbf29ce484222325

typedef struct word {
    char text[BUFFER_SIZE];
    int instances; // use for word count
    struct word *next;
} word;

extern word *hashTable[HASH_TABLE_SIZE];
extern int wordCount;

word *makeWord(char* text);
uint64_t hash(char* text);
void hashTableInsert(word *w, int index);
void hashTableAddWord(char *text);
void printWord(word *w);
void printTable();

#endif

