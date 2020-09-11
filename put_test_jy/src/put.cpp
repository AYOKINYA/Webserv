#include <iostream>
#include <string>
#include <fstream>

std::string trim_url(const std::string& str)
{
    size_t first = str.find_last_of('/');

    // if (std::string::npos == first)
        // return str;
    size_t last = str.find_last_of(' ');
    return (str.substr(first + 1, (last - first + 1)));
}

void method_put()
{
	std::string url;
	std::string filename;
	std::ofstream ofs;

	url = "/Users/jiyoonhur/Webserv/put_test_jy/src/index.html";
	filename = trim_url(url);
	/*
	remove(url.c_str());
	std::ofstream ofs(url);
	ofs << "this will be the contents from body";
	*/
	ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
	ofs << "this will be the contents from body!!!!!!!!!!";
	ofs.close();
}

int main()
{
	method_put();
}
