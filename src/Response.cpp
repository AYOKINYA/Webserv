#include "Response.hpp"

Response::Response()
{};

Response::Response(Request request) : _request(request)
{
	init_status_table();
	set_vars_response();
};

Response::Response(const Response &copy)
{
	*this = copy;
}

Response& Response::operator=(const Response &copy)
{
	if (this == &copy)
		return (*this);

	_request = copy._request;
	_vars_response = copy._vars_response;
	_status = copy._status;
	_start_line = copy._start_line;

	return (*this);
}

Response::~Response()
{};

void Response::init_status_table(void)
{
	_status_table.insert(std::make_pair(200, "OK"));
	_status_table.insert(std::make_pair(201, "CREATED"));
	_status_table.insert(std::make_pair(204, "No Content"));
	_status_table.insert(std::make_pair(400, "Bad Request"));
	_status_table.insert(std::make_pair(404, "Not Found"));
	_status_table.insert(std::make_pair(405, "Not Allowed Method"));
	_status_table.insert(std::make_pair(411, "Length Required"));
	_status_table.insert(std::make_pair(413, "Request Entity Too Large"));
	_status_table.insert(std::make_pair(415, "Unsupported Media Type"));
	_status_table.insert(std::make_pair(501, "Not Implemented"));
	_status_table.insert(std::make_pair(505, "HTTP Version Not Supported"));
}

void Response::setStatus(int num)
{
	_status.first = num;
	_status.second = (_status_table.find(num))->second;
}

void Response::setDate()
{
	struct timeval 	cur_time;
	struct tm 		time;
	char 			buf[128];
	std::string 	date;

	gettimeofday(&cur_time, 0);
	strptime(std::to_string(cur_time.tv_sec).c_str(), "%s", &time); // seconds in tm format
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format
	cgi_header.insert(std::pair<std::string, std::string>("Date", buf));
	_vars_response.insert(std::pair<std::string, std::string>("Date", buf));
}

void Response::setContentType(const std::string &content)
{
	/* mime.types 참고. excel 텍스트 나누기로 split했다*/
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

	std::string types[103] =
	{
		"text/html", "text/html", "text/html", "text/css", "text/xml","image/gif", "image/jpeg", "image/jpeg","application/javascript","application/atom+xml",
		"application/rss+xml","text/mathml","text/plain","text/vnd.sun.j2me.app-descriptor","text/vnd.wap.wml","text/x-component","image/png","image/tiff", "image/tiff","image/vnd.wap.wbmp",
		"image/x-icon","image/x-jng","image/x-ms-bmp","image/svg+xml", "image/svg+xml", "image/webp","application/font-woff","application/java-archive", "application/java-archive", "application/java-archive",
		"application/json","application/mac-binhex40","application/msword","application/pdf","application/postscript",
		"application/postscript", "application/postscript","application/rtf","application/vnd.apple.mpegurl","application/vnd.ms-excel",
		"application/vnd.ms-fontobject", "application/vnd.ms-powerpoint","application/vnd.wap.wmlc","application/vnd.google-earth.kml+xml","application/vnd.google-earth.kmz",
		"application/x-7z-compressed","application/x-cocoa","application/x-java-archive-diff","application/x-java-jnlp-file","application/x-makeself",
		"application/x-perl", "application/x-perl","application/x-pilot", "application/x-pilot","application/x-rar-compressed",
		"application/x-redhat-package-manager","application/x-sea","application/x-shockwave-flash","application/x-stuffit","application/x-tcl",
		"application/x-tcl","application/x-x509-ca-cert", "application/x-x509-ca-cert", "application/x-x509-ca-cert","application/x-xpinstall",
		"application/xhtml+xml","application/xspf+xml","application/zip","application/octet-stream", "application/octet-stream",
		"application/octet-stream","application/octet-stream","application/octet-stream","application/octet-stream", "application/octet-stream",
		"application/octet-stream", "application/octet-stream",  "application/octet-stream", "application/vnd.openxmlformats-officedocument.wordprocessingml.document","application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
		"application/vnd.openxmlformats-officedocument.presentationml.presentation","audio/midi", "audio/midi", "audio/midi","audio/mpeg",
		"audio/ogg","audio/x-m4a","audio/x-realaudio","video/3gpp", "video/3gpp",
		"video/mp2t","video/mp4","video/mpeg", "video/mpeg","video/quicktime",
		"video/webm","video/x-flv","video/x-m4v","video/x-mng","video/x-ms-asf",
		"video/x-ms-asf", "video/x-ms-wmv", "video/x-msvideo"
	};

	std::size_t pos = content.rfind(".");
	std::string res = "";
	int i = 0;
	if (pos != std::string::npos)
	{
		std::string ext = content.substr(pos + 1);
		for (; i < 103 ; ++i)
		{
			if (ext == extensions[i])
			{
				res = types[i];
				break ;
			}
		}
	}
	if (res.length() == 0)
		res = "text/plain";

	_vars_response.insert(std::pair<std::string, std::string>("Content-Type", res));

}

