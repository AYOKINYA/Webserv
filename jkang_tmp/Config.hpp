#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <map>

# include "util.cpp" // 나중에 Util class로 만들어 hpp도 만들어줘야 한다.
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
// # include <exception>

class Config
{
    private:
            char        *_conf_path;
            std::string _str;
            typedef std::map<std::string, std::string> elem;
            typedef std::map<std::string, elem> conf;
            conf _conf;
 
    public:
            Config(char *conf);
            Config(const Config& copy);
            Config& operator=(const Config& other);
            virtual ~Config();
            int readfile(char *path);
            int parse();
            int get_conf(std::string line, std::string &context);
            int set_location(std::string &line, std::string &context);
            void validate();
};

#endif