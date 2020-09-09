#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include "libft/libft.h"
#include <fcntl.h>
#include <algorithm>
#include <map>
#include <vector>

class Message
{
	private:
		int	method;
		std::string	path;
		std::map<char, char> vars;
		std::string _method;
		std::string _uri;
		std::string _http_ver;
	public:
		Message();
		~Message();
		//Message(const Message &copy);
		//Message&	operator=(const Message &message);
		void	receiveRequest(std::string req);
		void	sendRespond(int sd);
		void	parse_first_line(std::string req);
		void	parse_header(std::string req);
};
Message::Message(){}
Message::~Message(){}
//Message::Message(const Message &copy){}
//Message&	Message::operator=(const Message &message){}
void	ft_getline(std::string &b, std::string &line)
{
    size_t					pos;

    pos = b.find("\n");

    if (pos != std::string::npos)
    {
        line = std::string (b, 0, pos++);
        b = b.substr(pos);
    }
    else
    {
        if (b[b.size() - 1] == '\n')
            b = b.substr(b.size());
        else
        {
            line = b;
            b = b.substr(b.size());
        }
    }
}

std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
	//find_first_not_of 전달된 문자들 중 첫번째로 일치하지 않는 위치를 찾는다.
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');

    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

void	Message::parse_first_line(std::string req)
{
	std::string line;
	ft_getline(req, line);
	std::vector<std::string> tokens = split(line, ' ');
	if (tokens.size() != 3)
		return ;
	else
	{
		_method = tokens[0];
		_uri = tokens[1];
		_http_ver = tokens[2];
	}
	std::cout << "firstline parsing1: " << _method << std::endl;
	std::cout << "firstline parsing2: " << _uri<< std::endl;
	std::cout << "firstline parsing3: " << _http_ver<< std::endl;

}

void	Message::parse_header(std::string req)
{
	std::string line;
	std::size_t pos;
	std::string key;
	std::string value;

	while (!req.empty())
	{
		ft_getline(req, line);
		pos = line.find(":");
		//npos 찾는 문자열이 없을 경우니까 이중부정 고로 찾았을때 들어감
		if (pos != std::string::npos)
		{
			key = trim(line.substr(0, pos));
			value = trim(line.substr(pos + 1));
			std::cout << "key is " << key << std::endl;
			std::cout << "value is " << value << std::endl;
			if (key.empty())
				break ;
		}
	}
}

void	Message::receiveRequest(std::string req)
{
	std::cout <<"====Client Request====" << std::endl;
	std::cout << req << std::endl; // read message from client
	std::cout << "======================" << std::endl;

	parse_first_line(req);
	parse_header(req);

	const char *c_req = req.c_str();
	if (ft_strncmp(c_req, "GET", 3) == 0)
		this->method = 0;
	else if (ft_strncmp(c_req, "POST", 4) == 0)
		this->method = 1;
	else if (ft_strncmp(c_req, "HEAD", 4) == 0)
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
