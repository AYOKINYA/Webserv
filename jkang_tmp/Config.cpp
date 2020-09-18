#include "Config.hpp"

Config::Config(std::string conf) : _conf(conf);
{};

Config::Config(const Config& copy)
{
    *this = copy;
}

Config& operator=(const Config& other)
{
    //deep copy를 할 일이 없을 것 같지만 하게 되면 여기를 바꾸자

    if (this == &other)
        return (*this);
    
    _conf = other._conf;

    return (*this);
}

Config::~Config()
{};

int Config::parse(void)
{
    int o_bracket = 0;
    int c_bracket = 0;
    
    int fd = open(_conf, O_RDWR, 0644);
    if (fd == -1)
    {
        std::cerr << "Config open : " << strerror(errno) << std::endl;
        return (0);
    }
    char buf[2];
    buf[0] = 0;
    buf[1] = 0;

    std::string str;
    int ret = 0;
    while ((ret = read(fd, buf, 1)) > 0)
    {
        if (buf[0] == '{')
            ++o_bracket;
        else if (buf[0] == '}')
            ++c_bracket;
        str += buf;
    }
    if (ret == -1)
    {
        std::cerr << "Config read : " << strerror(errno) << std::endl;
        return (0);
    }
    if (o_bracket != c_bracket)
    {
        std::cerr << " Not Proper Brackets " << std::endl;
        return (0);
    }

    int n_server = 0;
    int server_on = 0;
    std::size_t server_found = str.find("server");
    std::size_t bracket_found = 0;
    
    while (sever_found != std::string::npos)
    {
        o_bracket = 0;
        c_bracket = 0;
        bracket_found = str.find("{", server_found);
        if ()
        ++n_server;
        server_found = str.find("server", server_found + 1);
    }

    


    
}

int main(int argc, char **argv)
{
    if (argc != 3)
        return (-1);
    
    Config config(argv[1]);

    if (!config.parse())
    {
        std::cerr << "Not a Valid Config." << std::endl;
        return (-1);
    }
    return (0);
}