/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:33 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/29 12:30:08 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

IRCServer::IRCServer(char **av)
{
	init(av);
	serverManager();
}

IRCServer::~IRCServer() {}

void IRCServer::init(char **av)
{
	password = av[2];

	// set socket
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(av[1]));

	if ((serverSd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "Error Socket" << std::endl;
		exit(0);
	}

	int opt = true;

	if (setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cerr << "Error setsockopt" << std::endl;
		exit(0);
	}

	if ((bind(serverSd, (struct sockaddr *)&servAddr, sizeof(servAddr))) < 0)
	{
		std::cerr << "Error bind" << std::endl;
		close(serverSd);
		exit(0);
	}

	// listen for 5 request at a time
	if ((listen(serverSd, 5)) < 0)
	{
		std::cerr << "error listen" << std::endl;
		close(serverSd);
		exit(0);
	}
}

void IRCServer::serverManager()
{
	pollfd tmp;
	tmp.fd = serverSd;
	tmp.events = POLLIN;
	tmp.revents = 0;
	fds.push_back(tmp);

	while (true)
	{
		if ((poll(fds.data(), fds.size(), -1)) < 0)
		{
			std::cerr << "Error poll" << std::endl;
			break;
		}

		// new incoming connection
		if (connectClient() == false)
			continue;

		// check data from clients
		handleEvents();
	}
}

bool IRCServer::connectClient()
{
	sockaddr_in newSockAddress;
	socklen_t newSockAddressSize = sizeof(newSockAddress);
	pollfd	tmp;

	if (fds[0].revents & POLLIN)
	{
		newSd = accept(serverSd, (struct sockaddr *)&newSockAddress, &newSockAddressSize);
		if (newSd < 0)
		{
			std::cerr << "Error accept" << std::endl;
			return false;
		}
		// welcome message to client
		std::string input(getCompleteMsg(newSd));
		User usr(newSd);
		usr.parseInput(input);
		if (checkNewClient(newSd, usr))
		{
			newConnexionMsg(newSd, newSockAddress, usr);
			tmp.fd = newSd;
			tmp.events = POLLIN;
			tmp.revents = 0;
			fds.push_back(tmp);
		}
		else
		{
			close(newSd);
			return (false);
		}
	}
	return (true);
}
