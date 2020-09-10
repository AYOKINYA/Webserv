#include "Server.cpp"

int main()
{
	Server s("tmp", 8080);
	s.init_server();

	return (0);
}
