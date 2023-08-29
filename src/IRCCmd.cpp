/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCCmd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:27:52 by smessal           #+#    #+#             */
/*   Updated: 2023/08/29 14:36:52 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

void IRCServer::ping(std::string input, int sd)
{
	std::string response = "PONG " + input;
	reply(sd, response);
}

void IRCServer::join(std::string input, int sd)
{
	std::istringstream iss(input);
	User	newUser = findUserInstance(sd);
	std::string name;
	std::string pass;

	iss >> name >> pass;
	if (channels.find(name) != channels.end())
	{
		channels[name].addUser(newUser);
	} else {
		Channel newChannel(name, pass);
		channels[name] = newChannel;
		channels[name].addUser(newUser);
	}
	std::vector<User>	members = channels[name].getMembers();
	std::map<std::string, Role *> mode = channels[name].getRole();
	std::string			names;
	for (size_t i = 0; i < members.size(); i++) {
		names += mode[members[i].getNickName()]->getNickName();
		if (i < members.size() - 1)
			names += " ";
	}
	// std::cout << "Names: " << names <<"i"<<std::endl;
	for (size_t i = 0; i < members.size(); ++i) {
		// std::cout << "CA PASSE" << std::endl;
		std::string	joinMsg = ":" + newUser.getNickName() + "!" + newUser.getUserName() + "@" + newUser.getIp() + " JOIN" + " :" + channels[name].getName();
		reply(members[i].getSd(), joinMsg);
	}
	if (channels[name].getTopic() != "default") {
		std::stringstream ss;
		ss << channels[name].getTimeStamp();
		std::string msg333 = ":server 333 " + newUser.getNickName() + " " + channels[name].getName() + " " + newUser.getNickName() + "!" + newUser.getUserName() + "@" + newUser.getIp() + " " + ss.str() + "\r\n";
		reply(newUser.getSd(), RPL_TOPIC(newUser.getNickName(), channels[name].getName(), channels[name].getTopic()));
		reply(newUser.getSd(), msg333);
	} else
		reply(newUser.getSd(), RPL_NOTOPIC(newUser.getNickName(), channels[name].getName()));
	reply(sd, RPL_NAMREPLY(newUser.getNickName(), name, names));
	reply(sd, RPL_ENDOFNAMES(newUser.getNickName(), name));
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
		members = getChannelMembers(name);
	else
		members = getPrivateMember(name);
		
	// Check errors, user unknown, channel unknown, user not in channel
	if (pos == 0 && !userInChannel(members, sender.getNickName()))
	{
		reply(sender.getSd(), ERR_NOTONCHANNEL(sender.getNickName(), name));
		return ;
	}
	members.erase(std::remove(members.begin(), members.end(), sender), members.end());
	if (members.empty())
	{
		reply(sender.getSd(), ERR_NOSUCHNICK(sender.getNickName(), name));
		return ;
	}
	for (size_t i = 0; i < members.size(); i++)
		reply(members[i].getSd(), RPL_PRIVMSG(sender.getNickName() + "!" + sender.getUserName() + members[i].getIp(), name, userMsg));
}

void IRCServer::quit(std::string input, int sd)
{
	(void)input;
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
	users.erase(quit.getNickName());
}

void IRCServer::kick(std::string input, int sd)
{
	std::istringstream iss(input);
	std::string channelName;
	std::string kicked;

	iss >> channelName >> kicked;
	User kicker = findUserInstance(sd);
	User kickedUsr = findUserInstance(kicked);
	
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end() || kickedUsr.getNickName() == "default")
	{
		reply(kicker.getSd(), ERR_NOSUCHNICK(kicker.getNickName(), channelName));
		return ;
	}
	if (!userInChannel(channels[channelName].getMembers(), kickedUsr.getNickName()))
	{
		reply(kicker.getSd(), ERR_NOTONCHANNEL(kickedUsr.getNickName(), channelName));
		return ;
	}
	std::map<std::string, Role *> &role = channels[channelName].getRole();
	for (std::map<std::string, Role *>::iterator it = role.begin(); it != role.end(); it++)
	{
		if (it->first == kicker.getNickName())
			it->second->kick(kicked, channels[channelName]);
	}
	// role[kicker.getNickName()]->kick(kicked, channels[channelName]);
}

void	IRCServer::topic(std::string input, int sd) {
	std::istringstream	iss(input);
	std::string			channelName;
	std::string			topic;
	char				col;

	iss >> channelName >> col;
	getline(iss, topic);
	topic.resize(topic.size() - 1);
	User sender = findUserInstance(sd);
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
	{
		reply(sender.getSd(), ERR_NOSUCHNICK(sender.getNickName(), channelName));
		return ;
	}
	std::map<std::string, Role *> role = channels[channelName].getRole();

	role[sender.getNickName()]->topic(topic, channels[channelName]);
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
	// Pourquoi ca marche si ca rentre dans le premiere condition mais pas dans la deuxieme
	// LE MESSAGE EST LE MEEEEEEME
	if (it == channels.end() || receiver.getNickName() == "default")
	{
		reply(sender.getSd(), ERR_NOSUCHNICK(sender.getNickName(), channelName));
		return ;
	}
	std::map<std::string, Role *> role = channels[channelName].getRole();
	role[sender.getNickName()]->invite(receiver);
}
