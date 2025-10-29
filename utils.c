/*
 * utils.c
 * Utility functions for CCrypt
 * Chu-Cheng Yu and contributors
 * September 2025
 * This file contains utility functions for file operations, string handling,
 * memory management, and other common operations.
 */

#include "ccrypt.h"
#include "utils.h"

/* ========================================================================
 * UTILITY FUNCTIONS
 * ======================================================================== */

/*
 * Validate that a file path exists and is accessible
 * [Chu-Cheng Yu]
 */
int validate_file_path(const char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }
    fclose(file);
    return SUCCESS;
}

/*
 * Generate a secure filename for encrypted file
 * [Chu-Cheng Yu]
 */
int generate_encrypted_filename(const char *original_path, char *encrypted_filename, 
                               size_t buffer_size, unsigned long id)
{
    if (!original_path || !encrypted_filename || buffer_size == 0) {
        return ERROR_INVALID_PATH;
    }
    
    /* Extract the base filename from the path */
    const char *filename = strrchr(original_path, '/');
    if (!filename) {
        filename = strrchr(original_path, '\\');
    }
    filename = filename ? filename + 1 : original_path;
    
    /* Remove the original extension */
    char base_name[MAX_FILENAME_LENGTH];
    safe_string_copy(base_name, filename, sizeof(base_name));
    char *ext = strrchr(base_name, '.');
    if (ext) {
        *ext = '\0';  /* Truncate at the extension */
    }
    
    /* Create encrypted filename: original_name_without_ext.ccrypt */
    snprintf(encrypted_filename, buffer_size, "%s.ccrypt", base_name);
    return SUCCESS;
}

/*
 * Securely clear memory containing sensitive data
 * [Chu-Cheng Yu]
 */
void secure_memory_clear(void *data, size_t size)
{
    volatile unsigned char *ptr = (volatile unsigned char *)data;
    for (size_t i = 0; i < size; i++) {
        ptr[i] = 0;
    }
}

/*
 * Safe string copy with bounds checking
 * [Chu-Cheng Yu]
 */
int safe_string_copy(char *dest, const char *src, size_t dest_size)
{
    if (!dest || !src || dest_size == 0) {
        return ERROR_INVALID_PATH;
    }
    
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
    
    return SUCCESS;
}

/*
 * Compute a simple non-cryptographic checksum for a file
 * file_path Path to the file
 * checksum Buffer to receive a textual hex checksum
 * buffer_size Size of the checksum buffer
 * SUCCESS on success, or an error code on failure
 * [Chu-Cheng Yu]
 */
int calculate_file_checksum(const char *file_path, char *checksum, size_t buffer_size)
{
    if (!file_path || !checksum || buffer_size < 3) return ERROR_INVALID_PATH;
    /* Simple non-cryptographic checksum: sum of bytes mod 65536 as hex */
    FILE *f = fopen(file_path, "rb");
    if (!f) return ERROR_FILE_NOT_FOUND;
    unsigned long sum = 0;
    int c;
    while ((c = fgetc(f)) != EOF) sum = (sum + (unsigned char)c) & 0xFFFFFFFFu;
    fclose(f);
    snprintf(checksum, buffer_size, "%08lx", sum);
    return SUCCESS;
}

/*
 * Extract the extension (without dot) from a filename
 * filename Input filename string
 * extension Buffer to receive the extension
 * buffer_size Size of extension buffer
 * SUCCESS if extension extracted, ERROR_INVALID_PATH otherwise
 * [Chu-Cheng Yu]
 */
int get_file_extension(const char *filename, char *extension, size_t buffer_size)
{
    if (!filename || !extension || buffer_size == 0) return ERROR_INVALID_PATH;
    const char *p = strrchr(filename, '.');
    if (!p || *(p + 1) == '\0') {
        extension[0] = '\0';
        return ERROR_INVALID_PATH;
    }
    safe_string_copy(extension, p + 1, buffer_size);
    return SUCCESS;
}

/*
 * Convert a raw byte size into a human readable string
 * size File size in bytes
 * buffer Buffer to receive formatted string
 * buffer_size Size of the buffer
 * void
 * [Chu-Cheng Yu]
 */
void format_file_size(long size, char *buffer, size_t buffer_size)
{
    if (!buffer || buffer_size == 0) return;
    const char *units[] = {"B", "KB", "MB", "GB"};
    double s = (double)size;
    int unit = 0;
    while (s >= 1024.0 && unit < 3) { s /= 1024.0; unit++; }
    snprintf(buffer, buffer_size, "%.2f %s", s, units[unit]);
}