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
			//int 								_status_code;
			//std::string 						_status_msg;
			std::map<std::string, std::string>	vars;
	public:
			Response();
			Response(std::map<std::string, std::string> vars);
			Response& operator=(const Response &copy);
			Response(const Response &copy);
			virtual ~Response();
			
			void getStartLine(void);
			void getDate(void);
			void getContentType(const std::string &content);
			void getContentLocation(const std::string &loc);
			void getContentLanguage();
			void getContentLength(const std::string &content);
			void getAllow();
			void getLastModified(const std::string &content);
			void getServer();
			void getTransferEncoding();
			void printItem(const std::string &key);
			void header();
			void body();
};

#endif