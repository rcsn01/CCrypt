/*
 * encryption.c
 * File encryption and decryption functions for CCrypt
 * Chu-Cheng Yu and contributors
 * September 2025
 * This file contains all encryption, decryption, and compression related functions.
 */

#include "ccrypt.h"
#include "encryption.h"
#include "ui.h"
#include "library.h"
#include "utils.h"
/* ========================================================================
 * FILE ENCRYPTION FUNCTIONS
 * ======================================================================== */

/*
 * Complete workflow for encrypting a user-specified file
 * [Agam Grewal]
 */
int encrypt_file_workflow(encryption_library_t *library)
{
    char file_path[MAX_PATH_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char encrypted_filename[MAX_FILENAME_LENGTH];
    file_metadata_t metadata;
    int use_compression;
    int result;
    encryption_method_t method;
    
    /* Get file path from user */
    result = get_file_path_from_user(file_path, sizeof(file_path));
    if (result != SUCCESS) {
        return result;
    }

    /* Ask user for encryption method */
    printf("Choose encryption method:\n");
    printf(" 1) AES (placeholder - not implemented)\n");
    printf(" 2) XOR (placeholder - not implemented)\n");
    printf(" 3) Caesar shift (placeholder - not implemented)\n");
    result = get_user_choice("Encryption method (1-3): ", 1, 3);
    method = (encryption_method_t)result;
    
    /* Ask about compression */
    use_compression = ask_compression_preference();
    if (use_compression < 0) {
        return ERROR_INVALID_PATH;
    }
    
    /* Get password */
    printf("Enter encryption password: ");
    /* TODO: Implement hidden password input */
    if (!fgets(password, sizeof(password), stdin)) {
        return ERROR_INVALID_PASSWORD;
    }
    /* strip newline */
    size_t pwlen = strlen(password);
    if (pwlen > 0 && (password[pwlen - 1] == '\n' || password[pwlen - 1] == '\r')) {
        password[pwlen - 1] = '\0';
    }
    
    /* Generate encrypted filename using library next_id */
    result = generate_encrypted_filename(file_path, encrypted_filename, sizeof(encrypted_filename), library->next_id);
    if (result != SUCCESS) {
        return result;
    }
    
    /* Perform encryption */
    result = encrypt_file(file_path, encrypted_filename, password, use_compression, method, &metadata);
    if (result != SUCCESS) {
        return result;
    }
    
    /* Add to library */
    /* Set metadata id and add to library */
    metadata.encryption_id = library->next_id;
    result = add_file_to_library(library, &metadata);
    if (result == SUCCESS) {
        library->next_id++;
        printf("File encrypted successfully and added to library\n");
    }
    
    /* Clear password from memory */
    secure_memory_clear(password, sizeof(password));
    
    return result;
}

/*
 * Encrypt a file with optional compression
 * [Agam Grewal]
 */
int encrypt_file(const char *input_path, const char *output_path, 
                 const char *password, int use_compression, 
                 encryption_method_t method,
                 file_metadata_t *metadata)
{
    /* TODO: Implement file encryption logic */
    printf("Encrypting file: %s -> %s\n", input_path, output_path);
    printf("Compression: %s\n", use_compression ? "enabled" : "disabled");
    
    /* Initialize metadata structure */
    memset(metadata, 0, sizeof(file_metadata_t));
    safe_string_copy(metadata->original_filename, input_path, sizeof(metadata->original_filename));
    safe_string_copy(metadata->encrypted_filename, output_path, sizeof(metadata->encrypted_filename));
    metadata->is_compressed = use_compression;
    /* Record chosen encryption method */
    metadata->encryption_method = (int)method;
    /* Assign a unique encryption id (set by caller) */
    /* metadata->encryption_id should already be set by caller */
    
    return SUCCESS; /* Placeholder */
}

/*
 * Complete workflow for decrypting a file from the library
 * [Agam Gewal]
 */
int decrypt_file_workflow(encryption_library_t *library)
{
    /* TODO: Implement decryption workflow */
    printf("Decryption workflow - to be implemented\n");
    return SUCCESS;
}

/*
 * Present library to user and select an entry for decryption (stub)
 * library Pointer to the encryption library instance
 * selected_index Out parameter to receive the selected index (0-based)
 * SUCCESS if a selection was made, or ERROR_FILE_NOT_FOUND if library empty
 * [Empty]
 */
int select_file_for_decryption(encryption_library_t *library, int *selected_index)
{
    if (!library || library->count == 0) return ERROR_FILE_NOT_FOUND;
    /* Default to first entry for the stub */
    if (selected_index) *selected_index = 0;
    return SUCCESS;
}

/*
 * Decrypt an encrypted file (placeholder implementation)
 * encrypted_path Path to the encrypted input file
 * output_path Path where the decrypted output should be written
 * password Password used for decryption
 * method Encryption method used (encryption_method_t)
 * metadata Optional pointer to file metadata associated with the file
 * SUCCESS on success, or an error code on failure
 * [Empty]
 */
int decrypt_file(const char *encrypted_path, const char *output_path,
                 const char *password, encryption_method_t method, const file_metadata_t *metadata)
{
    /* TODO: Implement full decryption: read file, decrypt, verify checksum, write output */
    printf("[not implemented] Decrypting file: %s -> %s (method=%d)\n", encrypted_path, output_path, (int)method);
    (void)password; (void)metadata;
    return SUCCESS;
}

/* ========================================================================
 * ENCRYPTION/COMPRESSION ALGORITHMS
 * ======================================================================== */

/*
 * Apply compression algorithm to file data
 * [Gordon Huang]
 */
int compress_data(const unsigned char *input_data, long input_size,
                  unsigned char *output_data, long *output_size)
{
    if (!input_data || input_size <= 0 || !output_data || !output_size)
        return ERROR_INVALID_PATH;
long out_index = 0;
long i = 0;

    while (i < input_size) {
        unsigned char current = input_data[i];
        unsigned char count = 1;

        while ((i + count < input_size) && 
            (input_data[i + count] == current) && 
            (count < 255)) {
        count++;
        }
        output_data[out_index++] = count;
        output_data[out_index++] = current;

        i += count;
    }
*output_size = out_index;
if (*output_size >= input_size) {
    memcpy(output_data, input_data, (size_t)input_size);
    *output_size = input_size;
    return SUCCESS; 
    }
}

/*
 * Apply encryption cipher to file data
 * [Agam Grewal]
 */
int encrypt_data(const unsigned char *input_data, long data_size,
                 const char *password, unsigned char *output_data)
{
    /* TODO: Implement XOR-based encryption with key derivation */
    /* Simple XOR placeholder - replace with proper implementation */
    return SUCCESS;
}

/*
 * Decrypt a buffer of data using the supplied password (stub)
 * encrypted_data Pointer to input encrypted bytes
 * data_size Size of input buffer in bytes
 * password Password used to derive the decryption key
 * output_data Output buffer to receive decrypted bytes (must be allocated)
 * SUCCESS on success, error code on invalid input
 * [Empty]
 */
int decrypt_data(const unsigned char *encrypted_data, long data_size,
                 const char *password, unsigned char *output_data)
{
    /* TODO: Implement decrypt for supported methods (XOR/Caesar) */
    (void)password;
    if (!encrypted_data || data_size <= 0 || !output_data) return ERROR_INVALID_PATH;
    /* simple passthrough stub */
    for (long i = 0; i < data_size; ++i) output_data[i] = encrypted_data[i];
    return SUCCESS;
}

/*
 * Decompress a buffer produced by compress_data (stub)
 * compressed_data Pointer to compressed input bytes
 * compressed_size Size of compressed input in bytes
 * output_data Output buffer to receive decompressed bytes (must be allocated)
 * output_size Out parameter to receive number of decompressed bytes
 * SUCCESS on success, error code on invalid input
 * [Gordon Huang]
 */
int decompress_data(const unsigned char *compressed_data, long compressed_size,
                    unsigned char *output_data, long *output_size)
{
    /* TODO: Implement decompression that matches compress_data (RLE or similar) */
    if (!compressed_data || compressed_size <= 0 || !output_data || !output_size) return ERROR_INVALID_PATH;
    /* passthrough */
    long out_index = 0;
    for (long i = 0; i < compressed_size; i += 2) {
        unsigned char count = compressed_data[i];
        unsigned char value = compressed_data[i + 1];
        for (unsigned char j = 0; j < count; j++) {
            output_data[out_index++] = value;
        }
    }

    *output_size = out_index;
    return SUCCESS;
}
