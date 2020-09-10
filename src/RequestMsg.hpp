#ifndef REQUESTMSG_HPP
# define REQUESTMSG_HPP

#include <iostream>
#include "libft/libft.h"
#include <fcntl.h>
#include <algorithm>
#include <map>
#include <sys/stat.h>
#include <vector>

class   Request
{
	private:
		std::string	_method;
		std::string	_path;
		std::string	_http_ver;
		std::map<std::string, std::string>	vars_request;
		std::string	_body;
	public:
		Request();
		~Request();
		Request(Request const &other);
		Request	&operator=(Request const &other);
		void	parse_request(std::string req);
		void	parse_first_line();
		void	parse_file();

};



#endif