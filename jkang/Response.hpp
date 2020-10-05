#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <ctime>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "Request.hpp"

class Response
{
	private:
			
	public:
			std::map<std::string, std::string>	_header;
			std::pair<int, std::string> 		_status;
			std::string							_start_line;
			std::map<int, std::string>			_status_table;
			std::map<std::string, std::string>	_cgi_header;
			std::string							_cgi_body;
			std::string							_body;
			Response();
			Response(Request request);
			Response& operator=(const Response &copy);
			Response(const Response &copy);
			virtual ~Response();

			void	init_status_table();
			void 	clear(void);

			

};

#endif
