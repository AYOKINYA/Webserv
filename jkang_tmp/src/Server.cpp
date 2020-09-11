#include "Server.hpp"
//#include "Client.hpp"

Server::Server(const std::string &name, int port)
{
	_name = name;
	_port = port;
}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server& Server::operator=(const Server &server)
{
	if (this == &server)
		return (*this);
	_name = server._name;
	_port = server._port;
	_sockfd = server._sockfd;
	_server_addr = server._server_addr;
	return (*this);
}

Server::~Server()
{
	std::cout << "server is dead" << std::endl;
};

int	Server::getSockfd(void)
{
	return (this->_sockfd);
}

void	Server::init_server(void)
{
	if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Error: " << _name << " init_server() socket: " << strerror(errno) << std::endl;
		exit(1);
	}

	int opt = 1;
	// Forcefully attaching socket to the port 8080
		if( setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	ft_memset((void *)&_server_addr, 0, (unsigned long)sizeof(_server_addr)); // 왜 libft ft_memset link가 안 될까?
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons( this->_port );

	// Forcefully attaching socket to the port 8080
	if (bind(_sockfd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1)
	{
		std::cerr << "Error: " << _name << " init_server() bind: " << strerror(errno) << std::endl;
		exit(1);
	}

	if (listen(_sockfd, 10) == -1)
	{
		std::cerr << "Error: " << _name << " init_server() listen: " << strerror(errno) << std::endl;
		exit(1);
	}
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error: " << _name << " init_server() fcntl: " << strerror(errno) << std::endl;
		exit(1);
	}

	char buf[3001];
	int new_socket;

	fd_set readfds;
	int	max_sd, client_socket[30], max_clients = 30, sd, selected;

	socklen_t addrlen = sizeof(_server_addr);
	for (int i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}
	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(_sockfd, &readfds);
		max_sd = _sockfd;
		for (int i = 0 ; i < max_clients; i++)
		{
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);

			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
		struct timeval time;
		time.tv_sec = 5;
		time.tv_usec = 5000;
		selected = select( max_sd + 1 , &readfds , NULL , NULL , &time);

		if ((selected < 0) && (errno!=EINTR))
		{
			printf("select error");
		}
		usleep(2000);
		if (FD_ISSET(_sockfd, &readfds))
		{
			if ((new_socket = accept(_sockfd, (struct sockaddr *)&_server_addr, &addrlen)) == -1)
			{
				std::cerr << "Error: " << _name << " init_server() accept: " << strerror(errno) << std::endl;
				exit(1);
			}
			usleep(2000);
			ft_memset(buf, 0, 3001);
			for (int i = 0; i < max_clients; i++)
			{
				//if position is empty
				if(client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}

		for (int i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET(sd , &readfds))
			{
				int	valread;

				if (fcntl(sd, F_SETFL, O_NONBLOCK) == -1)
				{
					std::cerr << "Error: " << _name << " init_server() fcntl: " << strerror(errno) << std::endl;
					exit(1);
				}
				std::string req = "";
				while ((valread = read(sd , buf, 1024)) != 0 && req.find("\r\n\r\n") == std::string::npos)
				{
					if (valread > 0)
					{
						buf[valread] = '\0';
					//ReceieveRequest 여기서!
					//첫번째 read한 buf로 리퀘스트 파싱 처리
					//2번째 read부터는 request 바디에 추가?
					
					req += buf;
					}

				}
				if (valread == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
					std::cout << "recv error" << std::endl;
				else if (valread == 0)
				{
					close( sd );
					client_socket[i] = 0;
				}
				std::cout << req << std::endl;
				request.parse_request(req);
				
				// m.receiveRequest(buf);
				// m.sendRespond(sd);
				std::cout << request.get_path() << std::endl;
				
				std::string response_msg = response.getStartLine();
				response_msg += "\n";
				response_msg += response.header(request.get_path());
				response_msg += "\n";
				response_msg += response.body(request.get_path());
				std::cout << response_msg << std::endl;
				send(sd, response_msg.c_str(), response_msg.length(), 0);
				std::cout << "Server sent message" << std::endl;

				//buffer[valread] = '\0';
				//send(sd , buffer , strlen(buffer) , 0 );
				
				ft_memset(buf, 0, 3001);
			}
		}


	}
}