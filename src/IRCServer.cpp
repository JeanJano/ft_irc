/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:33 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/18 18:06:55 by jsauvage         ###   ########.fr       */
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
		newConnexionMsg(newSd, newSockAddress);
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
				std::cout << buf << "|" << std::endl;
				parseCmd(buf);
				while (cmd.size() > 0)
					checkCmd(fds[i].fd);
			}
		}
	}
}

void	IRCServer::newConnexionMsg(int sd, sockaddr_in addr) {
	std::cout << "new client connected" << std::endl;
	std::string input(getCompleteMsg(sd, NULL));
	std::cout << input << std::endl;
	User usr(sd);
	usr.parseInput(input);
	if (checkpassword(sd, usr) == false)
		return ;
	usr.setIp(inet_ntoa(addr.sin_addr));
	std::cout << "addr ip: " << usr.getIp() << std::endl;
	users[usr.getNickName()] = usr;
	// for (std::map<std::string, User>::iterator it = users.begin(); it != users.end(); ++it)
	// 	it->second.printInfos();
	std::string msg001 = ":server 001 " + usr.getNickName() + " :Welcome to the Internet Relay Network, " + usr.getNickName() + "!\r\n";
	send(sd, (char*)msg001.c_str(), msg001.size(), 0);
}

bool	IRCServer::checkpassword(int sd, User client) {
	std::string msg;
	if (client.getPassWord().compare(password) != 0)
	{
		std::cout << "client try to connect with wrong password" << std::endl;
		msg = ":server 464 " + client.getNickName() + " :Password incorrect\r\n";
		send(sd, msg.c_str(), msg.size(), 0);
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

void    IRCServer::parseCmd(std::string buf) {
	std::istringstream	iss(buf);
	std::string	line;
	while (getline(iss, line)) {
		t_cmd	command;
		size_t	pos = line.find(' ');
        command.typeCmd = line.substr(0, pos);
        command.text = line.substr(pos + 1);
		cmd.push(command);
	}
}

void	IRCServer::checkCmd(int sd) {
	typedef void	(IRCServer::*functionPtr)(std::string, int);
	t_cmd	tmp = cmd.front();
	cmd.pop();

	std::string cmdArr[3] = {"JOIN", "PRIVMSG", "PING"};
	functionPtr	functPtr[3] = {&IRCServer::join, &IRCServer::privmsg, &IRCServer::ping};

	for (int i = 0; i < 3; i++) {
		if (tmp.typeCmd == cmdArr[i])
			(this->*functPtr[i])(tmp.text, sd);
	}
}

void	IRCServer::ping(std::string input, int sd) {
	std::string response = "PONG " + input + "\r\n";
	send(sd, response.c_str(), response.size(), 0);
}

void	IRCServer::join(std::string input, int sd) {
	std::istringstream iss(input);
	std::string	name;
	std::string pass;

	iss >> name >> pass;
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (!name.empty() && !it->first.empty() && it->first.compare(name) == 0) {
			it->second.setMember(findUserInstance(sd));
			return;
		}
	}
	Channel newChannel('#' + name, pass);
	newChannel.setMember(findUserInstance(sd));
	channels[name] = newChannel;
	std::cout << "Channels: " << std::endl;
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		std::cout << it->first << std::endl;
}

std::vector<User>	IRCServer::getChannelMembers(std::string name, std::string sender) {
	std::vector<User>	members;
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (it->first == name) {
			members = it->second.getMembers(sender);
			break;
		}
	}
	return (members);
}

std::vector<User>	IRCServer::getPrivateMember(std::string name) {
	std::vector<User> members;
	for (std::map<std::string, User>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->first == name) {
			members.push_back(it->second);
			break;
		}
	}
	return (members);
}


void	IRCServer::privmsg(std::string input, int sd) {
	char				col;
	std::string			msg;
	std::string			name;
	std::string			userMsg;
	std::vector<User>	members;
	std::istringstream	iss(input);
	User				sender = findUserInstance(sd);

	iss >> name >> col;
	getline(iss, userMsg);
	userMsg.resize(userMsg.size() - 1);
	size_t pos = name.find_first_of("+?#");
	if (pos == 0)
		members = getChannelMembers(name, sender.getNickName());
	else
		members = getPrivateMember(name);
	for (size_t i = 0; i < members.size(); i++) {
		msg = ":" + sender.getNickName() + "!" + sender.getUserName() + members[i].getIp() + " PRIVMSG " + name + " :" + userMsg + "\r\n";
		std::cout << members[i].getNickName() << std::endl;
		send(members[i].getSd(), msg.c_str(), msg.size(), 0);
	}
}

// void	IRCServer::privateMsg(t_cmd	priv)
// {
// 	size_t	posCol = priv.text.find(":", 0);
// 	std::string	msg;
// 	if (posCol != std::string::npos)
// 	{
// 		std::string	nick = priv.text.substr(0, posCol - 1);
// 		std::string	msg = priv.text.substr(posCol + 1, priv.text.length() - posCol);
// 		User	sender = findUser(fds[*nowFd].fd);
// 		User	receiver = findUser(nick);
// 		if (!receiver) {
// 			std::cout << "Client not found" << std::endl;
// 			return;
// 		}
// 		msg = ":" + sender.getNickName() +"!~" + sender.getUserName() +"@localhost PRIVMSG " + receiver.getNickName() + " :" + msg + "\r\n";
// 		send(receiver.getSd(), (char*)msg.c_str(), msg.size(), 0);
// 	}
// 	return ;
// }

// User	IRCServer::findUser(std::string nick)
// {
// 	std::map<std::string, User*>::iterator	it = users.find(nick);
// 	if (it != users.end())
// 		return (it->second);
// 	else
// 		return (NULL);
// }

User	IRCServer::findUserInstance(int sd)
{
	std::map<std::string, User>::iterator	it = users.begin();
	while (it != users.end())
	{
		if (it->second.getSd() == sd)
			return (it->second);
		it++;
	}
	User nul;
	return (nul);
}

std::string	IRCServer::findUserNickName(int sd)
{
	std::map<std::string, User>::iterator	it = users.begin();
	while (it != users.end())
	{
		if (it->second.getSd() == sd)
			return (it->first);
		it++;
	}
	return (NULL);
}