/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 14:04:36 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/28 15:41:13 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"

Operator::Operator() {
	sender = NULL;
	channelName = "Default";
	return ;
}

Operator::Operator(User *sender, std::string channelName) {
	this->sender = sender;
    this->channelName = channelName;
}

Operator::~Operator() {
	
}

void	Operator::kick(const std::string& kicked, Channel& channel) {
	std::vector<User> &channelMembers = channel.getMembers();
		
	// std::string kickMsg = ":" + sender->getNickName() + "!"+ sender->getUserName() +"@" + sender->getIp() + " KICK " + channelName + " " + kicked + " :Kicked by operator\r\n";
	for (size_t i = 0; i < channelMembers.size(); i++)
		reply(channelMembers[i].getSd(), RPL_KICK(sender->getNickName() + "!"+ sender->getUserName() +"@" + sender->getIp(), channelName, kicked, "No reason specified"));

    for (size_t i = 0; i < channelMembers.size(); i++) {
        if (channelMembers[i].getNickName() == kicked)
			channel.removeUser(channelMembers[i]);
    }
}

void	Operator::topic(const std::string& topic, Channel& channel) {
	channel.setTopic(topic);
	std::time_t now;
	std::time(&now);
	channel.setTimeStamp(now);
	std::vector<User> &channelMembers = channel.getMembers();

	std::string	topicMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getIp() + " TOPIC " + channelName + " :" + topic;
	for (size_t i = 0; i < channelMembers.size(); i++)
		reply(channelMembers[i].getSd(), topicMsg);
}

void	Operator::invite(User receiver)
{
	std::string	inviteMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getIp() + " INVITE " + receiver.getNickName() + " :" + channelName;
	reply(receiver.getSd(), inviteMsg);
}
