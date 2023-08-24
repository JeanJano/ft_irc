/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:43 by smessal           #+#    #+#             */
/*   Updated: 2023/08/25 00:15:56 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"

Role::~Role() {
	return ;
}

Regular::Regular() {
	nickname = "Default";
	channelName = "Default";
	sd = 0;
	return ;
}

Regular::Regular(std::string nickname, int sd, std::string channelName) {
	this->nickname = nickname;
    this->sd = sd;
    this->channelName = channelName;
}

Regular::~Regular() {}

void	Regular::kick(const std::string& kicked, Channel& channel) {
	std::string msg;

	msg = ":server 482 " + nickname + " " + channelName + " :You're not channel operator\r\n";
	send(sd, msg.c_str(), msg.size(), 0);
}

Operator::Operator(std::string n, int s, std::string c) {
	this->nickname = nickname;
    this->sd = sd;
    this->channelName = channelName;
}

Operator::~Operator() {}

void	Operator::kick(const std::string& kicked, Channel& channel) {
	std::vector<User> channelMembers = channel.getMembers();

    for (size_t i = 0; i < channelMembers.size(); i++) {
        if (channelMembers[i].getNickName() == kicked)
            channel.removeUser(channelMembers[i]);
    }
}