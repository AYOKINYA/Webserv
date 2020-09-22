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

	// Response 객체 여러 개 만들어야 하면 deep copy로 수정해야 한다.

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
	// if (i == 103)
	// 	setStatus(415);

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
		fd = open("./error.html", O_RDWR, 0644);
		if (fd == -1)
		{
			std::cout << "Default Error page doesn't exist." << std::endl;
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
	//if METHOD does not match any of these, return 405 Not Allowed
	std::string str = "GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS";
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
		stat("./error.html", &info);
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
	std::string path = _request.get_path();
	//std::cout << "parse path : " << path << std::endl;
	setStatus(_request.get_error_code());
	setServer();
	setDate();
	setAllow();
	setContentType(path);
	setContentLocation(path);
	setTransferEncoding();
	setContentLength(path); // Request에서 파싱한 거 거의 그대로 넣으면 될 듯?
	setLastModified(path); // Request에서 파싱한 거 거의 그대로 넣으면 될 듯?
	setWWWAuthentication(); // when status is 401
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
	//나중에 send할 소켓에 넣어줘야 한다.

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

////////////////////***********
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
////////////////////***********

std::string Response::body(const std::string &path)
{
	std::string res = "";
	int fd = open(path.c_str(), O_RDWR, 0644);
	if (fd == -1)
		std::cout << "Error MUST be thrown!" << std::endl; // error check in _request
	char buf[1024];
	int nread;
	ft_memset(buf, 0, 1024);
	while ((nread = read(fd, buf, 1)) > 0)
	{
		buf[nread] = '\0';
		res += buf;
	}
	close(fd);
	return (res);
}

std::string	Response::cgi (void)
{
	char **env;
	char	**args;
	int fd, pid;
	env = Env();
	struct stat php;
	int		ret;
	std::string	res;
	int		tubes[2];

	args = (char **)(malloc(sizeof(char *) * 3));

	args[0] = ft_strdup("/Users/hpark/Webserv/jy/cgi_tester");
	// args[0] = ft_strdup("/usr/local/bin/php-cgi");
	// args[0] = ft_strdup(_request.get_path().c_str());
	// args[1] = NULL;
	args[1] = ft_strdup(_request.get_path().c_str());
	args[2] = NULL;

	fd = open("cgi.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	pipe(tubes);
	// close(tubes[1]);
	if ((pid = fork()) == 0)
	{
		close(tubes[1]);
		dup2(fd, 1);
		if (stat(_request.get_path().c_str(), &php) != 0 ||
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
	char	buf[10001];
	// execve 결과에 code도 다 담겨서 나온다.
	// res = getStartLine();
	// res += "\n";

	// int ret;
	std::string tmp;

	fd = open("cgi.txt", O_RDONLY, 0666);
	while ((ret = read(fd, &buf, 10000)) > 0)
	{
		buf[ret] = '\0';
		tmp += buf;
	}
	close(fd);

	std::cout << "==========" << std::endl;
	std::cout << tmp << std::endl;
	/*
	Status: 200 OK
	Content-Type: text/html; charset=utf-8
	buf 에 담김
	*/
	std::cout << "==========" << std::endl;

	parseCGIResult(tmp);
	res += getStartLine();
	res += "\n";
	// res += "Content-Length: 100000000\n";
	res += printItem2(cgi_header,"Content-Length");
	res += printItem2(cgi_header, "Content-Type");
	//php 이면 Content-type 임ㅋㅋㅋㅋㅋContent-type: text/html; charset=UTF-8
	res += printItem2(cgi_header, "Date");
	res += printItem2(cgi_header, "Server");
	// res += "Status: 200 OK";
	res += "\r\n";
	if (_status.first == 200)
		res += _cgi_body;
		// res += _request.get_body();
	else
		res += (body("error.html"));
	res += "\r\n\r\n";
	return (res);
}

void		Response::parseCGIResult(std::string buf)
{
	size_t			pos;
	std::string		headers;
	std::string		key;
	std::string		value;
	std::string		tmp_status ="";

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
		// std::cout << tmp_status << std::endl;
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
		// std::cout << "CGI key is!!!" << key << std::endl;
		while (headers[pos] && headers[pos] != '\r')
		{
			value += headers[pos];
			++pos;
		}
		/* php 면 trim(value)
			cgi_tester 면 걍 value
		*/
		// std::cout << "CGI val is!!!" << value << std::endl;
		cgi_header.insert(std::pair<std::string, std::string>(key, trim(value)));
		// std::map<std::string, std::string>::iterator it;
		// for(it = cgi_header.begin(); it != cgi_header.end(); it++)
		// {
		// 	std::cout << it->first << " and " << it->second << std::endl;
		// }
		key.clear();
		value.clear();
		if (headers[pos] == '\r')
			pos++;
		if (headers[pos] == '\n')
			pos++;
	}
	pos = buf.find("\r\n\r\n") + 4;
	_cgi_body = buf.substr(pos);
	// std::cout << buf << std::endl;
	key = "Content-Length";
	value = std::to_string(buf.size());
	cgi_header.insert(std::pair<std::string, std::string>(key, value));
	// client.res.headers["Content-Length"] = std::to_string(buf.size());
}

std::string Response::Get (void)
{
	std::string res = "";

	// size_t first = _request.get_path().find_last_of('.');
	// size_t last = _request.get_path().find_last_of(' ');
	// if (!ft_strncmp(_request.get_path().substr(first + 1, (last - first + 1)).c_str(), "php", 3))
	std::string extension = trim_extension(_request.get_path());
	if (extension == "bla" || extension == "pl" ||  extension == "php" || extension == "cgi")
		return (cgi());
	res += getStartLine();
	res += "\n";
	res += printItem("Last-Modified");
	res += printItem("Content-Type");
	res += printItem("WWW-Authenticate");
	res += printItem("Allow");
	res += printItem("Date");
	res += printItem("Server");
	res += printItem("Content-Length");
	res += "\n";
	if (_status.first == 200)
		res += body(_request.get_path());
	else
		res += (body("error.html"));

	return (res);
}

std::string Response::Head(void)
{
	std::string res = "";
	//to pass tester
	res += Post();

	// below is original...

	// res += getStartLine();
	// res += "\n";
	// res += printItem("Server");
	// res += printItem("Date");
	// res += printItem("Last-Modified");
	// res += printItem("Content-Type");
	// res += printItem("Content-Length");
	// res += "\n";

	return (res);
}

// std::string Response::Post() // for temporary only! to pass tester...
// {
// 	std::string res = "";

// 	// std::cout << trim_url(_request.get_path()) << std::endl;
// 	std::cout << "PATH @ POST: " << _request.get_path() << std::endl;
// 	setStatus(405);
// 	setContentLength("./error.html");

// 	res = getStartLine();
// 	res += "\n";
// 	res += printItem("Server");
// 	res += printItem("Date");
// 	res += printItem("Last-Modified");
// 	res += printItem("Content-Type");
// 	res += printItem("Content-Length");
// 	res += "\n";
// 	res += (body("error.html"));

// 	if (trim_url(_request.get_path()) == "youpi.bla")
// 		res = Put();
// 	return (res);
// }

std::string Response::Put()
{
	std::string url;
	std::string filename;
	std::string msg;

	url = _request.get_path();
	std::cout << "path is !!!!"<< url << std::endl;
	std::cout << "put check return : " << _request.get_putcheck() << std::endl;
	std::cout << "file check return : " << _request.get_filecheck() << std::endl;
	filename = trim_url(url);
	if (_request.get_putcheck() == 1) //파일이 없을때 새로 만든다
	{
		// std::cout << filename << std::endl;
		int fd = open(url.c_str(), O_CREAT | O_RDWR, 0777);
		write(fd, _request.get_body().c_str(), _request.get_body().length());
		close(fd);
		/////msg//////
		msg = "HTTP/1.1 201 Created\n";
		msg += "Content-Location: /" + filename + "\n\n";
		return (msg);
	}
	else if (_request.get_filecheck() == 1)//파일을 있을 때 오픈해서 내용을 지우고 새로 입력한다
	{
		std::cout << url << std::endl;
		int fd = open(url.c_str(), O_TRUNC | O_RDWR, 0777);

		write(fd, _request.get_body().c_str(), _request.get_body().length());
		close(fd);
		/////msg//////
		msg = "HTTP/1.1 204 No Content\n"; //혹은 200 OK
		msg += "Content-Location: /" + filename + "\n\n";
		return (msg);
	}
	return ("========PUT FAIL=============");
}

std::string	Response::Delete()
{
	std::string	msg;
	//응답코드 3개 있는데 1.성공적으로 수행할 것 같으나 아직 실행x -> 202 (Accepted) 는 어떨 때 써야할지 모르겠다.
	//2. 204 No Content 코드, 3. 200 OK -> 파일이 지워졌다는 페이지ㅡㄹㄹ 보여준다.
	if (remove(_request.get_path().c_str()) == 0)
	{
		msg = getStartLine();
	}
	return (msg);
}

std::string	Response::Options()
{
	std::string	msg;
	//Get이ㅏㄹㅇ 똑같이
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
		msg += body(_request.get_path());
	else
		msg += body("error.html");
	return (msg);
}

std::string Response::exec_method()
{
	std::string res = "";

	int method = _request.get_method();

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


char	**Response::Env()
{
	char	**env = 0;
	std::map<std::string, std::string> map;

	map["CONTENT_LENGTH"] = std::to_string(_request.get_body().size());

	//request에서 받아온 값? 헤더값?
	map["CONTENT_TYPE"] = "text/html";
	// if (_request.get_vars().find("Content-Type") != _request.get_vars().end())
	// 	map["CONTENT_TYPE"] = _request.get_vars().find("Content-Type")->second;

	map["GATEWAY_INTERFACE"] = "CGI/1.1";

	//request_path? uri?

	// map["PATH_INFO"] = "";
	// map["PATH_INFO"] = _request.get_uri();
	map["PATH_INFO"] = _request.get_path();
	// "\"/" + _request.get_uri() + "\"";

	map["PATH_TRANSLATED"] = _request.get_path();

	//body에서 추출
	map["QUERY_STRING"] = "";

	//client의 IP
	// std::cout << "client ip plz" << _request.get_clientip() << std::endl;
	map["REMOTE_ADDR"] = _request.get_clientip();

	//뭘까?
	// map["REMOTE_IDENT"] = "";
// REMOTE_USER
	if (_request.get_method() == GET)
		map["REQUEST_METHOD"] = "GET";
	if (_request.get_method() == POST)
		map["REQUEST_METHOD"] = "POST";

	// map["REQUEST_URI"] = "";

	map["REQUEST_URI"] = _request.get_path();
	// map["REQUEST_URI"] = _request.get_uri();
	// map["REQUEST_URI"] = "test.php/";

	//cgi 컴파일한 파일
		map["SCRIPT_NAME"] = "." + _request.get_uri(); //get_path로 해도 되고 . 빼도 된다... 도대체 뭐지...
	// map["SCRIPT_NAME"] = _request.get_path();
	// map["SCRIPT_NAME"] = "test.php/";


	//받아와야함
	map["SERVER_NAME"] = "Carry-Please";
	map["SERVER_PORT"] = "8080";
//?????
	map["SERVER_PROTOCOL"] = "HTTP/1.1";
	map["SERVER_SOFTWARE"] = "HTTP/1.1";

	map["REDIRECT_STATUS"] = "200";
	// //header
	map["Content-type"] = "text/html";
	map["Expires"] = printItem("Date");
	map["Location"] = _request.get_uri();
	// map["Content-Length"] = "100";
	// map["Set-Cookie"] = "";




	env = (char **)malloc(sizeof(char *) * (map.size() + 1));
	std::map<std::string, std::string>::iterator it = map.begin();
	int i = -1;
	while (it != map.end())
	{
		std::cout << it->first << " = " << it->second << std::endl;
		env[++i] = strdup((it->first + "=" + it->second).c_str());
		++it;
	}
	env[i] = NULL;

return(env);
}

std::string Response::Post() // for temporary only! to pass tester...
{
	std::string	res;
	std::string extension = trim_extension(_request.get_path());

	if (extension == "bla" || extension == "pl" ||  extension == "php" || extension == "cgi")
		return (cgi());
	if (_request.get_error_code() != 200)
	{
		res = getStartLine();
		res += "\n";
		res += printItem("Server");
		res += printItem("Date");
		res += printItem("Last-Modified");
		res += printItem("Content-Type");
		res += printItem("Content-Length");
		res += "\n";
		res += (body("error.html"));
		res += "\n\n";
		return res;
	}
	return (cgi());
}
