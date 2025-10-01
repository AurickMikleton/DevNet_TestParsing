#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 64

bool endOfWord(char c) {
    if (c == ' ' || c == ',' || c == '\n') return true;
    if (c == '.' || c == '!' || c == '?') return true;
    return false;
}

void parse(FILE *file, char* name) { //count one "***" as a word
    FILE *newFile;
    char newFileName[256] = "parsed/";
    strcat(newFileName, name);
    printf("%s \n", newFileName);
    newFile = fopen(newFileName, "w");
    if (newFile == NULL) {
        perror("Error parsing file");
        return;
    }

    char currentWord[BUFFER_SIZE];
    char c;
    int i = 0;
    bool textInBook = false;
    int astriskTagCount = 0;

    while ((c = fgetc(file)) != EOF) {
        c = tolower(c);
        currentWord[i] = '\0';
        if (textInBook) fputc(c, newFile);
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
            i = 0;
            continue;
        }
        currentWord[i] = c;
        i++;
    }
    fclose(newFile);
}

int main(int argc, char** argv) {
    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error parsing file");
        return EXIT_FAILURE;
    }
    parse(file, argv[1]);
    fclose(file);
    return EXIT_SUCCESS;
}
