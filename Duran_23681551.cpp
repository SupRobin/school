#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

//-----------------------------------------------------------------------------
// Struct definition to hold an employee's information.
// This struct uses fixed-size character arrays for both name and salary.
//-----------------------------------------------------------------------------
static int arr[100] = {0}; // Static array to store encountered indices (for collision tracking)

struct Employee {
    char name[16]{}; // Employee name (up to 15 characters + null terminator)
    char salary[16]{}; // Employee salary stored as a string

    // Default constructor (does nothing special, uses default initialization)
    Employee() = default;

    // All-arguments constructor: copies given name and salary into the struct.
    Employee(char name[16], char salary[16]) {
        strcpy(this->name, name); // Copy the provided name into the struct's name field
        strcpy(this->salary, salary); // Copy the provided salary into the struct's salary field
    }

    // Destructor: default behavior is sufficient here.
    ~Employee() = default;

    // toString function to print out the employee's details.
    void toString() const {
        std::cout << "Name: " << name << ", Salary: " << salary << std::endl;
    }
};

//-----------------------------------------------------------------------------
// A simple hashing function that computes a hash value from a name.
// The hash function multiplies the current hash by 31 (a common prime)
// and adds the ASCII value of each character.
//-----------------------------------------------------------------------------
int hash(const char *name, int tableSize) {
    int hashed = 0;
    // Loop through each character in the name until the null terminator
    for (int i = 0; name[i] != '\0'; i++) {
        hashed = hashed * 31 + name[i]; // Multiply current hash by 31 and add the current character
    }
    // Loop through an external array "arr" to check for collisions (if any)
    // NOTE: 'arr' is defined as a static array in findEmployee. This loop is meant
    // to check if the computed index is already present and then adjust the hash.
    for (int i: arr) {
        if (i == (hashed % tableSize)) {
            std::cout << i << std::endl;
            // Recursively adjust the tableSize if the current index is already taken
            hash(name, tableSize - 1);
        }
    }
    // Ensure the hash value is non-negative
    if (hashed < 0) hashed = -hashed;
    // Return the index by taking modulo tableSize
    return hashed % tableSize;
}

//-----------------------------------------------------------------------------
// Function to find an employee's salary in a binary file using a hash-based approach.
// It uses lseek() to jump to the correct position and read() to get the Employee record.
//-----------------------------------------------------------------------------
void findEmployee(const char *fileName, const char *empName, int tableSize) {
    Employee employee; // Temporary Employee object to store read data
    static int key = 0; // Static counter for number of indices stored in arr
    int fd = open(fileName, O_RDONLY); // Open the binary file in read-only mode

    if (fd == -1) {
        // Check if file open failed
        perror("cannot open file");
        exit(1);
    }

    // Calculate the initial index using the hash function.
    int index = hash(empName, tableSize);
    int start = 1; // Start the linear probing from slot 1 (this can be adjusted as needed)

    // Loop through the binary file using linear probing to locate the desired employee record.
    while (true) {
        // Calculate the file offset: current probing position * size of an Employee record.
        off_t size = start * sizeof(Employee);
        // Move the file pointer to the computed offset.
        if (lseek(fd, size, SEEK_SET) == -1) {
            perror("cannot lseek");
            // If lseek fails, print a "not found" message and exit.
            std::cout << "Employee name: " << empName
                    << " Salary: " << "Not found"
                    << " Index: " << "Not found" << std::endl;
            exit(-1);
        }

        // Read an Employee record from the file at the current offset.
        ssize_t bytesRead = read(fd, &employee, sizeof(Employee));
        if (bytesRead != sizeof(Employee)) {
            // If we cannot read a full record, break out of the loop (likely end-of-file).
            break;
        }

        // Check if the employee name in the record matches the search key.
        if (strcmp(employee.name, empName) == 0) {
            close(fd); // Close the file when the record is found.
            arr[key] = index; // Save the found index in our static array.
            key++; // Increment our counter.
            // Output the found employee information.
            std::cout << "Employee name: " << employee.name
                    << " Salary: " << employee.salary
                    << " Index: " << index << std::endl;
            return;
        }
        // Increment the probing index and continue searching.
        start = start + 1;
        if (start == tableSize) {
            // If we've probed all buckets, stop the search.
            break;
        }
    }

    close(fd); // Close the file if the employee wasn't found.
    // Output a "not found" message.
    std::cout << "Employee name: " << empName
            << " Salary: " << "Not found"
            << " Index: " << "Not found" << std::endl;
}

//-----------------------------------------------------------------------------
// Function to open the employee text file, read its contents, and write the data
// as binary records into a binary file. This populates the hash table stored on disk.
//-----------------------------------------------------------------------------
void openReadWriteAndClose() {
    // Open the employee text file for reading and the binary file for read/write.
    int fd = open("Employee.txt", O_RDWR, 0644);
    int bf = open("data.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1 || bf == -1) {
        // Check for file open/create errors.
        perror("open/create");
        exit(1);
    }
    std::cout << "Opened file " << fd << std::endl;
    std::cout << "Created file " << bf << std::endl;

    // Initialize character arrays to hold the name and salary from the text file.
    char name[16] = {'\0'};
    char salary[16] = {'\0'};
    char apples = '\0'; // Variable to hold each character read from the file.
    int nCounter = 0, sCounter = 0; // Counters for name and salary arrays.

    // Read the text file one character at a time.
    while (read(fd, &apples, 1) > 0) {
        // If the character is an alphabet letter, assume it's part of the name.
        if (apples >= 65 && apples <= 122) {
            name[nCounter] = apples;
            nCounter++;
        }
        // If the character is a digit, assume it's part of the salary.
        else if (apples >= 48 && apples <= 57) {
            salary[sCounter] = apples;
            sCounter++;
        }
        // When a newline is encountered, it signifies the end of one employee record.
        if (apples == '\n') {
            // Create an Employee object using the accumulated name and salary.
            Employee employee = Employee(name, salary);
            // Write the Employee record to the binary file.
            ssize_t bytesWritten = write(bf, &employee, sizeof(Employee));
            if (bytesWritten != sizeof(Employee)) {
                perror("write");
            }
            // Reset counters and clear the name and salary arrays for the next record.
            nCounter = 0, sCounter = 0;
            for (int i = 0; i < 16; i++) {
                name[i] = '\0';
                salary[i] = '\0';
            }
        }
    }
    // Close both files when finished.
    close(fd);
    close(bf);
}

//-----------------------------------------------------------------------------
// Main function: entry point of the program.
// It populates the binary hash table and then processes search queries.
//-----------------------------------------------------------------------------
int main(const int argc, const char *argv[]) {
    // Read the employee text file and write data into a binary file.
    openReadWriteAndClose();

    // For each command-line argument (expected to be an employee name),
    // call findEmployee to search for the employee in the binary file.
    for (int i = 0; i < argc; i++) {
        findEmployee("data.bin", argv[i], 200);
    }

    return 0;
}
