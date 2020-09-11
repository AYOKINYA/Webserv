#include <iostream>
#include <string>
#include <sys/stat.h>

std::string trim_url_2(const std::string& str)
{
    size_t first = str.find_first_of('/');
	size_t last = str.find_last_of('/');

    return (str.substr(first, (last - first)));
}

std::string trim_extension(const std::string& str)
{
	size_t first = str.find_last_of('.');
	size_t last = str.find_last_of(str);
	return(str.substr(first + 1, (last - first + 1)));
}

// void	parse_file()
// {
// 	struct stat	info;
// 	int _putcheck = 0;
// 	std::string	root = "/Users/jiyoonhur/Webserv/put_test_jy/src";
// 	std::string extensions[103] =
// 	{
// 		"html", "htm", "shtml", "css", "xml", "gif","jpeg", "jpg", "js", "atom",
// 		"rss", "mml", "txt", "jad", "wml", "htc", "png", "tif", "tiff", "wbmp",
// 		"ico", "jng", "bmp", "svg", "svgz", "webp", "woff", "jar", "war", "ear",
// 		"json", "hqx", "doc", "pdf", "ps", "eps", "ai", "rtf", "m3u8", "xls",
// 		"eot", "ppt", "wmlc", "kml", "kmz", "7z", "cco", "jardiff", "jnlp", "run",
// 		"pl", "pm", "prc", "pdb", "rar", "rpm", "sea", "swf", "sit", "tcl",
// 		"tk", "der", "pem",	"crt", "xpi", "xhtml", "xspf", "zip", "bin", "exe",
// 		"dll", "deb", "dmg", "iso", "img", "msi", "msp", "msm", "docx", "xlsx",
// 		"pptx", "mid", "midi","kar","mp3", "ogg","m4a", "ra", "3gpp", "3gp",
// 		"ts","mp4","mpeg", "mpg", "mov", "webm", "flv", "m4v", "mng", "asx",
// 		"asf","wmv","avi"
// 	};

// 	if (uri[0] == '/')
// 		_path = root + uri;
// 	else
// 		_path = root + "/" + "_uri";

// 	if (stat(_path.c_str(), &info) == 0)
// 	{
// 		std::cout << "aaaaaa" << std::endl;
// 		if (S_ISDIR(info.st_mode))
// 		{
// 			if (_path[_path.size() - 1] == '/')
// 				_path += "index.html";
// 			else
// 				_path += "/index.html";
// 		}
// 	}
// 	else
// 	{
// 		//파일이 존재하지 않을 경우에 여기로 빠져서 처리해놈 PUT 일 경우 에러로 처리하면 안돼서
// 		int ex_chk = 0;
// 		for(int i = 0; i < 103 ; i++)
// 		{
// 			if (ft_strncmp(trim_extension(_path).c_str(), extensions[i].c_str(), ft_strlen(extensions[i].c_str())) == 0)
// 			{
// 				ex_chk = 1;
// 				break ;
// 			}
// 		}
// 		if (ex_chk == 1 && stat(trim_url_2(_path).c_str(), &info) == 0 && _method == PUT)
// 		{
// 			std::cout << "bbbbbbbbbbb" << std::endl;
// 			_putcheck = 1;
// 		}
// 		else
// 			_error_code = 404;
// 		//400? //404? 유효하지 않은 주소
// 	}
// }
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*u_s1;
	unsigned char	*u_s2;

	i = 0;
	u_s1 = (unsigned char*)s1;
	u_s2 = (unsigned char*)s2;
	if (n == 0)
		return (0);
	while (u_s1[i] != '\0' && u_s2[i] != '\0' && i < n)
	{
		if (u_s1[i] != u_s2[i])
			return ((u_s1[i] - u_s2[i]));
		else
			++i;
	}
	if (i == n)
		return (0);
	return (u_s1[i] - u_s2[i]);
}

size_t	ft_strlen(const char *s)
{
	size_t count;

	count = 0;
	while (s[count] != '\0')
		++count;
	return (count);
}

void stat_test()
{
	struct stat	info;
	std::string _path = "/Users/jiyoonhur/Webserv/put_test_jy/srcs/indesds.html";
	std::string _path1 = "/Users/jiyoonhur/Webserv/put_test_jy/srcs";
	std::string extensions[103] =
	{
		"html", "htm", "shtml", "css", "xml", "gif","jpeg", "jpg", "js", "atom",
		"rss", "mml", "txt", "jad", "wml", "htc", "png", "tif", "tiff", "wbmp",
		"ico", "jng", "bmp", "svg", "svgz", "webp", "woff", "jar", "war", "ear",
		"json", "hqx", "doc", "pdf", "ps", "eps", "ai", "rtf", "m3u8", "xls",
		"eot", "ppt", "wmlc", "kml", "kmz", "7z", "cco", "jardiff", "jnlp", "run",
		"pl", "pm", "prc", "pdb", "rar", "rpm", "sea", "swf", "sit", "tcl",
		"tk", "der", "pem",	"crt", "xpi", "xhtml", "xspf", "zip", "bin", "exe",
		"dll", "deb", "dmg", "iso", "img", "msi", "msp", "msm", "docx", "xlsx",
		"pptx", "mid", "midi","kar","mp3", "ogg","m4a", "ra", "3gpp", "3gp",
		"ts","mp4","mpeg", "mpg", "mov", "webm", "flv", "m4v", "mng", "asx",
		"asf","wmv","avi"
	};

	std::cout << trim_extension(_path) << std::endl;
	if (stat(_path.c_str(), &info) == 0)
	{
		std::cout << "yyyyyyy" << std::endl;
	}
	else
	{
		int ex_chk = 0;
		for(int i = 0; i < 103 ; i++)
		{
			if (ft_strncmp(trim_extension(_path).c_str(), extensions[i].c_str(), ft_strlen(extensions[i].c_str())) == 0)
			{
				ex_chk = 1;
				break ;
			}
		}
		if (ex_chk == 1 && stat(trim_url_2(_path).c_str(), &info) == 0)
		{
			// std::cout << trim_url_2(_path) << std::endl;
			std::cout << "wow" << std::endl;
		}
		else
		{
			std::cout << trim_url_2(_path) << std::endl;
			std::cout << "eeee" << std::endl;
		}
	}
}

int main()
{
	stat_test();
}
