#ifndef SORT_C
#define SORT_C

#include "hashtable.c"
#include "banlist.c"

int scentenceCount = 0;

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

word *sort(FILE *bannedWordsList, char *name) {
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
    printf("%s \\ ", name);
    while (i < 5 && tmp != NULL) {
        if (banned(tmp->text, bannedWordsList)) {
            tmp = tmp->next;
            continue;
        } // check if word is on "banned list"
        rewind(bannedWordsList); // sets file pointer back to start
        printf("%s ", tmp->text);
        tmp = tmp->next;
        i++;
    }
    printf("\\ %d\n", wordCount/scentenceCount);
    return first;
}

#endif
