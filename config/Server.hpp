#ifndef SERVER_HPP
# define SERVER_HPP
# include "Config.hpp"

class Server
{
	friend class Config;
	typedef std::map<std::string, std::string> 	elem;
	typedef std::map<std::string, elem>			conf;

	private:
			int						_fd;
			int						_max_fd;
			int						_port;

			std::vector<conf>		_conf;

			fd_set					*rset;
			fd_set					*wset;
			fd_set					*cp_rset;
			fd_set					*cp_wset;
	public:
			Server() : _fd(-1), _max_fd(-1), _port(-1) {};
			~Server() {};

			void init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset)
			{
				this->rset = rset;
				this->wset = wset;
				this->cp_rset = cp_rset;
				this->cp_wset = cp_wset;

				std::cout << _conf[0]["server|"]["listen"] << std::endl;
				std::cout << _conf[0]["server|"]["server_name"] << std::endl;
				std::cout << _conf[0]["server|"]["error_page"] << std::endl;
				std::cout << _conf[0]["server|"]["index"] << std::endl;
	
			}

};

#endif