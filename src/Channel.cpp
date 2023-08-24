/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/25 00:17:52 by zel-kass         ###   ########.fr       */
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
	if (members.size() == 1)
		mode[user.getNickName()] = new Operator(user.getNickName(), user.getSd(), name); 
	else
		mode[user.getNickName()] = new Regular(user.getNickName(), user.getSd(), name);
	std::cout << "Joined: " << user.getNickName() << std::endl;
	// std::cout << "Nick: " << user.getNickName() << " Channel: " << name << std::endl;
}

void	Channel::removeUser(User user) {
	std::vector<User>::iterator it = std::remove(members.begin(), members.end(), user);
	members.erase(it, members.end());
}

std::string	&Channel::getName() {
	return (name);
}

std::string	&Channel::getPass() {
	return (pass);
}

std::vector<User> &Channel::getMembers() {
	return (members);
}

std::map<std::string, Role*>	&Channel::getMode() {
	return (mode);
}
