#include "RequestMsg.hpp"

Request::Request(){_error_code = 0; _body = "";}
Request::~Request(){}
Request::Request(Request const &other){*this = other;}
Request	&Request::operator=(Request const &other){(void)other; return(*this);}

void Request::header_check(void)
{
	std::string header[10] = {
		"Accept-Charsets", "Accept-Language", "Authorization",
		"Host", "Location", "Referer", "Retry-After",
		"Transfer-Encoding", "User-Agent", "WWW-Authenticate"
	};
	std::map<std::string, std::string>::iterator it;
	it = vars_request.begin();
	for(; it != vars_request.end(); it++)
		for(int i = 0; i < 10; i++)
			if (ft_strncmp(it->first.c_str(), header[i].c_str(), ft_strlen(header[i].c_str())))
				_error_code = 400;
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
	std::cout << _chunkbody << std::endl;
}

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
			header_check();
			std::cout << "key is " << key << std::endl;
			std::cout << "value is " << value << std::endl;
			if (key.empty())
				break ;
		}
		else
			break;
	}
	int	len;
	if (ft_strncmp(vars_request.find("Transfer-Encoding")->second.c_str(), "chunked", 7))
	{
		if (vars_request.find("Content-Length") != vars_request.end())
		{
			len = std::stoi(vars_request.find("Content-Length")->second);
			_body = req.substr(0, len);
		}
		else if (_method == POST && _method == PUT)
			_error_code = 411;
	}
	else
		parse_chunk(req);
}

void	Request::parse_first_line(std::string line)
{
	std::vector<std::string> tokens = split(line, ' ');
	if (tokens.size() != 3)
		return ;
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
	std::string	root = "/Users/hpark/Webserv/res_tmp/src";


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