/**
 * File Locking Mechanism Implementation
 * 
 * This file implements system-level file locking:
 * - Preventing concurrent file access
 * - Managing file locks
 * - Handling lock release
 */

#include "fileLock.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <iostream>

/**
 * Acquire a lock on specified file
 * 
 * Process:
 * 1. Open file for read/write
 * 2. Attempt to acquire exclusive lock
 * 3. Return file descriptor or error
 * 
 * Features:
 * - Non-blocking operation
 * - Exclusive access
 * - Error reporting
 * 
 * @param filename File to lock
 * @return int File descriptor or -1 on failure
 */
int acquireLock(const std::string& filename) {
    int fd = open(filename.c_str(), O_RDWR);
    if (fd == -1) {
        std::cerr << "Error opening file for locking" << std::endl;
        return -1;
    }

    // Try to acquire exclusive, non-blocking lock
    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}

/**
 * Release a previously acquired lock
 * 
 * Process:
 * 1. Remove lock
 * 2. Close file descriptor
 * 
 * Safety:
 * - Checks for valid file descriptor
 * - Ensures complete cleanup
 * 
 * @param fd File descriptor to unlock
 */
void releaseLock(int fd) {
    if (fd != -1) {
        flock(fd, LOCK_UN);  // Remove lock
        close(fd);           // Close file
    }
}
