/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:30:49 by smessal           #+#    #+#             */
/*   Updated: 2023/09/04 17:17:24 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

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

	std::string cmdArr[10] = {"JOIN", "PRIVMSG", "PING", "QUIT", "KICK", "TOPIC", "INVITE", "MODE", "PART", "NOTICE"};
	functionPtr functPtr[10] = {&IRCServer::join, &IRCServer::privmsg, &IRCServer::ping, &IRCServer::quit, &IRCServer::kick, &IRCServer::topic, &IRCServer::invite, &IRCServer::modeManager, &IRCServer::part, &IRCServer::notice};

	for (int i = 0; i < 10; i++)
	{
		if (tmp.typeCmd == cmdArr[i])
			(this->*functPtr[i])(tmp.text, sd);
	}
}


std::vector<User> IRCServer::getChannelMembers(std::string name)
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

User &IRCServer::findUserInstance(std::string nick)
{
	std::map<std::string, User>::iterator it = users.begin();
	while (it != users.end())
	{
		if (it->first == nick)
			return (it->second);
		it++;
	}
	return (*empty);
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
	for (size_t i = 0; i < members.size(); i++)
	{
		if (members[i].getNickName() == nick)
			return (true);
	}
	return (false);
}