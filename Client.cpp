#include "Server.hpp"
#include "Client.hpp"

int main(void)
{
	Client c("client", 8080);
	c.init_client();
	return (0);
}