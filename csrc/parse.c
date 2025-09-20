#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 64 // longest english word is 45 letters, this is sufficient for reasonable cases
#define HASH_TABLE_SIZE 255 // bigger number = faster look ups and more memmory use
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

// FNV - 1 hash
unsigned int hash(char* text) {
    int length = strnlen(text, BUFFER_SIZE);
    unsigned long long hashValue = FNV_OFFSET_BASIS;
    for (int i = 0; i < length; i++) {
        hashValue *= FNV_PRIME;
        hashValue ^= text[i];
    }
    return hashValue % HASH_TABLE_SIZE;
}

word *makeWord(char* text) {
    word *output = (word*) malloc(sizeof(word));
    strcpy_s(output->text, BUFFER_SIZE, text);
    return output;
}

// TODO: pass in index to remove redundant process
void hashTableInsert(word *w) {
    if (w == NULL) return;
    int index = hash(w->text);
    w->next = hashTable[index];
    hashTable[index] = w;
}

void hashTableAddWord(char *text) {
    int index = hash(text);
    word *tmp = hashTable[index];
    while (tmp != NULL && strncmp(text, tmp->text, BUFFER_SIZE) != 0) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        word *newWord = makeWord(text);
        newWord->instances = 1;
        newWord->next = NULL;
        hashTableInsert(newWord);
    } else {
        tmp->instances += 1;
    }
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
        i+=1;
    }
}

int main() {
    FILE *file;
    fopen_s(&file, "foo.txt", "r");
    if (file == NULL) return 1;
    chunkWords(file);
    printTable();
    freeTable();
    fclose(file);
    return 0;
}
