#include "irc.hpp"

// void	checkpassword(int sd, char **av)
// {
// 	char msg[1500];
// 	memset(&msg, 0, sizeof(msg));
// 	recv(sd, (char*)&msg, sizeof(msg), 0);
// 	int checkpass = 0;
// 	std::cout << "pass recv: " << msg << std::endl;
// 	std::cout << "pass send: " << av[2] << std::endl;
// 	if (strcmp(msg, av[2]))
// 	{
// 		std::cout << "client try to connect with wrong password" << std::endl;
// 		memset(&msg, 0, sizeof(msg));
// 		strcpy(msg, "Error");
// 		send(sd, (int*)&checkpass, sizeof(checkpass), 0);
// 	}
// 	checkpass = 1;
// 	memset(&msg, 0, sizeof(msg));
// 	send(sd, (char*)&checkpass, sizeof(checkpass), 0);

// 	memset(&msg, 0, sizeof(msg));
// 	strcpy(msg, "Welcome");
// 	send(sd, msg, strlen(msg), 0);
// 	std::cout << "Welcome message send" << std::endl;
// }

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
			// checkpassword(server->newSd, av);
			std::cout << "new client connected" << std::endl;
			// welcome message to client
			char msg[1500];
			memset(&msg, 0, sizeof(msg));
			recv(server->newSd, (char*)msg, sizeof(msg), 0);
			std::cout << msg << std::endl;

			memset(&msg, 0, sizeof(msg));
			strcpy(msg, ":server 001 jsauvage :Welcome to the Internet Relay Network, jsauvage!\r\n");
			send(server->newSd, (char*)msg, sizeof(msg), 0);

			server->fds.push_back({server->newSd, POLLIN | POLLOUT, 0});
		}

		// check data from clients
		for (size_t i = 1; i < server->fds.size(); ++i)
		{
			if (server->fds[i].revents & POLLIN)
			{
				// char buffer[BUFFER_SIZE];
				// int bytesRead = recv(server->fds[i].fd, buffer, BUFFER_SIZE, 0);

				// if (bytesRead <= 0)
				// {
				// 	if (bytesRead == 0)
				// 		std::cout << "Client disconnected" << std::endl;
				// 	else
				// 		std::cerr << "Error recv" << std::endl;
				// 	close(server->fds[i].fd);
				// 	server->fds.erase(server->fds.begin() + i);
				// 	--i;
				// }
				// else if (buffer[0] != '\0')
				// 	std::cout << "Received from client " << i << ": " << buffer << std::endl;
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