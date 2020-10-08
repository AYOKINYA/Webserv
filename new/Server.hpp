#ifndef SERVER_HPP
# define SERVER_HPP


# include "Config.hpp"
# include "Client.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Handler.hpp"


class Server
{
	friend class Config;
	typedef std::map<std::string, std::string> 	elem;
	typedef std::map<std::string, elem>			conf;

	private:
			int						_fd;
			int						_max_fd;
			int						_port;

			struct sockaddr_in		_server_addr;

			std::vector<conf>		_conf;

			fd_set					*_rset;
			fd_set					*_wset;
			fd_set					*_cp_rset;
			fd_set					*_cp_wset;

			std::string				_tmp;
			Handler _handler;

			
	public:
			std::vector<Client*>	_clients;
			Server();
			~Server();

			void	init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset);
			int		get_max_fd(void);
			int		get_fd(void);


			void	accept_client(void);
			int		read_request(std::vector<Client*>::iterator it);
			int		write_response(std::vector<Client *>::iterator it);
			int		get_time_diff(long start_sec);
			int		disconnect_client(std::vector<Client*>::iterator it);



			class	ServerException : public std::exception
			{
				private:
					std::string	function;
					std::string	error;
				public:
					ServerException();
					ServerException(std::string function, std::string error);
					virtual ~ServerException() throw();
					virtual const char	*what() const throw();
			};

};

#endif