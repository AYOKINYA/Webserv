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

class Response
{
	private:
			std::pair<int, std::string> 		_status;
			std::string							_start_line;
			std::map<std::string, std::string>	_vars_response;
	public:
			Response();
			Response(std::map<std::string, std::string> _vars_response);
			Response& operator=(const Response &copy);
			Response(const Response &copy);
			virtual ~Response();
			
			void setStatus(std::pair<int, std::string> input);
			
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

			std::string printItem(const std::string &key);
			std::string getStartLine(void);
			std::string header(const std::string &path);
			std::string body(const std::string &path);
};

#endif