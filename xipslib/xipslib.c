#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "xipslib.h"

static unsigned char _PATCH[] = { 0x50,0x41,0x54,0x43,0x48 };
static unsigned char _EOF[] = { 0x45,0x4F,0x46 };

bool createBak(const char* src, const char* dst) {
    FILE* fsrc = fopen(src, "rb");
    if (!fsrc) return false;

    FILE* fdst = fopen(dst, "wb");
    if (!fdst) {
        fclose(fsrc);
        return false;
    }

    int c;
    while ((c = fgetc(fsrc)) != EOF) fputc(c, fdst);

    fclose(fsrc);
    fclose(fdst);
    return true;
}

bool applyIPS(const char* ips, const char* src) {
    FILE* fips = fopen(ips, "rb");
    if (!fips) return false;

    char buf[5] = { 0 };
    fread(buf, 1, 5, fips);
    if (memcmp(buf, _PATCH, 5) != 0) {
        fclose(fips);
        return false;
    }

    FILE* fsrc = fopen(src, "rb+");
    if (!fsrc) {
        fclose(fips);
        return false;
    }

    for (;;) {
        char offset[3] = { 0 };
        fread(offset, 1, 3, fips);

        if (memcmp(offset, _EOF, 3) == 0) break;
        long o = ((unsigned char)offset[0] << 16) | ((unsigned char)offset[1] << 8) | (unsigned char)offset[2];

        char size[2] = { 0 };
        fread(size, 1, 2, fips);
        int s = ((unsigned char)size[0] << 8) | (unsigned char)size[1];
        unsigned char* data = malloc(s * sizeof(char));
        if (data == NULL) {
            fclose(fips);
            fclose(fsrc);
            return false;
        }

        fread(data, 1, s, fips);
        fseek(fsrc, o, SEEK_SET);
        fwrite(data, 1, s, fsrc);

        free(data);
    }

    fclose(fips);
    fclose(fsrc);

    return true;
}
