#include "Server.hpp"

void Server::init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset)
{
	this->_rset = rset;
	this->_wset = wset;
	this->_cp_rset = cp_rset;
	this->_cp_wset = cp_wset;

	std::cout << _conf[0]["server|"]["listen"] << std::endl;
	_port = std::stoi(_conf[0]["server|"]["listen"]);
	//std::stoi exception 처리

	int opt = 1;

	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw ServerException("socket()", std::string(strerror(errno)));

	if( setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
		throw ServerException("setsockopt()", std::string(strerror(errno)));

	ft_memset((void *)&_server_addr, 0, (unsigned long)sizeof(_server_addr));
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons(_port);

	// Forcefully attaching socket to the port 8080
	if (bind(_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1)
		throw ServerException("bind()", std::string(strerror(errno)));
	if (listen(_fd, 256) == -1)
		throw ServerException("listen()", std::string(strerror(errno)));
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ServerException("fcntl()", std::string(strerror(errno)));
	FD_SET(_fd, _rset);
	_max_fd = _fd;
}

int Server::get_max_fd(void)
{
	return (_max_fd);
}

int Server::get_fd(void)
{
	return (_fd);
}

void	Server::accept_client(void)
{
	int	new_socket;
	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);
	
	ft_memset((void *)&client_addr, 0, (unsigned long)sizeof(client_addr)); // 왜 libft ft_memset link가 안 될까?

	if ((new_socket = accept(_fd, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		throw ServerException("accept()", std::string(strerror(errno)));
		//throw 로 에러 처리하기
	if (new_socket > _max_fd)
		_max_fd = new_socket;
	getsockname(new_socket, (struct sockaddr *)&client_addr, &addrlen);
	std::string client_ip = inet_ntoa(client_addr.sin_addr);
	Client *client = new Client(new_socket, _rset, _wset, client_ip);
	_clients.push_back(client); //clients_fd에 넣음
	std::cout << "Here comes a new client at " << _conf[0]["server|"]["listen"] << std::endl;
}

int Server::read_request(std::vector<Client*>::iterator it)
{
	int valread;
	char buf[BUFFER_SIZE + 1];

	Client *c;
	c = *it;

	if ((valread = read(c->get_fd(), buf, BUFFER_SIZE)) > 0)
	{
		buf[valread] = '\0';
		c->_tmp += buf;
		if (c->_req.parse_request(c->_tmp, _conf))
		{
			FD_SET(c->_fd, _wset);
			return (1);
		}
	}
	if (valread == 0)
	{
		*it = NULL;
		_clients.erase(it);
		if (c)
			delete (c);
		return (0);
	}
	return (0);
}

void Server::set_request(Client &c, Request &request)
{
	c._req = request;
}

int	Server::write_response(std::vector<Client *>::iterator it)
{
	Client *c;
	int	ret;

	c = *it;
	if (c->_status == 0)
	{
		Response	response(c->_req);
		c->_res_msg = response.exec_method();
		c->_status = 1;
	}

	ret = write(c->get_fd(), c->_res_msg.c_str(), c->_res_msg.length());
	if ((unsigned long)ret < c->_res_msg.length())
		c->_res_msg = c->_res_msg.substr(ret);
	else
	{
		c->_req.clear();
		c->_status = 0;
		c->_res_msg.clear();
		FD_CLR(c->get_fd(), _wset);
	}
	return (0);
}

Server::ServerException::ServerException()
{
	this->error = "Undefined Server Exception";
}

Server::ServerException::ServerException(std::string function, std::string error)
{
	this->error = function + ": " + error;
}

Server::ServerException::~ServerException() throw(){}

const char	*Server::ServerException::what() const throw()
{
	return (this->error.c_str());
}