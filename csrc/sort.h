#ifndef SORT_H
#define SORT_H

#define _CRT_SECURE_NO_WARNINGS
#include "hashtable.h"
#include "banlist.h"

word* getMiddle(word* head);
word* mergeSort(word* head);
word *sort(FILE *bannedWordsList, char *name, int scentenceCount);

#endif
