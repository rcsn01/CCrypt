/*
 * ui.h
 * Header file for user interface functions
 * Chu-Cheng Yu
 * September 2025
 * This header defines the user interface functions for CCrypt including
 * menu display, input handling, and user interaction.
 */

#ifndef UI_H
#define UI_H

#include "ccrypt.h"

/* ========================================================================
 * USER INTERFACE FUNCTION DECLARATIONS
 * ======================================================================== */

/*
 * Display the main menu options to the user
 */
void display_main_menu(void);

/*
 * Get and validate user input for menu selection
 * prompt Prompt string to display to user
 * min_value Minimum acceptable value
 * max_value Maximum acceptable value
 * Valid user choice within the specified range
 */
int get_user_choice(const char *prompt, int min_value, int max_value);

/*
 * Main program loop displaying menu and processing user commands
 * library Pointer to the encryption library
 * SUCCESS on normal exit, error code on failure
 */
int main_menu_loop(encryption_library_t *library);

/*
 * Process user menu selection and call appropriate function
 * choice User's menu choice
 * library Pointer to the encryption library
 * SUCCESS on successful operation, error code on failure
 */
int process_user_command(int choice, encryption_library_t *library);

/*
 * Display error message to user with context
 * error_code Error code to display
 * context Context string describing where error occurred
 */
void display_error(int error_code, const char *context);

/*
 * Get and validate file path from user input
 * file_path Buffer to store the file path
 * buffer_size Size of the file path buffer
 * SUCCESS if valid path obtained, error code otherwise
 */
int get_file_path_from_user(char *file_path, size_t buffer_size);

/*
 * Ask user whether to compress file before encryption
 * 1 for yes, 0 for no, negative for error
 */
int ask_compression_preference(void);

/*
 * Prompt user to confirm an operation (yes/no)
 * operation_description Prompt string to display
 * 1 if user confirms (y/Y), 0 otherwise
 */
int get_user_confirmation(const char *operation_description);

/*
 * Prompt user for a decryption password (non-hidden input)
 * password Buffer to receive the password string
 * buffer_size Size of the password buffer
 * SUCCESS on success, or ERROR_INVALID_PASSWORD on failure
 */
int get_decryption_password(char *password, size_t buffer_size);

/*
 * Display and handle file management submenu
 * library Pointer to the encryption library
 * SUCCESS on normal operation, error code on failure
 */
int file_management_menu(encryption_library_t *library);

#endif /* UI_H */