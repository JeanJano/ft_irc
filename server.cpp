#include "irc.hpp"

void	checkpassword(int newSd, char **av)
{
	char msg[1500];
	memset(&msg, 0, sizeof(msg));
	recv(newSd, (char*)&msg, sizeof(msg), 0);
	int checkpass = 0;
	if (strcmp(msg, av[2]))
	{
		std::cout << "client try to connect with wrong password" << std::endl;
		memset(&msg, 0, sizeof(msg));
		strcpy(msg, "Error");
		send(newSd, (int*)&checkpass, sizeof(checkpass), 0);
	}
	checkpass = 1;
	memset(&msg, 0, sizeof(msg));
	send(newSd, (char*)&checkpass, sizeof(checkpass), 0);
}

void	initServer(t_server *server, char **av)
{
	// set socket
	bzero((char*)&server->servAddr, sizeof(server->servAddr));
	server->servAddr.sin_family = AF_INET;
	server->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server->servAddr.sin_port = htons(atoi(av[1]));

	server->serverSd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->serverSd < 0)
	{
		std::cerr << "Error Socket" << std::endl;
		exit(0);
	}

	// bind the socket to its local address
	server->bindStatus = bind(server->serverSd, (struct sockaddr*) &server->servAddr, sizeof(server->servAddr));
	if (server->bindStatus < 0)
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

	// listen for 5 request at a time
	listen(server.serverSd, 5);

	// receive request from client
	// need a new address to connect with the client
	sockaddr_in newSockAddress;
	socklen_t newSockAddressSize = sizeof(newSockAddress);

	// accept, create a new socket descriptor to
	// handle the new connection with the client
	int newSd = accept(server.serverSd, (sockaddr*)&newSockAddress, &newSockAddressSize);
	if (newSd < 0)
	{
		std::cerr << "Error accepting request from client" << std::endl;
		exit(1);
	}
	std::cout << "Connected with client" << std::endl;

	// check password of the client
	checkpassword(newSd, av);

	while (1)
	{
		// receive a message from the client (listen)
		std::cout << "Awaiting client response.." << std::endl;
		memset(&msg, 0, sizeof(msg));
		recv(newSd, (char*)&msg, sizeof(msg), 0);
		std::cout << "Client: " << msg << std::endl;
		if (!strcmp(msg, "exit"))
		{
			std::cout << "Client has quit the session" << std::endl;
			break ;
		}
	}
	close(newSd);
	close(server.serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Connection closed..." << std::endl;
    return 0;
}