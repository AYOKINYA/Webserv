#ifndef REQUESTMSG_HPP
# define REQUESTMSG_HPP

#include <iostream>
#include "libft/libft.h"
#include <fcntl.h>
#include <algorithm>
#include <map>
#include <sys/stat.h>
#include <vector>
#include <sstream>
#include <iomanip>

// #include "util.cpp"

enum	method
{
	GET, // 0
	HEAD, // 1
	POST, // 2
	PUT, // 3
	DELETE, // 4
	CONNECT, // 5
	OPTIONS // 6
};

class   Request
{
	private:
		int			_method;
		std::string	_path;
		std::string	_body;
		std::string _chunkbody;
		std::map<std::string, std::string>	vars_request;
		int			_error_code;
		int			_filecheck;
		int			_putcheck;
		std::string	_uri;
		std::string _client_ip;

	public:
		Request();
		Request(std::string client_ip);
		~Request();
		Request(Request const &other);
		Request	&operator=(Request const &other);
		void		parse_request(std::string req);
		void		parse_first_line(std::string line);
		void		parse_file(std::string _uri);
		int			get_putcheck();
		int			get_filecheck();
		int			get_method();
		int			get_error_code();
		void		parse_chunk(std::string body);
		std::string	get_path();
		std::string	get_body();
		std::string	get_chunkbody();
		void		header_check();
		std::map<std::string, std::string>	get_vars();
		std::string	get_uri();
		std::string	get_clientip();

};

void						ft_getline(std::string &b, std::string &line);
std::vector<std::string>	split(const std::string& str, char delim);
std::string					trim(const std::string& str);
std::string trim_url(const std::string& str);
std::string trim_url_2(const std::string& str);
std::string trim_extension(const std::string& str);


#endif
