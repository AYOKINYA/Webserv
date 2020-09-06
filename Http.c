#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define PORT 8080

int ft_strlen(char *s)
{
	int count = 0;
	while (s[count] != '\0')
		++count;
	return (count);
}

long long	ft_abs(long long n)
{
	if (n < 0)
		n = n * -1;
	return (n);
}

int			ft_len(long long n)
{
	int	len;

	len = 0;
	if (n <= 0)
		++len;
	while (n != 0)
	{
		n = n / 10;
		++len;
	}
	return (len);
}

char		*ft_itoa(long long n)
{
	char		*res;
	int			len;
	long		nbr;

	nbr = n;
	len = ft_len(nbr);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	res[len] = '\0';
	if (nbr < 0)
	{
		res[0] = '-';
		nbr = ft_abs(nbr);
	}
	if (nbr == 0)
		res[0] = '0';
	while (nbr != 0)
	{
		--len;
		res[len] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*joined_str;
	int	i;
	int	j;

	joined_str = (char *)malloc(sizeof(char) * \
							(ft_strlen(s1) + ft_strlen(s2) + 1));
	if (joined_str == 0)
		return (0);
	i = -1;
	while (++i < ft_strlen(s1))
		joined_str[i] = s1[i];
	j = -1;
	while (++j < ft_strlen(s2))
		joined_str[i++] = s2[j];
	joined_str[i] = '\0';
	free(s1);
	return (joined_str);
}

char		*ft_strdup(char *s)
{
	int		len;
	int		i;
	char	*temp;

	len = ft_strlen(s);
	temp = (char *)malloc(sizeof(char) * (len + 1));
	if (temp == 0)
		return (0);
	i = 0;
	while (i < len)
	{
		temp[i] = s[i];
		++i;
	}
	temp[i] = '\0';
	return (temp);
}

int main(void)
{
	int sockfd_;
	int new_sock;
	long valread;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int fd;
	char rbuf[1024];
	char *tmp;
	
	char *msg;
	msg = ft_strdup("");
	msg = ft_strjoin(msg, "HTTP/1.1 200 OK\n");
	msg = ft_strjoin(msg, "Content-Type: text/html\n");
	msg = ft_strjoin(msg, "Content-Length: ");

	fd = open("index.html", O_RDWR, 0644);
	valread = read(fd, rbuf, 1023);
	rbuf[valread] = '\0';
	tmp = ft_itoa(ft_strlen(rbuf));
	msg = ft_strjoin(msg, tmp);
	free(tmp);
	msg = ft_strjoin(msg, "\n\n");
	msg = ft_strjoin(msg, rbuf);
	printf("%s\n", msg);
	
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