/**
 * @file ccrypt.h
 * @brief Header file for CCrypt - A CLI file encryption and compression tool
 * @author Chu-Cheng Yu,               - Add your name for submission
 * @date September 2025
 * 
 * CCrypt is a command-line tool for encrypting and decrypting files with optional
 * compression. It maintains a library of encrypted files and provides management
 * capabilities including sorting, renaming, and deletion.
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
 * MAIN PROGRAM FUNCTIONS
 * ======================================================================== */

/**
 * @brief Main entry point for the CCrypt program
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return EXIT_SUCCESS on successful completion, EXIT_FAILURE on error
 * 
 * Initializes the program, loads the encryption library, starts the main menu loop,
 * and performs cleanup operations before termination.
 */
int main(int argc, char *argv[]);

/**
 * @brief Initialize program components and load encryption library
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on successful initialization, error code on failure
 * 
 * Sets up program state, initializes data structures, and loads the existing
 * encryption library from disk if it exists.
 */
int initialize_program(encryption_library_t *library);

/**
 * @brief Main program loop displaying menu and processing user commands
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on normal exit, error code on failure
 * 
 * Displays the main menu, gets user input, and routes commands to appropriate
 * handler functions. Continues until user chooses to exit.
 */
int main_menu_loop(encryption_library_t *library);

/**
 * @brief Clean up program resources and save library before exit
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on successful cleanup, error code on failure
 * 
 * Saves the encryption library to disk, frees allocated memory, closes open files,
 * and performs any other necessary cleanup operations.
 */
int cleanup_program(encryption_library_t *library);

/* ========================================================================
 * USER INTERFACE FUNCTIONS
 * ======================================================================== */

/**
 * @brief Display the main menu options to the user
 * 
 * Shows all available commands including encrypt file, decrypt file,
 * list library, manage files, and exit program.
 */
void display_main_menu(void);

/**
 * @brief Get and validate user input for menu selection
 * @param prompt String prompt to display to user
 * @param min_value Minimum valid input value
 * @param max_value Maximum valid input value
 * @return Valid user input within specified range
 * 
 * Prompts user for input, validates the input is within the specified range,
 * and re-prompts if invalid input is provided.
 */
int get_user_choice(const char *prompt, int min_value, int max_value);

/**
 * @brief Process user menu selection and call appropriate function
 * @param choice User's menu selection
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on successful command execution, error code on failure
 * 
 * Routes user menu selections to the appropriate handler functions based on
 * the selected command.
 */
int process_user_command(int choice, encryption_library_t *library);

/**
 * @brief Display error message to user with context
 * @param error_code Error code from failed operation
 * @param context Additional context string for the error
 * 
 * Shows user-friendly error messages based on error codes and provides
 * context about what operation failed.
 */
void display_error(int error_code, const char *context);

/* ========================================================================
 * FILE ENCRYPTION FUNCTIONS
 * ======================================================================== */

/**
 * @brief Complete workflow for encrypting a user-specified file
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on successful encryption, error code on failure
 * 
 * Guides user through file selection, compression choice, password entry,
 * performs encryption, and adds the file to the library.
 */
int encrypt_file_workflow(encryption_library_t *library);

/**
 * @brief Get and validate file path from user input
 * @param file_path Buffer to store the validated file path
 * @param buffer_size Size of the file_path buffer
 * @return SUCCESS if valid path obtained, error code on failure
 * 
 * Prompts user for file path, validates the path exists and is accessible,
 * and stores the absolute path in the provided buffer.
 */
int get_file_path_from_user(char *file_path, size_t buffer_size);

/**
 * @brief Ask user whether to compress file before encryption
 * @return 1 if user wants compression, 0 if not, -1 on error
 * 
 * Presents compression option to user and validates their choice.
 * Explains benefits and trade-offs of compression.
 */
int ask_compression_preference(void);

/**
 * @brief Encrypt a file with optional compression
 * @param input_path Path to the original file to encrypt
 * @param output_path Path where encrypted file will be saved
 * @param password Password to use for encryption
 * @param use_compression Whether to compress before encrypting
 * @param metadata Pointer to metadata structure to populate
 * @return SUCCESS on successful encryption, error code on failure
 * 
 * Performs the complete encryption process including optional compression,
 * encryption using the provided password, and populating metadata.
 */
