#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include "libft/libft.h"
#include <fcntl.h>
#include <algorithm>
#include <map>
#include <sys/stat.h>
#include <vector>


// Setup the configuration file as follow:
// - / must answer to GET request ONLY
// - /put_test/* must answer to PUT request and save files to a directory of your choice
// - any file with .bla as extension must answer to POST request by calling the cgi_test executable
// - /post_body must answer anything to POST request with a maxBody of 100
// - /directory/ must answer to GET request and the root of it would be the repository YoupiBanane and if no file are requested, it should search for youpi.bad_extension files

class Message
{
	private:
		int	method;
		std::string	_path;
		std::map<std::string, std::string>vars_request;
		std::map<std::string, std::string>vars_response;
		std::string _body;
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
		void	parse_header_body(std::string req);
		void	parse_file();
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

void	Message::parse_header_body(std::string req)
{
	std::string line;
	std::size_t pos;
	std::string key;
	std::string value;
	//근데 바디에 : 가 있는 경우 장담못함
	ft_getline(req, line);
	while (!req.empty())
	{
		ft_getline(req, line);
		pos = line.find(":");
		//npos 찾는 문자열이 없을 경우니까 이중부정 고로 찾았을때 들어감
		if (pos != std::string::npos)
		{
			key = trim(line.substr(0, pos));
			value = trim(line.substr(pos + 1));
			vars_request.insert(std::pair<std::string, std::string>(key, value));
			std::cout << "key is " << key << std::endl;
			std::cout << "value is " << value << std::endl;
			if (key.empty())
				break ;
		}
		else
			break;
	}
	_body = req;
}

void	Message::parse_file()
{
	struct stat	info;
	std::string	root = "/Users/hpark/Webserv/src";

	if (_uri[0] == '/')
		_path = root + _uri;
	else
		_path = root + "/" + "_uri";

	if (stat(_path.c_str(), &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
		{
			if (_path[_path.size() - 1] == '/')
				_path += "index.html";
			else
				_path += "/index.html";
		}
	}
	else
	{
		//400? //404? 유효하지 않은 주소
	}
}

void	Message::receiveRequest(std::string req)
{
	std::cout <<"====Client Request====" << std::endl;
	std::cout << req << std::endl; // read message from client
	std::cout << "======================" << std::endl;

	parse_first_line(req);
	parse_header_body(req);
	parse_file();

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
