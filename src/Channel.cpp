/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/17 20:52:09 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : name("default") {}

Channel::Channel(std::string n, std::string p) : name(n), pass(p) {}

Channel::Channel(const Channel &cpy) : name(cpy.name), members(cpy.members) {}

Channel::~Channel() {}

Channel	&Channel::operator=(const Channel &cpy) {
	name = cpy.name;
	members = cpy.members;
	return (*this);
}

void	Channel::addUser(User *user) {
	if (user->getStatus() == true) {
		members.push_back(*user);
	}
}

void	Channel::removeUser(User *user) {
	for (size_t i = 0; i < members.size(); i++) {
		
	}
}

std::string	Channel::getName() {
	return (name);
}

std::string	Channel::getPass() {
	return (pass);
}

std::vector<User> Channel::getMembers(std::string skip) {
	std::vector<User>	vctr;
	for (size_t i = 0; i < members.size(); i++) {
		if (members[i].getNickName() == skip) {
			continue;
		} else {
			vctr.push_back(members[i]);
		}
	}
	return (vctr);
}

void	Channel::setMember(User member) {
	members.push_back(member);
}