#include "Server.hpp"
# in

std::vector<Server> g_servers;

int get_max_fd()
{
	int		max = 0;
	int		fd;

	for (std::vector<Server>::iterator it(g_servers.begin()); it != g_servers.end(); ++it)
	{
		fd = it->getMaxFd();
		if (fd > max)
			max = fd;
	}
	return (max);
}

int main()
{
	fd_set	rset;
	fd_set	wset;
	fd_set	cp_rset;
	fd_set	cp_wset;

	struct timeval time;
	time.tv_sec = 5;
	time.tv_usec = 5000;
	Server s("tmp", 8080);
	g_servers.push(s);
	for (std::vector<Server>::iterator s = g_servers.begin(); s != g_servers.end(); ++s)
	{
		s.init_server();
	}

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
			s->accept_client(&rset, &wset);
			for (std::vector<int>::iterator c(s->_clients_fd.begin()); c != s->_clients_fd.end(); ++c)
			{
				if (FD_ISSET(s->get_fd(), &cp_rset))
					if (!s->read_request(c))
						break ;
				if (FD_ISSET(s->get_fd(), &cp_wset))
					if (!s->write_response(c))
						break ;
			}
		}
		
	}

	return (0);
}
