/*
 * library.c
 * Library management functions for CCrypt
 * Chu-Cheng Yu and contributors
 * September 2025
 * This file contains functions for managing the encryption library including
 * loading, saving, sorting, searching, and displaying library entries.
 */

/* ========================================================================
 * DEBUG MODE TURN OFF BEFORE FINAL SUBMISSION
 * ======================================================================== */
#define DEBUG

#include "ccrypt.h"
#include "library.h"
#include "ui.h"
#include "utils.h"

/* forward declarations for comparators used by qsort */
static int cmp_name(const void *a, const void *b);
static int cmp_date(const void *a, const void *b);
static int cmp_size(const void *a, const void *b);
static int cmp_type(const void *a, const void *b);

/* ========================================================================
 * LIBRARY MANAGEMENT FUNCTIONS
 * ======================================================================== */

/*
 * Load encryption library from disk
 * [Gordon Huang]
 */
int load_encryption_library(encryption_library_t *library)
{
    /* Minimal loader stub: clear list */
    if (!library) return ERROR_INVALID_PATH;
    library->head = NULL;
    library->count = 0;
    library->is_modified = 0;
    library->next_id = 1;
    return SUCCESS;
}

/*
 * Save encryption library to disk
 * [Agam Grewal]
 */
int save_encryption_library(encryption_library_t *library)
{
    /* Minimal saver stub: nothing to do for now */
    (void)library;
    return SUCCESS;
}

/*
 * Add new encrypted file entry to library
 * [Chu-Cheng Yu]
 */
int add_file_to_library(encryption_library_t *library, const file_metadata_t *metadata)
{
    if (!library || !metadata) return ERROR_INVALID_PATH;

    file_node_t *node = (file_node_t *)malloc(sizeof(file_node_t));
    if (!node) return ERROR_MEMORY_ALLOCATION;
    node->data = *metadata;
    node->next = NULL;

    /* append to end */
    if (!library->head) {
        library->head = node;
    } else {
        file_node_t *cur = library->head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    library->count++;
    library->is_modified = 1;

    #ifdef DEBUG
        fprintf(stderr, "[DEBUG] Added file to library: %s\n", metadata->original_filename);
        fprintf(stderr, "[DEBUG] library count: %d\n", library->count);
    #endif

    return SUCCESS;
}

/*
 * Remove a library entry at the specified index
 * library Pointer to the encryption library
 * index Index of entry to remove (0-based)
 * SUCCESS on success, or error code for invalid parameters
 * [Chu-Cheng Yu]
 */
int remove_file_from_library(encryption_library_t *library, int index)
{
    if (!library) return ERROR_INVALID_PATH;
    if (index < 0 || index >= library->count) return ERROR_INVALID_PATH;

    file_node_t *prev = NULL;
    file_node_t *cur = library->head;
    int i = 0;
    while (cur && i < index) {
        prev = cur;
        cur = cur->next;
        i++;
    }
    if (!cur) return ERROR_INVALID_PATH;
    if (prev) prev->next = cur->next;
    else library->head = cur->next;
    free(cur);
    library->count--;
    library->is_modified = 1;
    return SUCCESS;
}

/*
 * Display all files in the encryption library
 * [Chu-Cheng Yu]
 */
void display_library_contents(encryption_library_t *library, sort_option_t sort_option)
{
    if (!library || library->count == 0) {
        printf("No encrypted files in library.\n");
        return;
    }

    /* Build a temporary array for sorting/display */
    int n = library->count;
    file_metadata_t *arr = (file_metadata_t *)malloc(sizeof(file_metadata_t) * n);
    if (!arr) {
        printf("Memory error\n");
        return;
    }
    file_node_t *cur = library->head;
    for (int i = 0; i < n && cur; ++i, cur = cur->next) arr[i] = cur->data;

    /* Sort array based on option */
    switch (sort_option) {
        case SORT_BY_NAME:
            qsort(arr, n, sizeof(file_metadata_t), cmp_name);
            break;
        case SORT_BY_DATE:
            qsort(arr, n, sizeof(file_metadata_t), cmp_date);
            break;
        case SORT_BY_SIZE:
            qsort(arr, n, sizeof(file_metadata_t), cmp_size);
            break;
        case SORT_BY_TYPE:
            qsort(arr, n, sizeof(file_metadata_t), cmp_type);
            break;
        default:
            break;
    }

    printf("\nEncrypted Files Library (%d entries):\n", n);
    printf("=====================================\n");
    printf("%-3s %-20s %-10s %-12s %-10s\n", "No.", "Filename", "Size", "Date", "Compressed");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%-3d %-20s %-10ld %-12lu %-10s\n",
               i + 1,
               arr[i].original_filename,
               arr[i].original_size,
               arr[i].encryption_id,
               arr[i].is_compressed ? "Yes" : "No");
    }
    free(arr);
}

