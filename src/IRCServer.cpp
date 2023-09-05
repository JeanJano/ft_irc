/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:33 by zel-kass          #+#    #+#             */
/*   Updated: 2023/09/05 11:42:16 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

IRCServer::IRCServer(char **av)
{
	std::time_t now;
	creation = std::ctime(&now);
    if (!creation.empty() && creation[creation.size() - 1] == '\n') {
        creation.resize(creation.size() - 1);
    }
	if (!init(av))
		return ;
	empty = new User;
	serverManager();
}

IRCServer::~IRCServer() {
	for(size_t i = 0; i < fds.size(); i++) {
		close(fds[i].fd);
	}
	close(serverSd);
	fds.clear();
	channels.clear();
	users.clear();
	while (!cmd.empty()) {
        cmd.pop(); // Remove the front element
    }
	if (empty)
		delete empty;
}

int IRCServer::init(char **av)
{
	password = av[2];

	// set socket
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(av[1]));
	
	if ((serverSd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cout << "Error Socket" << std::endl;
		return(0);
	}

	int opt = true;

	if (setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cout << "Error setsockopt" << std::endl;
		return(0);
	}
	if (fcntl(serverSd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << "Could not set server fd to non blocking" << std::endl;
		return(0);
	}
	if ((bind(serverSd, (struct sockaddr *)&servAddr, sizeof(servAddr))) < 0)
	{
		std::cout << "Error bind" << std::endl;
		close(serverSd);
		return(0);
	}

	// listen for 5 request at a time
	if ((listen(serverSd, 5)) < 0)
	{
		std::cout << "error listen" << std::endl;
		close(serverSd);
		return(0);
	}
	return (1);
}

void IRCServer::serverManager()
{
	pollfd tmp;
	tmp.fd = serverSd;
	tmp.events = POLLIN;
	tmp.revents = 0;
	fds.push_back(tmp);
	signal(SIGINT, signalHandler);
	while (run)
	{
		if ((poll(fds.data(), fds.size(), -1)) < 0)
		{
			std::cout << "Error poll" << std::endl;
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
			std::cout << "Error accept" << std::endl;
			return false;
		}
		if (fcntl(newSd, F_SETFL, O_NONBLOCK) < 0)
		{
			std::cout << "Could not set client fd to non blocking" << std::endl;
			close(newSd);
			return false;
		}
		// welcome message to client
		std::string input(getCompleteMsg(newSd));
		User usr(newSd);
		usr.parseInput(input);
		usr.printInfos();
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
	// else
	// 	return (false);
	return (true);
}
