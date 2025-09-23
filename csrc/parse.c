#define __STDC_WANT_LIB_EXT1__ 1

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>


#define BUFFER_SIZE 64 // longest english word is 45 letters, this is sufficient for reasonable cases
#define HASH_TABLE_SIZE 1024 // bigger number = faster look ups and more memmory use
#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET_BASIS 0xcbf29ce484222325

typedef struct word {
    char text[BUFFER_SIZE];
    int instances; // use for word count
    struct word *next;
} word;

word *hashTable[HASH_TABLE_SIZE];
int scentenceCount = 0;
int wordCount = 0;
int uniqueWords = 0; // currently unused
word *biggest[5] = {0};

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

word *makeWord(char* text) {
    word *output = (word*) malloc(sizeof(word));
    strcpy_s(output->text, BUFFER_SIZE, text);
    //uniqueWords += 1;
    return output;
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

void sort() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i] == NULL) continue;
        word *tmp = hashTable[i];
        while (tmp != NULL) {
            printWord(tmp);
            tmp = tmp->next;
        }
    }
}

void freeTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i] == NULL) continue;
        word *tmp = hashTable[i];
        while (tmp != NULL) {
            word *next = tmp->next;
            free(tmp);
            tmp = next;
        }
    }
}

bool endOfWord(char c) {
    if (c == ' ' || c == ',' || c == '\n') return true;
    if (c == '.' || c == '!' || c == '?') {
        scentenceCount += 1;
        return true;
    }
    return false;
}

void chunkWords(FILE *file) {
    char currentWord[BUFFER_SIZE];
    char c;
    int i = 0;
    while ((c = fgetc(file)) != EOF) {
        c = tolower(c);
        if (endOfWord(c)) {
            if (i == 0) continue;
            currentWord[i] = '\0';
            hashTableAddWord(currentWord);
            i = 0;
            continue;
        }
        currentWord[i] = c;
        i++;
    }
}

int main(int argc, char** argv) {
    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL) return 1;
    chunkWords(file);
    printTable();
    //sort(); // likely final placement of sort
    freeTable();
    fclose(file);
    return 0;
}
