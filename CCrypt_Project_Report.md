# CCrypt Project Report

## 1. Executive Summary
CCrypt is a command line encryption tool developed in C that allows users to encrypt and decrypt individual \files with 
the option to apply compression before encryption. The program addresses the need for secure file storage on third party 
cloud platforms by ensuring that data is protected locally before it is uploaded. This approach gives users full control 
over the confidentiality and integrity of their information, even if the storage provider experiences a security breach.

The tool includes password based symmetric encryption, file integrity verification, a persistent encryption library for 
tracking files, and an interactive command line 
interface. Users can manage encrypted files by listing, renaming, and ]deleting them, and can organise their library using 
different sorting options. The program is designed to be lightweight, portable, and compatible with standard C compilers, 
with no reliance on external libraries.

CCrypt is intended for both everyday users who require a simple security solution and for professionals who need a practical 
method for protecting cloud stored data. It demonstrates the implementation of fundamental security and file management 
functionality within the constraints of the C programming language, highlighting both technical capability and thoughtful 
system design.
## 2. Project Introduction

### 2.1 Project Idea
CCrypt is a command line program developed in the C programming language that provides functionality for the secure encryption 
and decryption of individual files, with an optional compression feature applied prior to encryption. The primary objective of 
the tool is to enable users to safeguard their data before it is stored on third party cloud platforms. All encryption and file
management processes are implemented within the constraints of standard C libraries, ensuring portability and transparency without 
reliance on external dependencies.
### 2.2 Why is this program needed?
The increasing use of cloud storage services has introduced significant concerns regarding data privacy and security. When files 
are stored on third party servers, users are required to trust the service provider’s security mechanisms, which may be 
susceptible to breaches or unauthorised access. Even with strong organisational controls, the transfer of data to external 
infrastructure reduces user control over confidentiality and integrity. Implementing local encryption before uploading data 
provides an effective method of mitigating these risks, as files remain encrypted even if the storage provider’s systems are 
compromised. This approach is particularly relevant for individuals and organisations handling sensitive information such as 
personal records, financial data, or intellectual property.
### 2.3 How would this program be used?
CCrypt is designed to be operated through a command line interface. Users specify the file path and a password, and select 
whether compression should be applied. The program then performs encryption locally and outputs an encrypted file, which can 
subsequently be stored on any cloud platform such as Google Drive or OneDrive. When access is required, the user retrieves 
the file and uses the same tool to decrypt it by providing the correct credentials. The program maintains a persistent local 
library that records metadata for each encrypted file, enabling users to list, sort, rename, or remove files efficiently while 
preserving an organised encryption history.
### 2.4 Who is our target user?
The tool is intended for two main user groups. The primary group consists of general users who make regular use of cloud storage
services but do not necessarily possess advanced technical expertise. For these users, CCrypt offers an accessible and reliable 
method of enhancing data security. The secondary group comprises information technology professionals and organisations that 
depend on cloud infrastructure for data storage. For this group, CCrypt provides a lightweight and portable encryption solution 
that can be incorporated into broader data protection frameworks without significant overhead.
- **Primary Users**: 
  - Everday Joe
  - Anyone who uses cloud storage
- **Secondary Users**:
  - IT professionals
  - Large cloud depended cooperations

## 3. Project Scope

### 3.1 Functional Requirements

#### 3.1.1 Core Encryption/Decryption Features
- **File Encryption**: Ability to encrypt individual files using symmetric encryption algorithms
- **File Decryption**: Ability to decrypt previously encrypted files with correct credentials
- **Password Protection**: Secure password-based encryption with user-defined keys
- **File Integrity**: Ensure encrypted files maintain data integrity during encryption/decryption process

#### 3.1.2 Compression Features  
- **Optional Compression**: User choice to compress files before encryption to reduce file size
- **Compression Algorithms**: Implementation of basic compression techniques within C standard library constraints
- **Size Optimization**: Reduce storage space requirements for encrypted files

-Compression/Decompression Technique used
The Compression Technique used for this project is Run-Length Encoding (RLE).This simple compression technique compresses data by replacing sequences of data with single instances of the data. For example, aaabbcc will become 3a2b2c.This technique was especially used for this program since the main file type that is used is txt files.In the case of how this technique is used in the program.For instance,a txt file that shows “Helloooooo world!!!” the function scans this from left to right and stores it in an array of memory where the number of the value goes first followed up with the value e.g [1]['w'][1]['o'][1]['r'][1]['l'][1]['d'].

For the decompression part, the function reads through 2 bytes at a time because of the count and value of the array and expand and reconstruct it into a output array which will show the original content of the txt file.

#### 3.1.3 Library Management System
- **Encryption Registry**: Maintain a persistent record of all encrypted files and their metadata
- **File Tracking**: Track location, encryption date, and file properties of encrypted items
- **Status Monitoring**: Monitor which files are currently encrypted vs decrypted

