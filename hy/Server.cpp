#include "Server.hpp"
//#include "Client.hpp"

Server::Server(const std::string &name, int port)
{
	_name = name;
	_port = port;
	_req = "";
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
	_fd = server._fd;
	_server_addr = server._server_addr;

	_rset = server._rset;
	_wset = server._wset;
	return (*this);
}

Server::~Server()
{
	std::cout << "server is dead" << std::endl;
};

int	Server::get_server_fd(void)
{
	return (this->_fd);
}

int Server::read_request(Client c)
{
	// std::string req = "";
	int complen = 0;
	int valread;
	char buf[BUFFER_SIZE + 1];
	struct sockaddr_in client_addr;
	socklen_t c_addrlen = sizeof(client_addr);

	getsockname(c.get_fd(), (struct sockaddr *)&client_addr, &c_addrlen);
	std::string client_ip = inet_ntoa(client_addr.sin_addr);
	valread = read(c.get_fd() , buf, BUFFER_SIZE);
	if (valread != 0)
	{
		if (ft_strncmp(buf, "\x04", 1) == 0) // ctrl + d from telnet!
		{
			valread = 0; // to close client's socket.
			return 0;
		}
		if (valread > 0)
		{
			buf[valread] = '\0';
			_req += buf;
		}
		complen = _req.length();
		if ((complen > 3 && _req.substr(complen - 4) == "\r\n\r\n")) // body max size 조건 추가헤야 한다....
			return 0;
	}
	else if (valread == -1)
	{
		std::cout << "recv error" << std::endl;
		return 0;
	}
	else if (valread == 0)
	{
		close(c.get_fd());
		delete(c);
	}
	Request request(client_ip);
	request.parse_request(_req);
	//////////
	Response	response(request);
	std::string response_msg = response.exec_method();
	std::cout << response_msg << std::endl;
	send(client_fd, response_msg.c_str(), response_msg.length(), 0);
	std::cout << "Server sent message" << std::endl;
	ft_memset(buf, 0, 1024);
	//////////
}

int	Server::write_response()
{

}


int	Server::get_max_fd(void)
{
	return (this->_max_fd);
}

