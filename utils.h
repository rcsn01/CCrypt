/*
 * utils.h
 * Header file for utility functions
 * Chu-Cheng Yu and contributors
 * September 2025
 * This header defines utility functions for file operations, string handling,
 * memory management, and other common operations used throughout CCrypt.
 */

#ifndef UTILS_H
#define UTILS_H

#include "ccrypt.h"

/* ========================================================================
 * UTILITY FUNCTION DECLARATIONS
 * ======================================================================== */

/*
 * Validate that a file path exists and is accessible
 * file_path Path to the file to validate
 * SUCCESS if file exists and is accessible, ERROR_FILE_NOT_FOUND otherwise
 */
int validate_file_path(const char *file_path);

/*
 * Generate a secure filename for encrypted file
 * original_path Path to the original file
 * encrypted_filename Buffer to store generated filename
 * buffer_size Size of the filename buffer
 * id Unique ID to use in filename generation
 * SUCCESS on success, error code on failure
 */
int generate_encrypted_filename(const char *original_path, char *encrypted_filename, 
                               size_t buffer_size, unsigned long id);

/*
 * Securely clear memory containing sensitive data
 * data Pointer to memory to clear
 * size Size of memory to clear in bytes
 */
void secure_memory_clear(void *data, size_t size);

/*
 * Safe string copy with bounds checking
 * dest Destination buffer
 * src Source string
 * dest_size Size of destination buffer
 * SUCCESS on success, error code on failure
 */
int safe_string_copy(char *dest, const char *src, size_t dest_size);

/*
 * Compute a simple non-cryptographic checksum for a file
 * file_path Path to the file
 * checksum Buffer to receive a textual hex checksum
 * buffer_size Size of the checksum buffer
 * SUCCESS on success, or an error code on failure
 */
int calculate_file_checksum(const char *file_path, char *checksum, size_t buffer_size);

/*
 * Extract the extension (without dot) from a filename
 * filename Input filename string
 * extension Buffer to receive the extension
 * buffer_size Size of extension buffer
 * SUCCESS if extension extracted, ERROR_INVALID_PATH otherwise
 */
int get_file_extension(const char *filename, char *extension, size_t buffer_size);

/*
 * Convert a raw byte size into a human readable string
 * size File size in bytes
 * buffer Buffer to receive formatted string
 * buffer_size Size of the buffer
 */
void format_file_size(long size, char *buffer, size_t buffer_size);

#endif /* UTILS_H */