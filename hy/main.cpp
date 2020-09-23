#include "Config.hpp"
#include "Server.hpp"

std::vector<Server> g_servers;

int get_max_fd()
{
	int		max = 0;
	int		fd;

	for (std::vector<Server>::iterator it(g_servers.begin()); it != g_servers.end(); ++it)
	{
		fd = it->get_max_fd();
		if (fd > max)
			max = fd;
	}
	return (max);
}

int err_msg(const std::string &str)
{
    std::cerr << str << std::endl;
    return (1);
}

int main(int argc, char **argv)
{
    fd_set rset;
    fd_set wset;
    fd_set cp_rset;
    fd_set cp_wset;

	Client *client;

	struct timeval time;
	time.tv_sec = 1;
	time.tv_usec = 0;

    if (argc != 2)
        return (err_msg("./webserv ./any_config_file"));
    
    Config config(argv[1]);

    if (!config.parse())
        return (err_msg("Not a Valid Config."));
    config.init(&rset, &wset, &cp_rset, &cp_wset);
    
    while (1)
	{
		cp_rset = rset;
		cp_wset = wset;

		if (select(get_max_fd(), &cp_rset, &cp_wset, NULL, &time) < 0)
			printf("select error");
		
		// usleep(2000);

		for (std::vector<Server>::iterator s(g_servers.begin()); s != g_servers.end(); ++s)
		{
			if (FD_ISSET(s->get_fd(), &cp_rset))
				s->accept_client();
			for (std::vector<Client *>::iterator c(s->_clients.begin()); c != s->_clients.end(); ++c)
			{
				client = *c;
				if (FD_ISSET(client->get_fd(), &rset))
				{
					if (!s->read_request(c))
						break ;
				}
				if (FD_ISSET(client->get_fd(), &wset))
				{
					if (!s->write_response(c))
						break ;
				}
			}
		}

	}
    return (0);
}