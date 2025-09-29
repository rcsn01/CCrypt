# CCrypt Structure Chart

```
                                    MAIN
                                     |
                    +----------------+----------------+
                    |                                 |
                INITIALIZE                        CLEANUP
                    |                                 |
            +-------+-------+                   +-----+-----+
            |               |                   |           |
        LOAD_LIBRARY    SETUP_SYSTEM       SAVE_LIBRARY  FREE_MEMORY
                                                |
                                        +-------+-------+
                                        |               |
                                   WRITE_METADATA   CLOSE_FILES
                    
                                MAIN_MENU_LOOP
                                     |
                    +----------------+----------------+
                    |                |                |
                DISPLAY_MENU    GET_USER_INPUT   PROCESS_COMMAND
                    |                                 |
            +-------+-------+                       |
            |               |                       |
        SHOW_OPTIONS    SHOW_LIBRARY           +----+----+
                            |                  |         |
                    +-------+-------+      ENCRYPT   DECRYPT
                    |               |         |         |
                LIST_FILES      SORT_FILES   |    +----+----+
                    |               |        |    |         |
            +-------+-------+   +----+----+  |  FIND_FILE VERIFY_KEY
            |               |   |         |  |
        FORMAT_DISPLAY  GET_METADATA  BY_NAME BY_DATE
                                      |         |
                                 BY_SIZE   BY_TYPE


                            ENCRYPT_FILE_WORKFLOW
                                     |
                    +----------------+----------------+
                    |                |                |
                GET_FILE_PATH   ASK_COMPRESSION   PERFORM_ENCRYPTION
                    |                |                |
            +-------+-------+   +----+----+      +----+----+
            |               |   |         |      |         |
        VALIDATE_PATH   CHECK_EXISTS  COMPRESS  ENCRYPT   ADD_TO_LIBRARY
            |               |         |         |         |
        RESOLVE_PATH    GET_STATS   APPLY_ALGO APPLY_CIPHER UPDATE_METADATA
                                       |         |
                               +-------+    +----+----+
                               |            |         |
                           LZ77_COMPRESS SIMPLE_XOR ADVANCED_CIPHER
                           

                            DECRYPT_FILE_WORKFLOW
                                     |
                    +----------------+----------------+
                    |                |                |
                SELECT_FROM_LIBRARY GET_PASSWORD  PERFORM_DECRYPTION
                    |                |                |
            +-------+-------+   +----+----+      +----+----+
            |               |   |         |      |         |
        SHOW_ENCRYPTED   VALIDATE_SELECTION VERIFY_KEY DECRYPT_FILE
            |               |         |         |         |
        LIST_AVAILABLE  GET_INDEX  CHECK_PASSWORD REVERSE_CIPHER DECOMPRESS
                           |                       |         |
                    BOUNDS_CHECK              +----+----+  +----+----+
                                              |         |  |         |
                                         XOR_DECRYPT ADVANCED_DECRYPT EXTRACT_ORIGINAL


                            FILE_MANAGEMENT
                                     |
                    +----------------+----------------+
                    |                |                |
                DELETE_FILE      RENAME_FILE      SHOW_INFO
                    |                |                |
            +-------+-------+   +----+----+      +----+----+
            |               |   |         |      |         |
        CONFIRM_DELETE  REMOVE_FROM_FS UPDATE_NAME GET_METADATA DISPLAY_INFO
            |               |         |         |         |
        GET_CONFIRMATION DELETE_ACTUAL VALIDATE_NAME UPDATE_LIBRARY FORMAT_INFO
                           |         |         |
                    +------+    CHANGE_FS_NAME UPDATE_RECORDS
                    |
                SECURE_DELETE


                            UTILITY_FUNCTIONS
                                     |
                    +----------------+----------------+
                    |                |                |
                ERROR_HANDLING   INPUT_VALIDATION   STRING_OPERATIONS
                    |                |                |
            +-------+-------+   +----+----+      +----+----+
            |               |   |         |      |         |
        LOG_ERROR       DISPLAY_ERROR VALIDATE_PATH SAFE_COPY SAFE_COMPARE
            |               |         |         |         |
        WRITE_LOG       SHOW_MESSAGE CHECK_FORMAT BOUNDS_CHECK CONSTANT_TIME
                                      |
                               +------+------+
                               |             |
                           CHECK_LENGTH  CHECK_CHARS


                            LIBRARY_MANAGEMENT
                                     |
                    +----------------+----------------+
                    |                |                |
                LOAD_LIBRARY     SAVE_LIBRARY     SEARCH_LIBRARY
                    |                |                |
            +-------+-------+   +----+----+      +----+----+
            |               |   |         |      |         |
        READ_METADATA   PARSE_RECORDS WRITE_RECORDS SERIALIZE LINEAR_SEARCH BINARY_SEARCH
            |               |         |         |         |         |
        OPEN_FILE       VALIDATE_FORMAT CREATE_BACKUP FORMAT_RECORD BY_NAME  SORTED_SEARCH
                           |
                    +------+------+
                    |             |
                CHECK_VERSION  VERIFY_INTEGRITY
```

