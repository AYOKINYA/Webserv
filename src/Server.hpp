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

class Server
{
	private:
			std::string			name_;
			int					port_;
			int					sockfd_;
			struct sockaddr_in	server_addr_;
			std::string			msg_;
			//std::vector<Client>		clients_;
			Server() {};
	public:
			Server(const std::string &name, int port)
			{
				name_ = name;
				port_ = port;
			}
			Server(const Server &copy)
			{
				*this = copy;
			}
			Server& operator=(const Server &server)
			{
				if (this == &server)
					return (*this);
				name_ = server.name_;
				port_ = server.port_;
				sockfd_ = server.sockfd_;
				server_addr_ = server.server_addr_;
				return (*this);
			}
			~Server()
			{
				std::cout << "server is dead" << std::endl;
			};

			int	getSockfd(void)
			{
				return (this->sockfd_);
			}

			void init_server(void)
			{
				if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
				{ 
					std::cerr << "Error: " << name_ << " init_server() socket: " << strerror(errno) << std::endl;
					exit(1); 
				} 
				
				int opt = 1;
				// Forcefully attaching socket to the port 8080
				if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
															&opt, sizeof(opt)) == -1) 
				{ 
					std::cerr << "Error: " << name_ << " init_server() setsockopt: " << strerror(errno) << std::endl;
					exit(1); 
				}

				ft_memset((void *)&server_addr_, 0, (unsigned long)sizeof(server_addr_)); // 왜 libft ft_memset link가 안 될까?
				server_addr_.sin_family = AF_INET; 
				server_addr_.sin_addr.s_addr = INADDR_ANY; 
				server_addr_.sin_port = htons( this->port_ ); 
				
				// Forcefully attaching socket to the port 8080 
				if (bind(sockfd_, (struct sockaddr *)&server_addr_, sizeof(server_addr_)) == -1) 
				{ 
					std::cerr << "Error: " << name_ << " init_server() bind: " << strerror(errno) << std::endl;
					exit(1);
				}

				if (listen(sockfd_, 10) == -1) 
				{ 
					std::cerr << "Error: " << name_ << " init_server() listen: " << strerror(errno) << std::endl;
					exit(1); 
				}

				msg_ = "HTTP/1.1 200 OK\n";
				msg_ += "Content-Type: text/html\n";
				msg_ += "Content-Length: ";

				int fd = open("index.html", O_RDWR, 0644);
				char rbuf[1024];
				int nread = read(fd, rbuf, 1023);
				rbuf[nread] = '\0';
				char *tmp = ft_itoa(ft_strlen(rbuf));
				msg_ += tmp;
				free(tmp);
				msg_ += "\n\n";
				msg_ += rbuf;

				char buf[3001];
				int new_socket;
				socklen_t addrlen = sizeof(server_addr_);
				while (1)
				{

					if ((new_socket = accept(sockfd_, (struct sockaddr *)&server_addr_, &addrlen)) == -1) 
					{ 
						std::cerr << "Error: " << name_ << " init_server() accept: " << strerror(errno) << std::endl;
						exit(1);
					}
					
					ft_memset(buf, 0, 3001);
					read(new_socket, buf, 3000);
					std::cout << "====Client Request====" << std::endl;
					std::cout << buf << std::endl; // read message from client
					std::cout << "======================" << std::endl;
					if (ft_strncmp(buf, "GET", 3) == 0)
						write(new_socket, msg_.c_str(), msg_.length());
					else if (ft_strncmp(buf, "POST", 3) == 0)
					{
						ft_memset((void *)msg_.c_str(), 0, sizeof(msg_.c_str()));
						msg_ = "HTTP/1.1 405 Not Allowed\n\n";
						write(new_socket, msg_.c_str(), msg_.length());
					}
					else
						write(new_socket, msg_.c_str(), msg_.length());

					std::cout << "Server sent message" << std::endl;
					close(new_socket);	
				}	
			}

};

#endif