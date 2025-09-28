#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "hashtable.c"
#include "banlist.c"
#include "sort.c"

#define SORT_SIZE 5

int scentenceCount = 0;

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

void freeList(word *first) {
    word *tmp = first;
    while (tmp != NULL) {
        word *next = tmp->next;
        free(tmp);
        tmp = next;
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

void chunkWords(FILE *file) { //count one "***" as a word
    char currentWord[BUFFER_SIZE];
    char c;
    int i = 0;
    bool textInBook = false;
    int astriskTagCount = 0;
    while ((c = fgetc(file)) != EOF) {
        c = tolower(c);
        currentWord[i] = '\0';
        if (strcmp(currentWord, "***") == 0) {
            astriskTagCount += 1;
            if (textInBook) {
                break;
            }
            else if (astriskTagCount > 1){
                textInBook = true;
            }
        }
        if (endOfWord(c)) {
            if (i == 0) continue;
            if (textInBook) hashTableAddWord(currentWord);
            i = 0;
            continue;
        }
        currentWord[i] = c;
        i++;
    }
}

int main(int argc, char** argv) {
    FILE *file, *csv;
    file = fopen(argv[1], "r");
    csv = fopen(argv[2], "r");
    if (file == NULL || csv == NULL) return 1;
    chunkWords(file);
    fclose(file);
    //printTable();
    word* first = sort(csv);
    fclose(csv);
    freeList(first);
    //freeTable();
    return 0;
}
