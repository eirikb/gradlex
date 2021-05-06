#include <stdio.h>

int main() {
    FILE *fp = fopen("gradlex2", "rb");
    unsigned long long checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
        checksum += fgetc(fp);
    }
    fclose(fp);
    printf("%ld", checksum);
    return 0;
}