#ifndef SERVER_HPP
# define SERVER_HPP

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
//# include "Client.hpp"
# include <vector>
# include "RequestMsg.hpp"
# include "Response.hpp"
# include <unistd.h>

class Server
{
	private:
			std::string			_name;
			int					_port;
			int					_sockfd;
			struct sockaddr_in	_server_addr;
			struct sockaddr_in	client_addr;
			std::string			_msg;

			//Request				request;
			//std::vector<Client>		clients_;
			Server() {};
	public:
			Server(const std::string &name, int port);
			Server(const Server &copy);
			Server& operator=(const Server &server);
			~Server();
			int	getSockfd(void);
			void init_server(void);

};

#endif
