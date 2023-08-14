#include "irc.hpp"

void	initServer(t_server *server, char **av)
{
	server->password = av[2];

	// set socket
	bzero((char*)&server->servAddr, sizeof(server->servAddr));
	server->servAddr.sin_family = AF_INET;
	server->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server->servAddr.sin_port = htons(atoi(av[1]));

	if ((server->serverSd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "Error Socket" << std::endl;
		exit(0);
	}

	int opt = true;

	if (setsockopt(server->serverSd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cerr << "Error setsockopt" << std::endl;
		exit(0);
	}

	if ((bind(server->serverSd, (struct sockaddr*) &server->servAddr, sizeof(server->servAddr))) < 0)
	{
		std::cerr << "Error bind" << std::endl;
		close(server->serverSd);
		exit(0);
	}

	// listen for 5 request at a time
	if ((listen(server->serverSd, 5)) < 0)
	{
		std::cerr << "error listen" << std::endl;
		close(server->serverSd);
		exit(0);
	}
}
