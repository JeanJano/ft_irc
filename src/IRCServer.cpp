/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:33 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/28 11:30:10 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

IRCServer::IRCServer(char **av)
{
	init(av);
	serverManager(av);
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

void IRCServer::serverManager(char **av)
{

	fds.push_back({serverSd, POLLIN, 0});

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

	if (fds[0].revents & POLLIN)
	{
		newSd = accept(serverSd, (struct sockaddr *)&newSockAddress, &newSockAddressSize);
		if (newSd < 0)
		{
			std::cerr << "Error accept" << std::endl;
			return false;
		}
		// welcome message to client
		std::string input(getCompleteMsg(newSd, NULL));
		User usr(newSd);
		usr.parseInput(input);
		if (checkNewClient(newSd, usr))
		{
			newConnexionMsg(newSd, newSockAddress, usr);
			fds.push_back({newSd, POLLIN | POLLOUT, 0});
		}
		else
		{
			close(newSd);
			return (false);
		}
	}
	return (true);
}

void IRCServer::handleEvents()
{
	for (size_t i = 1; i < fds.size(); ++i)
	{
		nowFd = &i;
		if (fds[i].revents & POLLIN)
		{
			std::string buf = getCompleteMsg(fds[i].fd, &i);
			if (!buf.empty())
			{
				std::cout << buf << std::endl;
				parseCmd(buf);
				while (cmd.size() > 0)
					treatCmd(fds[i].fd);
			}
		}
	}
}

void IRCServer::newConnexionMsg(int sd, sockaddr_in addr, User &usr)
{
	std::cout << "New client connected" << std::endl;
	usr.setIp(inet_ntoa(addr.sin_addr));
	std::cout << "addr ip: " << usr.getIp() << std::endl;
	users[usr.getNickName()] = usr;
	std::string msg001 = ":server 001 " + usr.getNickName() + " :Welcome to the Internet Relay Network, " + usr.getNickName() + "!\r\n";
	send(sd, (char *)msg001.c_str(), msg001.size(), 0);
}

bool IRCServer::checkNewClient(int sd, User client)
{
	std::string msg;
	if (client.getPassWord().compare(password) != 0)
	{
		std::cout << "client try to connect with wrong password" << std::endl;
		msg = ":server 464 " + client.getNickName() + " :Password incorrect\r\n";
		std::cout << msg << std::endl;
		send(sd, msg.c_str(), msg.size(), 0);
		return (false);
	}
	if (nickIsUsed(client.getNickName()))
	{
		std::cout << "client try to connect with used nickname" << std::endl;
		msg = ":server 433 " + client.getNickName() + " :Nickname is already in use\r\n";
		std::cout << msg << std::endl;
		send(sd, msg.c_str(), msg.size(), 0);
		return (false);
	}
	return (true);
}

std::string IRCServer::getCompleteMsg(int sd, size_t *i)
{
	char msg[1500];
	int bytesread;
	std::string received;

	struct timeval tv;
	tv.tv_sec = 1; // 5 seconds timeout
	tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
	while (true)
	{
		memset(&msg, 0, sizeof(msg));
		bytesread = recv(sd, msg, sizeof(msg), 0);
		if (bytesread < 0)
			break;
		msg[bytesread] = '\0';
		received += msg;
		if (received.length() >= 2 && received.substr(received.length() - 2) == "/r/n")
			break;
	}
	return (received);
}

void IRCServer::parseCmd(std::string buf)
{
	std::istringstream iss(buf);
	std::string line;
	while (getline(iss, line))
	{
		t_cmd command;
		size_t pos = line.find(' ');
		command.typeCmd = line.substr(0, pos);
		command.text = line.substr(pos + 1);
		cmd.push(command);
	}
}

void IRCServer::treatCmd(int sd)
{
	typedef void (IRCServer::*functionPtr)(std::string, int);
	t_cmd tmp = cmd.front();
	cmd.pop();

	std::string cmdArr[7] = {"JOIN", "PRIVMSG", "PING", "QUIT", "KICK", "TOPIC", "INVITE"};
	functionPtr functPtr[7] = {&IRCServer::join, &IRCServer::privmsg, &IRCServer::ping, &IRCServer::quit, &IRCServer::kick, &IRCServer::topic, &IRCServer::invite};

	for (int i = 0; i < 7; i++)
	{
		if (tmp.typeCmd == cmdArr[i])
			(this->*functPtr[i])(tmp.text, sd);
	}
}

void IRCServer::ping(std::string input, int sd)
{
	std::string response = "PONG " + input + "\r\n";
	send(sd, response.c_str(), response.size(), 0);
}

void IRCServer::join(std::string input, int sd)
{
	std::istringstream iss(input);
	std::string name;
	std::string pass;

	iss >> name >> pass;
	if (channels.find(name) != channels.end())
	{
		std::stringstream ss;
		ss << channels[name].getTimeStamp();
		User	newUser = findUserInstance(sd);
		std::cout << "Channel exists !" << std::endl;
		channels[name].addUser(newUser);
		std::string msg332 = ":server 332 " + newUser.getNickName() + " " +channels[name].getName() + " :" + channels[name].getTopic() + "\r\n";
		std::string msg333 = ":server 333 " + newUser.getNickName() + " " +channels[name].getName() + " " + newUser.getNickName() + "!" + newUser.getUserName() + "@" + newUser.getIp() + " " + ss.str() + "\r\n";
		send(newUser.getSd(), msg332.c_str(), msg332.size(), 0);
		send(newUser.getSd(), msg333.c_str(), msg333.size(), 0);
	}
	else
	{
		std::cout << "Channel created !" << std::endl;
		Channel newChannel(name, pass);
		channels[name] = newChannel;
		channels[name].addUser(findUserInstance(sd));
	}
	std::cout << "Channels: " << std::endl;
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		std::cout << it->first << std::endl;
}

std::vector<User> IRCServer::getChannelMembers(std::string name, std::string sender)
{
	std::map<std::string, Channel>::iterator it = channels.begin();
	std::vector<User>							empty;
	
	while (it != channels.end())
	{
		if (it->first == name)
		{
			return (it->second.getMembers());
		}
		it++;
	}
	return (empty);
}

std::vector<User> IRCServer::getPrivateMember(std::string name)
{
	std::vector<User> members;
	for (std::map<std::string, User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->first == name)
		{
			members.push_back(it->second);
			break;
		}
	}
	return (members);
}

