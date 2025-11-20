#include "hashtable.h"

int uniqueWords = 0; // currently unused
word *hashTable[HASH_TABLE_SIZE];
int wordCount = 0;

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
