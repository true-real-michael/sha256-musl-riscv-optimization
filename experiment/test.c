//
// Created by michael on 11/28/22.
//
//#include <crypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int parseArgs(int argc, char **argv, int *messageLen, char *inputPath, char *outputPath) {
    if (argc < 4)
        return 1;

    char *ptr;
    *messageLen = (int)strtol(argv[1], &ptr, 10);
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
        if (fgets(inputStrings + i * (messageLen+1), messageLen, file) == NULL) {

//        if (fgets(inputStrings[i], messageLen, file) == NULL) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}


int writeOutput(char *path, char *results, int nMessages, struct timeval start, struct timeval stop) {
    FILE *file = fopen(path, "a");
    double secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

    fprintf(file, "%lf\n", secs);

    for (int i = 0; i < nMessages; i++) {
        fprintf(file, "%s\n", results + (i * 64));
    }

    fclose(file);
}

int writeOutputTime(char *path, int nMessages, struct timeval start, struct timeval stop) {
    FILE *file = fopen(path, "a");
    double secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

    fprintf(file, "%lf\n", secs);

    fclose(file);
}

int main(int argc, char **argv) {
//    const int totalBytes = 1024 * 512;
    const int totalBytes = 1024 * 16;
    int messageLen;
    char outputPath[256];
    char inputPath[256];
    char setting[] = "$5$";
    char *hash;


    if (parseArgs(argc, argv, &messageLen, inputPath, outputPath)) {
        printf("Incorrect syntax. Expected arguements:\n"
               "\t1. Int from 1 to 256\tLength of a single message\n"
               "\t2. Valid path to a file\tInput file\n"
               "\t3. Valid path to a file\tOutput file\n");
        return 1;
    }

    int nMessages = totalBytes / messageLen;
    char *inputStrings = calloc(totalBytes + nMessages, sizeof(char));
    char *results = calloc(nMessages * 64, sizeof(char));

    if (loadInput(inputPath, inputStrings, nMessages, messageLen)) {
        printf("Failed to load data\n");
        return 1;
    }



    printf("running...\n");
    struct timeval start, stop;
    gettimeofday(&start, NULL);

    for (int i = 0; i < nMessages; i ++) {
        hash = crypt(inputStrings + i * (messageLen + 1), setting);
//        puts(hash);
    }

    gettimeofday(&stop, NULL);

    writeOutputTime(outputPath, nMessages, start, stop);




//    for (int i = 0; i < еще; i++) {
//        fgets(key, KEY_LEN, fin);
//        char *retval = crypt(key, setting);
//        fputs(retval, fout);
//        fputc('\n', fout);
//    }
//
//    gettimeofday(&stop, NULL);
//
//    fclose(fout);
//    fclose(fin);
//
//    printf("done\n");
//
//    double secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
//    long double rate = KEY_LEN * KEY_NUM / 1024 / secs;
////    printf("took %lu s, %lu us\n", stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec);
//    printf("MODI: took %lf\n", secs);
//    printf("MODI: throughput: %Lf KB / Sec\n", rate);

    return 0;
}
