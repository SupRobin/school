#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, const char *argv[]) {
    std::cout << "hello";
    pid_t pid = fork();

    if (pid < 0) {
        write(STDERR_FILENO, "Can't fork the process\n", 23);
        return 2;
    } else if (pid == 0) {
        int ret = execlp("./showfile/exe", "showfile.exe", "myFile.txt", (char *) NULL);
        if (ret < 0) {
            write(STDERR_FILENO, "Can't execute showfile.exe", 28);
            return 3;
        }
    } else {
        int status;

        if (wait(&status) == pid) {
            if (WIFEXITED(status)) {
                write(STDERR_FILENO, "Child normal termination\n", 25);
                int childstat = WEXITSTATUS(status);
                std::cout << "child exit status is " << childstat << std::endl;
            }
            else if (WIFSIGNALED(status)){
                write(STDOUT_FILENO, "Child abnormal termination", 24);
            }
        }
        else {
            write(STDERR_FILENO, "wait() call failed\n", 19);
            return 4;
        }
    }

    return 0;
}