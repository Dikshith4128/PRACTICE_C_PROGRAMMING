#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("testfile.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    const char *message = "Hello, Linux!\n";
    write(fd, message, 14);
    close(fd);

    fd = open("testfile.txt", O_RDONLY);
    char buffer[50];
    read(fd, buffer, 14);
    printf("Read from file: %s", buffer);
    close(fd);

    return 0;
}

