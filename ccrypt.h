/**
 * @file ccrypt.h
 * @brief Main header file for CCrypt - A CLI file encryption and compression tool
 * @author Chu-Cheng Yu
 * @date September 2025
 * 
 * CCrypt is a command-line tool for encrypting and decrypting files with optional
 * compression. It maintains a library of encrypted files and provides management
 * capabilities including sorting, renaming, and deletion.
 * 
 * This main header contains core data structures, constants, and type definitions
 * used throughout the CCrypt application.
 * 
 * Constraints: Uses only standard C libraries (stdio.h, stdlib.h, string.h, math.h)
 */

#ifndef CCRYPT_H
#define CCRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ========================================================================
 * CONSTANTS AND MACROS
 * ======================================================================== */

#define MAX_PATH_LENGTH 260
#define MAX_FILENAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 64
#define MAX_LIBRARY_ENTRIES 1000
#define BUFFER_SIZE 4096
#define ENCRYPTION_SIGNATURE "CCRYPT1.0"
#define LIBRARY_FILENAME "ccrypt_library.dat"

/* Error codes */
#define SUCCESS 0
#define ERROR_FILE_NOT_FOUND -1
#define ERROR_INVALID_PATH -2
#define ERROR_PERMISSION_DENIED -3
#define ERROR_INVALID_PASSWORD -4
#define ERROR_MEMORY_ALLOCATION -5
#define ERROR_LIBRARY_CORRUPT -6
#define ERROR_ENCRYPTION_FAILED -7
#define ERROR_COMPRESSION_FAILED -8

/* Sort options */
typedef enum {
    SORT_BY_NAME = 1,
    SORT_BY_DATE = 2,
    SORT_BY_SIZE = 3,
    SORT_BY_TYPE = 4
} sort_option_t;

/* Encryption methods supported */
typedef enum {
    ENC_AES = 1,
    ENC_XOR = 2,
    ENC_CAESAR = 3
} encryption_method_t;

/* ========================================================================
 * DATA STRUCTURES
 * ======================================================================== */

/**
 * @struct file_metadata
 * @brief Structure to store metadata for encrypted files
 */
typedef struct {
    char original_filename[MAX_FILENAME_LENGTH];
    char encrypted_filename[MAX_FILENAME_LENGTH];
    char file_path[MAX_PATH_LENGTH];
    long original_size;
    long encrypted_size;
    unsigned long encryption_id;
    int encryption_method; /* encryption_method_t value used for this file */
    int is_compressed;
    char file_type[10];
    char checksum[33]; /* MD5-style checksum for integrity */
} file_metadata_t;

/**
 * @struct encryption_library
 * @brief Structure to manage the library of encrypted files
 */
typedef struct {
    file_metadata_t entries[MAX_LIBRARY_ENTRIES];
    int count;
    int is_modified;
    unsigned long next_id;
} encryption_library_t;

/* ========================================================================
 * CORE PROGRAM FUNCTION DECLARATIONS
 * ======================================================================== */

/**
 * @brief Initialize program components and load encryption library
 * @param library Pointer to library structure to initialize
 * @return SUCCESS on success, error code on failure
 */
int initialize_program(encryption_library_t *library);

/**
 * @brief Clean up program resources and save library before exit
 * @param library Pointer to library structure to cleanup
 * @return SUCCESS on success, error code on failure
 */
int cleanup_program(encryption_library_t *library);

#endif /* CCRYPT_H */