#include "Handler.hpp"

void    Handler::exec_method(Client &client)
{
	std::string res = "";

	if (client._status == Client::START)
    {
        if (client._req.get_body().size() > (unsigned long)client._req.get_limit())
            client._req.set_error_code(413);
    }
    int method = client._req.get_method();

	if (method == GET)
		Get(client);
	else if (method == HEAD)
		Head(client);
	else if (method == POST)
		Post(client);
	else if (method == PUT)
		Put(client);
}

void Handler::setDate(Client &client)
{
	struct timeval 	cur_time;
	struct tm 		time;
	char 			buf[128];
	std::string 	date;

	gettimeofday(&cur_time, 0);
	strptime(std::to_string(cur_time.tv_sec).c_str(), "%s", &time); // seconds in tm format
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format

	client._res._header.insert(std::pair<std::string, std::string>("Date", buf));
}

void Handler::setContentType(Client &client)
{
	std::string content = client._req.get_conf()["path"];
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

	client._res._header.insert(std::pair<std::string, std::string>("Content-Type", res));

}

void Handler::setContentLocation(Client &client)
{
	std::string loc = client._req.get_conf()["path"];
    client._res._header.insert(std::pair<std::string, std::string>("Content-Location", loc));
}

void Handler::setContentLanguage(Client &client)
{
	client._res._header.insert(std::pair<std::string, std::string>("Content-Language", "en"));
}


void Handler::setAllow(Client &client)
{
	std::string str = client._req.get_conf()["method_allowed"];
	client._res._header.insert(std::pair<std::string, std::string>("Allow", str));

}

void Handler::setLastModified(Client& client)
{
	struct	stat info;
	struct	tm	time;
	char	buf[1024];

	std::string content = client._req.get_conf()["path"];
    stat(content.c_str(), &info);
	if (!S_ISDIR(info.st_mode))
		stat(client._req.get_conf()["error_page"].c_str(), &info);
	strptime(std::to_string(info.st_mtime).c_str(), "%s", &time);
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format

	client._res._header.insert(std::pair<std::string, std::string>("Last-Modified", buf));

}

void Handler::setWWWAuthentication(Client &client)
{
	client._res._header.insert(std::pair<std::string, std::string>("WWW-Authentication", "Basic"));
}

void Handler::setServer(Client &client)
{
	client._res._header.insert(std::pair<std::string, std::string>("Server", "Carry-Please"));
}

void Handler::setTransferEncoding(Client &client)
{
	client._res._header.insert(std::pair<std::string, std::string>("Transfer-Encoding", "identity"));
}

std::string Handler::getStartLine(Client &client)
{
	int code = client._req.get_error_code();
    std::string value = client._res._status_table.find(code)->second;
    
    std::string res = "HTTP/1.1 ";
	res += std::to_string(code);
	res += " ";
	res += value;

	return (res);
}

void Handler::Get (Client &client)
{
	std::string res;
	struct stat info;

	if (client._status == Client::START)
    {
		std::cout << client._req.get_conf()["path"]<< std::endl;
		stat(client._req.get_conf()["path"].c_str(), &info);
        if (S_ISDIR(info.st_mode) && client._req.get_conf()["autoindex"] == "on")
            client.autoidx_flag = 1;

        std::string file_extension = "." + trim_extension(client._req.get_conf()["path"]);
        std::vector<std::string> extensions = split(client._req.get_conf()["cgi_extension"], ' ');

        for (unsigned long i = 0; i < extensions.size(); i++)
        {
            if (extensions[i] == file_extension)
            {
                cgi(extensions[i], client);
                client._status = Client::CGI;
                return ;
            }
        }
		std::cout << client._req.get_error_code() << std::endl;
        if (client._req.get_error_code() == 200)
        {
            if (client.autoidx_flag == 1)
                client._res._body = autoindex(client);
            else
                client.read_fd = open(client._req.get_conf()["path"].c_str(), O_RDONLY);
        }
        else
            client.read_fd = open(client._req.get_conf()["error_page"].c_str(), O_RDONLY);
    
        client._status = Client::HEADERS;
        client.set_read_file(true);
        return ;
    }
    else if (client._status == Client::CGI)
    {
        parseCGIResult(client);
        client._status = Client::HEADERS;
        return ;
    }
    else if (client._status == Client::HEADERS)
    {
		client._res._start_line = getStartLine(client);

        if (client._req.get_error_code() == 401)
            setWWWAuthentication(client);
        setAllow(client);
        setDate(client);
        setLastModified(client);
        setServer(client);

        if (client.autoidx_flag == 1 || client._req.get_error_code() != 200)
            client._res._header["Content-Type"] = "text/html";
        else
            setContentType(client);
        client._status = Client::BODY;
        
    }
	else if (client._status == Client::BODY)
    {
		if (client.read_fd == -1)
        {
            client._res._header["Content-Length"] = std::to_string(client._res._body.size());
            create_response(client);
            client._status = Client::RESPONSE;
        }
    }
}

