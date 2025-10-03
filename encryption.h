/*
 * encryption.h
 * Header file for encryption and decryption functions
 * Chu-Cheng Yu and contributors
 * September 2025
 * This header defines the encryption, decryption, and compression functions
 * for CCrypt including workflows and low-level data processing.
 */

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "ccrypt.h"

/* ========================================================================
 * ENCRYPTION FUNCTION DECLARATIONS
 * ======================================================================== */

/*
 * Complete workflow for encrypting a user-specified file
 * library Pointer to the encryption library
 * SUCCESS on successful encryption, error code on failure
 */
int encrypt_file_workflow(encryption_library_t *library);

/*
 * Encrypt a file with optional compression
 * input_path Path to input file
 * output_path Path to output encrypted file
 * password Encryption password
 * use_compression Whether to compress before encryption
 * method Encryption method to use
 * metadata Pointer to metadata structure to populate
 * SUCCESS on success, error code on failure
 */
int encrypt_file(const char *input_path, const char *output_path, 
                 const char *password, int use_compression, 
                 encryption_method_t method, file_metadata_t *metadata);

/*
 * Complete workflow for decrypting a file from the library
 * library Pointer to the encryption library
 * SUCCESS on successful decryption, error code on failure
 */
int decrypt_file_workflow(encryption_library_t *library);

/*
 * Present library to user and select an entry for decryption
 * library Pointer to the encryption library instance
 * selected_index Out parameter to receive the selected index (0-based)
 * SUCCESS if a selection was made, or ERROR_FILE_NOT_FOUND if library empty
 */
int select_file_for_decryption(encryption_library_t *library, int *selected_index);

/*
 * Decrypt an encrypted file
 * encrypted_path Path to the encrypted input file
 * output_path Path where the decrypted output should be written
 * password Password used for decryption
 * method Encryption method used (encryption_method_t)
 * metadata Optional pointer to file metadata associated with the file
 * SUCCESS on success, or an error code on failure
 */
int decrypt_file(const char *encrypted_path, const char *output_path,
                 const char *password, encryption_method_t method, 
                 const file_metadata_t *metadata);

/* ========================================================================
 * LOW-LEVEL ENCRYPTION/COMPRESSION FUNCTIONS
 * ======================================================================== */

/*
 * Apply compression algorithm to file data
 * input_data Pointer to input data
 * input_size Size of input data in bytes
 * output_data Pointer to output buffer
 * output_size Pointer to variable to receive output size
 * SUCCESS on success, error code on failure
 */
int compress_data(const unsigned char *input_data, long input_size,
                  unsigned char *output_data, long *output_size);

/*
 * Apply encryption cipher to file data
 * input_data Pointer to input data
 * data_size Size of input data in bytes
 * password Password for encryption
 * output_data Pointer to output buffer
 * SUCCESS on success, error code on failure
 */
int encrypt_data(const unsigned char *input_data, long data_size,
                 const char *password, unsigned char *output_data);

/*
 * Decrypt a buffer of data using the supplied password
 * encrypted_data Pointer to input encrypted bytes
 * data_size Size of input buffer in bytes
 * password Password used to derive the decryption key
 * output_data Output buffer to receive decrypted bytes (must be allocated)
 * SUCCESS on success, error code on invalid input
 */
int decrypt_data(const unsigned char *encrypted_data, long data_size,
                 const char *password, unsigned char *output_data);

/*
 * Decompress a buffer produced by compress_data
 * compressed_data Pointer to compressed input bytes
 * compressed_size Size of compressed input in bytes
 * output_data Output buffer to receive decompressed bytes (must be allocated)
 * output_size Out parameter to receive number of decompressed bytes
 * SUCCESS on success, error code on invalid input
 */
int decompress_data(const unsigned char *compressed_data, long compressed_size,
                    unsigned char *output_data, long *output_size);

#endif /* ENCRYPTION_H */