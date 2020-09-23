#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Config.hpp"
# include "Utils.hpp"
# include <fstream>
#include <sstream>
#include <string>

enum	method
{
	GET, // 0
	HEAD, // 1
	POST, // 2
	PUT, // 3
	DELETE, // 4
	CONNECT, // 5
	OPTIONS, // 6
	TRACE // 7
};

class Request
{
	private:
		typedef std::map<std::string, std::string> 	elem;
		typedef std::map<std::string, elem>			conf;
		std::map<std::string, std::string>			_conf;
		std::map<std::string, std::string>			_headers;

		int				_method;
		std::string		_uri;
		std::string		_body;
		int				_error_code;
		std::string 	_client_ip;
		std::string		_method_str;

	public:
		void	clear();
		Request () {};
		Request(std::string client_ip);
		Request (const Request& copy);
		Request& operator=(const Request& other);
		~Request() {};
		void	feed_conf(std::vector<conf> &conf_input);
		void	parse_chunk(std::string &body);
		void	parse_request(std::string &req, std::vector<conf> &conf);
		void	parse_header(std::string &req);
		void	parse_first_line(std::string &line);

		int			get_method();
		std::string	get_method_str();
		int			get_error_code();
		std::string	get_body();
		std::string	get_uri();
		std::string	get_client_ip();
		void		set_error_code(int error_code);
		std::map<std::string, std::string>	get_conf();
		std::map<std::string, std::string>	get_headers();
};

#endif