void Handler::Head(Client &client)
{
	std::string res;
	struct stat info;

	if (client._status == Client::START)
    {
		std::cout << client._req.get_conf()["path"]<< std::endl;
		stat(client._req.get_conf()["path"].c_str(), &info);
        if (S_ISDIR(info.st_mode) && client._req.get_conf()["autoindex"] == "on")
            client.autoidx_flag = 1;

		std::cout << client._req.get_error_code() << std::endl;
        if (client._req.get_error_code() == 200)
        {
            if (client.autoidx_flag == 1)
                client._res._body = autoindex(client);
            else
                client.read_fd = open(client._req.get_conf()["path"].c_str(), O_RDONLY);
        }
        else
            client.read_fd = open(client._req.get_conf()["error_page"].c_str(), O_RDONLY);
    
        client._status = Client::HEADERS;
        client.set_read_file(true);
        return ;
    }
    else if (client._status == Client::HEADERS)
    {
		client._res._start_line = getStartLine(client);

        if (client._req.get_error_code() == 401)
            setWWWAuthentication(client);
        setAllow(client);
        setDate(client);
        setLastModified(client);
        setServer(client);

        if (client.autoidx_flag == 1 || client._req.get_error_code() != 200)
            client._res._header["Content-Type"] = "text/html";
        else
            setContentType(client);
        client._status = Client::BODY;
    }
	else if (client._status == Client::BODY)
    {
		if (client.read_fd == -1)
        {
            client._res._header["Content-Length"] = std::to_string(client._res._body.size());
			client._res._body = '\n';
            create_response(client);
            client._status = Client::RESPONSE;
        }
    }
}

void Handler::Post (Client &client)
{
	std::string res;

	if (client._status == Client::START)
    {
		std::cout << client._req.get_conf()["path"]<< std::endl;

        std::string file_extension = "." + trim_extension(client._req.get_conf()["path"]);
        std::vector<std::string> extensions = split(client._req.get_conf()["cgi_extension"], ' ');

        for (unsigned long i = 0; i < extensions.size(); i++)
        {
            if (extensions[i] == file_extension)
            {
                cgi(extensions[i], client);
                client._status = Client::CGI;
                return ;
            }
        }
		std::cout << client._req.get_error_code() << std::endl;
        if (client._req.get_error_code() == 200)
		{
 			client.write_fd = open(client._req.get_conf()["path"].c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
			client.set_write_file(true);
		}
		else
		{
            client.read_fd = open(client._req.get_conf()["error_page"].c_str(), O_RDONLY);
			client.set_read_file(true);
		}
        client._status = Client::HEADERS;
        
        return ;
    }
    else if (client._status == Client::CGI)
    {
        parseCGIResult(client);
        client._status = Client::HEADERS;
		client._res._body = "File modified\n";
        return ;
    }
    else if (client._status == Client::HEADERS)
    {
		client._res._start_line = getStartLine(client);

        if (client._req.get_error_code() == 401)
            setWWWAuthentication(client);
        setAllow(client);
        setDate(client);
        setLastModified(client);
        setServer(client);

        if (client._req.get_error_code() != 200)
            client._res._header["Content-Type"] = "text/html";
        else
            setContentType(client);
        client._status = Client::BODY;
    }
	else if (client._status == Client::BODY)
    {
		if (client.read_fd == -1 && client.write_fd == -1)
        {
            client._res._header["Content-Length"] = std::to_string(client._res._body.size());
            create_response(client);
            client._status = Client::RESPONSE;
        }
    }
}


void	Handler::cgi(std::string extension, Client &client)
{
	char	**args;
	int		ret;
	int		tubes[2];
	std::string	res;
	struct stat	php;
	char **env = Env(client);

	args = (char **)(malloc(sizeof(char *) * 3));
	if (extension == ".php")
	{
		args[0] = ft_strdup("/usr/local/bin/php-cgi");
		args[1] = ft_strdup(client._req.get_conf()["path"].c_str());
	}
	else if (extension == client._req.get_conf()["cgi_test"])
	{
		args[0] = ft_strdup(client._req.get_conf()["cgi_exec"].c_str());
		args[1] = ft_strdup(client._req.get_conf()["path"].c_str());
	}
	else
	{
		args[0] = ft_strdup(client._req.get_conf()["path"].c_str());
		args[1] = NULL;
	}
	args[2] = NULL;

	client.tmp_fd = open("cgi.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	pipe(tubes);
	if ((client.cgi_pid = fork()) == 0)
	{
		close(tubes[1]);
		dup2(tubes[0], 0);
		dup2(client.tmp_fd, 1);
		if (stat(client._req.get_conf()["path"].c_str(), &php) != 0 ||
		!(php.st_mode & S_IFREG))
		{
			std::cerr << "Error CGI" << std::endl;
			exit(1);
		}
		
		if ((ret = execve(args[0], args, env)) == -1)
		{
			std::cerr << std::string(strerror(errno)) << std::endl;
			exit(1);
		}
	}
	else
	{	client.write_fd = tubes[1];
		client.read_fd = open("cgi.txt", O_RDONLY);
		close(tubes[0]);
        client.set_write_file(true);
        client.set_read_file(true);
	}

	ft_free(args);
	ft_free(env);
}

void		Handler::parseCGIResult(Client &client)
{
	size_t			pos;
	std::string		headers;
	std::string		key;
	std::string		value;
	std::string		tmp_status;

	std::string buf = client._res._body;

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
		client._req.set_error_code(ft_atoi(tmp_status.c_str()));
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

		client._res._header.insert(std::pair<std::string, std::string>(key, trim(value)));

		key.clear();
		value.clear();
		if (headers[pos] == '\r')
			pos++;
		if (headers[pos] == '\n')
			pos++;
	}
	pos = buf.find("\r\n\r\n") + 4;
	client._res._body = buf.substr(pos);

	key = "Content-Length";
	value = std::to_string(client._res._body.size());
	client._res._header.insert(std::pair<std::string, std::string>(key, value));

}

char	**Handler::Env(Client &client)
{
	char	**env = 0;
	std::map<std::string, std::string> map;

	map["CONTENT_LENGTH"] = std::to_string(client._req.get_body().size());
	map["CONTENT_TYPE"] = "text/html"; //client._res._header["Content-Type"];

	map["GATEWAY_INTERFACE"] = "CGI/1.1";
	map["PATH_INFO"] = client._req.get_conf()["path"];
	map["PATH_TRANSLATED"] = client._req.get_conf()["path"];
	if (client._req.get_uri().find('?') != std::string::npos)
		map["QUERY_STRING"] = client._req.get_uri().substr(client._req.get_uri().find('?') + 1);
	else
		map["QUERY_STRING"] = "";
	map["REMOTE_ADDR"] = client._req.get_client_ip();

	if (client._req.get_method() == GET)
		map["REQUEST_METHOD"] = "GET";
	if (client._req.get_method() == POST)
		map["REQUEST_METHOD"] = "POST";

	map["REQUEST_URI"] = client._req.get_conf()["path"];

	map["SCRIPT_NAME"] = "." + client._req.get_uri();

	map["SERVER_NAME"] = client._req.get_conf()["server_name"];
	map["SERVER_PORT"] = client._req.get_conf()["listen"];
	map["SERVER_PROTOCOL"] = "HTTP/1.1";
	map["SERVER_SOFTWARE"] = "webserv";

	map["REDIRECT_STATUS"] = "200";

	if (client._req._headers.find("Authorization") != client._req._headers.end())
	{
		size_t pos = client._req._headers["Authorization"].find(" ");
		map["AUTH_TYPE"] = client._req._headers["Authorization"].substr(0, pos);
		map["REMOTE_USER"] = client._req._headers["Authorization"].substr(pos + 1);
		map["REMOTE_IDENT"] = client._req._headers["Authorization"].substr(pos + 1);
	}

	std::map<std::string, std::string>::iterator b = client._req._headers.begin();
	while (b != client._req._headers.end())
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
		std::cout << env[i] << std::endl;
		++it;
	}
	env[i] = NULL;

	return (env);
}

