#include "./sum.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * Downloads http://gradlex.blob.core.windows.net/poc/gradle.png . That's it.
 * Using HTTP, not HTTPS. Hard coded checksum.
 **/

int main() {
    const long bufferSize = 65536;
    const char *host = "gradlex.blob.core.windows.net";

    char filename[100];
    snprintf(filename, 100, "gradlex2-%llu", sum);

    char path[100];
    snprintf(path, 100, "/poc/%s", filename);

    printf("Filename %s. Checksum %llu\n", filename, sum);
    printf("Connecting...\n");

    struct sockaddr_in serv_addr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *server = gethostbyname(host);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    char header[1024];
    snprintf(header, sizeof(header), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
    send(sock, header, strlen(header), 0);

    char buffer[bufferSize];
    long res;
    long dataSize = 0;
    long messageSize = 0;
    long totalSize = 0;
    int p = 0;
    printf("Downloading file...\n");
    do {
        res = read(sock, buffer, bufferSize);
        if (dataSize == 0) {
            char *cl = strstr(strstr(buffer, "Content-Length"), " ");
            char *to = strstr(cl, "\r");
            dataSize = strtol(cl, &to, 10);
            char *end = strstr(buffer, "\r\n\r\n");
            messageSize = end - buffer + dataSize + 4;
            totalSize = messageSize;
            fwrite(end + 4, 1, res - (messageSize - dataSize), f);
        } else {
            fwrite(buffer, 1, res, f);
        }
        messageSize -= res;
        int np = 100 - (int) ((double) messageSize / (double) totalSize * 100);
        if (np != p) {
            p = np;
            printf("%d%%\n", p);
        }
    } while (messageSize > 0);

    fclose(f);

    printf("File downloaded, checking checksum...\n");

    // Only POC!
    // Required because of http (no https support)
    FILE *fp = fopen(filename, "rb");
    unsigned long long checksum = 0;
    while (!feof(fp) && !ferror(fp)) {
        checksum += fgetc(fp);
    }
    fclose(fp);

    if (checksum != sum) {
        printf("Checksum failed :( : %ld\n", checksum);
        return 1;
    }

    printf("Done!\n");
    return 0;
}