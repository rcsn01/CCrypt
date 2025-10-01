/**
 * @file ccrypt.c
 * @brief Main implementation file for CCrypt - A CLI file encryption and compression tool
 * @author Chu-Cheng Yu,               - Add your name for submission
 * @date September 2025
 * 
 * This file contains the main implementation of CCrypt, a command-line tool for
 * encrypting and decrypting files with optional compression and library management.
 * 
 * 
 * $env:Path = 'C:\msys64\mingw64\bin;' + $env:Path
 * gcc --version
 * Compilation: gcc -o ccrypt ccrypt.c -lm
 * Usage: ./ccrypt
 */

#include "ccrypt.h"

/* ========================================================================
 * GLOBAL VARIABLES
 * ======================================================================== */

/* Global encryption library instance */
static encryption_library_t g_library;

/* ========================================================================
 * MAIN PROGRAM FUNCTIONS
 * ======================================================================== */

/**
 * @brief Main entry point for the CCrypt program
 * @author [Chu-Cheng Yu]
 */
int main(int argc, char *argv[])
{
    printf("CCrypt v1.0 - File Encryption and Compression Tool\n");
    printf("==================================================\n\n");
    
    /* Initialize program and load library */
    if (initialize_program(&g_library) != SUCCESS) {
        fprintf(stderr, "Error: Failed to initialize program\n");
        return EXIT_FAILURE;
    }
    
    /* Run main program loop */
    int result = main_menu_loop(&g_library);
    
    /* Cleanup and save library */
    if (cleanup_program(&g_library) != SUCCESS) {
        fprintf(stderr, "Warning: Failed to properly cleanup program\n");
    }
    
    printf("\nThank you for using CCrypt!\n");
    return (result == SUCCESS) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/**
 * @brief Initialize program components and load encryption library
 * @author [Chu-Cheng Yu]
 */
int initialize_program(encryption_library_t *library)
{
    /* Initialize library structure */
    memset(library, 0, sizeof(encryption_library_t));
    library->count = 0;
    library->is_modified = 0;
    /* Initialize ID counter */
    library->next_id = 1;
    
    /* Load existing library from disk */
    int result = load_encryption_library(library);
    if (result == SUCCESS) {
        printf("Loaded encryption library with %d entries\n", library->count);
    } else if (result == ERROR_FILE_NOT_FOUND) {
        printf("Creating new encryption library\n");
        result = SUCCESS; /* New library is okay */
    }
    
    return result;
}

/**
 * @brief Main program loop displaying menu and processing user commands
 * @author [Chu-Cheng Yu]
 */
int main_menu_loop(encryption_library_t *library)
{
    int choice;
    int result = SUCCESS;
    
    do {
        printf("\n");
        display_main_menu();
        choice = get_user_choice("Select an option: ", 1, 6);
        
        result = process_user_command(choice, library);
        
        if (result != SUCCESS && choice != 6) {
            display_error(result, "Command execution");
            result = SUCCESS; /* Continue loop on non-fatal errors */
        }
        
    } while (choice != 6 && result == SUCCESS);
    
    return result;
}

/**
 * @brief Clean up program resources and save library before exit
 * @author [Chu-Cheng Yu]
 */
int cleanup_program(encryption_library_t *library)
{
    int result = SUCCESS;
    
    /* Save library if modified */
    if (library->is_modified) {
        printf("Saving encryption library...\n");
        result = save_encryption_library(library);
    }
    
    /* Clear sensitive data from memory */
    secure_memory_clear(library, sizeof(encryption_library_t));
    
    return result;
}

/* ========================================================================
 * USER INTERFACE FUNCTIONS
 * Author: [Chu-Cheng Yu]
 * ======================================================================== */

/**
 * @brief Display the main menu options to the user
 * @author [Chu-Cheng Yu]
 */
void display_main_menu(void)
{
    printf("========================================\n");
    printf("               MAIN MENU                \n");
    printf("========================================\n");
    printf("1. Encrypt a file\n");
    printf("2. Decrypt a file\n");
    printf("3. Show encrypted files library\n");
    printf("4. Manage encrypted files\n");
    printf("5. Sort library\n");
    printf("6. Exit program\n");
    printf("========================================\n");
}

/**
 * @brief Get and validate user input for menu selection
 * @author [Chu-Cheng Yu]
 */
int get_user_choice(const char *prompt, int min_value, int max_value)
{
    char line[64];
    long val;
    char *endptr;

    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) {
            /* input error - return a safe default */
            return min_value;
        }

        /* strip leading spaces */
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;

        val = strtol(p, &endptr, 10);
        if (endptr == p) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        /* allow trailing newline or NUL only */
        if (*endptr != '\n' && *endptr != '\0') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (val < min_value || val > max_value) {
            printf("Please enter a number between %d and %d\n", min_value, max_value);
            continue;
        }

        return (int)val;
    }
}