void	Server::init_server(void)
{
	int opt = 1;

	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		exit(1); //나중에 throw로 처리
	// Forcefully attaching socket to the port 8080
		if( setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
		exit(EXIT_FAILURE); //나중에 throw로 처리

	ft_memset((void *)&_server_addr, 0, (unsigned long)sizeof(_server_addr)); // 왜 libft ft_memset link가 안 될까?
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons(_port);

	// Forcefully attaching socket to the port 8080
	if (bind(_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1)
		exit(1); //나중에 throw로 처리
	if (listen(_fd, 10) == -1)
		exit(1); //나중에 throw로 처리
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		exit(1); //나중에 throw로 처리
	FD_SET(_fd, _rset);
    _max_fd = _fd;
}


void	Server::accept_client()
{
	int	new_socket;
	socklen_t addrlen = sizeof(_server_addr);

	if (FD_ISSET(_fd, _rset))
	{
		if ((new_socket = accept(_fd, (struct sockaddr *)&_server_addr, &addrlen)) == -1)
			exit(1);
			//throw 로 에러 처리하기
		usleep(2000);
	}
	if (new_socket > _max_fd)
		_max_fd = new_socket;
	// fcntl(*new_socket, F_SETFL, O_NONBLOCK);
	// FD_SET(*new_socket, _rset); //set
	// FD_SET(*new_socket, _wset);
	Client *client = new Client(new_socket, _rset, _wset);
	_clients.push_back(client); //clients_fd에 넣음
}

// void	Server::init_server(void)
// {
// 	if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
// 	{
// 		std::cerr << "Error: " << _name << " init_server() socket: " << strerror(errno) << std::endl;
// 		exit(1);
// 	}

// 	int opt = 1;
// 	// Forcefully attaching socket to the port 8080
// 		if( setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
// 	{
// 		perror("setsockopt");
// 		exit(EXIT_FAILURE);
// 	}
// 	ft_memset((void *)&_server_addr, 0, (unsigned long)sizeof(_server_addr)); // 왜 libft ft_memset link가 안 될까?
// 	_server_addr.sin_family = AF_INET;
// 	_server_addr.sin_addr.s_addr = INADDR_ANY;
// 	_server_addr.sin_port = htons( this->_port );

// 	// Forcefully attaching socket to the port 8080
// 	if (bind(_sockfd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1)
// 	{
// 		std::cerr << "Error: " << _name << " init_server() bind: " << strerror(errno) << std::endl;
// 		exit(1);
// 	}

// 	if (listen(_sockfd, 10) == -1)
// 	{
// 		std::cerr << "Error: " << _name << " init_server() listen: " << strerror(errno) << std::endl;
// 		exit(1);
// 	}
// 	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
// 	{
// 		std::cerr << "Error: " << _name << " init_server() fcntl: " << strerror(errno) << std::endl;
// 		exit(1);
// 	}
// 	char buf[1024];
// 	int new_socket;

// 	fd_set readfds;
// 	int	max_sd, client_socket[30], max_clients = 30, sd, selected;

// 	socklen_t addrlen = sizeof(_server_addr);
// 	for (int i = 0; i < max_clients; i++)
// 	{
// 		client_socket[i] = 0;
// 	}
// 	while (1)
// 	{
// 		FD_ZERO(&readfds);
// 		FD_SET(_sockfd, &readfds);
// 		max_sd = _sockfd;
// 		for (int i = 0 ; i < max_clients; i++)
// 		{
// 			//socket descriptor
// 			sd = client_socket[i];

// 			//if valid socket descriptor then add to read list
// 			if(sd > 0)
// 				FD_SET( sd , &readfds);

// 			//highest file descriptor number, need it for the select function
// 			if(sd > max_sd)
// 				max_sd = sd;
// 		}
// 		struct timeval time;
// 		time.tv_sec = 5;
// 		time.tv_usec = 5000;
// 		selected = select( max_sd + 1 , &readfds , NULL , NULL , &time);

// 		if ((selected < 0) && (errno!=EINTR))
// 		{
// 			printf("select error");
// 		}
// 		usleep(2000);
// 		if (FD_ISSET(_sockfd, &readfds))
// 		{
// 			if ((new_socket = accept(_sockfd, (struct sockaddr *)&_server_addr, &addrlen)) == -1)
// 			{
// 				std::cerr << "Error: " << _name << " init_server() accept: " << strerror(errno) << std::endl;
// 				exit(1);
// 			}
// 			usleep(2000);
// 			ft_memset(buf, 0, 1024);
// 			for (int i = 0; i < max_clients; i++)
// 			{
// 				//if position is empty
// 				if(client_socket[i] == 0)
// 				{
// 					client_socket[i] = new_socket;
// 					printf("Adding to list of sockets as %d\n" , i);
// 					break;
// 				}
// 			}
// 		}

// 		// struct sockaddr_in client_addr;
// 		socklen_t c_addrlen = sizeof(client_addr);
// 		for (int i = 0; i < max_clients; i++)
// 		{
// 			sd = client_socket[i];

// 			if (FD_ISSET(sd , &readfds))
// 			{
// 				int	valread;

// 				getsockname(sd, (struct sockaddr *)&client_addr, &c_addrlen);
// 				std::string client_ip = inet_ntoa(client_addr.sin_addr);
// 				// std::cout << "client ip is " << client_ip << std::endl;
// 				if (fcntl(sd, F_SETFL, O_NONBLOCK) == -1)
// 				{
// 					std::cerr << "Error: " << _name << " init_server() fcntl: " << strerror(errno) << std::endl;
// 					exit(1);
// 				}
// 				std::string req = "";
// 				int complen = 0;
// 				while ((valread = read(sd , buf, 1023)) != 0)
// 				{
// 					if (ft_strncmp(buf, "\x04", 1) == 0) // ctrl + d from telnet!
// 					{
// 						valread = 0; // to close client's socket.
// 						break;
// 					}
// 					if (valread > 0)
// 					{
// 						buf[valread] = '\0';
// 						req += buf;
// 					}
// 					complen = req.length();
// 					if ((complen > 3 && req.substr(complen - 4) == "\r\n\r\n")) // body max size 조건 추가헤야 한다....
// 						break ;
// 				}

// 				if (valread == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
// 					std::cout << "recv error" << std::endl;
// 				else if (valread == 0)
// 				{
// 					close( sd );
// 					client_socket[i] = 0;
// 				}
// 				else
// 				{
// 					std::cout << req << std::endl;
// 					Request request(client_ip);
// 					request.parse_request(req);

// 					Response	response(request);

// 					std::string response_msg = response.exec_method();

// 					std::cout << response_msg << std::endl;
// 					send(sd, response_msg.c_str(), response_msg.length(), 0);
// 					std::cout << "Server sent message" << std::endl;

// 					ft_memset(buf, 0, 1024);
// 				}
// 			}
// 		}
// 	}
// }