int encrypt_file(const char *input_path, const char *output_path, 
                 const char *password, int use_compression, 
                encryption_method_t method, file_metadata_t *metadata);

/**
 * @brief Apply compression algorithm to file data
 * @param input_data Pointer to input data buffer
 * @param input_size Size of input data in bytes
 * @param output_data Pointer to output buffer (must be allocated)
 * @param output_size Pointer to store compressed data size
 * @return SUCCESS on successful compression, error code on failure
 * 
 * Applies a simple compression algorithm (LZ77-style) to reduce file size
 * before encryption. Uses only standard C library functions.
 */
int compress_data(const unsigned char *input_data, long input_size,
                  unsigned char *output_data, long *output_size);

/**
 * @brief Apply encryption cipher to file data
 * @param input_data Pointer to input data buffer
 * @param data_size Size of data in bytes
 * @param password Password for encryption key derivation
 * @param output_data Pointer to output buffer for encrypted data
 * @return SUCCESS on successful encryption, error code on failure
 * 
 * Applies encryption cipher to data using password-derived key.
 * Uses XOR-based encryption with key strengthening techniques.
 */
int encrypt_data(const unsigned char *input_data, long data_size,
                 const char *password, unsigned char *output_data);

/* ========================================================================
 * FILE DECRYPTION FUNCTIONS
 * ======================================================================== */

/**
 * @brief Complete workflow for decrypting a file from the library
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on successful decryption, error code on failure
 * 
 * Shows encrypted files, lets user select one, prompts for password,
 * and performs decryption with optional decompression.
 */
int decrypt_file_workflow(encryption_library_t *library);

/**
 * @brief Display encrypted files and let user select one
 * @param library Pointer to the encryption library structure
 * @param selected_index Pointer to store index of selected file
 * @return SUCCESS if file selected, error code on failure
 * 
 * Shows list of encrypted files with basic information and lets user
 * select which file to decrypt by entering its number.
 */
int select_file_for_decryption(encryption_library_t *library, int *selected_index);

/**
 * @brief Securely get password from user for decryption
 * @param password Buffer to store the password
 * @param buffer_size Size of the password buffer
 * @return SUCCESS if password obtained, error code on failure
 * 
 * Prompts user for password with hidden input (no echo to screen).
 * Validates password length and format.
 */
int get_decryption_password(char *password, size_t buffer_size);

/**
 * @brief Decrypt a file and optionally decompress
 * @param encrypted_path Path to the encrypted file
 * @param output_path Path where decrypted file will be saved
 * @param password Password for decryption
 * @param metadata Pointer to file metadata structure
 * @return SUCCESS on successful decryption, error code on failure
 * 
 * Performs complete decryption process including password verification,
 * decryption, and optional decompression based on metadata.
 */
int decrypt_file(const char *encrypted_path, const char *output_path,
                 const char *password, encryption_method_t method, const file_metadata_t *metadata);

/**
 * @brief Apply decryption cipher to encrypted data
 * @param encrypted_data Pointer to encrypted data buffer
 * @param data_size Size of encrypted data in bytes
 * @param password Password for key derivation
 * @param output_data Pointer to output buffer for decrypted data
 * @return SUCCESS on successful decryption, error code on failure
 * 
 * Reverses the encryption process using the same password and key
 * derivation method used during encryption.
 */
int decrypt_data(const unsigned char *encrypted_data, long data_size,
                 const char *password, unsigned char *output_data);

/**
 * @brief Apply decompression algorithm to compressed data
 * @param compressed_data Pointer to compressed data buffer
 * @param compressed_size Size of compressed data in bytes
 * @param output_data Pointer to output buffer for decompressed data
 * @param output_size Pointer to store decompressed data size
 * @return SUCCESS on successful decompression, error code on failure
 * 
 * Reverses the compression algorithm to restore original file data.
 * Must match the compression algorithm used during encryption.
 */
int decompress_data(const unsigned char *compressed_data, long compressed_size,
                    unsigned char *output_data, long *output_size);

/* ========================================================================
 * LIBRARY MANAGEMENT FUNCTIONS
 * ======================================================================== */

/**
 * @brief Load encryption library from disk
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on successful load, error code on failure
 * 
 * Reads the encryption library file from disk and populates the library
 * structure. Creates new library if file doesn't exist.
 */
