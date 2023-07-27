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
		close(server->serverSd);
		exit(0);
	}

	// listen for Max_Client request at a time
	if ((listen(server->serverSd, MAX_CLIENT)) < 0)
	{
		std::cerr << "error listen" << std::endl;
		close(server->serverSd);
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

	// receive request from client
	// need a new address to connect with the client
	sockaddr_in newSockAddress;
	socklen_t newSockAddressSize = sizeof(newSockAddress);

	// try connect several client

	std::vector<struct pollfd> fds;
	fds.push_back({server.serverSd, POLLIN, 0});
	int newSd;

	while (true)
	{
		int activity = poll(fds.data(), fds.size(), -1);
		if (activity < 0)
		{
			std::cerr << "Error poll" << std::endl;
			break ;
		}

		// new incoming connection
		if (fds[0].revents & POLLIN)
		{
			newSd = accept(server.serverSd, (struct sockaddr *)&newSockAddress, &newSockAddressSize);
			if (newSd < 0)
			{
				std::cerr << "Error accept" << std::endl;
				continue ;
			}
			std::cout << "new client connected" << std::endl;
			fds.push_back({newSd, POLLIN | POLLOUT, 0});
		}

		// check data from clients
		for (size_t i = 1; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				char buffer[BUFFER_SIZE];
				// memset(&buffer, 0, sizeof(buffer));
				int bytesRead = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);

				if (bytesRead <= 0)
				{
					if (bytesRead == 0)
						std::cout << "Client disconnected" << std::endl;
					else
						std::cerr << "Error recv" << std::endl;
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
				}
				else
					std::cout << "Received from client " << i << ": " << buffer << std::endl;
			}
		}
	}

	for (const auto& fd : fds)
		close(fd.fd);

	close(newSd);
	close(server.serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Connection closed..." << std::endl;
	return 0;
}