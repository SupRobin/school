#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

const unsigned int BYTES = 32;

struct Employee {
    char *name;
    char *salary;

    void toString() const {
        std::cout << "Name: " << name << ", Salary: " << salary << std::endl;
    }


};

char* fksHash(Employee name) {
    static char hash[BYTES];

    return hash;
}

void openReadHashAndClose(const char *fileName, char apples){
    int fd = open(fileName, O_RDWR);
    int bf = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1 || bf == -1) {
        perror("open/create");
        exit(1);
    }

    std::cout << "Opened file " << fd << std::endl;
    std::cout << "Created file " << bf << std::endl;
    char name[16] = {'\0'};
    char salary[16] = {'\0'};
    apples = '\0';
    int nCounter = 0;
    int sCounter = 0;
    while (read(fd, &apples, 1) > 0) {
        if (apples >= 65 && apples <= 122 ) {
            name[nCounter] = apples;
            nCounter++;
        }
        else if (apples>= 48 && apples<= 57 ) {
            salary[sCounter] = apples;
            sCounter++;
        }
        if (apples == '\n') {
            Employee employee = {
                name,
                salary
            };
            ssize_t bytesWritten = write(bf, &employee,sizeof(Employee));
            if ( bytesWritten != sizeof(Employee) ) {
                perror("write");
            } else {
                std::cout << "Wrote Employee name: " << &employee.name << "Employee salary: " << &employee.salary << std::endl;
                employee.toString();
            }
            nCounter = 0;
            sCounter = 0;
            for (int i = 0; i < 16; i++) {
                name[i] = '\0';
                salary[i] = '\0';
            }
        }
    }

    close(fd);
}

int main(const int argc, const char *argv[]) {
    char apples;
    const char *fileName = argv[1];
    std::cout << argc << std::endl;
    std::cout << fileName << std::endl;

    openReadHashAndClose(fileName, apples);

    return 0;
}

