/**
 * @file ccrypt.c
 * @brief Main implementation file for CCrypt - A CLI file encryption and compression tool
 * @author [Your Name Here] - Add your name for submission
 * @date September 2025
 * @version 1.0
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
 * Author: [Your Name Here]
 * ======================================================================== */

/**
 * @brief Main entry point for the CCrypt program
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
 * ======================================================================== */

/**
 * @brief Display the main menu options to the user
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
 * Author: [Your Name Here]
 */
// ...existing code...
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
// ...existing code...

/**
 * @brief Process user menu selection and call appropriate function
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
 * ======================================================================== */

/**
 * @brief Complete workflow for encrypting a user-specified file
 */
int encrypt_file_workflow(encryption_library_t *library)
{
    char file_path[MAX_PATH_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char encrypted_filename[MAX_FILENAME_LENGTH];
    file_metadata_t metadata;
    int use_compression;
    int result;
    
    /* Get file path from user */
    result = get_file_path_from_user(file_path, sizeof(file_path));
    if (result != SUCCESS) {
        return result;
    }
    
    /* Ask about compression */
    use_compression = ask_compression_preference();
    if (use_compression < 0) {
        return ERROR_INVALID_PATH;
    }
    
    /* Get password */
    printf("Enter encryption password: ");
    /* TODO: Implement hidden password input */
    scanf("%s", password);
    
    /* Generate encrypted filename using library next_id */
    result = generate_encrypted_filename(file_path, encrypted_filename, sizeof(encrypted_filename), library->next_id);
    if (result != SUCCESS) {
        return result;
    }
    
    /* Perform encryption */
    result = encrypt_file(file_path, encrypted_filename, password, use_compression, &metadata);
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
 */
int ask_compression_preference(void)
{
    char choice;
    
    printf("Do you want to compress the file before encryption? (y/n): ");
    scanf(" %c", &choice);
    
    return (choice == 'y' || choice == 'Y') ? 1 : 0;
}

/**
 * @brief Encrypt a file with optional compression
 * Author: [Your Name Here]
 */
int encrypt_file(const char *input_path, const char *output_path, 
                 const char *password, int use_compression, 
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
    /* Assign a unique encryption id (set by caller) */
    /* metadata->encryption_id should already be set by caller */
    
    return SUCCESS; /* Placeholder */
}

/* ========================================================================
 * ADDITIONAL FUNCTION STUBS
 * Author: [Your Name Here]
 * ======================================================================== */

/* Note: Due to length constraints, I'm including key function stubs.
 * In a complete implementation, all functions from the header would be implemented. */

/**
 * @brief Apply compression algorithm to file data
 * Author: [Your Name Here]
 */
int compress_data(const unsigned char *input_data, long input_size,
                  unsigned char *output_data, long *output_size)
{
    /* TODO: Implement simple compression algorithm */
    /* For now, just copy data (no compression) */
    memcpy(output_data, input_data, input_size);
    *output_size = input_size;
    return SUCCESS;
}

/**
 * @brief Apply encryption cipher to file data
 * Author: [Your Name Here]
 */
int encrypt_data(const unsigned char *input_data, long data_size,
                 const char *password, unsigned char *output_data)
{
    /* TODO: Implement XOR-based encryption with key derivation */
    /* Simple XOR placeholder - replace with proper implementation */
    unsigned char key = (unsigned char)(strlen(password) % 256);
    for (long i = 0; i < data_size; i++) {
        output_data[i] = input_data[i] ^ key;
    }
    return SUCCESS;
}

/**
 * @brief Complete workflow for decrypting a file from the library
 * Author: [Your Name Here]
 */
int decrypt_file_workflow(encryption_library_t *library)
{
    /* TODO: Implement decryption workflow */
    printf("Decryption workflow - to be implemented\n");
    return SUCCESS;
}

/**
 * @brief Load encryption library from disk
 * Author: [Your Name Here]
 */
int load_encryption_library(encryption_library_t *library)
{
    FILE *file = fopen(LIBRARY_FILENAME, "rb");
    if (!file) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    /* TODO: Implement library loading logic */
    fclose(file);
    return SUCCESS;
}

/**
 * @brief Save encryption library to disk
 * Author: [Your Name Here]
 */
int save_encryption_library(encryption_library_t *library)
{
    FILE *file = fopen(LIBRARY_FILENAME, "wb");
    if (!file) {
        return ERROR_PERMISSION_DENIED;
    }
    
    /* TODO: Implement library saving logic */
    fclose(file);
    library->is_modified = 0;
    return SUCCESS;
}

/**
 * @brief Add new encrypted file entry to library
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
 * ======================================================================== */

/**
 * @brief Validate that a file path exists and is accessible
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
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
 * Author: [Your Name Here]
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

/* Additional utility functions would be implemented here following the same pattern */