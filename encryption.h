/**
 * @file encryption.h
 * @brief Header file for encryption and decryption functions
 * @author Chu-Cheng Yu and contributors
 * @date September 2025
 * 
 * This header defines the encryption, decryption, and compression functions
 * for CCrypt including workflows and low-level data processing.
 */

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "ccrypt.h"

/* ========================================================================
 * ENCRYPTION FUNCTION DECLARATIONS
 * ======================================================================== */

/**
 * @brief Complete workflow for encrypting a user-specified file
 * @param library Pointer to the encryption library
 * @return SUCCESS on successful encryption, error code on failure
 */
int encrypt_file_workflow(encryption_library_t *library);

/**
 * @brief Encrypt a file with optional compression
 * @param input_path Path to input file
 * @param output_path Path to output encrypted file
 * @param password Encryption password
 * @param use_compression Whether to compress before encryption
 * @param method Encryption method to use
 * @param metadata Pointer to metadata structure to populate
 * @return SUCCESS on success, error code on failure
 */
int encrypt_file(const char *input_path, const char *output_path, 
                 const char *password, int use_compression, 
                 encryption_method_t method, file_metadata_t *metadata);

/**
 * @brief Complete workflow for decrypting a file from the library
 * @param library Pointer to the encryption library
 * @return SUCCESS on successful decryption, error code on failure
 */
int decrypt_file_workflow(encryption_library_t *library);

/**
 * @brief Present library to user and select an entry for decryption
 * @param library Pointer to the encryption library instance
 * @param selected_index Out parameter to receive the selected index (0-based)
 * @return SUCCESS if a selection was made, or ERROR_FILE_NOT_FOUND if library empty
 */
int select_file_for_decryption(encryption_library_t *library, int *selected_index);

/**
 * @brief Decrypt an encrypted file
 * @param encrypted_path Path to the encrypted input file
 * @param output_path Path where the decrypted output should be written
 * @param password Password used for decryption
 * @param method Encryption method used (encryption_method_t)
 * @param metadata Optional pointer to file metadata associated with the file
 * @return SUCCESS on success, or an error code on failure
 */
int decrypt_file(const char *encrypted_path, const char *output_path,
                 const char *password, encryption_method_t method, 
                 const file_metadata_t *metadata);

/* ========================================================================
 * LOW-LEVEL ENCRYPTION/COMPRESSION FUNCTIONS
 * ======================================================================== */

/**
 * @brief Apply compression algorithm to file data
 * @param input_data Pointer to input data
 * @param input_size Size of input data in bytes
 * @param output_data Pointer to output buffer
 * @param output_size Pointer to variable to receive output size
 * @return SUCCESS on success, error code on failure
 */
int compress_data(const unsigned char *input_data, long input_size,
                  unsigned char *output_data, long *output_size);

/**
 * @brief Apply encryption cipher to file data
 * @param input_data Pointer to input data
 * @param data_size Size of input data in bytes
 * @param password Password for encryption
 * @param output_data Pointer to output buffer
 * @return SUCCESS on success, error code on failure
 */
int encrypt_data(const unsigned char *input_data, long data_size,
                 const char *password, unsigned char *output_data);

/**
 * @brief Decrypt a buffer of data using the supplied password
 * @param encrypted_data Pointer to input encrypted bytes
 * @param data_size Size of input buffer in bytes
 * @param password Password used to derive the decryption key
 * @param output_data Output buffer to receive decrypted bytes (must be allocated)
 * @return SUCCESS on success, error code on invalid input
 */
int decrypt_data(const unsigned char *encrypted_data, long data_size,
                 const char *password, unsigned char *output_data);

/**
 * @brief Decompress a buffer produced by compress_data
 * @param compressed_data Pointer to compressed input bytes
 * @param compressed_size Size of compressed input in bytes
 * @param output_data Output buffer to receive decompressed bytes (must be allocated)
 * @param output_size Out parameter to receive number of decompressed bytes
 * @return SUCCESS on success, error code on invalid input
 */
int decompress_data(const unsigned char *compressed_data, long compressed_size,
                    unsigned char *output_data, long *output_size);

#endif /* ENCRYPTION_H */