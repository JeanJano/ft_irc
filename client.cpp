#include "irc.hpp"

void	checkpassword(t_client* client, char **av)
{
	char msg[1500];

	memset(&msg, 0, sizeof(msg));
	strcpy(msg, av[2]);
	send(client->clientSd, (char*)&msg, strlen(msg), 0);
	memset(&msg, 0, sizeof(msg));
	int checkpass;
	recv(client->clientSd, (int*)&checkpass, sizeof(checkpass), 0);
	std::cout << "msg: " << checkpass << std::endl;
	if (checkpass == 0)
	{
		std::cerr << "Wrong password" << std::endl;
		exit(0);
	}
}

void	initClient(t_client *client, char **av)
{
	struct hostent* host = gethostbyname("127.0.0.1");
	bzero((char *)&client->sendSockAddr, sizeof(client->sendSockAddr));
	client->sendSockAddr.sin_family = AF_INET;
	client->sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
	client->sendSockAddr.sin_port = htons(atoi(av[1]));
	client->clientSd = socket(AF_INET, SOCK_STREAM, 0);

	// try to connect
	int status = connect(client->clientSd, (sockaddr*)&client->sendSockAddr, sizeof(client->sendSockAddr));
	if (status < 0)
	{
		std::cerr << "Error connecting to socket" << std::endl;
		exit(1);
	}
}

int main(int ac, char **av)
{
	t_client	client;
	char		msg[1500];

	initClient(&client, av);

	std::cout << "Connected to the server" << std::endl;

	// Connection with password
	// checkpassword(&client, av);

	// memset(&msg, 0, sizeof(msg));
	// recv(client.clientSd, (char*)&msg, sizeof(msg), 0);
	// std::cout << msg << std::endl;

	while (1)
	{
		std::cout << "> ";
		std::string data;
		std::getline(std::cin, data);
		memset(&msg, 0, sizeof(msg));
		strcpy(msg, data.c_str());
		if (data == "exit")
		{
			send(client.clientSd, (char *)&msg, strlen(msg), 0);
			break ;
		}
		send(client.clientSd, (char *)&msg, sizeof(msg), 0);
	}
	close(client.clientSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Connection closed" << std::endl;
}