/*
 * Print detailed information about a library entry
 * library Pointer to the encryption library
 * index Index of the entry to display (0-based)
 * void
 * [Chu-Cheng Yu]
 */
void display_file_information(encryption_library_t *library, int index)
{
    if (!library) return;
    if (index < 0 || index >= library->count) return;
    int i = 0;
    file_node_t *cur = library->head;
    while (cur && i < index) { cur = cur->next; i++; }
    if (!cur) return;
    file_metadata_t *m = &cur->data;
    printf("File information for entry %d:\n", index + 1);
    printf(" Original: %s\n", m->original_filename);
    printf(" Encrypted: %s\n", m->encrypted_filename);
    printf(" Original size: %ld\n", m->original_size);
    printf(" Encrypted size: %ld\n", m->encrypted_size);
    printf(" Compressed: %s\n", m->is_compressed ? "Yes" : "No");
    printf(" Method: %d\n", m->encryption_method);
}

/*
 * Search the library for filenames containing a substring
 * library Pointer to encryption library
 * search_pattern Substring to search for
 * results Array to receive matched indices
 * max_results Maximum number of results to store in results array
 * Number of matches found (may be 0)
 * [Chu-Cheng Yu]
 */
int search_library_by_name(encryption_library_t *library, const char *search_pattern,
                           int *results, int max_results)
{
    if (!library || !search_pattern || !results || max_results <= 0) return 0;
    int found = 0;
    int i = 0;
    file_node_t *cur = library->head;
    while (cur && found < max_results) {
        if (strstr(cur->data.original_filename, search_pattern)) {
            results[found++] = i;
        }
        cur = cur->next;
        i++;
    }
    return found;
}

/*
 * Delete an encrypted file from disk and remove it from the library
 * library Pointer to the encryption library
 * index Index of the entry to delete (0-based)
 * SUCCESS on success, or error code on failure
 * [Haoyang Che]
 */
int delete_encrypted_file(encryption_library_t *library, int index)
{
    file_metadata_t* cur_file = get_library_entry(library, index);
    if (!cur_file) {
        return ERROR_INVALID_PATH;
    }

    int ret = remove(cur_file->original_filename);
    if (ret != SUCCESS) {
        return ERROR_DELETE_FAILED;
    }

    return remove_file_from_library(library, index);
}

/*
 * Rename an encrypted file on disk and update library metadata
 * library Pointer to the encryption library
 * index Index of the entry to rename (0-based)
 * new_name New filename to apply
 * SUCCESS on success, or an error code on failure
 * [Haoyang Che]
 */
int rename_encrypted_file(encryption_library_t *library, int index, const char *new_name)
{
    if (!new_name) {
        return ERROR_NEW_FILE_NAME;
    }
    file_metadata_t* cur_file = get_library_entry(library, index);
    if (!cur_file) {
        return ERROR_INVALID_PATH;
    }

    // Rename the encrypted file from disk
    // TODO path + file need concat ?
    int ret = rename(cur_file->original_filename, new_name);
    if (ret != SUCCESS) {
        return ERROR_RENAME_FAILED;
    }

    // update library metadata of this file
    strncpy(cur_file->encrypted_filename, new_name, MAX_FILENAME_LENGTH - 1);
    // for safety
    cur_file->encrypted_filename[MAX_FILENAME_LENGTH - 1] = '\0';

    return SUCCESS;
}

/* Helper: return count */
int get_library_count(encryption_library_t *library)
{
    if (!library) return 0;
    return library->count;
}

/* Helper: return pointer to metadata for index (NULL if OOB) */
file_metadata_t *get_library_entry(encryption_library_t *library, int index)
{
    if (!library || index < 0 || index >= library->count) return NULL;
    int i = 0;
    file_node_t *cur = library->head;
    while (cur && i < index) { cur = cur->next; i++; }
    if (!cur) return NULL;
    return &cur->data;
}

