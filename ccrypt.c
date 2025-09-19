/*
 Simple C encrypt/compress utility
 Uses only: stdio.h, stdlib.h, string.h, math.h

 Features:
 - RLE compression (binary-safe)
 - RLE decompression
 - XOR stream cipher using a simple LCG PRNG seeded from password
 - Single-file CLI program

 Header format for compressed+encrypted files:
 4 bytes: magic 'CCRY'
 1 byte : flags (bit0: compressed)
 4 bytes: seed (unsigned int, little-endian)
 then payload (possibly compressed) XORed with PRNG stream

 Limitations: This is educational code. The encryption is NOT cryptographically secure.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAGIC0 'C'
#define MAGIC1 'C'
#define MAGIC2 'R'
#define MAGIC3 'Y'

enum { FLAG_COMPRESSED = 1 };

/* Simple LCG PRNG: returns next value in [0,255] */
static unsigned int lcg_next(unsigned int *state) {
    /* constants from Numerical Recipes */
    *state = (*state * 1664525u + 1013904223u);
    return (*state >> 16) & 0xFFu;
}

/* XOR buffer with PRNG stream */
static void xor_stream(unsigned char *buf, size_t len, unsigned int seed) {
    unsigned int st = seed;
    size_t i;
    for (i = 0; i < len; ++i) {
        unsigned int byte = lcg_next(&st);
        buf[i] ^= (unsigned char)byte;
    }
}

/* RLE compress: returns newly allocated buffer and sets out_len. Format: (count byte)(value) pairs.
   count of 0 means 256 repetitions. */
static unsigned char *rle_compress(const unsigned char *in, size_t in_len, size_t *out_len) {
    if (in_len == 0) {
        *out_len = 0;
        return NULL;
    }
    /* Worst-case: every pair is (1,byte) -> 2 * in_len */
    unsigned char *out = (unsigned char *)malloc(in_len * 2 + 16);
    if (!out) return NULL;
    size_t oi = 0;
    size_t i = 0;
    while (i < in_len) {
        unsigned char v = in[i];
        size_t run = 1;
        while (i + run < in_len && in[i + run] == v && run < 256) run++;
        unsigned char count = (unsigned char)(run == 256 ? 0 : run);
        out[oi++] = count;
        out[oi++] = v;
        i += run;
    }
    *out_len = oi;
    return out;
}

/* RLE decompress: returns newly allocated buffer and sets out_len. */
static unsigned char *rle_decompress(const unsigned char *in, size_t in_len, size_t *out_len) {
    if (in_len == 0) {
        *out_len = 0;
        return NULL;
    }
    /* Estimate output: unknown. We'll expand dynamically. */
    size_t cap = 256;
    unsigned char *out = (unsigned char *)malloc(cap);
    if (!out) return NULL;
    size_t oi = 0;
    size_t i = 0;
    while (i + 1 <= in_len - 1) {
        unsigned char count = in[i++];
        unsigned char val = in[i++];
        size_t run = (count == 0) ? 256 : (size_t)count;
        if (oi + run > cap) {
            while (oi + run > cap) cap *= 2;
            out = (unsigned char *)realloc(out, cap);
            if (!out) return NULL;
        }
        size_t k;
        for (k = 0; k < run; ++k) out[oi++] = val;
    }
    *out_len = oi;
    return out;
}

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s c input output password [--compress]   # encrypt (and optional compress)\n"
        "  %s d input output password               # decrypt (and optional decompress)\n",
        prog, prog);
}