int load_encryption_library(encryption_library_t *library);

/**
 * @brief Save encryption library to disk
 * @param library Pointer to the encryption library structure
 * @return SUCCESS on successful save, error code on failure
 * 
 * Writes the current encryption library to disk, creating a backup
 * of the previous version if it exists.
 */
int save_encryption_library(encryption_library_t *library);

/**
 * @brief Add new encrypted file entry to library
 * @param library Pointer to the encryption library structure
 * @param metadata Pointer to file metadata to add
 * @return SUCCESS on successful addition, error code on failure
 * 
 * Adds a new encrypted file entry to the library with all metadata.
 * Validates that library has space and metadata is complete.
 */
int add_file_to_library(encryption_library_t *library, const file_metadata_t *metadata);

/**
 * @brief Remove file entry from library
 * @param library Pointer to the encryption library structure
 * @param index Index of file entry to remove
 * @return SUCCESS on successful removal, error code on failure
 * 
 * Removes a file entry from the library and compacts the remaining entries.
 * Validates index is within valid range.
 */
int remove_file_from_library(encryption_library_t *library, int index);

/**
 * @brief Display all files in the encryption library
 * @param library Pointer to the encryption library structure
 * @param sort_option How to sort the displayed files
 * 
 * Shows formatted list of all encrypted files with metadata including
 * name, size, date, and type. Sorts according to specified option.
 */
void display_library_contents(encryption_library_t *library, sort_option_t sort_option);

/**
 * @brief Search for files in library by name pattern
 * @param library Pointer to the encryption library structure
 * @param search_pattern Pattern to search for in filenames
 * @param results Array to store indices of matching files
 * @param max_results Maximum number of results to return
 * @return Number of matching files found
 * 
 * Searches library entries for files matching the given pattern.
 * Supports wildcards and partial matches.
 */
int search_library_by_name(encryption_library_t *library, const char *search_pattern,
                           int *results, int max_results);

/* ========================================================================
 * FILE MANAGEMENT FUNCTIONS
 * ======================================================================== */

/**
 * @brief Delete an encrypted file from library and filesystem
 * @param library Pointer to the encryption library structure
 * @param index Index of file to delete
 * @return SUCCESS on successful deletion, error code on failure
 * 
 * Prompts user for confirmation, removes encrypted file from filesystem,
 * and removes entry from library. Provides secure deletion option.
 */
int delete_encrypted_file(encryption_library_t *library, int index);

/**
 * @brief Rename an encrypted file in library and filesystem
 * @param library Pointer to the encryption library structure
 * @param index Index of file to rename
 * @param new_name New name for the file
 * @return SUCCESS on successful rename, error code on failure
 * 
 * Updates both the filesystem filename and library metadata.
 * Validates new name doesn't conflict with existing files.
 */
int rename_encrypted_file(encryption_library_t *library, int index, const char *new_name);

/**
 * @brief Display detailed information about an encrypted file
 * @param library Pointer to the encryption library structure
 * @param index Index of file to show information for
 * 
 * Shows comprehensive metadata including original size, encrypted size,
 * compression ratio, encryption date, file type, and integrity status.
 */
void display_file_information(encryption_library_t *library, int index);

/**
 * @brief Get user confirmation for destructive operations
 * @param operation_description Description of the operation requiring confirmation
 * @return 1 if user confirms, 0 if user cancels
 * 
 * Prompts user to confirm potentially destructive operations like
 * file deletion. Requires explicit "yes" response.
 */
int get_user_confirmation(const char *operation_description);

/* ========================================================================
 * SORTING AND ORGANIZATION FUNCTIONS
 * ======================================================================== */

/**
 * @brief Sort library entries by filename
 * @param library Pointer to the encryption library structure
 * 
 * Sorts library entries alphabetically by original filename using
 * case-insensitive comparison.
 */
void sort_library_by_name(encryption_library_t *library);

/**
 * @brief Sort library entries by encryption date
 * @param library Pointer to the encryption library structure
 * 
 * Sorts library entries by encryption date, most recent first.
 */
void sort_library_by_date(encryption_library_t *library);

