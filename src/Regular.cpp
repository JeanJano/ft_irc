/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Regular.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 14:04:52 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/30 17:59:58 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"

Regular::Regular() {
	sender = NULL;
	channelName = "Default";
	return ;
}

Regular::Regular(User &sender, std::string channelName) {
	this->sender = new User(sender);
    this->channelName = channelName;
}

Regular	&Regular::operator=(const Regular &cpy)
{
	if (this != &cpy) {
        Role::operator=(cpy);
    }
	return *this;
}

Regular::~Regular() {
	delete sender;
}

std::string	Regular::getNickName() {
	return (this->sender->getNickName());
}

int	Regular::getSd() {
	return (this->sender->getSd());
}

void	Regular::kick(const std::string& kicked, Channel& channel) {
	(void)kicked;
	(void)channel;
	std::cout << "Sender REG: "<< this->sender->getNickName() << std::endl;
	std::cout << "SD REG: "<< this->sender->getSd() << std::endl;
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}

void	Regular::topic(const std::string& topic, Channel& channel) {
	std::map<char, bool> mode = channel.getMode();

	if (mode['t'] == false) {
		channel.setTopic(topic);
		std::time_t now;
		std::time(&now);
		channel.setTimeStamp(now);
		std::vector<User> &channelMembers = channel.getMembers();

		std::string	topicMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@" + sender->getIp() + " TOPIC " + channelName + " :" + topic;
		for (size_t i = 0; i < channelMembers.size(); i++)
			reply(channelMembers[i].getSd(), topicMsg);
	} else {
		reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
	}
}

void	Regular::invite(User receiver) {
	(void)receiver;
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}