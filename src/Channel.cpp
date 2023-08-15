/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/15 12:59:50 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : name("default") {}

Channel::Channel(std::string n) : name(n) {}

Channel::Channel(const Channel &cpy) : name(cpy.name), members(cpy.members) {}

Channel::~Channel() {}

Channel	&Channel::operator=(const Channel &cpy) {
	name = cpy.name;
	members = cpy.members;
	return (*this);
}

// void	Channel::addUser(User *user) {
// 	if (user->getStatus() == true) {
// 		members.push_back(*user);
// 	}
// }

std::string	Channel::getName() {
	return (name);
}

std::vector<User> Channel::getMembers() {
	return (members);
}

std::vector<std::string>	Channel::getMessages() {
	return (messages);
}