/* Helper: free entire library list */
void free_library(encryption_library_t *library)
{
    if (!library) return;
    file_node_t *cur = library->head;
    while (cur) {
        file_node_t *next = cur->next;
        free(cur);
        cur = next;
    }
    library->head = NULL;
    library->count = 0;
    library->is_modified = 0;
}

/* ========================================================================
 * LIBRARY SORTING FUNCTIONS
 * Author Chu-Cheng Yu
 * ======================================================================== */

/* Simple sort helpers: use qsort with wrappers */
static int cmp_name(const void *a, const void *b)
{
    const file_metadata_t *x = (const file_metadata_t *)a;
    const file_metadata_t *y = (const file_metadata_t *)b;
    ///return strcasecmp(x->original_filename, y->original_filename);
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
    ///return strcasecmp(x->file_type, y->file_type);
}

/*
 * Sort library entries alphabetically by original filename
 * library Pointer to the encryption library
 * [Chu-Cheng Yu]
 */
void sort_library_by_name(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    int n = library->count;
    file_metadata_t *arr = malloc(sizeof(file_metadata_t) * n);
    if (!arr) return;
    file_node_t *cur = library->head;
    for (int i = 0; i < n && cur; ++i, cur = cur->next) arr[i] = cur->data;
    qsort(arr, n, sizeof(file_metadata_t), cmp_name);
    /* rebuild linked list in sorted order */
    cur = library->head;
    int i = 0;
    while (cur && i < n) { cur->data = arr[i++]; cur = cur->next; }
    free(arr);
}

/*
 * Sort library entries by encryption id (most recent first)
 * library Pointer to the encryption library
 * [Chu-Cheng Yu]
 */
void sort_library_by_date(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    int n = library->count;
    file_metadata_t *arr = malloc(sizeof(file_metadata_t) * n);
    if (!arr) return;
    file_node_t *cur = library->head;
    for (int i = 0; i < n && cur; ++i, cur = cur->next) arr[i] = cur->data;
    qsort(arr, n, sizeof(file_metadata_t), cmp_date);
    cur = library->head; int i = 0;
    while (cur && i < n) { cur->data = arr[i++]; cur = cur->next; }
    free(arr);
}

/*
 * Sort library entries by original file size (largest first)
 * library Pointer to the encryption library
 * [Chu-Cheng Yu]
 */
void sort_library_by_size(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    int n = library->count;
    file_metadata_t *arr = malloc(sizeof(file_metadata_t) * n);
    if (!arr) return;
    file_node_t *cur = library->head;
    for (int i = 0; i < n && cur; ++i, cur = cur->next) arr[i] = cur->data;
    qsort(arr, n, sizeof(file_metadata_t), cmp_size);
    cur = library->head; int i = 0;
    while (cur && i < n) { cur->data = arr[i++]; cur = cur->next; }
    free(arr);
}

/*
 * Sort library entries by file type/extension
 * library Pointer to the encryption library
 * [Chu-Cheng Yu]
 */
void sort_library_by_type(encryption_library_t *library)
{
    if (!library || library->count <= 1) return;
    int n = library->count;
    file_metadata_t *arr = malloc(sizeof(file_metadata_t) * n);
    if (!arr) return;
    file_node_t *cur = library->head;
    for (int i = 0; i < n && cur; ++i, cur = cur->next) arr[i] = cur->data;
    qsort(arr, n, sizeof(file_metadata_t), cmp_type);
    cur = library->head; int i = 0;
    while (cur && i < n) { cur->data = arr[i++]; cur = cur->next; }
    free(arr);
}

/*
 * Compare two metadata entries according to a sort option
 * a Pointer to first metadata entry
 * b Pointer to second metadata entry
 * sort_type Sorting criteria (see sort_option_t)
 * negative/zero/positive as in strcmp-style comparators
 * [Chu-Cheng Yu]
 */
int compare_metadata_entries(const file_metadata_t *a, const file_metadata_t *b, 
                            sort_option_t sort_type)
{
    if (!a || !b) return 0;
    switch (sort_type) {
        ///case SORT_BY_NAME: return strcasecmp(a->original_filename, b->original_filename);
        case SORT_BY_DATE: return (a->encryption_id < b->encryption_id) ? 1 : (a->encryption_id > b->encryption_id) ? -1 : 0;
        case SORT_BY_SIZE: return (a->original_size < b->original_size) ? 1 : (a->original_size > b->original_size) ? -1 : 0;
        ///case SORT_BY_TYPE: return strcasecmp(a->file_type, b->file_type);
        default: return 0;
    }
}
