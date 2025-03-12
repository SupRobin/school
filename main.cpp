#include <iostream>
#include <fcntl.h>
#include <unistd.h>

const unsigned int BYTES = 32;

struct Employee {
    char* name;
    int salary = 0;
};

char* fksHash(Employee name) {
    static char hash[BYTES];

    return hash;
}

void openReadWriteAndClose(const char *fileName, char *apples){
    int fd = open(fileName, O_RDWR);
    int bf = open("data.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1 || bf == -1) {
        perror("open/create");
        exit(1);
    }

    std::cout << "Opened file " << fd << std::endl;
    std::cout << "Created file " << bf << std::endl;
    ssize_t bytesRead;
    ssize_t bytesWritten;
    while ((bytesRead  = read(fd, apples, BYTES)) > 0) {
        apples[bytesRead] = '\0';
        bytesWritten = write(bf, apples, BYTES);
        if (bytesWritten != sizeof(Employee)) {
            perror("write");
        }
    }

    close(fd);
}

int main(const int argc, const char *argv[]) {
    char *apples;
    const char *fileName = argv[1];
    std::cout << argc << std::endl;
    std::cout << fileName << std::endl;

    openReadWriteAndClose(fileName, apples);

    return 0;
}

