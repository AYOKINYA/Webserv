#include "Response.hpp"

Response::Response() : _status(std::pair<int, std::string>(-1, "")), _start_line("")
{
	// req = NULL;
};

Response::Response(std::map<std::string, std::string> _vars_response)
	: _status(std::pair<int, std::string>(0, "")), _start_line(""), _vars_response(_vars_response)
{};

Response::Response(const Response &copy)
{
	*this = copy;
}

Response& Response::operator=(const Response &copy)
{
	if (this == &copy)
		return (*this);
	_status = copy._status;
	_start_line = copy._start_line;
	_vars_response = copy._vars_response;

	return (*this);
}

Response::~Response()
{};

void Response::method_put_exec()
{
	std::string url;
	std::ofstream ofs;
	std::string filename;

	url = req.get_path();
	if (req.get_putcheck() == 1) //파일이 없을때 새로 만든다
	{
		filename = trim_url(url);
		std::ofstream ofs(filename);
		ofs << req.get_body();
	}
	else //파일을 있을 때 오픈해서 내용을 지우고 새로 입력한다
	{
		ofs.open(url, std::ofstream::out | std::ofstream::trunc);
		ofs << req.get_body();
		ofs.close();
	}
	// 성공적인 파일 생성시 : 201 created
	// 성공적인 수정시 : 200 ok 204 no content
}

void Response::method_put_msg()
{
	std::string response_msg; //나중에 private 변수로 추가하거나 해야할듯
	(void)response_msg;
}

void Response::setStatus(std::pair<int, std::string> input)
{
	_status.first = input.first;
	_status.second = input.second;
}

std::string Response::getStartLine(void)
{
	//temporary
	setStatus(std::pair<int, std::string>(200, "OK"));

	_start_line = "HTTP/1.1 ";
	_start_line += std::to_string(_status.first);
	_start_line += " ";
	_start_line += _status.second;

	std::cout << _start_line << std::endl;
	return(_start_line);
}

void Response::getDate()
{
	struct timeval 	cur_time;
	struct tm 		time;
	char 			buf[128];
	std::string 	date;

	gettimeofday(&cur_time, 0);
	strptime(std::to_string(cur_time.tv_sec).c_str(), "%s", &time); // seconds in tm format
	// ft_itoa대신 std::to_string(cur_time.tv_sec).c_str()을 쓰면 free할 필요도 없고 간편해진다.
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format
	_vars_response.insert(std::pair<std::string, std::string>("Date", buf));
}

void Response::getContentType(const std::string &content)
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
	//else, 415 Unsupported Media Type Error must be thrown.

	_vars_response.insert(std::pair<std::string, std::string>("Content-Type", res));

}

void Response::getContentLocation(const std::string &loc)
{
	_vars_response.insert(std::pair<std::string, std::string>("Content-Location", loc));
}

void Response::getContentLanguage()
{
	_vars_response.insert(std::pair<std::string, std::string>("Content-Language", "en"));
}

void Response::getContentLength(const std::string &content)
{
	// 일단 요청하는 컨텐트가 없다고 가정! 기본 index.html 읽는다.
	// 올바른 메소드가 아니면 error.html 읽어서 내보내고
	const char *dir = content.c_str();
	int fd = open(dir, O_RDWR, 0644);
	int count = 0;
	if (fd > 0)
	{
		char buf[2];
		buf[0] = 0;
		buf[1] = 0;
		while (read(fd, buf, 1) > 0)
			++count;
	}
	else
	{
		std::cout << "404 Not Found Error should be thrown." << std::endl;
		return ;
	}

	_vars_response.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(count)));
}


void Response::getAllow()
{
	//if METHOD does not match any of these, return 405 Not Allowed
	std::string str = "GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS";
	_vars_response.insert(std::pair<std::string, std::string>("Allow", str));

}

void Response::getLastModified(const std::string &content)
{
	struct	stat info;
	struct	tm	time;
	char	buf[1024];

	stat(content.c_str(), &info);
	strptime(std::to_string(info.st_mtime).c_str(), "%s", &time);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format

	_vars_response.insert(std::pair<std::string, std::string>("Last-Modified", buf));

}

void Response::getServer()
{
	_vars_response.insert(std::pair<std::string, std::string>("Server", "Carry-Please"));
}

void Response::getTransferEncoding()
{
	_vars_response.insert(std::pair<std::string, std::string>("Transfer-Encoding", "identity"));
}

void Response::getWWWAuthentication()
{
	_vars_response.insert(std::pair<std::string, std::string>("WWW-Authentication", "Basic"));
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
		std::cout << it->first << ": " << it->second << std::endl;
	}

	return(res);
}

std::string Response::header(const std::string &path)
{

	getServer();
	getDate();
	getAllow();
	getContentType(path);
	getContentLocation(path);
	getTransferEncoding();
	getContentLength(path); // Request에서 파싱한 거 거의 그대로 넣으면 될 듯?
	getLastModified(path); // Request에서 파싱한 거 거의 그대로 넣으면 될 듯?
	getWWWAuthentication(); // when status is 401

	std::string res = "";

	res += printItem("Server");
	res += printItem("Date");
	res += printItem("Last-Modified");
	res += printItem("Content-Type");
	res += printItem("Content-Length");
	return (res);
}

std::string Response::body(const std::string &path)
{
	std::string res = "";
	int fd = open(path.c_str(), O_RDWR, 0644);
	if (fd == -1)
		std::cout << "Error MUST be thrown!" << std::endl;
	char buf[2];
	int nread;
	buf[0] = 0;
	buf[1] = 0;
	while ((nread = read(fd, buf, 1)) > 0)
		res += buf;
	close(fd);
	return (res);
}
