#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Utils.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Client
{
	public:
	
		Request _req;
	
		int		_fd;
		fd_set *_rset;
		fd_set *_wset;

		int		_status;

		std::string	_rbuf;
		std::string	_res_msg;

		long _time;

		Client(int fd, fd_set *rset, fd_set *wset, std::string ip);
		~Client();
		Client(const Client &copy);
		Client& operator=(const Client &other);

		int get_fd(void);

};

#endif