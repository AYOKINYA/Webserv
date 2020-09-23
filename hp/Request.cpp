#include "Request.hpp"

void	Request::clear()
{
    _method = -1;
    _uri.clear();
    _headers.clear();
    _body.clear();
    _method_str.clear();
}

Request::Request(std::string client_ip)
{
    _client_ip = client_ip;
}

Request::Request (const Request& copy)
{
    *this = copy;
};

Request& Request::operator=(const Request& other)
{
    if (this == &other)
        return (*this);

    _method = other._method;
    _method_str = other._method_str;
    _uri = other._uri;

    _headers = other._headers;
    _body = other._body;

    _error_code = other._error_code;
    _client_ip = other._client_ip;

    _conf = other._conf;

    return (*this);
}

void	Request::feed_conf(std::vector<conf> &conf_input)
{
    std::map<std::string, std::string> elem;
    std::string		tmp;
    std::string 	file;
    struct stat		info;
    conf			to_parse;

    std::vector<conf>::iterator it = conf_input.begin();
    for(; it != conf_input.end(); ++it)
    {
        if (_headers["Host"] == (*it)["server|"]["server_name"])
        {
            to_parse = *it;
            break ;
        }
    }
    if (it == conf_input.end())
        to_parse = conf_input[0];
 if (_uri[0] != '/')
        _uri = "/" + _uri;
    file = _uri.substr(_uri.find_last_of('/') + 1, _uri.find('?'));
    tmp = _uri.substr(0, _uri.find('?'));
    do
    {
        std::cout << "tmp is " << tmp << std::endl;
        if (to_parse.find("server|location " + tmp + "|") != to_parse.end())
        {
            elem = to_parse["server|location " + tmp + "|"];
            break ;
        }
        tmp = tmp.substr(0, tmp.find_last_of('/'));

    } while (tmp != "");

    if (elem.size() == 0)
    {
        std::cout << "tmp is ???"<< std::endl;
        if (to_parse.find("server|location /|") != to_parse.end())
            elem = to_parse["server|location /|"];
    }
	_conf = elem;
	_conf["path"] = _uri.substr(0, _uri.find("?"));
	std::cout <<"path is " << _conf["path"] << std::endl;
	if (elem.find("root") != elem.end())
		_conf["path"].replace(0, tmp.size(), elem["root"]);

    for (std::map<std::string, std::string>::iterator it(to_parse["server|"].begin()); it != to_parse["server|"].end(); ++it)
    {
        if (_conf.find(it->first) == _conf.end())
            _conf[it->first] = it->second;
    }
    lstat(_conf["path"].c_str(), &info);
    if (S_ISDIR(info.st_mode)) //directory면 default index page open
    {
		if (_method != GET)
		{
			_error_code = 405;
			return ;
		}
        if (_conf["index"][0] && _conf["autoindex"] != "on")
            _conf["path"] += "/" + elem["index"];
    }
    if (_method == GET)
        _conf["path_saved"] = _conf["path"];

    // std::cout << "============"<< std::endl;
    // for(std::map<std::string, std::string>::iterator it = _conf.begin(); it != _conf.end(); ++it)
    // 	std::cout << it->first << " " << it->second << std::endl;
    // std::cout << "============"<< std::endl;

    if (stat(_conf["path"].c_str(), &info) == -1 && _method != PUT)
        _error_code = 404;
	std::cout << _conf["root"].c_str() << std::endl;
	std::cout << _conf["path"].c_str() << std::endl;
}

void	Request::parse_header(std::string &req)
{
    std::string line;
    std::size_t pos;
    std::string key;
    std::string value;

    while (!req.empty())
    {
        ft_getline(req, line);
        pos = line.find(":");
        if (pos != std::string::npos)
        {
            key = trim(line.substr(0, pos));
            value = trim(line.substr(pos + 1));

            // std::cout << key << "=" << value << std::endl;

            if (key.empty() || value.empty())
            {
                break ;
            }

            _headers[key] = value;
        }
        else
        {
            break;
        }
    }
}

void	Request::parse_request(std::string &req, std::vector<conf> &conf)
{
    std::string line;

    _error_code = 200;
    if (req[0] == '\r')
        req.erase(req.begin());
    if (req[0] == '\n')
        req.erase(req.begin());

    ft_getline(req, line);
    parse_first_line(line); // feed method and uri. version is fixed as HTTP /1.1
    parse_header(req);

    if (_uri != "*" || _uri != "OPTIONS")
        feed_conf(conf);

    //parse body
    if (_headers.find("Transfer-Encoding") != _headers.end() && ft_strncmp(_headers["Transfer-Encoding"].c_str(), "chunked", 7) == 0)
        parse_chunk(req);
    else
    {
        if (_headers.find("Content-Length") != _headers.end())
        {
            int len = std::stoi(_headers["Content-Length"]);
            _body = req.substr(0, len);
            std::cout << _body << std::endl;
        }
        else if (_method == POST || _method == PUT)
            _error_code = 411;
    }
    std::cout << "in parse request" << std::endl;
}

void Request::parse_chunk(std::string &body)
{
	std::string line;

    std::cout << "in parse chunck start" << std::endl;
	_body = "";
	while(!body.empty())
	{
		ft_getline(body, line);//숫자 읽기
		int len = std::stoi(line, 0, 16);
		// std::cout << "chunked number!!!" <<line << std::endl;
		ft_getline(body, line);//문자열 읽기
		_body += line.substr(0, len);
		// _body += line.substr(0, line.size() - 1);
	}
    std::cout << "in parse chunck end" << std::endl;
}

void	Request::parse_first_line(std::string &line)
{
    std::vector<std::string> tokens = split(line, ' ');

    _method_str = tokens[0];
    if (tokens.size() != 3)
        _error_code = 400;
    else
    {
        if (!ft_strncmp("GET", tokens[0].c_str(), 3))
            _method = GET;
        else if (!ft_strncmp("POST", tokens[0].c_str(), 4))
            _method = POST;
        else if (!ft_strncmp("HEAD", tokens[0].c_str(), 4))
            _method = HEAD;
        else if (!ft_strncmp("PUT", tokens[0].c_str(), 3))
            _method = PUT;
        else if (!ft_strncmp("DELETE", tokens[0].c_str(), 6))
            _method = DELETE;
        else if (!ft_strncmp("CONNECT", tokens[0].c_str(), 7))
            _method = CONNECT;
        else if (!ft_strncmp("OPTIONS", tokens[0].c_str(), 7))
            _method = OPTIONS;
        else if (!ft_strncmp("TRACE", tokens[0].c_str(), 7))
            _method = TRACE;
        else
            _error_code = 400;

        _uri = tokens[1];

        if (strncmp("HTTP/1.1", tokens[2].c_str(), 8))
            _error_code = 505;

    }
}


int Request::get_method(){return (_method);}
std::string Request::get_method_str(){return (_method_str);}
int	Request::get_error_code(){return (_error_code);}
std::string	Request::get_uri(){return (_uri);}
std::string	Request::get_body(){return (_body);}
std::string	Request::get_client_ip(){return (_client_ip);}
std::map<std::string, std::string>	Request::get_conf(){return (_conf);}
std::map<std::string, std::string>  Request::get_headers(){return (_headers);}