/**
 * @brief Sort library entries by file size
 * @param library Pointer to the encryption library structure
 * 
 * Sorts library entries by original file size, largest first.
 */
void sort_library_by_size(encryption_library_t *library);

/**
 * @brief Sort library entries by file type
 * @param library Pointer to the encryption library structure
 * 
 * Sorts library entries by file extension/type, grouping similar
 * file types together.
 */
void sort_library_by_type(encryption_library_t *library);

/**
 * @brief Generic comparison function for sorting metadata entries
 * @param a Pointer to first metadata entry
 * @param b Pointer to second metadata entry
 * @param sort_type Type of sorting to apply
 * @return Negative if a < b, 0 if a == b, positive if a > b
 * 
 * Generic comparison function used by sorting algorithms to compare
 * two metadata entries based on the specified sort criteria.
 */
int compare_metadata_entries(const file_metadata_t *a, const file_metadata_t *b, 
                            sort_option_t sort_type);

/* ========================================================================
 * UTILITY AND HELPER FUNCTIONS
 * ======================================================================== */

/**
 * @brief Validate that a file path exists and is accessible
 * @param file_path Path to validate
 * @return SUCCESS if path is valid and accessible, error code otherwise
 * 
 * Checks that the specified path exists, is a regular file (not directory),
 * and can be read by the current user.
 */
int validate_file_path(const char *file_path);

/**
 * @brief Generate a secure filename for encrypted file
 * @param original_path Original file path
 * @param encrypted_filename Buffer to store generated filename
 * @param buffer_size Size of the encrypted_filename buffer
 * @return SUCCESS on successful generation, error code on failure
 * 
 * Creates a unique, secure filename for the encrypted version that doesn't
 * reveal information about the original file.
 */
int generate_encrypted_filename(const char *original_path, char *encrypted_filename, 
                               size_t buffer_size, unsigned long id);

/**
 * @brief Calculate simple checksum for file integrity verification
 * @param file_path Path to file for checksum calculation
 * @param checksum Buffer to store calculated checksum string
 * @param buffer_size Size of the checksum buffer
 * @return SUCCESS on successful calculation, error code on failure
 * 
 * Calculates a simple checksum (using standard library functions only)
 * to verify file integrity after encryption/decryption.
 */
int calculate_file_checksum(const char *file_path, char *checksum, size_t buffer_size);

/**
 * @brief Get file extension from filename
 * @param filename Input filename
 * @param extension Buffer to store extracted extension
 * @param buffer_size Size of the extension buffer
 * @return SUCCESS if extension found, error code if no extension
 * 
 * Extracts file extension from filename for categorization purposes.
 * Returns empty string if no extension found.
 */
int get_file_extension(const char *filename, char *extension, size_t buffer_size);

/**
 * @brief Securely clear memory containing sensitive data
 * @param data Pointer to data to clear
 * @param size Number of bytes to clear
 * 
 * Safely overwrites memory containing passwords or keys to prevent
 * sensitive data from remaining in memory after use.
 */
void secure_memory_clear(void *data, size_t size);

/**
 * @brief Safe string copy with bounds checking
 * @param dest Destination buffer
 * @param src Source string
 * @param dest_size Size of destination buffer
 * @return SUCCESS on successful copy, error code on buffer overflow
 * 
 * Safely copies string ensuring null termination and preventing
 * buffer overflows.
 */
int safe_string_copy(char *dest, const char *src, size_t dest_size);

/**
 * @brief Convert file size to human-readable format
 * @param size File size in bytes
 * @param buffer Buffer to store formatted size string
 * @param buffer_size Size of the buffer
 * 
 * Converts byte count to human-readable format (e.g., "1.5 MB", "234 KB")
 * for display purposes.
 */
void format_file_size(long size, char *buffer, size_t buffer_size);

/**
 * @brief Derive encryption key from password using key strengthening
 * @param password Input password
 * @param salt Salt value for key derivation
 * @param key_buffer Buffer to store derived key
 * @param key_length Length of key to generate
 * @return SUCCESS on successful key derivation, error code on failure
 * 
 * Derives encryption key from password using salt and multiple iterations
 * to strengthen the key against brute force attacks.
 */
int derive_encryption_key(const char *password, const char *salt,
                         unsigned char *key_buffer, size_t key_length);

#endif /* CCRYPT_H */