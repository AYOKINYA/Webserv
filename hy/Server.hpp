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
# include "Client.hpp"
# include <vector>
# include "RequestMsg.hpp"
# include "Response.hpp"
# include <unistd.h>
# define BUFFER_SIZE 32768
class Server
{
	private:
			std::string			_name;
			int					_port;
			struct sockaddr_in	_server_addr;
			struct sockaddr_in	_client_addr;

			std::string			_msg;
			std::string			_req;

			int					_fd;
			std::vector<Client *>	_clients;
			int					_max_fd;
			fd_set				*_rset;
			fd_set				*_wset;

			//Request				request;
			Server() {};
	public:
			Server(const std::string &name, int port);
			Server(const Server &copy);
			Server& operator=(const Server &server);
			~Server();
			int		get_server_fd(void);
			void	init_server(void); //정리필요

			void	accept_client(void); //만들어야됨 client받아서 _client에 넣는거
			int		read_request(Client c);
			int		write_response();
			int		get_max_fd(void); //이서버의 max_fd

};

#endif
