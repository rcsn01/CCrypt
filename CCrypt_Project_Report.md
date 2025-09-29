# CCrypt Project Report

## 1. Executive Summary
- Brief overview of the CCrypt tool
- Key features and capabilities
- Target audience summary

## 2. Project Introduction

### 2.1 Project Idea
A CLI C tool that lets you encrypt and decrypt files, optionally with compression or without.

### 2.2 Why is this program needed?
- **Data Security**: In an increasingly digital world, protecting sensitive files from unauthorized access is crucial
- **Privacy Protection**: Users need a simple way to secure personal documents, photos, and other sensitive data
- **Compliance Requirements**: Many professionals need to encrypt files to meet regulatory requirements
- **Portable Security**: A standalone CLI tool provides encryption capabilities without dependency on complex software suites
- **Educational Value**: Demonstrates fundamental cryptographic concepts and file manipulation in C

### 2.3 How would this program be used?
- **Personal Use**: Encrypt sensitive documents before storing them on cloud services or external drives
- **Professional Use**: Secure confidential business documents and client data
- **Developer Use**: Protect source code and configuration files containing sensitive information
- **Educational Use**: Learn about encryption, compression, and file management concepts
- **Backup Security**: Encrypt files before creating backups to ensure data remains secure even if backup media is compromised

### 2.4 Who is our target user?
- **Primary Users**: 
  - Computer science students learning about cryptography and file systems
  - Software developers needing lightweight encryption tools
  - Security-conscious individuals wanting to protect personal files
- **Secondary Users**:
  - IT professionals requiring simple encryption solutions
  - Researchers handling sensitive data
  - Anyone needing basic file encryption without complex software dependencies

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