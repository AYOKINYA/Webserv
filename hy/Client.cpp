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

int	Client::write_response()
{
    // Response	response(_req);
	// std::string response_msg = response.exec_method();
	// // std::cout << response_msg << std::endl;
	// send(_fd, response_msg.c_str(), response_msg.length(), 0);
	// // std::cout << "Server sent message" << std::endl;
	// ft_memset(buf, 0, 1024);
}

int	get_fd()
{

}
