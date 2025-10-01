/**
 * @file utils.h
 * @brief Header file for utility functions
 * @author Chu-Cheng Yu and contributors
 * @date September 2025
 * 
 * This header defines utility functions for file operations, string handling,
 * memory management, and other common operations used throughout CCrypt.
 */

#ifndef UTILS_H
#define UTILS_H

#include "ccrypt.h"

/* ========================================================================
 * UTILITY FUNCTION DECLARATIONS
 * ======================================================================== */

/**
 * @brief Validate that a file path exists and is accessible
 * @param file_path Path to the file to validate
 * @return SUCCESS if file exists and is accessible, ERROR_FILE_NOT_FOUND otherwise
 */
int validate_file_path(const char *file_path);

/**
 * @brief Generate a secure filename for encrypted file
 * @param original_path Path to the original file
 * @param encrypted_filename Buffer to store generated filename
 * @param buffer_size Size of the filename buffer
 * @param id Unique ID to use in filename generation
 * @return SUCCESS on success, error code on failure
 */
int generate_encrypted_filename(const char *original_path, char *encrypted_filename, 
                               size_t buffer_size, unsigned long id);

/**
 * @brief Securely clear memory containing sensitive data
 * @param data Pointer to memory to clear
 * @param size Size of memory to clear in bytes
 */
void secure_memory_clear(void *data, size_t size);

/**
 * @brief Safe string copy with bounds checking
 * @param dest Destination buffer
 * @param src Source string
 * @param dest_size Size of destination buffer
 * @return SUCCESS on success, error code on failure
 */
int safe_string_copy(char *dest, const char *src, size_t dest_size);

/**
 * @brief Compute a simple non-cryptographic checksum for a file
 * @param file_path Path to the file
 * @param checksum Buffer to receive a textual hex checksum
 * @param buffer_size Size of the checksum buffer
 * @return SUCCESS on success, or an error code on failure
 */
int calculate_file_checksum(const char *file_path, char *checksum, size_t buffer_size);

/**
 * @brief Extract the extension (without dot) from a filename
 * @param filename Input filename string
 * @param extension Buffer to receive the extension
 * @param buffer_size Size of extension buffer
 * @return SUCCESS if extension extracted, ERROR_INVALID_PATH otherwise
 */
int get_file_extension(const char *filename, char *extension, size_t buffer_size);

/**
 * @brief Convert a raw byte size into a human readable string
 * @param size File size in bytes
 * @param buffer Buffer to receive formatted string
 * @param buffer_size Size of the buffer
 */
void format_file_size(long size, char *buffer, size_t buffer_size);

#endif /* UTILS_H */