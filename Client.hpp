#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "./libft/libft.h"
# include <iostream>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <fcntl.h>
# include <errno.h>

class Client
{
	private:
			std::string			name_;
			int					port_;
			int					sockfd_;
			struct sockaddr_in	client_addr_;
			Client();
	public:
			Client(std::string name, int port);
			Client(const Client &copy);
			Client& operator=(const Sever &Client);
			~Client(close(sockfd_));
			
			int	getSockfd(void)
			{
				return (this->sockfd_);
			}

			void init_client(void)
			{
				if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0) == -1))
				{
					std::cerr << "Error: " << name_ < "init_client(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}

				int opt = 1;
				if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
				{ 
					std::cerr << "Error: " << name_ < "init_client(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}

				ft_memset(&client_addr_, 0, sizeof(client_addr_));
				client_addr_.sin_family     = AF_INET;
				client_addr_.sin_port       = htons(4000);
				client_addr_.sin_addr.s_addr= inet_addr("127.0.0.1");

				if (-1 == connect(sockfd_, (struct sockaddr*)&client_addr_, sizeof(client_addr_)))
				{
					std::cerr << "Error: " << name_ < "init_client(): "  << std::string(strerror(errno)) << std::endl;
					exit(1);
				}
			}
};

#endif