/**
 * @file ui.h
 * @brief Header file for user interface functions
 * @author Chu-Cheng Yu
 * @date September 2025
 * 
 * This header defines the user interface functions for CCrypt including
 * menu display, input handling, and user interaction.
 */

#ifndef UI_H
#define UI_H

#include "ccrypt.h"

/* ========================================================================
 * USER INTERFACE FUNCTION DECLARATIONS
 * ======================================================================== */

/**
 * @brief Display the main menu options to the user
 */
void display_main_menu(void);

/**
 * @brief Get and validate user input for menu selection
 * @param prompt Prompt string to display to user
 * @param min_value Minimum acceptable value
 * @param max_value Maximum acceptable value
 * @return Valid user choice within the specified range
 */
int get_user_choice(const char *prompt, int min_value, int max_value);

/**
 * @brief Main program loop displaying menu and processing user commands
 * @param library Pointer to the encryption library
 * @return SUCCESS on normal exit, error code on failure
 */
int main_menu_loop(encryption_library_t *library);

/**
 * @brief Process user menu selection and call appropriate function
 * @param choice User's menu choice
 * @param library Pointer to the encryption library
 * @return SUCCESS on successful operation, error code on failure
 */
int process_user_command(int choice, encryption_library_t *library);

/**
 * @brief Display error message to user with context
 * @param error_code Error code to display
 * @param context Context string describing where error occurred
 */
void display_error(int error_code, const char *context);

/**
 * @brief Get and validate file path from user input
 * @param file_path Buffer to store the file path
 * @param buffer_size Size of the file path buffer
 * @return SUCCESS if valid path obtained, error code otherwise
 */
int get_file_path_from_user(char *file_path, size_t buffer_size);

/**
 * @brief Ask user whether to compress file before encryption
 * @return 1 for yes, 0 for no, negative for error
 */
int ask_compression_preference(void);

/**
 * @brief Prompt user to confirm an operation (yes/no)
 * @param operation_description Prompt string to display
 * @return 1 if user confirms (y/Y), 0 otherwise
 */
int get_user_confirmation(const char *operation_description);

/**
 * @brief Prompt user for a decryption password (non-hidden input)
 * @param password Buffer to receive the password string
 * @param buffer_size Size of the password buffer
 * @return SUCCESS on success, or ERROR_INVALID_PASSWORD on failure
 */
int get_decryption_password(char *password, size_t buffer_size);

/**
 * @brief Display and handle file management submenu
 * @param library Pointer to the encryption library
 * @return SUCCESS on normal operation, error code on failure
 */
int file_management_menu(encryption_library_t *library);

#endif /* UI_H */