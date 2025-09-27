#include <stddef.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "hashtable.c"

#define SORT_SIZE 5

int scentenceCount = 0;
word *biggest[5] = {0};

bool banned(char *w, FILE *bannedList) {
	
	int wLength = strlen(w);
	int counter = 0;
	char c;
	bool isNotWord = false;

	while ((c = fgetc(bannedList)) != EOF) {
		if (isNotWord) {
			if (c != ',') continue;
			isNotWord = false;
			continue;
		}
		if (c == w[counter]) counter++;
		else isNotWord = true;
		if (c == ',') {
			if (counter == wLength) return true;
			counter = 0;
		}
		if (counter > wLength) isNotWord = true;
	}
	return false;

}

word* sorted_Merge(word* x, word* y) {
    word* result = NULL;

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

word *sort(FILE *bannedWordsList) {
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
        if (banned(tmp->text, bannedWordsList)) {
            tmp = tmp->next;
            continue;
        } // check if word is on "banned list"
        rewind(bannedWordsList); // sets file pointer back to start
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
