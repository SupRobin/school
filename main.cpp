#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

const unsigned int BYTES = 32;


struct Employee {
    char name[16]{};
    char salary[16]{};

    Employee() = default;

    Employee(char name[16], char salary[16]) {
        strcpy(this->name, name);
        strcpy(this->salary, salary);
    }

    ~Employee() = default;
    void toString() const {
        std::cout << "Name: " << name << ", Salary: " << salary << std::endl;
    }
};

//here for testing static Employee arr[100];


const char* fksHash(Employee name) {


    return  "Hellooo";
}


void readFromWrite(const char *fileName) {
    Employee employee;
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        perror("open/create");
        exit(1);
    }
    std::cout << "Opened file " << fd << std::endl;
    std::cout << "Reading: " << std::endl;
    while (read(fd, &employee, sizeof(Employee)) > 0) {
        employee.toString();
    }
    close(fd);
}

void openReadHashAndClose(const char *fileName, char apples) {
    int fd = open(fileName, O_RDWR, 0644);
    int bf = open("data.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
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
    //here for testing: int empCounter = 0;
    while (read(fd, &apples, 1) > 0) {
        if (apples >= 65 && apples <= 122) {
            name[nCounter] = apples;
            nCounter++;
        } else if (apples >= 48 && apples <= 57) {
            salary[sCounter] = apples;
            sCounter++;
        }
        if (apples == '\n') {
            Employee employee = Employee(name, salary);
            // here for testing: arr[empCounter] = employee;
            ssize_t bytesWritten = write(bf, &employee, sizeof(Employee));
            if (bytesWritten != sizeof(Employee)) {
                perror("write");
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
    close(bf);
}

int main(const int argc, const char *argv[]) {
    char apples = '\0';
    const char *fileName = argv[1];
    std::cout << argc << std::endl;
    std::cout << fileName << std::endl;
    openReadHashAndClose(fileName, apples);

    readFromWrite("data.bin");

    return 0;
}

