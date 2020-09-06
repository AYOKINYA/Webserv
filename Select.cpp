# include "./libft/libft.h"
# include <iostream>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <fcntl.h>
# include <errno.h>
# include <arpa/inet.h>

# include <vector>

int main(void)
{
	int max_clients = 30;
	int client_socket[30];
	char buf[1025];

	//a set of socker descriptors
	fd_set readfds;

	for (int i = 0; i < max_clients; ++i)
		client_socket[i] = 0;
	
	int master_socket;
	int opt = 1;
	struct sockaddr_in addr;
	if ( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
		exit(1);
	if ( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
		exit(1);
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(8080);

	if (bind(master_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		exit(1);
	if (listen(master_socket, 3) < 0)
		exit(1);
	socklen_t addrlen = sizeof(addr);
	int max_sd, sd, activity, new_socket, valread;
	while (1)
	{
		FD_ZERO(&readfds);

		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		for (int i = 0; i < max_clients; ++i)
		{
			sd = client_socket[i];
			if (sd > 0)
				FD_SET(sd, &readfds);
			if (sd > max_sd)
				max_sd = sd;
		}

		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR))
			std::cerr << "select error\n";
		
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket, (struct sockaddr *)&addr, (socklen_t *)&addrlen)) < 0)
				exit(1);
			
			std::cout << "New connection! socket fd is " << new_socket;
			std::cout << " , port is " << addr.sin_port << std::endl;

			send(new_socket, "hiiii", 6, 0);
			std::cout << "hiiii is sent from the server" << std::endl;

			for (int i = 0; i < max_clients; ++i)
			{
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					break;
				}
			}

		}

		for (int i = 0; i < max_clients; ++i)
		{
			if (FD_ISSET(sd, &readfds))
			{
				if ((valread = read(sd , buf, 1024)) == 0)
				{
					close(sd);
					client_socket[i] = 0;
				}
				else
				{
					buf[valread] = '\0';
					send(sd, buf, strlen(buf), 0);
				}
			}
		}
		
	}

	
	return (0);
}