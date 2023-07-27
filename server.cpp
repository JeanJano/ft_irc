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

void	servConnectClient(t_server *server, char **av)
{
	sockaddr_in newSockAddress;
	socklen_t newSockAddressSize = sizeof(newSockAddress);

	server->fds.push_back({server->serverSd, POLLIN, 0});

	while (true)
	{
		if ((poll(server->fds.data(), server->fds.size(), -1)) < 0)
		{
			std::cerr << "Error poll" << std::endl;
			break ;
		}

		// new incoming connection
		if (server->fds[0].revents & POLLIN)
		{
			server->newSd = accept(server->serverSd, (struct sockaddr *)&newSockAddress, &newSockAddressSize);
			if (server->newSd < 0)
			{
				std::cerr << "Error accept" << std::endl;
				continue ;
			}
			checkpassword(server->newSd, av);
			std::cout << "new client connected" << std::endl;
			server->fds.push_back({server->newSd, POLLIN | POLLOUT, 0});
		}

		// check data from clients
		for (size_t i = 1; i < server->fds.size(); ++i)
		{
			if (server->fds[i].revents & POLLIN)
			{
				char buffer[BUFFER_SIZE];
				int bytesRead = recv(server->fds[i].fd, buffer, BUFFER_SIZE, 0);

				if (bytesRead <= 0)
				{
					if (bytesRead == 0)
						std::cout << "Client disconnected" << std::endl;
					else
						std::cerr << "Error recv" << std::endl;
					close(server->fds[i].fd);
					server->fds.erase(server->fds.begin() + i);
					--i;
				}
				else if (buffer[0] != '\0')
					std::cout << "Received from client " << i << ": " << buffer << std::endl;
			}
		}
	}
}

int main(int ac, char **av)
{
	t_server	server;

	initServer(&server, av);

	std::cout << "Waiting for the client to connect..." << std::endl;

	servConnectClient(&server, av);

	for (const auto& fd : server.fds)
		close(fd.fd);
	close(server.newSd);
	close(server.serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Connection closed..." << std::endl;
	return 0;
}