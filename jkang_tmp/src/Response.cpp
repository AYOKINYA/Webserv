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
	if (pos != std::string::npos)
	{
		std::string ext = content.substr(pos + 1);
		for (int i = 0; i < 103 ; ++i)
		{
			if (ext == extensions[i])
				res = types[i];
		}
	}
	if (res.length() == 0)
		res = "text/plain";
	else
		setStatus(415);
	
	_vars_response.insert(std::pair<std::string, std::string>("Content-Type", res));
	
}

void Response::setContentLocation(const std::string &loc)
{
	_vars_response.insert(std::pair<std::string, std::string>("Content-Location", loc));
}

void Response::setContentLanguage()
{
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

	_vars_response.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(count)));
}


void Response::setAllow()
{
	//if METHOD does not match any of these, return 405 Not Allowed
	std::string str = "GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS";
	_vars_response.insert(std::pair<std::string, std::string>("Allow", str));

}

void Response::setLastModified(const std::string &content)
{
	struct	stat info;
	struct	tm	time;
	char	buf[1024];

	stat(content.c_str(), &info);
	strptime(std::to_string(info.st_mtime).c_str(), "%s", &time);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format

	_vars_response.insert(std::pair<std::string, std::string>("Last-Modified", buf));

}

void Response::setServer()
{
	_vars_response.insert(std::pair<std::string, std::string>("Server", "Carry-Please"));
}

void Response::setTransferEncoding()
{
	_vars_response.insert(std::pair<std::string, std::string>("Transfer-Encoding", "identity"));
}

void Response::setWWWAuthentication()
{
	_vars_response.insert(std::pair<std::string, std::string>("WWW-Authentication", "Basic"));
}

void Response::set_vars_response()
{
	std::string path = _request.get_path();
	
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

std::string Response::Get (void)
{
	std::string res = "";

	res += getStartLine();
	res += "\n";
	res += printItem("Server");
	res += printItem("Date");
	res += printItem("Last-Modified");
	res += printItem("Content-Type");
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

	res += getStartLine();
	res += "\n";
	res += printItem("Server");
	res += printItem("Date");
	res += printItem("Last-Modified");
	res += printItem("Content-Type");
	res += printItem("Content-Length");
	res += "\n";

	return (res);
}

std::string Response::exec_method()
{
	std::string res = "";

	int method = _request.get_method();

	if (method == GET)
		res = Get();
	else if (method == HEAD)
		res = Head();
	// ....

	return (res);
}
