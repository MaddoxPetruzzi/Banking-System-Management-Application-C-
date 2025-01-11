# Banking System Management Application

A secure banking system implemented in C++ that supports multiple account types, role-based access control, and encrypted data storage.

## Features
- Multiple account types (Checking, Savings, Certificate of Deposit)
- Role-based access (Manager/Client)
- Secure authentication with password hashing
- Encrypted data storage
- Concurrent transaction handling with file locking
- Command-line interface with color-coded menus

## Technical Implementation
- Object-oriented design with inheritance hierarchies
- Smart pointers for memory management
- File I/O with encryption
- STL containers and algorithms
- Error handling and input validation

## Building and Running
```bash
# Compile
g++ *.cpp -o banking_system

# Run
./banking_system

Default Login Credentials

Manager Account:

Username: admin
Password: admin
