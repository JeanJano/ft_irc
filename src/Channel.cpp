/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/24 14:03:02 by zel-kass         ###   ########.fr       */
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

void	Channel::addUser(User &user) {
	members.push_back(user);
	// if (members.size() == 1)
	// 	mode[user.getNickName()] = new Operator(user, name); 
	// else
	std::cout << "Nick: " << user.getNickName() << " Channel: " << name << std::endl;
	mode[user.getNickName()] = new Regular(user.getNickName(), user.getSd(), name);
}

void	Channel::removeUser(User user) {
	std::vector<User>::iterator it = std::remove(members.begin(), members.end(), user);
	members.erase(it, members.end());
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

std::map<std::string, Role*>	Channel::getMode() {
	return (mode);
}
