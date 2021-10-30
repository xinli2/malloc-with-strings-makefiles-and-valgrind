#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 14
#define LINE 100

char *arrayOfStrings[SIZE];
int j = 0;
int full = 0;


char *readOneStr(FILE *fp, int *err);

// insert string and resort array
void insertStr(char *r)
{
    int i = 0;
    int k = 0;
    char *tmp = NULL;
    if (j == SIZE) {
        // replace first str
        free(arrayOfStrings[0]);
        for (i = 0; i < SIZE-1; i++) {
            arrayOfStrings[i] = arrayOfStrings[i+1];
        }
        // insert new str
        arrayOfStrings[j-1] = r;
    }
    else {
        // insert new str
        arrayOfStrings[j++] = r;
    }

    // sort the strs
    for (i = 0; i < j; i++) {
        for (k = i; k < j; k++) {
            if (arrayOfStrings[i] == NULL || arrayOfStrings[k] == NULL)
                continue;
            if (strcmp(arrayOfStrings[i], arrayOfStrings[k]) > 0) {
                tmp = arrayOfStrings[i];
                arrayOfStrings[i] = arrayOfStrings[k];
                arrayOfStrings[k] = tmp;
            }
        }
    }
}

char *readOneStr(FILE *fp, int *err)
{
    int n = 0;
    char c;
    int i = 0;
    char *r = NULL;
    char buf[LINE];
    int ret = 0;
    int k;

    while (1) {
        // if file is end, return NULL
        if (feof(fp)) return NULL;
        // read n
        ret = fscanf(fp, "%d", &n);
        // if n is negative, return NULL
        if (ret == -1) return NULL;
        if (n > 0) {
            // alloc memory for str
            r = (char *) malloc(sizeof(char) * (n + 1));
            // init str
            memset(r, 0, sizeof(char) * (n + 1));
            if (r == NULL) return NULL;
            // read str
            fscanf(fp, "%s", buf);
            strncpy(r, buf, n);

            insertStr(r);
            // output current strings
            printf("Current strings:\n  0: HEAD\n");
            for (k = 0; k < SIZE; k++) {
                if (arrayOfStrings[k] == NULL) {
                    printf("  %d: <null>\n", k + 1);
                } else {
                    printf("  %d: %s\n", k + 1, arrayOfStrings[k]);
                }
            }
            printf("  %d: TAIL\n", k + 1);
            for (i = n; i < strlen(buf); i++) {
                c = buf[i];
                printf("OOPS: Non-numeric input detected.  The byte was: 0x%x='%c'.\n", c, c);
                *err = 1;
            }
            break;
        } else if (n < 0) {
            printf("OOPS: The length read from input, %d, was zero or negative.\n", n);
        }
    }

    return r;
}


int main(int argn, char *argv[]) {

    int i;
    int k;
    FILE *fp;
    int err = 0;
    char *ret = NULL;
    printf("There are %d command-line arguments\n", argn-1);
    for (k = 0; k < SIZE; k++) {
        arrayOfStrings[k] = NULL;
    }
    for (i = 1; i < argn; i++) {
        // open file
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "ERROR: Cannot open the file '%s'\n", argv[i]);
        }
        else {
            printf("--- FILE: %s (argv[%d]) ---\n", argv[i], i);
            do {
                // read one str
                ret = readOneStr(fp, &err);
            } while (ret != NULL);
            printf("--- END OF FILE ---\n");

            fclose(fp);
        }
    }
    // free memory
    for (k = 0; k < SIZE; k++) {
        if (arrayOfStrings[k] != NULL) {
            free(arrayOfStrings[k]);
            arrayOfStrings[k] = NULL;
        }
    }
    return err;
}
