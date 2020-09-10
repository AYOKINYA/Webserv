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

void	Request::receiveRequest(std::string req)
{
	std::cout <<"====Client Request====" << std::endl;
	std::cout << req << std::endl; // read message from client
	std::cout << "======================" << std::endl;

	Request r;
	r->parse_file();

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
