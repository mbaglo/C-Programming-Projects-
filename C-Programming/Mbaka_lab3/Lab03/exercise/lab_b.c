/****************************************************************************
  Lab 3 student file
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *p;

    p = (char *) malloc(19); /* Allocation #1 */
    if (NULL == p) {
        printf("Malloc error\n");
        exit(99);
    }
    strncpy(p, "word 1", 19);
    printf("%s\n", p);
    free(p); // Free memory from Allocation #1

    p = (char *) malloc(12); /* Allocation #2 */
    if (NULL == p) {
        printf("Malloc error\n");
        exit(99);
    }
    strncpy(p, "word 2", 12);
    printf("%s\n", p);
    free(p); // Free memory from Allocation #2

    p = (char *) malloc(16); /* Allocation #3 */
    if (NULL == p) {
        printf("Malloc error\n");
        exit(99);
    }
    strncpy(p, "word 3", 16);
    printf("%s\n", p);
    free(p); // Free memory from Allocation #3

    return 0;
}
