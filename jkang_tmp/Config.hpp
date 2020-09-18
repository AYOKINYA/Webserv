#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <iostream>
# include <string>
# include <algoithm>
# include <vector>
# include <Util.cpp> // 나중에 Util class로 만들어 hpp도 만들어줘야 한다.
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
class Config
{
    private:
            std::string _conf;
    public:
            Config(std::string conf));
            Config(const Config& copy);
            Config& operator=(const Config& other);
            virtual ~Config();
            int parse();
            void validate();
};

#endif