void Response::setContentLocation(const std::string &loc)
{
	_vars_response.insert(std::pair<std::string, std::string>("Content-Location", loc));
}

void Response::setContentLanguage()
{
	cgi_header.insert(std::pair<std::string, std::string>("Content-Language", "en"));
	_vars_response.insert(std::pair<std::string, std::string>("Content-Language", "en"));
}

void Response::setContentLength(const std::string &content)
{
	const char *dir = content.c_str();
	int fd = open(dir, O_RDWR, 0644);
	int count = 0;
	char buf[2];
	ft_memset(buf, 0, 2);

	if (fd > 0)
	{
		while (read(fd, buf, 1) > 0)
			++count;
	}
	else
	{
		fd = open("./www/error.html", O_RDWR, 0644);
		if (fd == -1)
		{
			std::cerr << "Default Error page doesn't exist." << std::endl;
			exit(1);
		}
		while (read(fd, buf, 1) > 0)
			++count;
	}
	close(fd);

	if (_vars_response.find("Content-Length") == _vars_response.end())
		_vars_response.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(count)));
	else
		_vars_response["Content-Length"] = std::to_string(count);
}


void Response::setAllow()
{
	std::string str = _request.get_conf()["method_allowed"];
	cgi_header.insert(std::pair<std::string, std::string>("Allow", str));
	_vars_response.insert(std::pair<std::string, std::string>("Allow", str));

}

void Response::setLastModified(const std::string &content)
{
	struct	stat info;
	struct	tm	time;
	char	buf[1024];

	stat(content.c_str(), &info);
	if (!S_ISDIR(info.st_mode))
		stat("./www/error.html", &info);
	strptime(std::to_string(info.st_mtime).c_str(), "%s", &time);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format

	cgi_header.insert(std::pair<std::string, std::string>("Last-Modified", buf));
	_vars_response.insert(std::pair<std::string, std::string>("Last-Modified", buf));

}

void Response::setServer()
{
	cgi_header.insert(std::pair<std::string, std::string>("Server", "Carry-Please"));
	_vars_response.insert(std::pair<std::string, std::string>("Server", "Carry-Please"));
}

void Response::setTransferEncoding()
{
	cgi_header.insert(std::pair<std::string, std::string>("Transfer-Encoding", "identity"));
	_vars_response.insert(std::pair<std::string, std::string>("Transfer-Encoding", "identity"));
}

void Response::setWWWAuthentication()
{
	cgi_header.insert(std::pair<std::string, std::string>("WWW-Authentication", "Basic"));
	_vars_response.insert(std::pair<std::string, std::string>("WWW-Authentication", "Basic"));
}

void Response::set_vars_response()
{
	std::string path = _request.get_conf()["path"];
	setStatus(_request.get_error_code());
	setServer();
	setDate();
	setAllow();
	setContentType(path);
	setContentLocation(path);
	setTransferEncoding();
	setContentLength(path);
	setLastModified(path);
	setWWWAuthentication();
}

std::string Response::getStartLine(void)
{
	_start_line = "HTTP/1.1 ";
	_start_line += std::to_string(_status.first);
	_start_line += " ";
	_start_line += _status.second;

	return(_start_line);
}

std::string Response::printItem(const std::string &key)
{
	std::string res = "";

	std::map<std::string, std::string>::iterator it;
	it = _vars_response.find(key);
	if (it != _vars_response.end())
	{
		res += it->first;
		res += ": ";
		res += it->second;
		res += "\n";
	}

	return (res);
}

std::string Response::printItem2(std::map<std::string, std::string> param, const std::string &key)
{
	std::string res = "";

	std::map<std::string, std::string>::iterator it;
	it = param.find(key);
	if (it != param.end())
	{
		res += it->first;
		res += ": ";
		res += it->second;
		res += "\n";
	}

	return (res);
}

