/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 14:04:36 by zel-kass          #+#    #+#             */
/*   Updated: 2023/09/05 11:39:21 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"

Operator::Operator() {
	sender = NULL;
	channelName = "Default";
	return ;
}

Operator::Operator(User &sender, std::string channelName) {
	this->sender = new User(sender);
    this->channelName = channelName;
}

Operator	&Operator::operator=(const Operator &cpy)
{
	if (this != &cpy) {
        Role::operator=(cpy);
    }
	return *this;
}

Operator::~Operator() {
	delete sender;
}

std::string	Operator::getNickName() {
	return ("@" + this->sender->getNickName());
}

int	Operator::getSd() {
	return (this->sender->getSd());
}

void	Operator::kick(const std::string& kicked, Channel& channel) {
	std::vector<User> &channelMembers = channel.getMembers();
		
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
	channel.setTopicSetter(*sender);
	std::vector<User> &channelMembers = channel.getMembers();

	std::string	topicMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getIp() + " TOPIC " + channelName + " :" + topic;
	for (size_t i = 0; i < channelMembers.size(); i++)
		reply(channelMembers[i].getSd(), topicMsg);
}

void	Operator::invite(User &receiver, Channel& channel)
{
	reply(sender->getSd(), RPL_INVITING(sender->getNickName(), receiver.getNickName(), channel.getName()));

	std::string	inviteMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getIp() + " INVITE " + receiver.getNickName() + " :" + channelName;
	reply(receiver.getSd(), inviteMsg);
	receiver.addInvit(channel.getName());
}
