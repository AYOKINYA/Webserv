#include "Utils.hpp"

int ft_is_space(char c)
{
    if (c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f' || c == ' ')
        return (1);
    return (0);
}

void    ft_getline(std::string &b, std::string &line)
{
    size_t					pos;

    pos = b.find("\n");

    if (pos != std::string::npos)
    {
        line = std::string (b, 0, pos++);
        b = b.substr(pos);
    }
    else
    {
        if (b[b.size() - 1] == '\n')
            b = b.substr(b.size());
        else
        {
            line = b;
            b = b.substr(b.size());
        }
    }
}

std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
	//find_first_not_of 전달된 문자들 중 첫번째로 일치하지 않는 위치를 찾는다.
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');

    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

std::string trim_url(const std::string& str)//파일이름
{
    size_t first = str.find_last_of('/');

    // if (std::string::npos == first)
        // return str;
    size_t last = str.find_last_of(' ');
    return (str.substr(first + 1, (last - first + 1)));
}

std::string trim_url_2(const std::string& str)//디렉터리
{
    size_t first = str.find_first_of('/');
	size_t last = str.find_last_of('/');

    return (str.substr(first, (last - first)));
}

std::string trim_extension(const std::string& str)
{
	size_t first = str.find_last_of('.');
	size_t last = str.find_last_of(' ');
	return(str.substr(first + 1, (last - first + 1)));
}

void        ft_free(char **arr)
{
    if (!arr)
        return ;
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        ++i;
    }
    free(arr);
}

int     ft_getline_conf(std::string &b, std::string &line)
{
    size_t					pos;

    pos = b.find("\n");

    if (pos != std::string::npos)
    {
        line = std::string (b, 0, pos++);
        b = b.substr(pos);
        return 1;
    }
    else
    {
        if (b[b.size() - 1] == '\n')
            b = b.substr(b.size());
        else
        {
            line = b;
            b = b.substr(b.size());
        }
    }
    return (0);
}
