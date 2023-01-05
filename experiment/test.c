//
// Created by michael on 11/28/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int parseArgs(int argc, char **argv, int *messageLen, char *inputPath, char *outputPath) {
    if (argc < 4)
        return 1;

    char *ptr;
    *messageLen = (int) strtol(argv[1], &ptr, 10);
    if (ptr == argv[1] || *messageLen > 256 || *messageLen < 1)
        return 1;

    strcpy(inputPath, argv[2]);
    strcpy(outputPath, argv[3]);
    if (access(inputPath, R_OK) || access(outputPath, W_OK))
        return 1;

    return 0;
}


int loadInput(char *path, char *inputStrings, int nMessages, int messageLen) {
    FILE *file = fopen(path, "r");

    for (int i = 0; i < nMessages; i++) {
        if (fgets(inputStrings + i * (messageLen + 1), messageLen, file) == NULL) {

            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}


int writeOutputTime(const char *path, const struct timespec *start, const struct timespec *stop) {
    FILE *file = fopen(path, "a");
    double secs = (double) (stop->tv_nsec - start->tv_nsec) / 1000000000 + (double) (stop->tv_sec - start->tv_sec);

    fprintf(file, "%lf\n", secs);
    printf("%lf\n", secs);

    fclose(file);

    return 0;
}


int main(int argc, char **argv) {
    const int totalBytes = 1024 * 16;
    int messageLen;
    char outputPath[256];
    char inputPath[256];
    char setting[] = "$5$";

    if (parseArgs(argc, argv, &messageLen, inputPath, outputPath)) {
        printf("Incorrect syntax. Expected arguements:\n"
               "\t1. Int from 1 to 256\tLength of a single message\n"
               "\t2. Valid path to a file\tInput file\n"
               "\t3. Valid path to a file\tOutput file\n");
        return 1;
    }

    int nMessages = totalBytes / messageLen;
    char *inputStrings = calloc(totalBytes + nMessages, sizeof(char));

    if (loadInput(inputPath, inputStrings, nMessages, messageLen)) {
        printf("Failed to load data\n");
        return 1;
    }

    printf("running...\n");

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < nMessages; i++) {
        crypt(inputStrings + i * (messageLen + 1), setting);
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);

    writeOutputTime(outputPath, &start, &stop);

    return 0;
}