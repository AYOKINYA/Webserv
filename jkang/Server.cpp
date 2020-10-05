#include "Server.hpp"

Server::Server() : _fd(-1), _max_fd(-1), _port(-1), _tmp("")
{};

Server::~Server()
{
	if (_port != -1)
		std::cout << "[Server listening to " << _port << "] closes." << std::endl;
}

int		Server::get_max_fd()
{
	Client	*client;

	for (std::vector<Client*>::iterator it(_clients.begin()); it != _clients.end(); ++it)
	{
		client = *it;
		if (client->read_fd > _max_fd)
			_max_fd = client->read_fd;
		if (client->write_fd > _max_fd)
			_max_fd = client->write_fd;
	}
	return (_max_fd);
}


void Server::init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset)
{
	this->_rset = rset;
	this->_wset = wset;
	this->_cp_rset = cp_rset;
	this->_cp_wset = cp_wset;

	_port = std::stoi(_conf[0]["server|"]["listen"]);

	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw ServerException("socket()", std::string(strerror(errno)));

	int opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
		throw ServerException("setsockopt()", std::string(strerror(errno)));

	ft_memset((void *)&_server_addr, 0, (unsigned long)sizeof(_server_addr));
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = ft_htons(_port);

	if (bind(_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1)
		throw ServerException("bind()", std::string(strerror(errno)));
	if (listen(_fd, 256) == -1)
		throw ServerException("listen()", std::string(strerror(errno)));
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ServerException("fcntl()", std::string(strerror(errno)));
	FD_SET(_fd, _rset);
	_max_fd = _fd;

	std::cout << "[Server listening to " << _port << "] starts running." << std::endl;

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

	ft_memset(&client_addr, 0, sizeof(client_addr)); 

	if ((new_socket = accept(_fd, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		throw ServerException("accept()", std::string(strerror(errno)));
	
	if (new_socket > _max_fd)
		_max_fd = new_socket;

	getsockname(new_socket, (struct sockaddr *)&client_addr, &addrlen);
	std::string client_ip = ft_inet_ntoa(client_addr.sin_addr.s_addr);
	Client *client = new Client(new_socket, _rset, _wset, client_ip);
	_clients.push_back(client);
	
	std::cout << "Here comes a new client at " << _conf[0]["server|"]["listen"] << " count : " << _clients.size() << std::endl;
}

int Server::read_request(std::vector<Client*>::iterator it)
{
	int valread;
	char buf[BUFFER_SIZE + 1];

	Client *c;
	c = *it;

	if ((valread = read(c->get_fd(), buf, BUFFER_SIZE)) > 0)
	{
		c->_time = get_time();

		buf[valread] = '\0';
		c->_rbuf += buf;
		if (c->_req.parse_request(c->_rbuf, _conf))
		{
			FD_SET(c->_fd, _wset);
			return (1);
		}
	}
	else
	{	
		disconnect_client(it);
		return (0);
	}

	return (0);
}

int	Server::write_response(std::vector<Client *>::iterator it)
{
	Client *c;
	int	ret;

	c = *it;

	if (c->_status != Client::RESPONSE)
	{	
		_handler.exec_method(*c);
		c->_time = get_time();
	}
	else if (c->_status == Client::RESPONSE)
	{
		ret = write(c->get_fd(), c->_res_msg.c_str(), c->_res_msg.length());

		if (ret <= 0)
		{
			disconnect_client(it);
			return (0);
		}

		if ((unsigned long)ret < c->_res_msg.length())
			c->_res_msg = c->_res_msg.substr(ret);
		else
		{
			c->_req.clear();
			c->_status = Client::START;
			c->autoidx_flag = 0;
			c->_res_msg.clear();
			FD_CLR(c->get_fd(), _wset);
		}
		c->_time = get_time();
	}
	
	return (1);
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

int		Server::get_time_diff(long start_sec)
{
	struct timeval	time;

	gettimeofday(&time, NULL);

	return (time.tv_sec - start_sec);
}

int		Server::disconnect_client(std::vector<Client*>::iterator it)
{
	Client *c;
	c = *it;
	*it = NULL;
	_clients.erase(it);
	if (c)
		delete (c);
	return (1);
}
