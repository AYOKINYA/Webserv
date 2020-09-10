#include "RequestMsg.hpp"

Request::Request(){}
Request::~Request(){}
Request::Request(Request const &other){}
Request	&Request::operator=(Request const &other){}


void Request::parse_request(std::string req)
{
	std::string line;
	std::size_t pos;
	std::string key;
	std::string value;

	ft_getline(req, line);
    parse_first_line(line);
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

void	Request::parse_first_line(std::string line)
{
	std::vector<std::string> tokens = split(line, ' ');
	if (tokens.size() != 3)
		return ;
	else
	{
		_method = tokens[0];
		parse_file(tokens[1]);
		_http_ver = tokens[2];
	}
	// std::cout << "firstline parsing1: " << _method << std::endl;
	// std::cout << "firstline parsing2: " << _uri<< std::endl;
	// std::cout << "firstline parsing3: " << _http_ver<< std::endl;
}

void	Request::parse_file(std::string uri)
{
	struct stat	info;
	std::string	root = "/Users/hpark/Webserv/src";

	if (uri[0] == '/')
		_path = root + uri;
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

std::string Request::get_method(){return (_method);}

std::string	Request::get_path(){return (_path);}

std::string	Request::get_http_ver(){return (_http_ver);}

std::string	Request::get_body(){return (_body);}
