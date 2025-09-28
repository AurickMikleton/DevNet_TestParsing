#ifndef BANLIST_C
#define BANLIST_C

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

#endif
