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
#include "RequestMsg.hpp"
#include "util.cpp"
class Response
{
	private:
			Request *req;
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
			std::string getStartLine(void);
			void getDate(void);
			void getContentType(const std::string &content);
			void getContentLocation(const std::string &loc);
			void getContentLanguage();
			void getContentLength(const std::string &content);
			void getAllow();
			void getLastModified(const std::string &content);
			void getServer();
			void getTransferEncoding();
			void getWWWAuthentication();
			void method_put_exec();
			void method_put_msg();
			std::string printItem(const std::string &key);
			std::string header(const std::string &path);
			std::string body(const std::string &path);
};

#endif
