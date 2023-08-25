/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:43 by smessal           #+#    #+#             */
/*   Updated: 2023/08/25 15:44:49 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"
#include "Role.hpp"

Role::~Role() {
	return ;
}

Regular::Regular() {
	sender = NULL;
	channelName = "Default";
	return ;
}

Regular::Regular(User *sender, std::string channelName) {
	this->sender = sender;
    this->channelName = channelName;
}

Regular::~Regular() {
	
}

void	Regular::kick(const std::string& kicked, Channel& channel) {
	std::string msg;

	msg = ":server 482 " + sender->getNickName() + " " + channelName + " :You're not channel operator\r\n";
	send(sender->getSd(), msg.c_str(), msg.size(), 0);
}

Operator::Operator(User *sender, std::string c) {
	this->sender = sender;
    this->channelName = channelName;
}

Operator::~Operator() {
	
}

void	Operator::kick(const std::string& kicked, Channel& channel) {
	std::vector<User> &channelMembers = channel.getMembers();
		
	std::string kickMsg = ":" + sender->getNickName() + "!"+ sender->getUserName() +"@" + sender->getIp() + " KICK " + channelName + " " + kicked + " :Kicked by operator";
	for (size_t i = 0; i < channelMembers.size(); i++)
        send(channelMembers[i].getSd() , kickMsg.c_str(), kickMsg.size(), 0);

    for (size_t i = 0; i < channelMembers.size(); i++) {
        if (channelMembers[i].getNickName() == kicked)
			channel.removeUser(channelMembers[i]);
    }
}
