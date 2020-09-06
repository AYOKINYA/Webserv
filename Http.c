#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(void)
{
	int sockfd_;
	int new_sock;
	long valread;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	char *msg = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";

	if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		exit(1);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

	if (bind(sockfd_, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		exit(1);
	if (listen(sockfd_, 10) == -1)
		exit(1);
	while (1)
	{
		printf("\n=====Waiting for new connection=====\n");
		if ((new_sock = accept(sockfd_, (struct sockaddr *)&addr, &addrlen)) == -1)
			exit(1);

		char buf[30001];
		memset(buf, 0, sizeof(buf));
		valread = read(new_sock, buf, 30000);
		buf[valread] = '\0';
		printf("%s\n", buf);

		write(new_sock, msg, strlen(msg));
		printf("===Hello message sent===\n");
		close(new_sock);
	}		

	return (0);
}