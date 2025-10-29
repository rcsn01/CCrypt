/*
 * encryption.c
 * File encryption and decryption functions for CCrypt
 * Agam Grewal, Gordan Huang, Chu Cheng Yu
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

    /* XOR; set method directly */
    method = ENC_XOR;
    
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
FILE *fin = fopen(input_path, "rb");
    if (!fin) {
        printf("Error: could not open input file.\n");
        return ERROR_FILE_NOT_FOUND;
    }

    FILE *fout = fopen(output_path, "wb");
    if (!fout) {
        printf("Error: could not create output file.\n");
        fclose(fin);
        return ERROR_FILE_NOT_FOUND;
    }

    /* Determine file size */
    fseek(fin, 0, SEEK_END);
    long input_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    /* Read entire file */
    unsigned char *input_data = malloc(input_size);
    if (!input_data) {
        fclose(fin);
        fclose(fout);
        return ERROR_MEMORY_ALLOCATION;
    }
    fread(input_data, 1, input_size, fin);
    fclose(fin);

    /* Optional compression */
    unsigned char *processed_data = input_data;
    long processed_size = input_size;

    if (use_compression) {
        unsigned char *compressed_data = malloc(input_size * 2);
        if (!compressed_data) {
            free(input_data);
            fclose(fout);
            return ERROR_MEMORY_ALLOCATION;
        }
        int comp_result = compress_data(input_data, input_size, compressed_data, &processed_size);
        if (comp_result != SUCCESS) {
            printf("Error: compression failed.\n");
            free(input_data);
            free(compressed_data);
            fclose(fout);
            return comp_result;
        }
        free(input_data);
        processed_data = compressed_data;
    }

    /* XOR encryption */
    unsigned char *encrypted_data = malloc(processed_size);
    if (!encrypted_data) {
        free(processed_data);
        fclose(fout);
        return ERROR_MEMORY_ALLOCATION;
    }

    int enc_result = encrypt_data(processed_data, processed_size, password, encrypted_data);
    if (enc_result != SUCCESS) {
        free(processed_data);
        free(encrypted_data);
        fclose(fout);
        return enc_result;
    }

    /* Write encrypted output */
    fwrite(encrypted_data, 1, processed_size, fout);
    fclose(fout);

    /* Populate metadata */
    memset(metadata, 0, sizeof(file_metadata_t));
    safe_string_copy(metadata->original_filename, input_path, sizeof(metadata->original_filename));
    safe_string_copy(metadata->encrypted_filename, output_path, sizeof(metadata->encrypted_filename));
    metadata->is_compressed = use_compression;
    metadata->original_size = input_size;
    metadata->encrypted_size = processed_size;
    metadata->encryption_method = (int)method;

    free(encrypted_data);
    free(processed_data);

    printf("Encrypted: %s → %s (%ld bytes → %ld bytes)\n",
           input_path, output_path, input_size, processed_size);
    if (use_compression)
        printf("Compression applied before encryption.\n");

    return SUCCESS;
}

/*
 * Complete workflow for decrypting a file from the library
 * [Agam Gewal]
 */