std::string Response::body(const std::string &path)
{
	std::string res = "";
	int fd = open(path.c_str(), O_RDWR, 0644);
	if (fd == -1)
		std::cerr << "Error MUST be thrown!" << std::endl; 
	char buf[1025];
	int nread;
	ft_memset(buf, 0, 1025);
	while ((nread = read(fd, buf, 1024)) > 0)
	{
		buf[nread] = '\0';
		res += buf;
	}
	close(fd);
	return (res);
}

std::string Response::exec_method()
{
	std::string res = "";

	int method = _request.get_method();
	if (_request.get_body().size() > (unsigned long)_request.get_limit())
		_request.set_error_code(413);

	if (method == GET)
		res = Get();
	else if (method == HEAD)
		res = Head();
	else if (method == POST)
		res = Post();
	else if (method == PUT)
		res = Put();

	return (res);
}

std::string	Response::cgi(std::string extension)
{
	char	**args;
	pid_t	pid;
	int		fd;
	int		ret;
	int		tubes[2];
	std::string	res;
	struct stat	php;
	char **env = Env();

	args = (char **)(malloc(sizeof(char *) * 3));
	if (extension == ".php")
	{
		args[0] = ft_strdup("/usr/local/bin/php-cgi");
		args[1] = ft_strdup(_request.get_conf()["path"].c_str());
	}
	else if (extension == _request.get_conf()["cgi_test"])
	{
		args[0] = ft_strdup(_request.get_conf()["cgi_exec"].c_str());
		args[1] = ft_strdup(_request.get_conf()["path"].c_str());
	}
	else
	{
		args[0] = ft_strdup(_request.get_conf()["path"].c_str());
		args[1] = NULL;
	}
	args[2] = NULL;

	fd = open("cgi.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	pipe(tubes);
	if ((pid = fork()) == 0)
	{
		close(tubes[1]);
		dup2(fd, 1);
		if (stat(_request.get_conf()["path"].c_str(), &php) != 0 ||
		!(php.st_mode & S_IFREG))
		{
			std::cerr << "Error CGI" << std::endl;
			exit(1);
		}
		dup2(tubes[0], 0);
		if ((ret = execve(args[0], args, env)) == -1)
		{
			std::cerr << std::string(strerror(errno)) << std::endl;
			exit(1);
		}
	}
	else
	{	write(tubes[1],_request.get_body().c_str(),_request.get_body().length());
		close(tubes[1]);
		waitpid(pid, NULL, 0);
		close(fd);
		close(tubes[0]);
	}
	ft_free(args);
	ft_free(env);

	char	buf[10001];
	std::string tmp;

	fd = open("cgi.txt", O_RDONLY, 0666);
	while ((ret = read(fd, &buf, 10000)) > 0)
	{
		buf[ret] = '\0';
		tmp += buf;
	}
	close(fd);

	parseCGIResult(tmp);
	res += getStartLine();
	res += "\n";
	res += printItem2(cgi_header,"Content-Length");
	res += printItem2(cgi_header, "Content-Type");
	res += printItem2(cgi_header, "Date");
	res += printItem2(cgi_header, "Server");
	res += "\r\n";
	if (_request.get_method() == HEAD)
		return (res);
	else if (_status.first == 200)
		res += _cgi_body;
	else
		res += (body("./www/error.html"));

	return (res);
}

void		Response::parseCGIResult(std::string buf)
{
	size_t			pos;
	std::string		headers;
	std::string		key;
	std::string		value;
	std::string		tmp_status;

	if (buf.find("\r\n\r\n") == std::string::npos)
		return ;
	headers = buf.substr(0, buf.find("\r\n\r\n") + 1);
	pos = headers.find("Status");
	if (pos != std::string::npos)
	{
		pos += 8;
		while (headers[pos] != ' ')
		{
			tmp_status += headers[pos];
			pos++;
		}
		setStatus(ft_atoi(tmp_status.c_str()));
	}
	pos = 0;
	while (headers[pos])
	{
		while (headers[pos] && headers[pos] != ':')
		{
			key += headers[pos];
			++pos;
		}
		++pos;

		while (headers[pos] && headers[pos] != '\r')
		{
			value += headers[pos];
			++pos;
		}

		cgi_header.insert(std::pair<std::string, std::string>(key, trim(value)));

		key.clear();
		value.clear();
		if (headers[pos] == '\r')
			pos++;
		if (headers[pos] == '\n')
			pos++;
	}
	pos = buf.find("\r\n\r\n") + 4;
	_cgi_body = buf.substr(pos);

	key = "Content-Length";
	value = std::to_string(_cgi_body.size());
	cgi_header.insert(std::pair<std::string, std::string>(key, value));

}

char	**Response::Env()
{
	char	**env = 0;
	std::map<std::string, std::string> map;

	map["CONTENT_LENGTH"] = std::to_string(_request.get_body().size());
	map["CONTENT_TYPE"] = "text/html";

	map["GATEWAY_INTERFACE"] = "CGI/1.1";
	map["PATH_INFO"] = _request.get_conf()["path"];
	map["PATH_TRANSLATED"] = _request.get_conf()["path"];
	if (_request.get_uri().find('?') != std::string::npos)
		map["QUERY_STRING"] = _request.get_uri().substr(_request.get_uri().find('?') + 1);
	else
		map["QUERY_STRING"] = "";
	map["REMOTE_ADDR"] = _request.get_client_ip();

	if (_request.get_method() == GET)
		map["REQUEST_METHOD"] = "GET";
	if (_request.get_method() == POST)
		map["REQUEST_METHOD"] = "POST";

	map["REQUEST_URI"] = _request.get_conf()["path"];

	map["SCRIPT_NAME"] = "." + _request.get_uri();

	map["SERVER_NAME"] = "Carry-Please";
	map["SERVER_PORT"] = "8080";
	map["SERVER_PROTOCOL"] = "HTTP/1.1";
	map["SERVER_SOFTWARE"] = "HTTP/1.1";

	map["REDIRECT_STATUS"] = "200";

	std::map<std::string, std::string>::iterator b = _request._headers.begin();
	while (b != _request._headers.end())
	{
		map["HTTP_" + b->first] = b->second;
		++b;
	}

	env = (char **)malloc(sizeof(char *) * (map.size() + 1));
	std::map<std::string, std::string>::iterator it = map.begin();
	int i = -1;
	while (it != map.end())
	{
		env[++i] = strdup((it->first + "=" + it->second).c_str());
		++it;
	}
	env[i] = NULL;

	return (env);
}

std::string	Response::autoindex()
{
	DIR				*dir;
	struct dirent	*cur;
	std::string		res;

	dir = opendir(_request.get_conf()["path"].c_str());
	res += "<html>\n<body>\n";
	res += "<h1>Directory listing</h1>\n";
	while ((cur = readdir(dir)) != NULL)
	{
		if (cur->d_name[0] != '.')
		{
			res += "<a href=\"" + _request.get_uri();
			if (_request.get_uri() != "/")
				res += "/";
			res += cur->d_name;
			res += "\">";
			res += cur->d_name;
			res += "</a><br>\n";
		}
	}
	closedir(dir);
	res += "</body>\n</html>\n";
	return (res);
}


std::string Response::Get (void)
{
	std::string res = "";
	struct stat info;
	int			autoidx_flag = 0;

	stat(_request.get_conf()["path"].c_str(), &info);
	if (S_ISDIR(info.st_mode) && _request.get_conf()["autoindex"] == "on")
		autoidx_flag = 1;

	std::string file_extension = "." + trim_extension(_request.get_conf()["path"]);
	std::vector<std::string> extensions = split(_request.get_conf()["cgi_extension"], ' ');

	for(unsigned long i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == file_extension)
			return (cgi(extensions[i]));
	}
	res += getStartLine();
	res += "\n";
	res += printItem("Last-Modified");
	if (autoidx_flag == 1 || _request.get_error_code() != 200)
		res += "Content-Type: text/html\n";
	else
		res += printItem("Content-Type");
	res += printItem("WWW-Authenticate");
	res += printItem("Allow");
	res += printItem("Date");
	res += printItem("Server");

	if (autoidx_flag == 1)
	{
		int len = ft_strlen(autoindex().c_str());
		res += "Content-Length: " + std::to_string(len) + "\n";
	}
	else
		res += printItem("Content-Length");
	res += "\n";
	if (_request.get_method() == HEAD)
		return (res);
	else if (_status.first == 200)
	{
		if (autoidx_flag == 1)
			res += autoindex();
		else
			res += body(_request.get_conf()["path"]);
	}
	else
		res += (body("./www/error.html"));

	return (res);
}

