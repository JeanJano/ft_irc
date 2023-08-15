#include "irc.hpp"

std::string	getCompleteMsg(int sd)
{
	char		msg[1500];
	int			bytesread;
	std::string	received;

	struct timeval tv;
    tv.tv_sec = 1;  // 5 seconds timeout
    tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	while(true)
	{
		memset(&msg, 0, sizeof(msg));
		bytesread = recv(sd, msg, sizeof(msg), 0);
		if (bytesread <= 0)
			break ;
		msg[bytesread] = '\0';
		received += msg;
		if (received.length() >= 2 && received.substr(received.length() - 2) == "/r/n")
			break;
	}
	return (received);
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
			// welcome message to client
			welcomeMsg(server->newSd, server);
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
				{
					// command from client
					std::cout << "Received from client " << i << ": " << buffer << std::endl;
				}
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