void Handler::Put(Client &client)
{
	std::string res;
	std::string	url;

	url = client._req.get_conf()["path"];

	if (client._status == Client::START)
    {
		std::cout << url << std::endl;

		std::cout << client._req.get_error_code() << std::endl;
        client.write_fd = open(url.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0666);
    
        client._status = Client::HEADERS;
        client.set_write_file(true);
        return ;
    }
    else if (client._status == Client::HEADERS)
    {
		client._res._start_line = "HTTP/1.1 204 No Content\n";

        if (client._req.get_error_code() == 401)
            setWWWAuthentication(client);
        setDate(client);
        setContentLocation(client);
        setServer(client);

        client._status = Client::BODY;
    }
	else if (client._status == Client::BODY)
    {
		if (client.write_fd == -1)
        {
            //client._res._header["Content-Length"] = std::to_string(client._res._body.size());
			client._res._body = '\n';
            create_response(client);
            client._status = Client::RESPONSE;
        }
    }
}



std::string	Handler::autoindex(Client &client)
{
	DIR				*dir;
	struct dirent	*cur;
	std::string		res;

	dir = opendir(client._req.get_conf()["path"].c_str());
	res += "<html>\n<body>\n";
	res += "<h1>Directory listing</h1>\n";
	while ((cur = readdir(dir)) != NULL)
	{
		if (cur->d_name[0] != '.')
		{
			res += "<a href=\"" + client._req.get_uri();
			if (client._req.get_uri() != "/")
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


void Handler::create_response(Client& client)
{
    std::map<std::string, std::string>::iterator b;

	client._res_msg = client._res._start_line + "\r\n";
	b = client._res._header.begin();
	while (b != client._res._header.end())
	{
		if (b->second != "")
			client._res_msg += b->first + ": " + b->second + "\r\n";
		++b;
	}
	std::cout << client._res_msg << std::endl;
	client._res_msg += "\r\n";
	client._res_msg += client._res._body;
	//std::cout << client._res_msg << std::endl;
	client._res.clear();
}
