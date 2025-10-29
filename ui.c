/*
 * ui.c
 * User interface functions for CCrypt
 * Chu-Cheng Yu
 * September 2025
 * This file contains all user interface related functions including menu display,
 * user input handling, and command processing.
 */

#include "ccrypt.h"
#include "ui.h"
#include "encryption.h"
#include "library.h"
#include "utils.h"

/* ========================================================================
 * USER INTERFACE FUNCTIONS
 * ======================================================================== */

/*
 * Display the main menu options to the user
 * [Chu-Cheng Yu]
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

/*
 * Get and validate user input for menu selection
 * [Chu-Cheng Yu]
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

/*
 * Main program loop displaying menu and processing user commands
 * [Chu-Cheng Yu]
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

/*
 * Process user menu selection and call appropriate function
 * [Chu-Cheng Yu]
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
            result = file_management_menu(library);
            break;
        case 5:
            printf("Sort by: 1=Name, 2=Date, 3=Size\n");
            sort_choice = get_user_choice("Sort option: ", 1, 3);
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

/*
 * Display error message to user with context
 * [Chu-Cheng Yu]
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

/*
 * Get and validate file path from user input
 * [Chu-Cheng Yu]
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

/*
 * Ask user whether to compress file before encryption
 * [Chu-Cheng Yu]
 */
int ask_compression_preference(void)
{
    return get_user_confirmation("Compress before encryption? (y/n): ");
}

/*
 * Prompt user to confirm an operation (yes/no)
 * operation_description Prompt string to display
 * 1 if user confirms (y/Y), 0 otherwise
 * [Chu-Cheng Yu]
 */
int get_user_confirmation(const char *operation_description)
{
    if (!operation_description) return 0;
    printf("%s", operation_description);
    char line[8];
    if (!fgets(line, sizeof(line), stdin)) return 0;
    return (line[0] == 'y' || line[0] == 'Y') ? 1 : 0;
}

/*
 * Prompt user for a decryption password (non-hidden input)
 * password Buffer to receive the password string
 * buffer_size Size of the password buffer
 * SUCCESS on success, or ERROR_INVALID_PASSWORD on failure
 * [Gordon Huang]
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

/*
 * Display and handle file management submenu
 * library Pointer to the encryption library
 * SUCCESS on normal operation, error code on failure
 * [Chu-Cheng Yu]
 */
int file_management_menu(encryption_library_t *library)
{
    int choice;
    int result = SUCCESS;
    int file_index;
    char new_name[MAX_FILENAME_LENGTH];
    char search_pattern[MAX_FILENAME_LENGTH];
    int search_results[20]; // Max 20 search results
    int num_results;
    
    do {
        printf("\n========================================\n");
        printf("           FILE MANAGEMENT MENU         \n");
        printf("========================================\n");
        printf("1. View file details\n");
        printf("2. Search files by name\n");
        printf("3. Delete encrypted file\n");
        printf("4. Rename encrypted file\n");
        printf("5. Return to main menu\n");
        printf("========================================\n");
        
        choice = get_user_choice("Select an option: ", 1, 5);
        
        switch (choice) {
            case 1: // View file details
                if (get_library_count(library) == 0) {
                    printf("No files in library.\n");
                    break;
                }
                display_library_contents(library, SORT_BY_NAME);
                file_index = get_user_choice("Enter file number to view details (0 to cancel): ", 0, get_library_count(library));
                if (file_index > 0) {
                    display_file_information(library, file_index - 1);
                }
                break;
                
            case 2: // Search files
                printf("Enter search pattern: ");
                if (fgets(search_pattern, sizeof(search_pattern), stdin)) {
                    // Strip newline
                    size_t len = strlen(search_pattern);
                    if (len > 0 && search_pattern[len - 1] == '\n') {
                        search_pattern[len - 1] = '\0';
                    }
                    
                    num_results = search_library_by_name(library, search_pattern, search_results, 20);
                    if (num_results > 0) {
                        printf("Found %d matching files:\n", num_results);
                        for (int i = 0; i < num_results; i++) {
                                int idx = search_results[i];
                                file_metadata_t *m = get_library_entry(library, idx);
                                if (m) printf("  %d. %s\n", idx + 1, m->original_filename);
                            }
                    } else {
                        printf("No files found matching '%s'\n", search_pattern);
                    }
                }
                break;
                
            case 3: // Delete file
                if (get_library_count(library) == 0) {
                    printf("No files in library to delete.\n");
                    break;
                }
                display_library_contents(library, SORT_BY_NAME);
                file_index = get_user_choice("Enter file number to delete (0 to cancel): ", 0, get_library_count(library));
                if (file_index > 0) {
                    result = delete_encrypted_file(library, file_index - 1);
                    if (result == SUCCESS) {
                        printf("File deleted successfully.\n");
                    }
                }
                break;
                
            case 4: // Rename file
                if (get_library_count(library) == 0) {
                    printf("No files in library to rename.\n");
                    break;
                }
                display_library_contents(library, SORT_BY_NAME);
                file_index = get_user_choice("Enter file number to rename (0 to cancel): ", 0, get_library_count(library));
                if (file_index > 0) {
                    printf("Enter new filename: ");
                    if (fgets(new_name, sizeof(new_name), stdin)) {
                        // Strip newline
                        size_t len = strlen(new_name);
                        if (len > 0 && new_name[len - 1] == '\n') {
                            new_name[len - 1] = '\0';
                        }
                        
                        result = rename_encrypted_file(library, file_index - 1, new_name);
                        if (result == SUCCESS) {
                            printf("File renamed successfully.\n");
                        }
                    }
                }
                break;
                
            case 5: // Return to main menu
                printf("Returning to main menu...\n");
                break;
                
            default:
                result = ERROR_INVALID_PATH;
                break;
        }
        
        if (result != SUCCESS && choice != 5) {
            display_error(result, "File management operation");
            result = SUCCESS; /* Continue menu on non-fatal errors */
        }
        
    } while (choice != 5);
    
    return result;
}
