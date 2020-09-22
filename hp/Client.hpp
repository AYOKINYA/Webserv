#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Utils.hpp"
# include "Request.hpp"

class Client
{
	public:
	
		Request _req;
	
		int		_fd;
		fd_set *_rset;
		fd_set *_wset;

	
		Client(int fd, fd_set *rset, fd_set *wset)
		{
			_fd = fd;
			_rset = rset;
			_wset = wset;

			fcntl(fd, F_SETFL, O_NONBLOCK);
			FD_SET(fd, _rset);
			FD_SET(fd, _wset);
		}
		~Client()
		{
			if (_fd != -1)
			{
				close(_fd);
				FD_CLR(_fd, _rset);
				FD_CLR(_fd, _wset);
			}
			std::cout << "Bye bye Client~" << std::endl;
		};

		int get_fd(void)
		{
			return (_fd);
		}
};

#endif