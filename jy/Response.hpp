#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <ctime>
# include <iostream>
# include <sys/time.h>
# include <algorithm>
# include <map>
# include <string>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "RequestMsg.hpp"

class Response
{
	private:
			Request								_request;
			std::map<std::string, std::string>	_vars_response;
			std::pair<int, std::string> 		_status;
			std::string							_start_line;
			std::map<int, std::string>			_status_table;
			std::map<std::string, std::string>	cgi_header;
			std::string _cgi_body;
			Response();
	public:
			Response(Request request);
			Response& operator=(const Response &copy);
			Response(const Response &copy);
			virtual ~Response();

			void init_status_table();
			void set_vars_response();
			void setStatus(int num);
			void setDate(void);
			void setContentType(const std::string &content);
			void setContentLocation(const std::string &loc);
			void setContentLanguage();
			void setContentLength(const std::string &content);
			void setAllow();
			void setLastModified(const std::string &content);
			void setServer();
			void setTransferEncoding();
			void setWWWAuthentication();
			void		parseCGIResult(std::string buf);

			std::string printItem(const std::string &key);
			std::string printItem2(std::map<std::string, std::string> param, const std::string &key);
			std::string getStartLine(void);
			std::string header(const std::string &path);
			std::string body(const std::string &path);
			std::string exec_method(void);

			std::string Get(void);
			std::string Head(void);
			std::string Post();
			std::string Put();
			std::string	Delete();
			std::string	Options();
			std::string	cgi(void);
			char	**Env();
};

void	ft_getline(std::string &b, std::string &line);
std::vector<std::string> split(const std::string& str, char delim);
std::string trim(const std::string& str);
std::string trim_url(const std::string& str);
std::string trim_url_2(const std::string& str);
std::string trim_extension(const std::string& str);

#endif
