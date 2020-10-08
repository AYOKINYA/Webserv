#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Utils.hpp"
# include "Request.hpp"
# include "Response.hpp"
# define BUFFER_SIZE 32738


class Client
{
	public:
	
		Request _req;
		Response _res;
	
		int		_fd;
		int		write_fd;
		int		read_fd;
		int		cgi_pid;
		int		autoidx_flag;
		int		tmp_fd;
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
		void write_file(void);
		void read_file(void);
		void set_read_file(bool flag);
		void set_write_file(bool flag);

		enum status
		{
			START,
			BODY,
			HEADERS,
			CGI,
			RESPONSE,
			DONE
		};

};

#endif