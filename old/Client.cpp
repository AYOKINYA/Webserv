#include "Client.hpp"

Client::Client(int fd, fd_set *rset, fd_set *wset, std::string ip) : _req(ip)
{
    _fd = fd;
    _rset = rset;
    _wset = wset;

    fcntl(fd, F_SETFL, O_NONBLOCK);
    FD_SET(fd, _rset);

    _rbuf = "";
    _time = get_time();
    _status = 0;
}

Client::~Client()
{
    if (_fd != -1)
    {
        close(_fd);
        FD_CLR(_fd, _rset);
        FD_CLR(_fd, _wset);
    }
	std::cout << "Bye bye Client~" << std::endl;
}

Client::Client(const Client &copy) : _req("")
{
    *this = copy;
}

Client& Client::operator=(const Client &other)
{
    if (this == &other)
        return (*this);

    _req = other._req;
    _fd = other._fd;
    _rset = other._rset;
    _wset = other._wset;
    _rbuf = other._rbuf;
    _res_msg = other._res_msg;
    _time = other._time;

    return (*this);
}

int Client::get_fd(void)
{
    return (_fd);
}