/**
 * @brief Process user menu selection and call appropriate function
 * @author [Chu-Cheng Yu]
 */
int process_user_command(int choice, encryption_library_t *library)
{
    int result = SUCCESS;
    int sort_choice;
    
    switch (choice) {
        case 1:
            result = encrypt_file_workflow(library);
            break;
        case 2:
            result = decrypt_file_workflow(library);
            break;
        case 3:
            display_library_contents(library, SORT_BY_NAME);
            break;
        case 4:
            /* File management menu - to be implemented */
            printf("File management functionality coming soon...\n");
            break;
        case 5:
            printf("Sort by: 1=Name, 2=Date, 3=Size, 4=Type\n");
            sort_choice = get_user_choice("Sort option: ", 1, 4);
            display_library_contents(library, (sort_option_t)sort_choice);
            break;
        case 6:
            printf("Exiting program...\n");
            break;
        default:
            result = ERROR_INVALID_PATH; /* Invalid choice */
            break;
    }
    
    return result;
}

/**
 * @brief Display error message to user with context
 * @author [Chu-Cheng Yu]
 */
void display_error(int error_code, const char *context)
{
    printf("\nError in %s: ", context);
    
    switch (error_code) {
        case ERROR_FILE_NOT_FOUND:
            printf("File not found\n");
            break;
        case ERROR_INVALID_PATH:
            printf("Invalid file path\n");
            break;
        case ERROR_PERMISSION_DENIED:
            printf("Permission denied\n");
            break;
        case ERROR_INVALID_PASSWORD:
            printf("Invalid password\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("Memory allocation failed\n");
            break;
        case ERROR_LIBRARY_CORRUPT:
            printf("Library file is corrupted\n");
            break;
        case ERROR_ENCRYPTION_FAILED:
            printf("Encryption operation failed\n");
            break;
        case ERROR_COMPRESSION_FAILED:
            printf("Compression operation failed\n");
            break;
        default:
            printf("Unknown error (code: %d)\n", error_code);
            break;
    }
}

/* ========================================================================
 * FILE ENCRYPTION FUNCTIONS
 * ======================================================================== */

/**
 * @brief Complete workflow for encrypting a user-specified file
 * @author [Chu-Cheng Yu]
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

/**
 * @brief Get and validate file path from user input
 * @author [Chu-Cheng Yu]
 */
int get_file_path_from_user(char *file_path, size_t buffer_size)
{
    /* Prompt and read a line */
    printf("Enter the path to the file to encrypt: ");
    if (!fgets(file_path, (int)buffer_size, stdin)) {
        return ERROR_INVALID_PATH;
    }

    /* Remove trailing newline and CR characters */
    size_t len = strlen(file_path);
    while (len > 0 && (file_path[len - 1] == '\n' || file_path[len - 1] == '\r')) {
        file_path[--len] = '\0';
    }

    /* Trim leading spaces/tabs */
    char *start = file_path;
    while (*start == ' ' || *start == '\t') start++;

    /* Trim trailing spaces/tabs (start may have moved) */
    char *end = start + strlen(start);
    while (end > start && (*(end - 1) == ' ' || *(end - 1) == '\t')) {
        *(--end) = '\0';
    }

    /* Remove surrounding double or single quotes if present */
    if ((start[0] == '"' && end > start + 1 && end[-1] == '"') ||
        (start[0] == '\'' && end > start + 1 && end[-1] == '\'')) {
        start++;
        end[-1] = '\0';
    }

    /* Move cleaned string back to file_path buffer if trimmed */
    if (start != file_path) {
        size_t moved = strlen(start) + 1;
        if (moved > buffer_size) return ERROR_INVALID_PATH;
        memmove(file_path, start, moved);
    }

    /* Validate path (fopen inside validate_file_path) */
    return validate_file_path(file_path);
}

/**
 * @brief Ask user whether to compress file before encryption
 * @author [Chu-Cheng Yu]
 */
int ask_compression_preference(void)
{
    int choice = get_user_choice("Compress before encryption? 1=Yes 2=No: ", 1, 2);
    return (choice == 1) ? 1 : 0;
}

/**
 * @brief Encrypt a file with optional compression
 * @author [Your Name Here]
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

/* ========================================================================
 * ADDITIONAL FUNCTION STUBS
 * ======================================================================== */

/* Note: Due to length constraints, I'm including key function stubs.
 * In a complete implementation, all functions from the header would be implemented. */

/**
 * @brief Apply compression algorithm to file data
 * @author [Gordon Huang]
 */
int compress_data(const unsigned char *input_data, long input_size,
                  unsigned char *output_data, long *output_size)
{
    /* TODO: Implement simple compression algorithm */

    return SUCCESS;
}

/**
 * @brief Apply encryption cipher to file data
 * @author [Your Name Here]
 */
int encrypt_data(const unsigned char *input_data, long data_size,
                 const char *password, unsigned char *output_data)
{
    /* TODO: Implement XOR-based encryption with key derivation */
    /* Simple XOR placeholder - replace with proper implementation */
    return SUCCESS;
}

/**
 * @brief Complete workflow for decrypting a file from the library
 * @author [Your Name Here]
 */
int decrypt_file_workflow(encryption_library_t *library)
{
    /* TODO: Implement decryption workflow */
    printf("Decryption workflow - to be implemented\n");
    return SUCCESS;
}

/**
 * @brief Load encryption library from disk
 * @author [Gordon Huang]
 */
int load_encryption_library(encryption_library_t *library)
{
    /* TODO: Implement library loading logic */
    return SUCCESS;
}

/**
 * @brief Save encryption library to disk
 * @author [Your Name Here]
 */
int save_encryption_library(encryption_library_t *library)
{
    return SUCCESS;
}

/**
 * @brief Add new encrypted file entry to library
 * @author [Chu-Cheng Yu]
 */
int add_file_to_library(encryption_library_t *library, const file_metadata_t *metadata)
{
    if (library->count >= MAX_LIBRARY_ENTRIES) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    library->entries[library->count] = *metadata;
    library->count++;
    library->is_modified = 1;
    
    return SUCCESS;
}

/**
 * @brief Display all files in the encryption library
 * @author [Chu-Cheng Yu]
 */
void display_library_contents(encryption_library_t *library, sort_option_t sort_option)
{
    if (library->count == 0) {
        printf("No encrypted files in library.\n");
        return;
    }
    
    /* TODO: Implement sorting based on sort_option */
    
    printf("\nEncrypted Files Library (%d entries):\n", library->count);
    printf("=====================================\n");
    printf("%-3s %-20s %-10s %-12s %-10s\n", "No.", "Filename", "Size", "Date", "Compressed");
    printf("-------------------------------------------------------------\n");
    
    for (int i = 0; i < library->count; i++) {
        printf("%-3d %-20s %-10ld %-12lu %-10s\n", 
               i + 1,
               library->entries[i].original_filename,
               library->entries[i].original_size,
               library->entries[i].encryption_id,
               library->entries[i].is_compressed ? "Yes" : "No");
    }
}

/* ========================================================================
 * UTILITY FUNCTIONS
 * ======================================================================== */

/**
 * @brief Validate that a file path exists and is accessible
 * @author [Chu-Cheng Yu]
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

/**
 * @brief Generate a secure filename for encrypted file
 * @author [Chu-Cheng Yu]
 */
int generate_encrypted_filename(const char *original_path, char *encrypted_filename, 
                               size_t buffer_size, unsigned long id)
{
    /* TODO: Generate secure filename based on hash or timestamp */
    /* Use the provided id to create a deterministic filename without time.h */
    snprintf(encrypted_filename, buffer_size, "encrypted_%lu.ccrypt", id);
    return SUCCESS;
}

/**
 * @brief Securely clear memory containing sensitive data
 * @author [Chu-Cheng Yu]
 */
void secure_memory_clear(void *data, size_t size)
{
    volatile unsigned char *ptr = (volatile unsigned char *)data;
    for (size_t i = 0; i < size; i++) {
        ptr[i] = 0;
    }
}

/**
 * @brief Safe string copy with bounds checking
 * @author [Chu-Cheng Yu]
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

/**
 * @brief Decrypt an encrypted file (placeholder implementation)
 * @param encrypted_path Path to the encrypted input file
 * @param output_path Path where the decrypted output should be written
 * @param password Password used for decryption
 * @param method Encryption method used (encryption_method_t)
 * @param metadata Optional pointer to file metadata associated with the file
 * @return SUCCESS on success, or an error code on failure
 * @author [Empty]
 *
 * NOTE: This is a stub. A full implementation should read the encrypted file,
 * verify integrity (checksum), decrypt according to the chosen method,
 * optionally decompress, and write the resulting plaintext to output_path.
 */
int decrypt_file(const char *encrypted_path, const char *output_path,
                 const char *password, encryption_method_t method, const file_metadata_t *metadata)
{
    /* TODO: Implement full decryption: read file, decrypt, verify checksum, write output */
    printf("[not implemented] Decrypting file: %s -> %s (method=%d)\n", encrypted_path, output_path, (int)method);
    (void)password; (void)metadata;
    return SUCCESS;
}

/**
 * @brief Present library to user and select an entry for decryption (stub)
 * @param library Pointer to the encryption library instance
 * @param selected_index Out parameter to receive the selected index (0-based)
 * @return SUCCESS if a selection was made, or ERROR_FILE_NOT_FOUND if library empty
 * @author [Empty]
 *
 * TODO: Replace with interactive selection UI that lists entries and validates input.
 */
int select_file_for_decryption(encryption_library_t *library, int *selected_index)
{
    if (!library || library->count == 0) return ERROR_FILE_NOT_FOUND;
    /* Default to first entry for the stub */
    if (selected_index) *selected_index = 0;
    return SUCCESS;
}

/**
 * @brief Prompt user for a decryption password (non-hidden input)
 * @param password Buffer to receive the password string
 * @param buffer_size Size of the password buffer
 * @return SUCCESS on success, or ERROR_INVALID_PASSWORD on failure
 * @author [Empty]
 *
 * Note: This reads the password with fgets and does not hide input. For real
 * applications, a platform-specific no-echo input should be used.
 */
int get_decryption_password(char *password, size_t buffer_size)
{
    if (!password || buffer_size == 0) return ERROR_INVALID_PASSWORD;
    printf("Enter decryption password: ");
    if (!fgets(password, (int)buffer_size, stdin)) return ERROR_INVALID_PASSWORD;
    /* strip newline */
    size_t len = strlen(password);
    if (len > 0 && (password[len - 1] == '\n' || password[len - 1] == '\r')) password[len - 1] = '\0';
    return SUCCESS;
}

/**
 * @brief Decrypt a buffer of data using the supplied password (stub)
 * @param encrypted_data Pointer to input encrypted bytes
 * @param data_size Size of input buffer in bytes
 * @param password Password used to derive the decryption key
 * @param output_data Output buffer to receive decrypted bytes (must be allocated)
 * @return SUCCESS on success, error code on invalid input
 * @author [Empty]
 *
 * TODO: Implement algorithm-specific decryption (XOR, Caesar). This stub
 * currently copies input to output (no decryption).
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

/**
 * @brief Decompress a buffer produced by compress_data (stub)
 * @param compressed_data Pointer to compressed input bytes
 * @param compressed_size Size of compressed input in bytes
 * @param output_data Output buffer to receive decompressed bytes (must be allocated)
 * @param output_size Out parameter to receive number of decompressed bytes
 * @return SUCCESS on success, error code on invalid input
 * @author [Empty]
 *
 * TODO: Implement decompression algorithm consistent with compress_data.
 * Currently this stub performs a simple passthrough copy.
 */
int decompress_data(const unsigned char *compressed_data, long compressed_size,
                    unsigned char *output_data, long *output_size)
{
    /* TODO: Implement decompression that matches compress_data (RLE or similar) */
    if (!compressed_data || compressed_size <= 0 || !output_data || !output_size) return ERROR_INVALID_PATH;
    /* passthrough */
    memcpy(output_data, compressed_data, (size_t)compressed_size);
    *output_size = compressed_size;
    return SUCCESS;
}

/**
 * @brief Remove a library entry at the specified index
 * @param library Pointer to the encryption library
 * @param index Index of entry to remove (0-based)
 * @return SUCCESS on success, or error code for invalid parameters
 * @author [Empty]
 *
 * Shifts subsequent entries down to keep the array compact and marks the
 * library as modified for persistence.
 */
int remove_file_from_library(encryption_library_t *library, int index)
{
    if (!library) return ERROR_INVALID_PATH;
    if (index < 0 || index >= library->count) return ERROR_INVALID_PATH;
    /* Shift entries down to fill gap */
    for (int i = index; i + 1 < library->count; ++i) {
        library->entries[i] = library->entries[i + 1];
    }
    library->count--;
    library->is_modified = 1;
    return SUCCESS;
}

/**
 * @brief Search the library for filenames containing a substring
 * @param library Pointer to encryption library
 * @param search_pattern Substring to search for
 * @param results Array to receive matched indices
 * @param max_results Maximum number of results to store in results array
 * @return Number of matches found (may be 0)
 * @author [Empty]
 */
int search_library_by_name(encryption_library_t *library, const char *search_pattern,
                           int *results, int max_results)
{
    if (!library || !search_pattern || !results || max_results <= 0) return 0;
    int found = 0;
    for (int i = 0; i < library->count && found < max_results; ++i) {
        if (strstr(library->entries[i].original_filename, search_pattern)) {
            results[found++] = i;
        }
    }
    return found;
}

/**
 * @brief Delete an encrypted file from disk and remove it from the library
 * @param library Pointer to the encryption library
 * @param index Index of the entry to delete (0-based)
 * @return SUCCESS on success, or error code on failure
 * @author [Empty]
 *
 * Prompts for user confirmation, attempts to remove the file from the
 * filesystem (best-effort), and removes the metadata entry from the library.
 */
int delete_encrypted_file(encryption_library_t *library, int index)
{
    if (!library) return ERROR_INVALID_PATH;
    if (index < 0 || index >= library->count) return ERROR_INVALID_PATH;
    /* Ask for confirmation */
    char prompt[256];
    snprintf(prompt, sizeof(prompt), "Delete '%s'? (y/n): ", library->entries[index].encrypted_filename);
    if (!get_user_confirmation(prompt)) return SUCCESS; /* cancelled */
    /* Remove file from filesystem */
    if (remove(library->entries[index].encrypted_filename) != 0) {
        /* best-effort: still remove library entry */
        /* continue */
    }
    return remove_file_from_library(library, index);
}

/**
 * @brief Rename an encrypted file on disk and update library metadata
 * @param library Pointer to the encryption library
 * @param index Index of the entry to rename (0-based)
 * @param new_name New filename to apply
 * @return SUCCESS on success, or an error code on failure
 * @author [Chu-Cheng Yu]
 */
int rename_encrypted_file(encryption_library_t *library, int index, const char *new_name)
{
    if (!library || !new_name) return ERROR_INVALID_PATH;
    if (index < 0 || index >= library->count) return ERROR_INVALID_PATH;
    /* Attempt to rename on filesystem */
    if (rename(library->entries[index].encrypted_filename, new_name) != 0) {
        return ERROR_PERMISSION_DENIED;
    }
    safe_string_copy(library->entries[index].encrypted_filename, new_name, sizeof(library->entries[index].encrypted_filename));
    library->is_modified = 1;
    return SUCCESS;
}

/**
 * @brief Print detailed information about a library entry
 * @param library Pointer to the encryption library
 * @param index Index of the entry to display (0-based)
 * @return void
 * @author [Chu-Cheng Yu]
 */
void display_file_information(encryption_library_t *library, int index)
{
    if (!library) return;
    if (index < 0 || index >= library->count) return;
    file_metadata_t *m = &library->entries[index];
    printf("File information for entry %d:\n", index + 1);
    printf(" Original: %s\n", m->original_filename);
    printf(" Encrypted: %s\n", m->encrypted_filename);
    printf(" Original size: %ld\n", m->original_size);
    printf(" Encrypted size: %ld\n", m->encrypted_size);
    printf(" Compressed: %s\n", m->is_compressed ? "Yes" : "No");
    printf(" Method: %d\n", m->encryption_method);
}

/**
 * @brief Prompt user to confirm an operation (yes/no)
 * @param operation_description Prompt string to display
 * @return 1 if user confirms (y/Y), 0 otherwise
 * @author [Chu-Cheng Yu]
 */
int get_user_confirmation(const char *operation_description)
{
    if (!operation_description) return 0;
    printf("%s", operation_description);
    char line[8];
    if (!fgets(line, sizeof(line), stdin)) return 0;
    return (line[0] == 'y' || line[0] == 'Y') ? 1 : 0;
}

/* Simple sort helpers: use qsort with wrappers */
static int cmp_name(const void *a, const void *b)
{
    const file_metadata_t *x = (const file_metadata_t *)a;
    const file_metadata_t *y = (const file_metadata_t *)b;
    return strcasecmp(x->original_filename, y->original_filename);
}

static int cmp_date(const void *a, const void *b)
{
    const file_metadata_t *x = (const file_metadata_t *)a;
    const file_metadata_t *y = (const file_metadata_t *)b;
    if (x->encryption_id < y->encryption_id) return 1;
    if (x->encryption_id > y->encryption_id) return -1;
    return 0;
}

static int cmp_size(const void *a, const void *b)
{
    const file_metadata_t *x = (const file_metadata_t *)a;
    const file_metadata_t *y = (const file_metadata_t *)b;
    if (x->original_size < y->original_size) return 1;
    if (x->original_size > y->original_size) return -1;
    return 0;
}

static int cmp_type(const void *a, const void *b)
{
    const file_metadata_t *x = (const file_metadata_t *)a;
    const file_metadata_t *y = (const file_metadata_t *)b;
    return strcasecmp(x->file_type, y->file_type);
}

/**
 * @brief Sort library entries alphabetically by original filename
 * @param library Pointer to the encryption library
 * @author [Chu-Cheng Yu]
 */
void sort_library_by_name(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    qsort(library->entries, (size_t)library->count, sizeof(file_metadata_t), cmp_name);
}

/**
 * @brief Sort library entries by encryption id (most recent first)
 * @param library Pointer to the encryption library
 * @author [Chu-Cheng Yu]
 */
void sort_library_by_date(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    qsort(library->entries, (size_t)library->count, sizeof(file_metadata_t), cmp_date);
}

/**
 * @brief Sort library entries by original file size (largest first)
 * @param library Pointer to the encryption library
 * @author [Chu-Cheng Yu]
 */
void sort_library_by_size(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    qsort(library->entries, (size_t)library->count, sizeof(file_metadata_t), cmp_size);
}

/**
 * @brief Sort library entries by file type/extension
 * @param library Pointer to the encryption library
 * @author [Chu-Cheng Yu]
 */
void sort_library_by_type(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    qsort(library->entries, (size_t)library->count, sizeof(file_metadata_t), cmp_type);
}

/**
 * @brief Compare two metadata entries according to a sort option
 * @param a Pointer to first metadata entry
 * @param b Pointer to second metadata entry
 * @param sort_type Sorting criteria (see sort_option_t)
 * @return negative/zero/positive as in strcmp-style comparators
 * @author [Chu-Cheng Yu]
 */
int compare_metadata_entries(const file_metadata_t *a, const file_metadata_t *b, 
                            sort_option_t sort_type)
{
    if (!a || !b) return 0;
    switch (sort_type) {
        case SORT_BY_NAME: return strcasecmp(a->original_filename, b->original_filename);
        case SORT_BY_DATE: return (a->encryption_id < b->encryption_id) ? 1 : (a->encryption_id > b->encryption_id) ? -1 : 0;
        case SORT_BY_SIZE: return (a->original_size < b->original_size) ? 1 : (a->original_size > b->original_size) ? -1 : 0;
        case SORT_BY_TYPE: return strcasecmp(a->file_type, b->file_type);
        default: return 0;
    }
}

/**
 * @brief Compute a simple non-cryptographic checksum for a file
 * @param file_path Path to the file
 * @param checksum Buffer to receive a textual hex checksum
 * @param buffer_size Size of the checksum buffer
 * @return SUCCESS on success, or an error code on failure
 * @author [Chu-Cheng Yu]
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

/**
 * @brief Extract the extension (without dot) from a filename
 * @param filename Input filename string
 * @param extension Buffer to receive the extension
 * @param buffer_size Size of extension buffer
 * @return SUCCESS if extension extracted, ERROR_INVALID_PATH otherwise
 * @author [Chu-Cheng Yu]
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

/**
 * @brief Convert a raw byte size into a human readable string
 * @param size File size in bytes
 * @param buffer Buffer to receive formatted string
 * @param buffer_size Size of the buffer
 * @return void
 * @author [Chu-Cheng Yu]
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