void IRCServer::privmsg(std::string input, int sd)
{
	char col;
	std::string msg;
	std::string name;
	std::string userMsg;
	std::vector<User> members;
	std::istringstream iss(input);
	User sender = findUserInstance(sd);

	iss >> name >> col;
	getline(iss, userMsg);
	userMsg.resize(userMsg.size() - 1);
	size_t pos = name.find_first_of("+?#");
	if (pos == 0)
		members = getChannelMembers(name, sender.getNickName());
	else
		members = getPrivateMember(name);
		
	// Check errors, user unknown, channel unknown, user not in channel
	if (pos == 0 && !userInChannel(members, sender.getNickName()))
	{
		reply(sender.getSd(), ":server " + ERR_NOTONCHANNEL(sender.getNickName(), name));
		return ;
	}
	std::vector<User>::iterator it = members.erase(std::remove(members.begin(), members.end(), sender), members.end());
	if (members.empty())
	{
		reply(sender.getSd(), ":server " + ERR_NOSUCHNICK(sender.getNickName(), name));
		return ;
	}
	for (size_t i = 0; i < members.size(); i++)
		reply(members[i].getSd(), RPL_PRIVMSG(sender.getNickName() + "!" + sender.getUserName() + members[i].getIp(), name, userMsg));
}

void IRCServer::quit(std::string input, int sd)
{
	User quit = findUserInstance(sd);

	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::vector<User> cmp = it->second.getMembers();
		for (size_t i = 0; i < cmp.size(); i++)
		{
			if (quit.getNickName() == cmp[i].getNickName())
				it->second.removeUser(quit);
		}
	}
	close(sd);
	fds.erase(fds.begin() + *nowFd);
	*nowFd = *nowFd - 1;
	std::cout << "Client: " << quit.getNickName() << " disconnected" << std::endl;
	users.erase(quit.getNickName());
}

void IRCServer::kick(std::string input, int sd)
{
	std::istringstream iss(input);
	std::string channelName;
	std::string kicked;

	iss >> channelName >> kicked;

	std::map<std::string, Role *> mode = channels[channelName].getMode();
	User kicker = findUserInstance(sd);

	mode[kicker.getNickName()]->kick(kicked, channels[channelName]);

	// std::cout << "Channel: " << channelName << " after" << std::endl;
	// for (size_t i = 0; i < channels[channelName].getMembers().size(); i++)
	// 	std::cout << channels[channelName].getMembers()[i].getNickName() << std::endl;
	// std::cout << std::endl;
}

void	IRCServer::topic(std::string input, int sd) {
	std::istringstream	iss(input);
	std::string			channelName;
	std::string			topic;
	char				col;

	iss >> channelName >> col;
	getline(iss, topic);
	topic.resize(topic.size() - 1);

	std::map<std::string, Role *> mode = channels[channelName].getMode();
	User sender = findUserInstance(sd);

	mode[sender.getNickName()]->topic(topic, channels[channelName]);
}

void	IRCServer::invite(std::string input, int sd)
{
	std::istringstream	iss(input);
	std::string			nick;
	std::string			channelName;

	iss >> nick >> channelName;
	// Need to Check existence of channel
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	User sender = findUserInstance(sd);
	User receiver = findUserInstance(nick);
	std::cout << receiver.getNickName() << std::endl;
	// Pourquoi ca marche si ca rentre dans le premiere condition mais pas dans la deuxieme
	// LE MESSAGE EST LE MEEEEEEME
	if (it == channels.end() || receiver.getNickName() == "default")
	{
		reply(sender.getSd(), ":server " + ERR_NOSUCHNICK(sender.getNickName(), channelName));
		return ;
	}
	std::map<std::string, Role *> mode = channels[channelName].getMode();
	mode[sender.getNickName()]->invite(receiver);
}

User &IRCServer::findUserInstance(int sd)
{
	std::map<std::string, User>::iterator it = users.begin();
	while (it != users.end())
	{
		if (it->second.getSd() == sd)
			return (it->second);
		it++;
	}
	return (it->second);
}

User IRCServer::findUserInstance(std::string nick)
{
	User	empty;
	std::map<std::string, User>::iterator it = users.begin();
	while (it != users.end())
	{
		if (it->first == nick)
			return (it->second);
		it++;
	}
	return (empty);
}

std::string IRCServer::findUserNickName(int sd)
{
	std::map<std::string, User>::iterator it = users.begin();
	while (it != users.end())
	{
		if (it->second.getSd() == sd)
			return (it->first);
		it++;
	}
	std::string empty = "";
	return (empty);
}

bool IRCServer::nickIsUsed(std::string nickname)
{
	std::map<std::string, User>::iterator it = users.begin();
	while (it != users.end())
	{
		if (it->first == nickname)
			return (1);
		it++;
	}
	return (0);
}

bool	IRCServer::userInChannel(std::vector<User> &members, std::string nick)
{
	for (int i = 0; i < members.size(); i++)
	{
		if (members[i].getNickName() == nick)
			return (true);
	}
	return (false);
}