CCrypt - simple compress & encrypt utility (C)

Overview

ccrypt.c is a single-file C program that can compress and encrypt files, and reverse the operation. It intentionally uses only the following headers: stdio.h, stdlib.h, string.h and math.h.

Features

- RLE compression (simple run-length encoding, binary-safe)
- RLE decompression
- XOR stream cipher using a simple LCG PRNG seeded from the password (educational, not secure)
- Binary-safe file processing

Build

Use a standard C compiler (gcc/clang). Example:

```sh
gcc -std=c11 -O2 -o ccrypt ccrypt.c -lm
```

Usage

Encrypt (and optionally compress):

```sh
./ccrypt c inputfile outputfile password --compress
```

The `--compress` flag is optional. Without it the file is only encrypted.

Decrypt (and optionally decompress):

```sh
./ccrypt d inputfile outputfile password
```

The program checks a small header to know whether the payload was compressed.

Limitations & Security

- This project is educational. The encryption is NOT cryptographically secure. Do not use it for sensitive data.
- RLE is most effective on files with long runs of repeating bytes (e.g., some simple images); for other data it may expand the file.
- Uses only standard C library headers requested by the user.

Files

- `ccrypt.c` - the program source
- `README.md` - this file

License

Public domain / use as you wish.