int main(int argc, char **argv) {
    if (argc < 5) {
        print_usage(argv[0]);
        return 1;
    }
    char mode = 0;
    if (strcmp(argv[1], "c") == 0) mode = 'c';
    else if (strcmp(argv[1], "d") == 0) mode = 'd';
    else {
        print_usage(argv[0]);
        return 1;
    }
    const char *inpath = argv[2];
    const char *outpath = argv[3];
    const char *password = argv[4];
    int do_compress = 0;
    if (mode == 'c' && argc >= 6 && strcmp(argv[5], "--compress") == 0) do_compress = 1;

    /* Read entire input file */
    FILE *fi = fopen(inpath, "rb");
    if (!fi) {
        fprintf(stderr, "Failed to open input '%s'\n", inpath);
        return 2;
    }
    if (fseek(fi, 0, SEEK_END) != 0) { fclose(fi); return 3; }
    long fsz = ftell(fi);
    if (fsz < 0) { fclose(fi); return 3; }
    rewind(fi);
    unsigned char *buf = (unsigned char *)malloc((size_t)fsz);
    if (!buf) { fclose(fi); fprintf(stderr, "Out of memory\n"); return 4; }
    if (fsz > 0) {
        if (fread(buf, 1, (size_t)fsz, fi) != (size_t)fsz) {
            fclose(fi); free(buf); fprintf(stderr, "Failed read\n"); return 5;
        }
    }
    fclose(fi);

    if (mode == 'c') {
        unsigned char *payload = buf;
        size_t payload_len = (size_t)fsz;
        unsigned char *comp = NULL;
        size_t comp_len = 0;
        if (do_compress) {
            comp = rle_compress(payload, payload_len, &comp_len);
            if (!comp) { free(buf); fprintf(stderr, "Compress failed\n"); return 6; }
            payload = comp;
            payload_len = comp_len;
        }

        /* derive seed from password: simple hash */
        unsigned int seed = 2166136261u;
        size_t i;
        for (i = 0; i < strlen(password); ++i) {
            seed ^= (unsigned char)password[i];
            seed *= 16777619u;
        }

        /* XOR payload */
        xor_stream(payload, payload_len, seed);

        /* write header + payload */
        FILE *fo = fopen(outpath, "wb");
        if (!fo) { free(buf); if (comp) free(comp); fprintf(stderr, "Failed open output\n"); return 7; }
        unsigned char header[9];
        header[0] = MAGIC0; header[1] = MAGIC1; header[2] = MAGIC2; header[3] = MAGIC3;
        header[4] = do_compress ? FLAG_COMPRESSED : 0;
        /* store seed little-endian */
        header[5] = (unsigned char)(seed & 0xFFu);
        header[6] = (unsigned char)((seed >> 8) & 0xFFu);
        header[7] = (unsigned char)((seed >> 16) & 0xFFu);
        header[8] = (unsigned char)((seed >> 24) & 0xFFu);
        if (fwrite(header, 1, sizeof(header), fo) != sizeof(header)) { fclose(fo); free(buf); if (comp) free(comp); fprintf(stderr, "Write failed\n"); return 8; }
        if (payload_len > 0) {
            if (fwrite(payload, 1, payload_len, fo) != payload_len) { fclose(fo); free(buf); if (comp) free(comp); fprintf(stderr, "Write failed\n"); return 9; }
        }
        fclose(fo);
        free(buf);
        if (comp) free(comp);
        printf("Wrote %s\n", outpath);
        return 0;
    } else {
        /* decrypt */
        if (fsz < 9) { free(buf); fprintf(stderr, "Input too small\n"); return 10; }
        /* check magic */
        if (buf[0] != MAGIC0 || buf[1] != MAGIC1 || buf[2] != MAGIC2 || buf[3] != MAGIC3) {
            free(buf); fprintf(stderr, "Bad file format\n"); return 11;
        }
        unsigned char flags = buf[4];
        unsigned int seed = (unsigned int)buf[5] | ((unsigned int)buf[6] << 8) | ((unsigned int)buf[7] << 16) | ((unsigned int)buf[8] << 24);
        unsigned char *payload = buf + 9;
        size_t payload_len = (size_t)fsz - 9;

        /* derive seed from provided password and check equality to stored seed to detect wrong password */
        unsigned int seed2 = 2166136261u;
        size_t i;
        for (i = 0; i < strlen(password); ++i) {
            seed2 ^= (unsigned char)password[i];
            seed2 *= 16777619u;
        }
        if (seed2 != seed) {
            /* We still proceed but warn: decryption will produce garbage if wrong */
            fprintf(stderr, "Warning: password does not match header-derived seed; output may be garbage\n");
        }

        /* XOR to decrypt in-place */
        xor_stream(payload, payload_len, seed);

        unsigned char *outbuf = NULL;
        size_t outlen = 0;
        if (flags & FLAG_COMPRESSED) {
            outbuf = rle_decompress(payload, payload_len, &outlen);
            if (!outbuf && payload_len > 0) { free(buf); fprintf(stderr, "Decompress failed\n"); return 12; }
        } else {
            /* copy payload */
            outbuf = (unsigned char *)malloc(payload_len);
            if (payload_len > 0 && !outbuf) { free(buf); fprintf(stderr, "Out of memory\n"); return 13; }
            if (payload_len > 0) memcpy(outbuf, payload, payload_len);
            outlen = payload_len;
        }

        FILE *fo = fopen(outpath, "wb");
        if (!fo) { free(buf); if (outbuf) free(outbuf); fprintf(stderr, "Failed open output\n"); return 14; }
        if (outlen > 0) {
            if (fwrite(outbuf, 1, outlen, fo) != outlen) { fclose(fo); free(buf); if (outbuf) free(outbuf); fprintf(stderr, "Write failed\n"); return 15; }
        }
        fclose(fo);
        free(buf);
        if (outbuf) free(outbuf);
        printf("Wrote %s\n", outpath);
        return 0;
    }
}
