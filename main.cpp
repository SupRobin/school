#include <iostream>
#include <fcntl.h>
#include <unistd.h>

const unsigned int BYTES = 32;

struct Employee {
    std::string name;
    long salary;
};
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {



    int fd = open("./Employee.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    const char *apples[BYTES];
    while (read(fd, apples, BYTES)) {
        for (auto & apple : apples) {
            std::cout << apple;
        }
    }
    ssize_t bytes_read = read(fd, apples, BYTES);
    if (bytes_read == -1) {
        perror("read");
        exit(2);
    }

    std::cout << apples << std::endl;
    std::cout << apples[0] << std::endl;
    std::cout << "hello" << std::endl;

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        exit(3);
    }

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.