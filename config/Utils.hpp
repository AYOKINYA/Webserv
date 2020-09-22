#ifndef UTILS_HPP
# define UTILS_HPP
# include "Config.hpp"

int     ft_is_space(char c);

void	ft_getline(std::string &b, std::string &line);

std::vector<std::string> split(const std::string& str, char delim);

std::string trim(const std::string& str);

std::string trim_url(const std::string& str);

std::string trim_url_2(const std::string& str);

std::string trim_extension(const std::string& str);


#endif