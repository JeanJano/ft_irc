/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Regular.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 14:04:52 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/29 12:17:44 by smessal          ###   ########.fr       */
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

void	Regular::kick(const std::string& kicked, Channel& channel) {
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}

void	Regular::topic(const std::string& topic, Channel& channel) {
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}

void	Regular::invite(User receiver) {
	reply(sender->getSd(), ERR_CHANOPRIVSNEEDED(sender->getNickName(), channelName));
}