int decrypt_file_workflow(encryption_library_t *library)
{
    char encrypted_path[MAX_PATH_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char output_path[MAX_PATH_LENGTH];
    file_metadata_t dummy_metadata;  /* Used if file isn't from library */
    int result;

    /* Step 1: Get encrypted file path from user */
    printf("Enter the path to the encrypted file to decrypt: ");
    if (!fgets(encrypted_path, sizeof(encrypted_path), stdin)) {
        return ERROR_INVALID_PATH;
    }
    encrypted_path[strcspn(encrypted_path, "\r\n")] = 0;  /* Strip newline */

    /* Validate the path */
    result = validate_file_path(encrypted_path);
    if (result != SUCCESS) {
        printf("Error: could not open encrypted file '%s'\n", encrypted_path);
        return result;
    }

    /* Ask user for password */
    printf("Enter decryption password: ");
    if (!fgets(password, sizeof(password), stdin)) {
        return ERROR_INVALID_PASSWORD;
    }
    password[strcspn(password, "\r\n")] = 0;

    /* Create output filename automatically */
    snprintf(output_path, sizeof(output_path), "%s_dec", encrypted_path);

    /* Assume XOR and no compression unless metadata known */
    memset(&dummy_metadata, 0, sizeof(dummy_metadata));
    dummy_metadata.is_compressed = 0;
    dummy_metadata.original_size = 0;

    /* Perform actual decryption */
    result = decrypt_file(encrypted_path, output_path, password, ENC_XOR, &dummy_metadata);
    if (result == SUCCESS) {
        printf("Decryption complete.\n");
    } else {
        printf("Decryption failed (error %d).\n", result);
    }

    /* Clear password from memory */
    secure_memory_clear(password, sizeof(password));

    return result;
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
    FILE *fin = fopen(encrypted_path, "rb");
    if (!fin) {
        printf("Error: could not open encrypted file.\n");
        return ERROR_FILE_NOT_FOUND;
    }

    /* Determine encrypted file size */
    fseek(fin, 0, SEEK_END);
    long enc_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    /* Allocate memory for encrypted data */
    unsigned char *enc_data = malloc(enc_size);
    if (!enc_data) {
        fclose(fin);
        return ERROR_MEMORY_ALLOCATION;
    }

    fread(enc_data, 1, enc_size, fin);
    fclose(fin);

    /* Allocate memory for decrypted data */
    unsigned char *dec_data = malloc(enc_size);
    if (!dec_data) {
        free(enc_data);
        return ERROR_MEMORY_ALLOCATION;
    }

    /* Perform XOR decryption */
    int dec_result = decrypt_data(enc_data, enc_size, password, dec_data);
    if (dec_result != SUCCESS) {
        printf("Error: decryption failed.\n");
        free(enc_data);
        free(dec_data);
        return dec_result;
    }

    /* Handle decompression if metadata indicates compressed */
    unsigned char *final_data = dec_data;
    long final_size = enc_size;

    if (metadata && metadata->is_compressed) {
        unsigned char *decompressed = malloc(metadata->original_size * 2);
        if (!decompressed) {
            free(enc_data);
            free(dec_data);
            return ERROR_MEMORY_ALLOCATION;
        }

        int decomp_result = decompress_data(dec_data, enc_size, decompressed, &final_size);
        if (decomp_result != SUCCESS) {
            printf("Error: decompression failed.\n");
            free(enc_data);
            free(dec_data);
            free(decompressed);
            return decomp_result;
        }

        free(dec_data);
        final_data = decompressed;
    }

    /* Write decrypted (and possibly decompressed) data to output */
    FILE *fout = fopen(output_path, "wb");
    if (!fout) {
        printf("Error: could not create output file.\n");
        free(enc_data);
        free(final_data);
        return ERROR_FILE_NOT_FOUND;
    }

    fwrite(final_data, 1, final_size, fout);
    fclose(fout);

    /* Clean up */
    free(enc_data);
    free(final_data);

    printf("File decrypted successfully.\n");
    printf("Input: %s\n", encrypted_path);
    printf("Output: %s (%ld bytes)\n", output_path, final_size);
    if (metadata && metadata->is_compressed)
        printf("Decompression applied after decryption.\n");

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
    if (!input_data || !output_data || !password) return ERROR_INVALID_PATH;

    size_t pwlen = strlen(password);
    if (pwlen == 0) return ERROR_INVALID_PASSWORD;

    /* XOR */
    for (long i = 0; i < data_size; ++i) {
        output_data[i] = input_data[i] ^ password[i % pwlen];
    }

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
    if (!encrypted_data || !output_data || !password) return ERROR_INVALID_PATH;

    size_t pwlen = strlen(password);
    if (pwlen == 0) return ERROR_INVALID_PASSWORD;

    for (long i = 0; i < data_size; ++i) {
        output_data[i] = encrypted_data[i] ^ password[i % pwlen];
    }

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