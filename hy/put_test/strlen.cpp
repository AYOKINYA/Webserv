#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char buf[1023];
	int fd = open("./file_should_exist_after", O_RDWR);
	int size = read(fd, buf, 1023);
	std::cout << size;
}