#### 3.1.4 User Interface Features
- **Interactive CLI**: Menu-driven command-line interface for easy navigation
- **File Path Input**: Accept and validate user-provided file paths for encryption
- **User Prompts**: Clear prompts for compression options and other user choices
- **Error Handling**: Comprehensive error messages and input validation

#### 3.1.5 File Management Operations
- **List Encrypted Files**: Display all files currently managed by the encryption library
- **Delete Encrypted Files**: Remove encrypted files from both library and filesystem
- **Rename Operations**: Rename encrypted files while maintaining library consistency
- **File Information**: Display metadata including file size, type, and encryption date

#### 3.1.6 Sorting and Organization
- **Sort by Name**: Alphabetical sorting of encrypted files in the library
- **Sort by Date**: Chronological sorting by encryption date or last modified date
- **Sort by Size**: Sort files by original or encrypted file size
- **Sort by Type**: Group and sort files by file extension or type

### 3.2 Technical Constraints

#### 3.2.1 Library Restrictions
- **Standard Libraries Only**: Limited to stdio.h, stdlib.h, string.h, and math.h
- **No External Dependencies**: Cannot use third-party encryption libraries
- **Cross-Platform Compatibility**: Code must compile and run on standard C compilers

#### 3.2.2 Implementation Constraints
- **Custom Encryption**: Must implement encryption algorithms from scratch using allowed libraries
- **File I/O Operations**: All file operations must use standard C file handling functions
- **Memory Management**: Efficient memory usage with proper allocation and deallocation
- **Error Recovery**: Robust error handling without external logging libraries

### 3.3 Deliverables

#### 3.3.1 Source Code Files
- **ccrypt.h**: Header file with function prototypes and documentation
- **ccrypt.c**: Main implementation file with complete functionality
- **Additional modules**: Separate files for encryption, compression, and library management if needed

#### 3.3.2 Documentation
- **Function Documentation**: Block comments for all functions with parameter descriptions
- **Structure Chart**: Visual representation of program architecture and module relationships
- **User Manual**: Instructions for compiling and using the CCrypt tool
- **Technical Report**: This comprehensive project report

#### 3.3.3 Testing Materials
- **Test Cases**: Comprehensive test scenarios covering all functional requirements
- **Sample Files**: Various file types and sizes for testing encryption/decryption
- **Error Scenarios**: Test cases for error handling and edge cases

### 3.4 Limitations and Exclusions

#### 3.4.1 Current Scope Limitations
- **Folder Encryption**: Individual file encryption only (folder encryption may be considered for future versions)
- **Encryption Algorithms**: Limited to algorithms implementable with standard C libraries
- **Network Features**: No remote file access or network-based encryption
- **GUI Interface**: Command-line interface only, no graphical user interface

#### 3.4.2 Future Enhancements (Out of Current Scope)
- **Advanced Encryption**: Integration with industry-standard encryption libraries
- **Batch Operations**: Simultaneous encryption/decryption of multiple files
- **Cloud Integration**: Direct integration with cloud storage services
- **Mobile Support**: Versions for mobile platforms

## 4. System Architecture

### 4.1 High-Level Architecture
- Overall system design and component relationships
- Data flow between modules
- User interaction patterns

### 4.2 Module Design
- Encryption module structure
- Compression module design
- Library management module architecture
- User interface module organization

### 4.3 Data Structures
- File metadata structures
- Encryption key management
- Library database design

## 5. Detailed Design

### 5.1 Function Specifications
- Complete function prototypes with detailed descriptions
- Parameter specifications and return values
- Error handling mechanisms

### 5.2 Algorithm Design
- Encryption algorithm implementation details
- Compression algorithm specifications
- Sorting and search algorithms

### 5.3 File Format Specifications
- Encrypted file format structure
- Library database file format
- Configuration file specifications

## 6. Implementation Plan

### 6.1 Development Phases
- Phase 1: Core encryption/decryption functionality
- Phase 2: Library management system
- Phase 3: File management operations
- Phase 4: Sorting and user interface enhancements

### 6.2 Testing Strategy
- Unit testing approach
- Integration testing plan
- User acceptance testing criteria

### 6.3 Risk Management
- Technical risks and mitigation strategies
- Timeline risks and contingency plans

## 7. Evaluation and Testing

### 7.1 Testing Methodology
- Test case design principles
- Automated testing approach
- Performance testing criteria

### 7.2 Success Criteria
- Functional requirement validation
- Performance benchmarks
- User experience metrics

## 8. Conclusion

### 8.1 Project Summary
- Achievement of objectives
- Key learnings and challenges

### 8.2 Future Work
- Potential enhancements and improvements
- Scalability considerations

## 9. References and Appendices

### 9.1 References
- Technical references and standards
- Algorithm sources and documentation

### 9.2 Appendices
- Source code listings
- Test results and documentation
- User manual and installation guide
