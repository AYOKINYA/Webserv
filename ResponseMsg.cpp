#include <ctime>
#include <iostream>
#include <sys/time.h>
#include <algorithm>
#include <map>
#include <string>

//template으로 만들까?
void getDate(std::map<std::string, std::string> &vars)
{
	struct timeval 	cur_time;
	struct tm 		time;
	char 			buf[128];
	std::string 	date;

	gettimeofday(&cur_time, 0);
	strptime(std::to_string(cur_time.tv_sec).c_str(), "%s", &time); // seconds in tm format
	// ft_itoa대신 std::to_string(cur_time.tv_sec).c_str()을 쓰면 free할 필요도 없고 간편해진다.
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S KST", &time); // tm to regexp format
	std::cout << buf << std::endl;

	vars.insert(std::pair<std::string, std::string>("Date", buf));
	
	for (std::map<std::string, std::string>::iterator it = vars.begin() ; it != vars.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}

void getContentType(std::map<std::string, std::string> &vars, const std::string &content)
{
	/* mime.types 참고. excel 텍스트 나누기로 split했다*/
	std::string extensions[104] = 
	{
		"xml",
		"gif",
		"jpeg", "jpg",

		"js",
		"atom",
		"rss",
		"mml",
		"txt",
		"jad",
		"wml",
		"htc",
		"png",
		"tif", "tiff",

		"wbmp",
		"ico",
		"jng",
		"bmp",
		"svg", "svgz",

		"webp",
		"woff",
		"jar", "war",	"ear",

		"json",
		"hqx",
		"doc",
		"pdf",
		"ps", "eps",	"ai",

		"rtf",
		"m3u8",
		"xls",
		"eot",
		"ppt",
		"wmlc",
		"kml",
		"kmz",
		"7z",
		"cco",
		"jardiff",
		"jnlp",
		"run",
		"pl", "pm",

		"prc", "pdb",

		"rar",
		"rpm",
		"sea",
		"swf",
		"sit",
		"tcl", "tk",

		"der", "pem",	"crt",

		"xpi",
		"xhtml",
		"xspf",
		"zip",
		"bin", "exe",	"dll",

		"deb",
		"dmg",
		"iso", "img",

		"msi","msp",	"msm",

		"docx",
		"xlsx",
		"pptx",
		"mid", "midi",	"kar",

		"mp3",
		"ogg",
		"m4a",
		"ra",
		"3gpp", "3gp",

		"ts",
		"mp4",
		"mpeg", "mpg",

		"mov",
		"webm",
		"flv",
		"m4v",
		"mng",
		"asx", "asf",

		"wmv",
		"avi"
	}

	std::string types[80] = 
	{
		"text/html", 
		"text/css",
		"text/xml",
		"image/gif",
		"image/jpeg",
		"application/javascript",
		"application/atom+xml",
		"application/rss+xml",
		"text/mathml",
		"text/plain",
		"text/vnd.sun.j2me.app-descriptor",
		"text/vnd.wap.wml",
		"text/x-component",
		"image/png",
		"image/tiff",
		"image/vnd.wap.wbmp",
		"image/x-icon",
		"image/x-jng",
		"image/x-ms-bmp",
		"image/svg+xml",
		"image/webp",
		"application/font-woff",
		"application/java-archive",
		"application/json",
		"application/mac-binhex40",
		"application/msword",
		"application/pdf",
		"application/postscript",
		"application/rtf",
		"application/vnd.apple.mpegurl",
		"application/vnd.ms-excel",
		"application/vnd.ms-fontobject",
		"application/vnd.ms-powerpoint",
		"application/vnd.wap.wmlc",
		"application/vnd.google-earth.kml+xml",
		"application/vnd.google-earth.kmz",
		"application/x-7z-compressed",
		"application/x-cocoa",
		"application/x-java-archive-diff",
		"application/x-java-jnlp-file",
		"application/x-makeself",
		"application/x-perl",
		"application/x-pilot",
		"application/x-rar-compressed",
		"application/x-redhat-package-manager",
		"application/x-sea",
		"application/x-shockwave-flash",
		"application/x-stuffit",
		"application/x-tcl",
		"application/x-x509-ca-cert",
		"application/x-xpinstall",
		"application/xhtml+xml",
		"application/xspf+xml",
		"application/zip",
		"application/octet-stream",
		"application/octet-stream",
		"application/octet-stream",
		"application/octet-stream",
		"application/octet-stream",
		"application/vnd.openxmlformats-officedocument.wordprocessingml.document",
		"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
		"application/vnd.openxmlformats-officedocument.presentationml.presentation",
		"audio/midi",
		"audio/mpeg",
		"audio/ogg",
		"audio/x-m4a",
		"audio/x-realaudio",
		"video/3gpp",
		"video/mp2t",
		"video/mp4",
		"video/mpeg",
		"video/quicktime",
		"video/webm",
		"video/x-flv",
		"video/x-m4v",
		"video/x-mng",
		"video/x-ms-asf",
		"video/x-ms-wmv",
		"video/x-msvideo"
	}


}

int main(void)
{
	//나중에 vars를 멤버변수로 하고, 파싱 함수를 멤버함수로 하는 클래스 Response를 만들면 좋을 것 같다.
	//그렇게 되면 각 멤버함수 인자에 vars 제거!

	//map with string key & string value
	std::map<std::string, std::string> vars;
	
	//Date	
	getDate(vars);

	//Content-Type
	getContentType(vars, "html");
}