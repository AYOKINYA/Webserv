#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include "libft/libft.h"
#include <fcntl.h>
#include <algorithm>
#include <map>
class Message
{
	private:
		int	method;
		std::string	path;
		std::map<char, char> vars;
	public:
		Message();
		~Message();
		//Message(const Message &copy);
		//Message&	operator=(const Message &message);
		void	receiveRequest(char *buf);
		void	sendRespond(int sd);
};

Message::Message(){}
Message::~Message(){}
//Message::Message(const Message &copy){}
//Message&	Message::operator=(const Message &message){}

void	Message::receiveRequest(char *buf)
{
	std::cout <<"====Client Request====" << std::endl;
	std::cout << buf << std::endl; // read message from client
	std::cout << "======================" << std::endl;
	if (ft_strncmp(buf, "GET", 3) == 0)
		this->method = 0;
	else if (ft_strncmp(buf, "POST", 4) == 0)
		this->method = 1;
	else if (ft_strncmp(buf, "HEAD", 4) == 0)
		this->method = 2;
	else
		this->method = 9;
}

void	Message::sendRespond(int sd)
{
	std::string	msg_ = "";
	std::cout << "!!!!!!!!!!" <<method << std::endl;
	if (method == 0)
	{
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
	}
	else if (method == 1)
	{
		std::cout << "hhhhhgjsakfhajsdhfksjd" << std::endl;
		msg_ ="HTTP/1.1 405 Not Allowed\n";
		msg_ += "Content-Type: text/html\n";
		msg_ += "Content-Length: ";

		int fd = open("error.html", O_RDWR, 0644);
		char rbuf[1024];
		int nread = read(fd, rbuf, 1023);
		rbuf[nread] = '\0';
		char *tmp = ft_itoa(ft_strlen(rbuf));
		msg_ += tmp;
		free(tmp);
		msg_ += "\n\n";
		msg_ += rbuf;
	}
	else if (method == 2)
	{
		msg_ = "HTTP/1.1 405 Method Not Allowed\n"; //head - 405
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
	}
	else
	{
		//msg_ = "HTTP/1.1 405 Method Not Allowed\n\n";
		msg_ = "HTTP/1.1 404 Not Found\n\n";
		msg_ += "Content-Type: text/html\n";
		msg_ += "Content-Length: ";

		int fd = open("error.html", O_RDWR, 0644);
		char rbuf[1024];
		int nread = read(fd, rbuf, 1023);
		rbuf[nread] = '\0';
		char *tmp = ft_itoa(ft_strlen(rbuf));
		msg_ += tmp;
		free(tmp);
		msg_ += "\n\n";
		msg_ += rbuf;
	}
	write(sd, msg_.c_str(), msg_.length());
}


#endif
