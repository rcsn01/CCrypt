/**
 * @file main.c
 * @brief Main entry point and program initialization for CCrypt
 * @author Chu-Cheng Yu
 * @date September 2025
 * 
 * This file contains the main function and core program initialization/cleanup logic.
 * 
 * $env:Path = 'C:\msys64\mingw64\bin;' + $env:Path
 * gcc --version
 * Compilation: gcc -o ccrypt main.c ui.c encryption.c library.c utils.c -lm
 * Usage: ./ccrypt
 */

#include "ccrypt.h"
#include "ui.h"
#include "library.h"
#include "utils.h"

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