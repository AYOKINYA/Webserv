#include "Server.hpp"

//g_server 선언 필요

int main()
{
	int	client;
	fd_set	rset;
	fd_set	wset;
	fd_set	cp_rset;
	fd_set	cp_wset;

	struct timeval time;
	time.tv_sec = 5;
	time.tv_usec = 5000;
	Server s("tmp", 8080);
	s.init_server();
	while (1)
	{
		cp_rset = rset;
		cp_wset = wset;
		if (select(get_max_fd(), &cp_rset, &cp_wset, NULL, &time) < 0))
		{
			printf("select error");
		}
		usleep(2000);
		for (std::vector<Server>::iterator s(g_servers.begin()); s != g_servers.end(); ++s)
		{
			s->accept_client();
			for (std::vector<Client*>::iterator c(s->_clients.begin()); c != s->_clients.end(); ++c)
			{
				if (FD_ISSET(c->get_fd(), &cp_rset))
					if (!c->read_request())
						break ;
				if (FD_ISSET(c->get_fd(), &cp_wset))
					if (!c->write_response())
						break ;
			}
		}
		
	}

	return (0);
}
