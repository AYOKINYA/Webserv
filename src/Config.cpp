#include "Config.hpp"
#include "Server.hpp"

extern std::vector<Server> g_servers;

Config::Config(char *conf) : _conf_path(conf)
{};

Config::Config(const Config& copy)
{
    *this = copy;
}

Config& Config::operator=(const Config& other)
{
    if (this == &other)
        return (*this);
    
    _conf_path = other._conf_path;
    _str = other._str;
    _conf = other._conf;

    return (*this);
}

Config::~Config()
{};

void Config::init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset)
{
    {
        FD_ZERO(rset);
        FD_ZERO(wset);
        FD_ZERO(cp_rset);
        FD_ZERO(cp_wset);
        for (std::vector<Server>::iterator it(g_servers.begin()); it != g_servers.end(); ++it)
            it->init(rset, wset, cp_rset, cp_wset);

    }
}

void	Config::readfile(char *path)
{
    int fd = open(path, O_RDWR, 0644);
    if (fd == -1)
		throw ConfigException("Config open()", std::string(strerror(errno)));
    char buf[1024];
    int ret = 0;
    while ((ret = read(fd, buf, 1023)) > 0)
    {
        buf[ret] = '\0';
        _str += buf;
    }

    if (ret == -1)
		throw ConfigException("Config read()", std::string(strerror(errno)));
    close(fd);
}

void	Config::set_location(std::string &line, std::string &context)
{
    std::string loc;
    int i = 0;

    while (ft_is_space(line[i]))
        ++i;
    i = line.find("location") + 8;
    while (ft_is_space(line[i]))
        ++i;
    while (!ft_is_space(line[i]) && line[i] != '\0')
        loc += line[i++];
    while (ft_is_space(line[i]))
        ++i;
    if (line[i] != '\0' && line[i] != '{')
		throw ConfigException();
    if (line[i] == '{')
    {
        ++i;
        while (ft_is_space(line[i]))
            ++i;
        if (line[i])
			throw ConfigException();
    }

    context = "server|location " + loc + "|";
}

void	Config::get_conf(std::string input, std::string &context)
{
    std::string line;
    std::string key;
    std::string value;
    size_t i, tmp;
    int loc_flag = 0;

    context = input.substr(0, input.length() - 1) + '|';

    while (ft_is_space(line[0]))
        line.erase(line.begin());
  
    while (line[0] != '}')
    {
        ft_getline(_str, line);

		while (ft_is_space(line[0]))
			line.erase(line.begin());
		
		if (line.find("location") != std::string::npos)
		{
			set_location(line, context);
			loc_flag = 1;
		}
		else
		{
			i = 0;
			while (line[i] != '\0' && line[i] != ';' && line[i] != '{' && line[i] != '}')
			{
				while (line[i] != '\0' && !ft_is_space(line[i]))
					key += line[i++];
				while (ft_is_space(line[i]))
					++i;
				while (line[i] != '\0' && line[i] != ';' && line[i] != '{')
					value += line[i++];
			}
			tmp = 0;
			if (line[i] != '\0')
				++tmp;
			while (ft_is_space(line[i + tmp]))
				++tmp;
			if (line[i + tmp])
				throw ConfigException();
			if (line[i] != '{' && line[i] != '}' && line[i] != '\0')
			{
				std::pair<std::string, std::string> item(key, value);
				_conf[context].insert(item);

				key.clear();
				value.clear();
			}
		}
		
		if (line[i] == '}')
		{
			if (loc_flag)
			{
				ft_getline(_str, line);
				loc_flag = 0;
			}
			if (line.find("location") != std::string::npos)
			{
				set_location(line, context);
				loc_flag = 1;
			}
		}
    }
    if (line[0] == '}')
    {
        int i = 1;
        while (ft_is_space(line[i]))
            ++i;
        if (line[i])
			throw ConfigException();
        ft_getline(_str, line);
        if (line[0] != '\0')
			throw ConfigException();
    }
    else
    {
		std::cout << line[0] << std::endl;
		throw ConfigException();
    }
}

void	Config::parse(void)
{   
    
    readfile(_conf_path);

    std::string line;
    std::string context;
    Server server;

    while (!_str.empty())
    {
        ft_getline(_str, line);

        while (ft_is_space(line[0]))
            line.erase(line.begin());
        if (line.compare(0, 6, "server") == 0)
        {
            while (ft_is_space(line[6]))
                line.erase(6, 1);
            if (line[6] != '{')
				throw ConfigException();
            if (line.compare(0, 7, "server{") == 0)
            {
                while (ft_is_space(line[7]))
                    line.erase(7, 1);
                if (line[7])
                    throw ConfigException();
                get_conf(line, context);
                std::vector<Server>::iterator it(g_servers.begin());
				while (it != g_servers.end())
				{
					if (this->_conf["server|"]["listen"] == it->_conf.back()["server|"]["listen"])
					{
						if (this->_conf["server|"]["server_name"] == it->_conf.back()["server|"]["server_name"])
							throw ConfigException();
						else
							it->_conf.push_back(this->_conf);
						break ;
					}
					++it;
				}
				if (it == g_servers.end())
				{
					server._conf.push_back(this->_conf);
					g_servers.push_back(server);
				}
				server._conf.clear();
				this->_conf.clear();
				context.clear();
            }
            else
                throw ConfigException();
        }
        else if (line[0])
            throw ConfigException();
    }
}

Config::ConfigException::ConfigException()
{
    this->error = "Configuration file is not valid";
}

Config::ConfigException::ConfigException(std::string function, std::string error)
{
    this->error = function + ": " + error;
}

Config::ConfigException::~ConfigException() throw(){}

const char	*Config::ConfigException::what() const throw()
{
    return (this->error.c_str());
}