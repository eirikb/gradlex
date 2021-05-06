#include "./sum.h"
#include <stdio.h>
#include <urlmon.h>
#pragma comment(lib, "Urlmon.lib")

int __cdecl main() {
    const char *destFile = "gradlex2";

    char filename[100];
    snprintf(filename, 100, "gradlex2-%llu", sum);

    char path[1000];
    snprintf(path, 100, "http://gradlex.blob.core.windows.net/poc/%s", filename);

    printf("Downloading %s...\n", path);
    if (S_OK == URLDownloadToFile(NULL, path, destFile, 0, NULL)) {
        printf("Done!\n");
    } else {
        printf("Failed\n");
        return 1;
    }

    // Only POC!
    // Required because of http (no https support)
    printf("Checking the sum...\n");
    FILE *fp = fopen("gradlex2", "rb");
    unsigned long long checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
        checksum += fgetc(fp);
    }
    fclose(fp);

    if (checksum != sum) {
        printf("Checksum failed :(\n");
        return 1;
    }
    printf("Done!\n");
    return 0;
}