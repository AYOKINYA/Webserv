#include "Config.hpp"

extern std::vector<Server> g_servers;

Config::Config(char *conf) : _conf_path(conf)
{};

Config::Config(const Config& copy)
{
    *this = copy;
}

Config& Config::operator=(const Config& other)
{
    //deep copy를 할 일이 없을 것 같지만 하게 되면 여기를 바꾸자

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

int Config::readfile(char *path)
{
    int fd = open(path, O_RDWR, 0644);
    if (fd == -1)
    {
        std::cerr << "Config open : " << strerror(errno) << std::endl;
        return (0);
    }
    char buf[1024];
    int ret = 0;
    while ((ret = read(fd, buf, 1023)) > 0)
    {
        buf[ret] = '\0';
        _str += buf;
    }

    if (ret == -1)
    {
        std::cerr << "Config read : " << strerror(errno) << std::endl;
        return (0);
    }
    close(fd);
    return (1);
}

int Config::set_location(std::string &line, std::string &context)
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
        return (0);
    if (line[i] == '{')
    {
        ++i;
        while (ft_is_space(line[i]))
            ++i;
        if (line[i])
            return (0);
    }

    context = "server|location " + loc + "|";

    return (1);
}

int Config::get_conf(std::string input, std::string &context)
{
    std::string line;
    std::string key;
    std::string value;
    size_t i, tmp;
    int loc_flag = 0;

    context = input.substr(0, input.length() - 1) + '|';

    while (ft_is_space(line[0]))
        line.erase(line.begin());
    int j = 0;
    while (line[0] != '}' && j++ < 30)
    {
        ft_getline(_str, line);

        while (ft_is_space(line[0]))
            line.erase(line.begin());
       
        if (line.find("location") != std::string::npos)
        {
            if (!set_location(line, context))
                return (0);
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
                return (0);
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
                if (!set_location(line, context))
                    return (0);
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
            return (0);
        ft_getline(_str, line);
        if (line[0] != '\0')
            return (0);
    }
    else
        return (0);
    return (1);
}

int Config::parse(void)
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
                return (0);
            if (line.compare(0, 7, "server{") == 0)
            {
                while (ft_is_space(line[7]))
                    line.erase(7, 1);
                if (line[7])
                    return (0);
                if (!get_conf(line, context))
                    return (0);
                std::vector<Server>::iterator it(g_servers.begin());
				while (it != g_servers.end())
				{
					if (this->_conf["server|"]["listen"] == it->_conf.back()["server|"]["listen"])
					{
						if (this->_conf["server|"]["server_name"] == it->_conf.back()["server|"]["server_name"])
							return (0);
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
                return (0);
        }
        else if (line[0])
            return (0);
    }
    return (1);
}

