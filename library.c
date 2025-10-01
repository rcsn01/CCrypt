/**
 * @file library.c
 * @brief Library management functions for CCrypt
 * @author Chu-Cheng Yu and contributors
 * @date September 2025
 * 
 * This file contains functions for managing the encryption library including
 * loading, saving, sorting, searching, and displaying library entries.
 */

#include "ccrypt.h"
#include "library.h"
#include "ui.h"
#include "utils.h"

/* ========================================================================
 * LIBRARY MANAGEMENT FUNCTIONS
 * ======================================================================== */

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
 * @brief Remove a library entry at the specified index
 * @param library Pointer to the encryption library
 * @param index Index of entry to remove (0-based)
 * @return SUCCESS on success, or error code for invalid parameters
 * @author [Empty]
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
 * @brief Display all files in the encryption library
 * @author [Chu-Cheng Yu]
 */
void display_library_contents(encryption_library_t *library, sort_option_t sort_option)
{
    if (library->count == 0) {
        printf("No encrypted files in library.\n");
        return;
    }
    
    /* Sort library based on sort_option */
    switch (sort_option) {
        case SORT_BY_NAME:
            sort_library_by_name(library);
            break;
        case SORT_BY_DATE:
            sort_library_by_date(library);
            break;
        case SORT_BY_SIZE:
            sort_library_by_size(library);
            break;
        case SORT_BY_TYPE:
            sort_library_by_type(library);
            break;
    }
    
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

/* ========================================================================
 * LIBRARY SORTING FUNCTIONS
 * ======================================================================== */

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