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
# include "Message_parsing.hpp"
# include <unistd.h>

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
					if( setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
				{
					perror("setsockopt");
					exit(EXIT_FAILURE);
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
				if (fcntl(sockfd_, F_SETFL, O_NONBLOCK) == -1)
				{
					std::cerr << "Error: " << name_ << " init_server() fcntl: " << strerror(errno) << std::endl;
					exit(1);
				}



				char buf[3001];
				int new_socket;

				fd_set readfds;
				int	max_sd, client_socket[30], max_clients = 30, sd, activity;

				socklen_t addrlen = sizeof(server_addr_);
				for (int i = 0; i < max_clients; i++)
				{
					client_socket[i] = 0;
				}
				while (1)
				{
					FD_ZERO(&readfds);
					FD_SET(sockfd_, &readfds);
					max_sd = sockfd_;
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
					activity = select( max_sd + 1 , &readfds , NULL , NULL , &time);

					if ((activity < 0) && (errno!=EINTR))
					{
						printf("select error");
					}
					usleep(2000);
					if (FD_ISSET(sockfd_, &readfds))
					{
						if ((new_socket = accept(sockfd_, (struct sockaddr *)&server_addr_, &addrlen)) == -1)
						{
							std::cerr << "Error: " << name_ << " init_server() accept: " << strerror(errno) << std::endl;
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
							//Check if it was for closing , and also read the
							//incoming message
							int	valread;
							if ((valread = read( sd , buf, 1024)) == 0)
							{
								close( sd );
								client_socket[i] = 0;
							}

							//Echo back the message that came in
							else
							{
								if (fcntl(sd, F_SETFL, O_NONBLOCK) == -1)
								{
									std::cerr << "Error: " << name_ << " init_server() fcntl: " << strerror(errno) << std::endl;
									exit(1);
								}
								//while (valread = read)
								//set the string terminating NULL byte on the end
								//of the data read
								int n = valread;
								while ((valread = recv(sd , buf + n, 1024, 0)) > 0 && n < 1024)
								{
									n += valread;
									std::cout << "n is " << n << std::endl;
								}
								if (valread == -1 && errno == EAGAIN)
								{
									//error!!!안에 내용은 나중에 처리
									std::cout << "recv error" << std::endl;
								}
								Message m;

								m.receiveRequest(buf);
								m.sendRespond(sd);
								std::cout << "Server sent message" << std::endl;

								//buffer[valread] = '\0';
								//send(sd , buffer , strlen(buffer) , 0 );
							}
							ft_memset(buf, 0, 3001);
						}
					}


				}
			}

};

#endif