std::string Response::Head(void)
{
	std::string res = Get();

	return (res);
}

std::string Response::Post()
{
	std::string	res;
	std::string extension = trim_extension(_request.get_conf()["path"]);
	struct stat info;
	int			autoidx_flag = 0;

	stat(_request.get_conf()["path"].c_str(), &info);
	if (S_ISDIR(info.st_mode) && _request.get_conf()["autoindex"] == "on")
		autoidx_flag = 1;

	std::string file_extension = "." + trim_extension(_request.get_conf()["path"]);
	std::vector<std::string> extensions = split(_request.get_conf()["cgi_extension"], ' ');

	for(unsigned long i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == file_extension)
			return (cgi(extensions[i]));
	}

	if (_request.get_error_code() != 200)
	{
		setStatus(_request.get_error_code());

		res = getStartLine();
		res += "\n";
		res += printItem("Server");
		res += printItem("Date");
		res += printItem("Last-Modified");
		res += printItem("Content-Type");
		if (autoidx_flag == 1)
		{
			int len = ft_strlen(autoindex().c_str());
			res += "Content-Length: " + std::to_string(len) + "\n";
			res += autoindex();
		}
		else
		{
			res += "Content-Length: 14\n";
			res += "\n";
			res += "File modifed";
			res += "\n\n";
		}
		return res;
	}
	else
	{
		int fd = open(_request.get_conf()["path"].c_str(), O_APPEND, 0666);
		write(fd, _request.get_body().c_str(), _request.get_body().size());
		close(fd);
		res = getStartLine();
		res += "\n";
		res += printItem("Server");
		res += printItem("Date");
		res += printItem("Last-Modified");
		if (autoidx_flag == 1)
			res += "Content-Type: text/html\n";
		else
			res += printItem("Content-Type");
		res += "Content-Length: 14\n";
		res += "\n";
		res += "File modifed";
		res += "\n\n";
		return (res);
	}

	return (0);
}

