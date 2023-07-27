#include "irc.hpp"

void	checkpassword(int sd, char **av)
{
	char msg[1500];
	memset(&msg, 0, sizeof(msg));
	recv(sd, (char*)&msg, sizeof(msg), 0);
	int checkpass = 0;
	if (strcmp(msg, av[2]))
	{
		std::cout << "client try to connect with wrong password" << std::endl;
		memset(&msg, 0, sizeof(msg));
		strcpy(msg, "Error");
		send(sd, (int*)&checkpass, sizeof(checkpass), 0);
	}
	checkpass = 1;
	memset(&msg, 0, sizeof(msg));
	send(sd, (char*)&checkpass, sizeof(checkpass), 0);

	memset(&msg, 0, sizeof(msg));
	strcpy(msg, "Welcome");
	send(sd, msg, strlen(msg), 0);
	std::cout << "Welcome message send" << std::endl;
}

void	initServer(t_server *server, char **av)
{
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

	// bind the socket to its local address
	if ((bind(server->serverSd, (struct sockaddr*) &server->servAddr, sizeof(server->servAddr))) < 0)
	{
		std::cerr << "Error bind" << std::endl;
		exit(0);
	}
}

int main(int ac, char **av)
{
	t_server	server;
	char		msg[1500];

	initServer(&server, av);

	std::cout << "Waiting for the client to connect..." << std::endl;

	// std::cout << "new client try to connect" << std::endl;

	// listen for 5 request at a time
	if ((listen(server.serverSd, 3)) < 0)
	{
		std::cerr << "error listen" << std::endl;
		exit(0);
	}

	// receive request from client
	// need a new address to connect with the client
	sockaddr_in newSockAddress;
	socklen_t newSockAddressSize = sizeof(newSockAddress);

	// try connect several client

	int max_sd, sd, activity, newSd;

	while (true)
	{
		FD_ZERO(&server.readfds);
		FD_SET(server.serverSd, &server.readfds);
		max_sd = server.serverSd;

		//wait for an activity on one of the sockets
		activity = select(max_sd + 1, &server.readfds, NULL, NULL, NULL);
		if (activity < 0)
			std::cout << "Error select" << std::endl;
		
		if ((newSd = accept(server.serverSd, (struct sockaddr *)&newSockAddress, &newSockAddressSize)) < 0)
		{
			std::cerr << "Error accept" << std::endl;
			exit(0);
		}

		checkpassword(newSd, av);

		memset(&msg, 0, sizeof(msg));
		recv(newSd, (char*)&msg, sizeof(msg), 0);
		std::cout << "Client: " << msg << std::endl;
	}

	close(newSd);
	close(server.serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Connection closed..." << std::endl;
	return 0;
}