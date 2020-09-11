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

enum	method
{
	GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS
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

	public:
		Request();
		~Request();
		Request(Request const &other);
		Request	&operator=(Request const &other);
		void		parse_request(std::string req);
		void		parse_first_line(std::string line);
		void		parse_file(std::string _uri);
		int			get_method();
		int			get_error_code();
		void		parse_chunk(std::string body);
		std::string	get_path();
		std::string	get_body();
		std::string	get_chunkbody();
		std::map<std::string, std::string>	get_vars();

};

void						ft_getline(std::string &b, std::string &line);
std::vector<std::string>	split(const std::string& str, char delim);
std::string					trim(const std::string& str);


#endif