std::string Response::Put()
{
	std::string url;
	std::string filename;
	std::string msg;

	int fd;

	url =_request.get_conf()["path"];
	msg = "HTTP/1.1 204 No Content\n";
	if ((fd = open(url.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1)
	{
		msg = "HTTP/1.1 500 Internal Server Error\n";
		msg += printItem("Date");
		msg += printItem("Server") + "\n";
		return (msg);
	}
	write(fd, _request.get_body().c_str(), _request.get_body().length());
	close(fd);
	msg += printItem("Date");
	msg += "Content-Location: /" + filename + "\n";
	msg += printItem("Server") + "\n";
	return (msg);
}

std::string	Response::Delete()
{
	std::string	msg;

	if (remove(_request.get_conf()["path"].c_str()) == 0)
	{
		msg = getStartLine();
	}
	return (msg);
}

std::string	Response::Options()
{
	std::string	msg;

	msg += getStartLine();
	msg += "\n";
	msg += printItem("Allow");
	msg += printItem("Server");
	msg += printItem("Date");
	msg += printItem("Last-Modified");
	msg += printItem("Content-Type");
	msg += printItem("Content-Length");
	msg += "\n";
	if (_status.first == 200)
		msg += body(_request.get_conf()["path"]);
	else
		msg += body("./www/error.html");
	return (msg);
}

std::string	Response::Connect()
{
	std::string	msg;

	msg += getStartLine();
	msg += "\n";
	msg += printItem("Server");
	msg += printItem("Date");
	msg += "\n\n";

	return (msg);
}

std::string	Response::Trace()
{
	std::string	msg;

	msg += getStartLine();
	msg += "\n";
	msg += printItem("Server");
	msg += printItem("Date");
	if (_status.first == 200)
		msg += "Content-Type: message/http\n";
	else
		msg += printItem("Content-Type");
	msg += printItem("Content-Length");
	msg += "\n";
	if (_status.first == 200)
	{
		msg += _request.get_method_str() + " " + _request.get_uri() + " HTTP/1.1\n";
		for (std::map<std::string, std::string>::iterator it(_request._headers.begin());it != _request._headers.end(); ++it)
		{
			msg += it->first + ": " + it->second + "\r\n";
		}
	}
	else
		msg += body("./www/error.html");
	return (msg);
}
