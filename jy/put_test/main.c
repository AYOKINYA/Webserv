#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(void)
{
    int count = 0;
    int fd = open("../cgi.txt", O_RDWR, 0644);
    // int fd = open("file_should_exist_after", O_RDWR, 0644);
    char buf[2];
    buf[0] = 0;
    buf[1] = 0;
    while (read(fd, buf, 1) > 0)
        ++count;
    printf("count: %d\n", count);
    return (0);
}
