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
# include "Client.hpp"
# include <vector>

class Server
{
	private:
			std::string			name_;
			int					port_;
			int					sockfd_;
			struct sockaddr_in	server_addr_;
			vector<Client>		clients_;
			Server();
	public:
			Server(std::string name, int port);
			Server(const Server &copy);
			Server& operator=(const Sever &server);
			~Sever() {close(socketfd_);}

			void init_server(void)
			{
				
				if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0) == -1))
				{
					std::cerr << "Error: " << name_ < "init_server(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}
				
				int opt = 1;
				if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
				{ 
					std::cerr << "Error: " << name_ < "init_server(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}

				ft_memset(&server_addr_, 0, sizeof(server_addr_));
				server_addr_.sin_family      = AF_INET;
				server_addr_.sin_port        = htons(port_);
				server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);

				if (-1 == bind(sockfd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)))
				{
					std::cerr << "Error: " << name_ < "init_server(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}

				if (-1 == listen(sockfd_, 5))
				{
					std::cerr << "Error: " << name_ < "init_server(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}
			}

			void connect_client(void)
			{
				int new_sockfd;
				int addr_len = sizeof(server_addr_);

				if ((new_sockfd = accept(sockfd_, (struct sockaddr*)&server_addr_, &addr_len)) == -1)
				{
					std::cerr << "Error: " << name_ < "connect_client(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}

				Client *c = new Client(new_sockfd);
				clients_.push_back(c);
			}



};

#endif