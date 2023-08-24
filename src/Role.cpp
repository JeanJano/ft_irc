/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:43 by smessal           #+#    #+#             */
/*   Updated: 2023/08/24 14:26:39 by zel-kass         ###   ########.fr       */
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

Regular::~Regular() {
	return ;
}

void	Regular::kick() {
	std::string msg;

	msg = ":server 482 " + nickname + " " + channelName + " :You're not channel operator\r\n";
	send(sd, msg.c_str(), msg.size(), 0);
}

// Operator::Operator(User client, std::string channelName) : client(client), channelName(channelName)
// {
// 	return ;
// }

// Operator::~operator()
// {
// 	return ;
// }
