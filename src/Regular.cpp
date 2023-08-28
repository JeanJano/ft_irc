/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Regular.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 14:04:52 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/28 14:54:53 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"

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