#ifndef UTILS_HPP
# define UTILS_HPP

# include <sys/wait.h>
# include <string.h>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <string>
# include <map>
# include <algorithm>
# include <vector>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <sys/stat.h>
# include <iomanip>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include "libft/libft.h"



int     ft_is_space(char c);
void	ft_getline(std::string &b, std::string &line);
std::vector<std::string> split(const std::string& str, char delim);

std::string trim(const std::string& str);
std::string trim_url(const std::string& str);
std::string trim_url_2(const std::string& str);
std::string trim_extension(const std::string& str);


#endif