/**
 * File Locking Mechanism Header
 * 
 * Purpose:
 * Provides file locking functionality to prevent concurrent access.
 * Ensures data integrity during file operations.
 * 
 * Features:
 * - File-based locking
 * - Non-blocking operations
 * - Automatic lock cleanup
 */

#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <string>

// Acquire a lock on a file
// Returns file descriptor or -1 if lock cannot be acquired
int acquireLock(const std::string& filename);

// Release a previously acquired lock
void releaseLock(int fd);

#endif // FILE_LOCK_H
