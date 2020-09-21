#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "RequestMsg.hpp"
# include "Response.hpp"

class Client
{
    private:
        Request		_req;
        Response	_res;
		int	_fd;
		fd_set	*_rset; //원본
		fd_set	*_wset; //원본
		Client();
    public:
        Client(int fd, fd_set *rset, fd_set *wset);
		Client(Client const &other);
		Client	&operator=(Client const &other);
        ~Client();
		int	read_request();
		int	write_response();
		int	get_fd();
};


#endif