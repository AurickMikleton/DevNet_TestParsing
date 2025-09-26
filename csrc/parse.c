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
#define SORT_SIZE 5

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
    strcpy(output->text, text);
    uniqueWords += 1;
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

// Function to merge two sorted linked lists
word* sorted_Merge(word* x, word* y) {
    word* result = NULL;

    // Base cases for recursion
    if (!x)
        return y;
    if (!y)
        return x;

    // Pick either node from x or y, and continue merging
    if (x->instances >= y->instances) {
        result = x;
        result->next = sorted_Merge(x->next, y);
    } else {
        result = y;
        result->next = sorted_Merge(x, y->next);
    }
    return result;
}

// Function to find the middle of a linked list
word* getMiddle(word* head) {
    if (!head)
        return head;

    word *slow = head, *fast = head;
    // Use slow and fast pointers to find the middle node
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Function implementing the merge sort algorithm for linked list
word* mergeSort(word* head) {
    if (!head || !head->next)
        return head;

    // Split the linked list into two halves
    word *middle = getMiddle(head);
    word *nextOfMiddle = middle->next;
    middle->next = NULL;

    // Recursively sort the two halves
    word *left = mergeSort(head);
    word *right = mergeSort(nextOfMiddle);

    // Merge the sorted halves
    word *sortedList = sorted_Merge(left, right);
    return sortedList;
}

word *sort() {
    word* first = NULL;
    word* linkHead = NULL;
    word *prev = NULL;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hashTable[i] == NULL) continue;
        word *tmp = hashTable[i];
        if (first == NULL) first = tmp;
        if (linkHead != NULL) linkHead->next = tmp;
        while (tmp != NULL) {
            prev = tmp;
            tmp = tmp->next;
        }
        linkHead = prev;
    }
    word* tmp = mergeSort(first);
    int i = 0;
    while (i < 5 && tmp != NULL) {
        if (false) continue; // check if word is on "banned list"
        printWord(tmp);
        tmp = tmp->next;
        i++;
    }
    return first;
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
    fclose(file);
    //printTable();
    word* first = sort();
    freeList(first);
    //freeTable();
    return 0;
}
