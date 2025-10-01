#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#include "hashtable.c"
#include "banlist.c"
#include "sort.c"

#define SORT_SIZE 5


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
        if (endOfWord(c)) {
            if (i == 0) continue;
            hashTableAddWord(currentWord);
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
    if (argc == 4) csv = fopen(argv[2], "r");
    else csv = fopen("blankfile.txt", "r");
    if (file == NULL || csv == NULL) {
        perror("Error parsing file");
        return EXIT_FAILURE;
    }
    chunkWords(file);
    fclose(file);
    //printTable();
    word* first = sort(csv, argv[3]);
    fclose(csv);
    freeList(first);
    //freeTable();
    return EXIT_SUCCESS;
}
