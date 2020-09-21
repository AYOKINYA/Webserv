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
// # include "RequestMsg.hpp"
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
			struct sockaddr_in	_client_addr;

			std::vector<conf>		_conf;

			fd_set					*_rset;
			fd_set					*_wset;
			fd_set					*_cp_rset;
			fd_set					*_cp_wset;

			
	public:
			std::vector<Client*>	_clients;

			Server() : _fd(-1), _max_fd(-1), _port(-1) {};
			~Server() {};

			void init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset)
			{
				this->_rset = rset;
				this->_wset = wset;
				this->_cp_rset = cp_rset;
				this->_cp_wset = cp_wset;

				std::cout << _conf[0]["server|"]["listen"] << std::endl;
				_port = std::stoi(_conf[0]["server|"]["listen"]);
				//std::stoi exception 처리

				std::cout << _conf[0]["server|"]["server_name"] << std::endl;
				std::cout << _conf[0]["server|"]["error_page"] << std::endl;
				std::cout << _conf[0]["server|"]["index"] << std::endl;

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

			int get_max_fd(void)
			{
				return (_max_fd);
			}

			int get_fd(void)
			{
				return (_fd);
			}

			void	accept_client(void)
			{
				int	new_socket;
				socklen_t addrlen = sizeof(_server_addr);

				if ((new_socket = accept(_fd, (struct sockaddr *)&_server_addr, &addrlen)) == -1)
					exit(1);
					//throw 로 에러 처리하기
				usleep(2000);
				if (new_socket > _max_fd)
					_max_fd = new_socket;

				Client *client = new Client(new_socket, _rset, _wset);
				_clients.push_back(client); //clients_fd에 넣음
				std::cout << "Here comes a new client at " << _conf[0]["server|"]["listen"] << std::endl;
			}

			int read_request(std::vector<Client*>::iterator it)
			{
				std::string req = "";
				int complen = 0;
				int valread;
				char buf[BUFFER_SIZE + 1];
				struct sockaddr_in client_addr;
				socklen_t c_addrlen = sizeof(client_addr);
				Client *c;

				c = *it;
				getsockname(c->get_fd(), (struct sockaddr *)&client_addr, &c_addrlen);
				std::string client_ip = inet_ntoa(client_addr.sin_addr);
				valread = read(c->get_fd() , buf, BUFFER_SIZE);
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
						req += buf;
					}
					complen = req.length();
					if ((complen > 3 && req.substr(complen - 4) == "\r\n\r\n")) // body max size 조건 추가헤야 한다....
						return 0;
				}
				else if (valread == -1)
				{
					std::cout << "recv error" << std::endl;
					return 0;
				}
				else if (valread == 0)
				{
					close(c->get_fd());
					_clients.erase(it);
					delete (c);
				}
				std::cout << buf << std::endl;
				// Request request(client_ip);
				// request.parse_request(req);
				// set_request(*c, request);
				return (1);
			}


};

#endif