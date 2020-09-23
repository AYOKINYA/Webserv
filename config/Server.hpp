#ifndef SERVER_HPP
# define SERVER_HPP
# define BUFFER_SIZE 32738
# include "Config.hpp"

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
# include "Client.hpp"
# include "Request.hpp"
// # include "Response.hpp"
# include <unistd.h>

class Server
{
	friend class Config;
	typedef std::map<std::string, std::string> 	elem;
	typedef std::map<std::string, elem>			conf;

	private:
			int						_fd;
			int						_max_fd;
			int						_port;

			struct sockaddr_in	_server_addr;

			std::vector<conf>		_conf;

			fd_set					*_rset;
			fd_set					*_wset;
			fd_set					*_cp_rset;
			fd_set					*_cp_wset;

			
	public:
			std::vector<Client*>	_clients;

			Server() : _fd(-1), _max_fd(-1), _port(-1) {};
			~Server() {};

			void init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset);
			int get_max_fd(void);
			int get_fd(void);
			void	accept_client(void);
			int read_request(std::vector<Client*>::iterator it);
			void set_request(Client &c, Request &request);


};

#endif