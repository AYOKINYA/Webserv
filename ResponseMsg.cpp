#include <ctime>
#include <iostream>
#include <sys/time.h>
#include <algorithm>
#include <map>
#include <string>

int main(void)
{
	//map with string key & string value
	std::map<std::string, std::string> vars;
	
	// DATE

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