/*
 * library.h
 * Header file for library management functions
 * Chu-Cheng Yu and contributors
 * September 2025
 * This header defines functions for managing the encryption library including
 * loading, saving, sorting, searching, and displaying library entries.
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include "ccrypt.h"

/* ========================================================================
 * LIBRARY MANAGEMENT FUNCTION DECLARATIONS
 * ======================================================================== */

/*
 * Load encryption library from disk
 * library Pointer to library structure to populate
 * SUCCESS on success, error code on failure
 */
int load_encryption_library(encryption_library_t *library);

/*
 * Save encryption library to disk
 * library Pointer to library structure to save
 * SUCCESS on success, error code on failure
 */
int save_encryption_library(encryption_library_t *library);

/*
 * Add new encrypted file entry to library
 * library Pointer to the encryption library
 * metadata Pointer to file metadata to add
 * SUCCESS on success, error code on failure
 */
int add_file_to_library(encryption_library_t *library, const file_metadata_t *metadata);

/*
 * Remove a library entry at the specified index
 * library Pointer to the encryption library
 * index Index of entry to remove (0-based)
 * SUCCESS on success, or error code for invalid parameters
 */
int remove_file_from_library(encryption_library_t *library, int index);

/*
 * Display all files in the encryption library
 * library Pointer to the encryption library
 * sort_option Sorting option for display
 */
void display_library_contents(encryption_library_t *library, sort_option_t sort_option);

/*
 * Print detailed information about a library entry
 * library Pointer to the encryption library
 * index Index of the entry to display (0-based)
 */
void display_file_information(encryption_library_t *library, int index);

/*
 * Search the library for filenames containing a substring
 * library Pointer to encryption library
 * search_pattern Substring to search for
 * results Array to receive matched indices
 * max_results Maximum number of results to store in results array
 * Number of matches found (may be 0)
 */
int search_library_by_name(encryption_library_t *library, const char *search_pattern,
                           int *results, int max_results);

/*
 * Delete an encrypted file from disk and remove it from the library
 * library Pointer to the encryption library
 * index Index of the entry to delete (0-based)
 * SUCCESS on success, or error code on failure
 */
int delete_encrypted_file(encryption_library_t *library, int index);

/*
 * Rename an encrypted file on disk and update library metadata
 * library Pointer to the encryption library
 * index Index of the entry to rename (0-based)
 * new_name New filename to apply
 * SUCCESS on success, or an error code on failure
 */
int rename_encrypted_file(encryption_library_t *library, int index, const char *new_name);

/* Helper accessors for linked-list based library */
int get_library_count(encryption_library_t *library);
file_metadata_t *get_library_entry(encryption_library_t *library, int index);
void free_library(encryption_library_t *library);

/* ========================================================================
 * LIBRARY SORTING FUNCTION DECLARATIONS
 * ======================================================================== */

/*
 * Sort library entries alphabetically by original filename
 * library Pointer to the encryption library
 */
void sort_library_by_name(encryption_library_t *library);

/*
 * Sort library entries by encryption id (most recent first)
 * library Pointer to the encryption library
 */
void sort_library_by_date(encryption_library_t *library);

/*
 * Sort library entries by original file size (largest first)
 * library Pointer to the encryption library
 */
void sort_library_by_size(encryption_library_t *library);

/*
 * Sort library entries by file type/extension
 * library Pointer to the encryption library
 */
void sort_library_by_type(encryption_library_t *library);

/*
 * Compare two metadata entries according to a sort option
 * a Pointer to first metadata entry
 * b Pointer to second metadata entry
 * sort_type Sorting criteria (see sort_option_t)
 * negative/zero/positive as in strcmp-style comparators
 */
int compare_metadata_entries(const file_metadata_t *a, const file_metadata_t *b, 
                            sort_option_t sort_type);

#endif /* LIBRARY_H */