/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:33 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/15 19:13:57 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

IRCServer::IRCServer(char **av) {
	init(av);
	serverManager(av);
}

IRCServer::~IRCServer() {}

void	IRCServer::init(char **av) {
	password = av[2];

	// set socket
	bzero((char*)&servAddr, sizeof(servAddr));
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

	if ((bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr))) < 0)
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

void	IRCServer::serverManager(char **av) {

	fds.push_back({serverSd, POLLIN, 0});

	while (true)
	{
		if ((poll(fds.data(), fds.size(), -1)) < 0)
		{
			std::cerr << "Error poll" << std::endl;
			break ;
		}

		// new incoming connection
		if (connectClient() == false)
			continue ;

		// check data from clients
		handleEvents();
	}
}

bool	IRCServer::connectClient() {
	sockaddr_in newSockAddress;
	socklen_t newSockAddressSize = sizeof(newSockAddress);

	if (fds[0].revents & POLLIN)
	{
		newSd = accept(serverSd, (struct sockaddr *)&newSockAddress, &newSockAddressSize);
		if (newSd < 0)
		{
			std::cerr << "Error accept" << std::endl;
			return false;
		}
		// welcome message to client
		newConnexionMsg(newSd);
		fds.push_back({newSd, POLLIN | POLLOUT, 0});
	}
	return (true);
}

void	IRCServer::handleEvents() {
	for (size_t i = 1; i < fds.size(); ++i)
	{
		nowFd = &i;
		if (fds[i].revents & POLLIN)
		{
			std::string	buf = getCompleteMsg(fds[i].fd, &i);
			if (!buf.empty())
			{
				// command from client
				std::cout << "Received from client " << i << ": " << buf << std::endl;
				t_cmd    test = parseCmd(buf);
				// privateMsg(test);
				std::cout << "Cmd: " << test.typeCmd << std::endl;
				std::cout << "Text: " << test.text << std::endl;
			}
		}
	}
}

void	IRCServer::newConnexionMsg(int sd) {
	std::cout << "new client connected" << std::endl;
	std::string input(getCompleteMsg(sd, NULL));
	std::cout << input << std::endl;
	User usr(sd);
	usr.parseInput(input);
	usr.printInfos();
	if (checkpassword(sd, usr) == false)
		return ;
	users[usr.getUserName()] = &usr;

	std::string msg001;
	msg001 = ":server 001 " + usr.getNickName() + " :Welcome to the Internet Relay Network, " + usr.getNickName() + "!\r\n";
	send(sd, (char*)msg001.c_str(), msg001.size(), 0);
}

bool	IRCServer::checkpassword(int sd, User client) {
	std::string msg;
	if (client.getPassWord().compare(password) != 0)
	{
		std::cout << "client try to connect with wrong password" << std::endl;
		msg = ":server 464 " + client.getNickName() + " :Password incorrect\r\n";
		send(sd, (char*)msg.c_str(), msg.size(), 0);
		return (false);
	}
	return (true);
}

std::string	IRCServer::getCompleteMsg(int sd, size_t *i) {
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
		{
			if (bytesread == 0)
				std::cout << "Client disconnected" << std::endl;
			else
				break ;
			close(sd);
			fds.erase(fds.begin() + *i);
			*i = *i - 1;
		}
		msg[bytesread] = '\0';
		received += msg;
		if (received.length() >= 2 && received.substr(received.length() - 2) == "/r/n")
			break;
	}
	return (received);
}

t_cmd    IRCServer::parseCmd(std::string buf) {
    size_t        posSpace = buf.find(" ");
    t_cmd        command;

    if (posSpace != std::string::npos)
    {
        command.typeCmd = buf.substr(0, posSpace);
        command.text = buf.substr(posSpace + 1, buf.length() - posSpace);
    }
    return command;
}

// void	IRCServer::privateMsg(t_cmd	priv)
// {
// 	size_t	posCol = priv.text.find(":", 0);
// 	std::string	msg;
// 	if (posCol != std::string::npos)
// 	{
// 		std::string	nick = priv.text.substr(0, posCol - 1);
// 		std::string	msg = priv.text.substr(posCol + 1, priv.text.length() - posCol);
// 		User	*sender = findUser(fds[*nowFd].fd);
// 		User	*receiver = findUser(nick);
// 		if (!receiver)
// 		{
// 			std::cout << "Client not found" << std::endl;
// 			return ;
// 		}
// 		msg = ":" + sender->getNickName() +"!~" + sender->getUserName() +"@localhost PRIVMSG " + receiver->getNickName() + " :" + msg + "\r\n";
// 		send(receiver->getSd(), (char*)msg.c_str(), msg.size(), 0);
// 	}
// 	return ;
// }

// User	*IRCServer::findUser(std::string nick)
// {
// 	std::map<std::string, User*>::iterator	it = users.find(nick);
// 	if (it != users.end())
// 		return (it->second);
// 	else
// 		return (NULL);
// }

// User	*IRCServer::findUser(int sd)
// {
// 	std::map<std::string, User*>::iterator	it = users.begin();
// 	while (it != users.end())
// 	{
// 		if (it->second->getSd() == sd)
// 			return (it->second);
// 		it++;
// 	}
// 	return (NULL);
// }