## Module Descriptions

### Main Control Module
- **MAIN**: Entry point, coordinates all program operations
- **INITIALIZE**: Sets up program state, loads library, initializes data structures
- **CLEANUP**: Saves data, frees memory, closes files before program termination
- **MAIN_MENU_LOOP**: Core program loop handling user interaction

### User Interface Module
- **DISPLAY_MENU**: Shows available options to user
- **GET_USER_INPUT**: Handles user input with validation
- **PROCESS_COMMAND**: Routes user commands to appropriate functions

### Encryption Module
- **ENCRYPT_FILE_WORKFLOW**: Complete file encryption process
- **GET_FILE_PATH**: File selection and path validation
- **ASK_COMPRESSION**: User choice for compression option
- **PERFORM_ENCRYPTION**: Core encryption operations
- **COMPRESS**: File compression using available algorithms
- **ENCRYPT**: File encryption using cipher algorithms

### Decryption Module
- **DECRYPT_FILE_WORKFLOW**: Complete file decryption process
- **SELECT_FROM_LIBRARY**: Choose encrypted file from library
- **GET_PASSWORD**: Secure password input
- **PERFORM_DECRYPTION**: Core decryption operations
- **DECRYPT_FILE**: Reverse encryption process
- **DECOMPRESS**: Reverse compression if applied

### Library Management Module
- **LOAD_LIBRARY**: Read encrypted file database
- **SAVE_LIBRARY**: Write updated database to disk
- **SEARCH_LIBRARY**: Find files by various criteria
- **LIST_FILES**: Display encrypted files with metadata
- **SORT_FILES**: Organize files by name, date, size, type

### File Management Module
- **DELETE_FILE**: Remove encrypted files from library and filesystem
- **RENAME_FILE**: Change encrypted file names
- **SHOW_INFO**: Display detailed file information

### Utility Module
- **ERROR_HANDLING**: Comprehensive error management
- **INPUT_VALIDATION**: User input sanitization and verification
- **STRING_OPERATIONS**: Safe string manipulation functions

## Data Flow

1. **Program Start**: MAIN → INITIALIZE → LOAD_LIBRARY
2. **User Interaction**: MAIN_MENU_LOOP → DISPLAY_MENU → GET_USER_INPUT → PROCESS_COMMAND
3. **File Encryption**: ENCRYPT_FILE_WORKFLOW → GET_FILE_PATH → ASK_COMPRESSION → PERFORM_ENCRYPTION → ADD_TO_LIBRARY
4. **File Decryption**: DECRYPT_FILE_WORKFLOW → SELECT_FROM_LIBRARY → GET_PASSWORD → PERFORM_DECRYPTION
5. **Library Operations**: Various functions → SEARCH_LIBRARY → SORT_FILES → DISPLAY results
6. **Program Exit**: CLEANUP → SAVE_LIBRARY → FREE_MEMORY

## Module Dependencies

- All modules depend on **Utility Module** for error handling and validation
- **Encryption/Decryption Modules** depend on **Library Management** for metadata
- **File Management** depends on **Library Management** for record keeping
- **User Interface** coordinates with all functional modules
- **Main Control** orchestrates all module interactions