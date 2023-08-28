/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:43 by smessal           #+#    #+#             */
/*   Updated: 2023/08/28 11:22:49 by smessal          ###   ########.fr       */
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
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}

void	Regular::topic(const std::string& topic, Channel& channel) {
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}

void	Regular::invite(User receiver) {
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}

Operator::Operator(User *sender, std::string channelName) {
	this->sender = sender;
    this->channelName = channelName;
}

Operator::~Operator() {
	
}

void	Operator::kick(const std::string& kicked, Channel& channel) {
	std::vector<User> &channelMembers = channel.getMembers();
		
	std::string kickMsg = ":" + sender->getNickName() + "!"+ sender->getUserName() +"@" + sender->getIp() + " KICK " + channelName + " " + kicked + " :Kicked by operator\r\n";
	for (size_t i = 0; i < channelMembers.size(); i++)
        send(channelMembers[i].getSd() , kickMsg.c_str(), kickMsg.size(), 0);

    for (size_t i = 0; i < channelMembers.size(); i++) {
        if (channelMembers[i].getNickName() == kicked)
			channel.removeUser(channelMembers[i]);
    }
}

void	Operator::topic(const std::string& topic, Channel& channel) {
	channel.setTopic(topic);
	std::vector<User> &channelMembers = channel.getMembers();

	std::string	topicMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getIp() + " TOPIC " + channelName + " :" + topic + "\r\n";
	for (size_t i = 0; i < channelMembers.size(); i++)
		send(channelMembers[i].getSd() , topicMsg.c_str(), topicMsg.size(), 0);
}

void	Operator::invite(User receiver)
{
	std::string	inviteMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getIp() + " INVITE " + receiver.getNickName() + " :" + channelName + "\r\n";
	send(receiver.getSd(), inviteMsg.c_str(), inviteMsg.size(), 0);
}
