#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

const unsigned int SIZ = 32;

int main(int argc, const char *argv[]) {
    int f1;

    if (argc != 2) {
        write(STDERR_FILENO, "Wrong numberof command line argument\n", 36);
        return 0;
    }

    if ((f1 = open(argv[1], O_RDONLY, 0)) == -1) {
        write(STDERR_FILENO, "Can't open input file\n", 23);
        return 2;
    }

    char buf[SIZ];
    int n;

    while ((n = read(f1, buf, SIZ)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            write(STDERR_FILENO, "Can't display file\n", 19);
            close(f1);
            return 3;
        }
    }
    close(f1);

}
