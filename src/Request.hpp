#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <dirent.h>
# include "Config.hpp"
# include "Utils.hpp"

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

		int				_method;
		std::string		_uri;
		std::string		_body;
		int				_error_code;
		std::string 	_client_ip;
		std::string		_method_str;
		int				_chunk_len;
		int				_limit_body_size;

	public:
		std::map<std::string, std::string>			_headers;
		std::map<std::string, std::string>			_conf;
		void	clear();
		Request () {};
		Request(std::string client_ip);
		Request (const Request& copy);
		Request& operator=(const Request& other);
		~Request() {};
		void	feed_conf(std::vector<conf> &conf_input);
		int		parse_chunk(std::string &req);
		int		parse_request(std::string &req, std::vector<conf> &conf);
		void	parse_header(std::string &req);
		void	parse_first_line(std::string &line);
		int		parse_body(std::string &req);

		int			get_method();
		std::string	get_method_str();
		int			get_error_code();
		std::string	get_body();
		std::string	get_uri();
		std::string	get_client_ip();
		std::map<std::string, std::string>	get_conf();
		std::map<std::string, std::string>	get_headers();
		int			get_limit();
		void		set_error_code(int n);
};

#endif
