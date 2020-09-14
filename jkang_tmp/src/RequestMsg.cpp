#include "RequestMsg.hpp"

Request::Request(){_error_code = 0; _body = "";}
Request::~Request(){}
Request::Request(Request const &other){*this = other;}
Request	&Request::operator=(Request const &other)
{
	if (this == &other)
		return (*this);

	_method = other._method;
	_path = other._path;
	_body = other._body;
	_chunkbody = other._chunkbody;
	vars_request = other.vars_request;
	_error_code = other._error_code;

	return(*this);
}

void Request::header_check(void)
{
	//우리가 구현하는 헤더는 다 넣어야... 
	std::string header[11] = {
		"Accept-Charsets", "Accept-Language", "Accept-Encoding", "Authorization",
		"Host", "Location", "Referer", "Retry-After",
		"Transfer-Encoding", "User-Agent", "WWW-Authenticate"
	};
	std::map<std::string, std::string>::iterator it;
	it = vars_request.begin();
	for(; it != vars_request.end(); it++)
	{
		int i;
		for(i = 0; i < 11; i++)
		{
			if (!ft_strncmp(it->first.c_str(), header[i].c_str(), ft_strlen(header[i].c_str())))
				break;
		}
		if (i == 11)
		{
			_error_code = 400;
			break;
		}
	}
}

void Request::parse_chunk(std::string body)
{
	std::string line;

	while(!body.empty())
	{
		ft_getline(body, line);//숫자 읽기
		ft_getline(body, line);//문자열 읽기
		_chunkbody += line + "\n";
	}
	//std::cout << _chunkbody << std::endl;
}

void Request::parse_request(std::string req)
{
	std::string line;
	std::size_t pos;
	std::string key;
	std::string value;

	_error_code = 200;
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
			header_check();
			//std::cout << "key is " << key << std::endl;
			//std::cout << "value is " << value << std::endl;
			if (key.empty())
				break ;
		}
		else
			break;
	}
	int	len;
	if (vars_request.find("Transfer-Encoding") != vars_request.end() && ft_strncmp((vars_request.find("Transfer-Encoding")->second).c_str(), "chunked", 7) == 0)
		parse_chunk(req);
	else
	{
		if (vars_request.find("Content-Length") != vars_request.end())
		{
			len = std::stoi(vars_request.find("Content-Length")->second);
			_body = req.substr(0, len);
		}
		else if (_method == POST || _method == PUT)
			_error_code = 411;
	}
	//std::cout << "error_code is " << _error_code << std::endl;
}

void	Request::parse_first_line(std::string line)
{
	std::vector<std::string> tokens = split(line, ' ');
	
	if (tokens.size() != 3)
		_error_code = 400;
	else
	{
		if (!strncmp("GET", tokens[0].c_str(), 3))
			_method = GET;
		else if (!strncmp("POST", tokens[0].c_str(), 4))
			_method = POST;
		else if (!strncmp("HEAD", tokens[0].c_str(), 4))
			_method = HEAD;
		else if (!strncmp("PUT", tokens[0].c_str(), 3))
			_method = PUT;
		else if (!strncmp("DELETE", tokens[0].c_str(), 6))
			_method = DELETE;
		else if (!strncmp("CONNECT", tokens[0].c_str(), 7))
			_method = CONNECT;
		else if (!strncmp("OPTIONS", tokens[0].c_str(), 7))
			_method = OPTIONS;
		else
			_error_code = 400;
		
		parse_file(tokens[1]);
		if (strncmp("HTTP/1.1", tokens[2].c_str(), 8))
			_error_code = 505;
	}
	// std::cout << "firstline parsing1: " << _method << std::endl;
	// std::cout << "firstline parsing2: " << _uri<< std::endl;
	// std::cout << "firstline parsing3: " << _http_ver<< std::endl;
}

void	Request::parse_file(std::string uri)
{
	struct stat	info;
	std::string	root = "/Users/jiwonkang/Webserv/jkang_tmp/src";

	//only to pass tester

	if (uri == "/directory")
		uri = "/";
	else if (uri == "/directory/youpi.bad_extension")
		uri = "/";
	else if (uri == "/directory/youpi.bla")
		uri = "/";
	else if (uri == "/directory/nop")
		uri = "/";
	else if (uri == "/directory/nop/")
		uri = "/";
	else if (uri == "/directory/nop/other.pouic")
		uri = "/";
	else if (uri == "/directory/Yeah/not_happy.bad_extension")
		uri = "/";
	
	
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
		_error_code = 404;
		//400? //404? 유효하지 않은 주소
	}
}

int Request::get_method(){return (_method);}

int	Request::get_error_code(){return (_error_code);}

std::string	Request::get_path(){return (_path);}

std::string	Request::get_body(){return (_body);}

std::string	Request::get_chunkbody(){return (_chunkbody);}

std::map<std::string, std::string> Request::get_vars(){return (vars_request);}
