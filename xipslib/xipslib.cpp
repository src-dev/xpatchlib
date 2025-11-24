#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xipslib.h"

static unsigned char _PATCH[] = { 0x50,0x41,0x54,0x43,0x48 };
static unsigned char _EOF[] = { 0x45,0x4F,0x46 };

int createBak(const char* src, const char* ext, bool ovr) {
    FILE* fsrc = fopen(src, "rb");
    if (!fsrc) return E_FOPEN_SRC;

    char* dst = (char*)malloc(sizeof(char) * (strlen(src) + strlen(ext) + 2));
    if (dst == NULL) {
        fclose(fsrc);
        return E_OUT_OF_MEMORY;
    }

    strcpy(dst, src);
    strcat(dst, ".");
    strcat(dst, ext);

    if (!ovr) {
        FILE* fdst = fopen(dst, "rb");
        bool exists = false;
        if (fdst) {
            exists = true;
            fclose(fdst);
        }
        fclose(fsrc);
        return E_CANNOT_OVR;
    }

    FILE* fdst = fopen(dst, "wb");
    if (!fdst) {
        fclose(fsrc);
        return E_FOPEN_DST;
    }

    int c;
    while ((c = fgetc(fsrc)) != EOF) fputc(c, fdst);

    fclose(fsrc);
    fclose(fdst);
    return E_NO_ERROR;
}

int restoreBak(const char* src, const char* ext, bool ovr) {
    FILE* fsrc = fopen(src, "rb");
    if (!fsrc) return E_FOPEN_SRC;

    char* dst = (char*)malloc(sizeof(char) * (strlen(src) - strlen(ext)));
    if (dst == NULL) {
        fclose(fsrc);
        return E_OUT_OF_MEMORY;
    }

    memcpy(dst, src, strlen(src) - (strlen(ext) + 1));

    if (!ovr) {
        FILE* fdst = fopen(dst, "rb");
        bool exists = false;
        if (fdst) {
            exists = true;
            fclose(fdst);
        }
        fclose(fsrc);
        return E_CANNOT_OVR;
    }
    
    FILE* fdst = fopen(dst, "wb");
    if (!fdst) {
        fclose(fsrc);
        return E_FOPEN_DST;
    }

    int c;
    while ((c = fgetc(fsrc)) != EOF) fputc(c, fdst);

    fclose(fsrc);
    fclose(fdst);
    return E_NO_ERROR;
}

int applyIPS(const char* ips, const char* src) {
    FILE* fips = fopen(ips, "rb");
    if (!fips) return E_FOPEN_IPS;

    char buf[5] = { 0 };
    fread(buf, 1, 5, fips);
    if (memcmp(buf, _PATCH, 5) != 0) {
        fclose(fips);
        return E_NOT_IPS;
    }

    FILE* fsrc = fopen(src, "rb+");
    if (!fsrc) {
        fclose(fips);
        return E_FOPEN_SRC;
    }

    for (;;) {
        char offset[3] = { 0 };
        fread(offset, 1, 3, fips);

        if (memcmp(offset, _EOF, 3) == 0) break;
        long o = ((unsigned char)offset[0] << 16) | ((unsigned char)offset[1] << 8) | (unsigned char)offset[2];

        char size[2] = { 0 };
        fread(size, 1, 2, fips);
        int s = ((unsigned char)size[0] << 8) | (unsigned char)size[1];
        unsigned char* data = (unsigned char*)malloc(s * sizeof(char));
        if (data == NULL) {
            fclose(fips);
            fclose(fsrc);
            return E_OUT_OF_MEMORY;
        }

        fread(data, 1, s, fips);
        fseek(fsrc, o, SEEK_SET);
        fwrite(data, 1, s, fsrc);

        free(data);
    }

    fclose(fips);
    fclose(fsrc);

    return 0;
}
