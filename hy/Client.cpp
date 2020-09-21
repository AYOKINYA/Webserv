#include "Client.hpp"

Client::Client(int fd, fd_set *rset, fd_set *wset)
{
}

Client::Client(Client const &other)
{

}

Client::~Client()
{
    //wset, rset에서 해당 client의 fd 삭제하기
}

Client	&Client::operator=(Client const &other)
{

}

int Client::read_request()
{

}

int	Client::write_response()
{

}

int	get_fd()
{
    
}