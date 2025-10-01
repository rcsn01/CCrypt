/**
 * @file library.h
 * @brief Header file for library management functions
 * @author Chu-Cheng Yu and contributors
 * @date September 2025
 * 
 * This header defines functions for managing the encryption library including
 * loading, saving, sorting, searching, and displaying library entries.
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include "ccrypt.h"

/* ========================================================================
 * LIBRARY MANAGEMENT FUNCTION DECLARATIONS
 * ======================================================================== */

/**
 * @brief Load encryption library from disk
 * @param library Pointer to library structure to populate
 * @return SUCCESS on success, error code on failure
 */
int load_encryption_library(encryption_library_t *library);

/**
 * @brief Save encryption library to disk
 * @param library Pointer to library structure to save
 * @return SUCCESS on success, error code on failure
 */
int save_encryption_library(encryption_library_t *library);

/**
 * @brief Add new encrypted file entry to library
 * @param library Pointer to the encryption library
 * @param metadata Pointer to file metadata to add
 * @return SUCCESS on success, error code on failure
 */
int add_file_to_library(encryption_library_t *library, const file_metadata_t *metadata);

/**
 * @brief Remove a library entry at the specified index
 * @param library Pointer to the encryption library
 * @param index Index of entry to remove (0-based)
 * @return SUCCESS on success, or error code for invalid parameters
 */
int remove_file_from_library(encryption_library_t *library, int index);

/**
 * @brief Display all files in the encryption library
 * @param library Pointer to the encryption library
 * @param sort_option Sorting option for display
 */
void display_library_contents(encryption_library_t *library, sort_option_t sort_option);

/**
 * @brief Print detailed information about a library entry
 * @param library Pointer to the encryption library
 * @param index Index of the entry to display (0-based)
 */
void display_file_information(encryption_library_t *library, int index);

/**
 * @brief Search the library for filenames containing a substring
 * @param library Pointer to encryption library
 * @param search_pattern Substring to search for
 * @param results Array to receive matched indices
 * @param max_results Maximum number of results to store in results array
 * @return Number of matches found (may be 0)
 */
int search_library_by_name(encryption_library_t *library, const char *search_pattern,
                           int *results, int max_results);

/**
 * @brief Delete an encrypted file from disk and remove it from the library
 * @param library Pointer to the encryption library
 * @param index Index of the entry to delete (0-based)
 * @return SUCCESS on success, or error code on failure
 */
int delete_encrypted_file(encryption_library_t *library, int index);

/**
 * @brief Rename an encrypted file on disk and update library metadata
 * @param library Pointer to the encryption library
 * @param index Index of the entry to rename (0-based)
 * @param new_name New filename to apply
 * @return SUCCESS on success, or an error code on failure
 */
int rename_encrypted_file(encryption_library_t *library, int index, const char *new_name);

/* ========================================================================
 * LIBRARY SORTING FUNCTION DECLARATIONS
 * ======================================================================== */

/**
 * @brief Sort library entries alphabetically by original filename
 * @param library Pointer to the encryption library
 */
void sort_library_by_name(encryption_library_t *library);

/**
 * @brief Sort library entries by encryption id (most recent first)
 * @param library Pointer to the encryption library
 */
void sort_library_by_date(encryption_library_t *library);

/**
 * @brief Sort library entries by original file size (largest first)
 * @param library Pointer to the encryption library
 */
void sort_library_by_size(encryption_library_t *library);

/**
 * @brief Sort library entries by file type/extension
 * @param library Pointer to the encryption library
 */
void sort_library_by_type(encryption_library_t *library);

/**
 * @brief Compare two metadata entries according to a sort option
 * @param a Pointer to first metadata entry
 * @param b Pointer to second metadata entry
 * @param sort_type Sorting criteria (see sort_option_t)
 * @return negative/zero/positive as in strcmp-style comparators
 */
int compare_metadata_entries(const file_metadata_t *a, const file_metadata_t *b, 
                            sort_option_t sort_type);

#endif /* LIBRARY_H */