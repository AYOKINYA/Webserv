#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <map>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>

# include "Utils.hpp"
# include "Server.hpp"

// # include <exception>
// return 0들을 throw exception으로 바꾸기!

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

			void init(fd_set *rset, fd_set *wset, fd_set *cp_rset, fd_set *cp_wset);
			
			int readfile(char *path);
			int parse();
			int get_conf(std::string line, std::string &context);
			int set_location(std::string &line, std::string &context);
			void validate();
};

#endif