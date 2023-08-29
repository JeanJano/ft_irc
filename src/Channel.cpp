/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/29 14:30:55 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : name("default") {
	pass = "default";
	topic = "default";
	timestamp = 0;
	mode["i"] = false;
	mode["t"] = false;
	mode["k"] = false;
	mode["o"] = false;
	mode["l"] = false;
}

Channel::Channel(std::string n, std::string p) : name(n), pass(p) {
	topic = "default";
	timestamp = 0;
	mode["i"] = false;
	mode["t"] = false;
	mode["k"] = false;
	mode["o"] = false;
	mode["l"] = false;
}

Channel::Channel(const Channel &cpy) {
	if (this != &cpy)
		*this = cpy;
}

Channel::~Channel() {
	// Free role Operators
}

Channel	&Channel::operator=(const Channel &cpy) {
	name = cpy.name;
	pass = cpy.pass;
	topic = cpy.topic;
	timestamp = cpy.timestamp;
	members = cpy.members;
	banList = cpy.banList;
	role = cpy.role;
	mode = cpy.mode;
	return (*this);
}

void	Channel::addUser(User user) {
	members.push_back(user);
	if (members.size() == 1)
		role[user.getNickName()] = new Operator(user, name); 
	else
		role[user.getNickName()] = new Regular(user, name);
	std::cout << "Joined: " << user.getNickName() << std::endl;
	// std::cout << "Nick: " << user.getNickName() << " Channel: " << name << std::endl;
}

void	Channel::removeUser(User &user) {
	// std::vector<User>::iterator it = std::remove(members.begin(), members.end(), user);
	members.erase(std::remove(members.begin(), members.end(), user), members.end());
}

void	Channel::setTopic(std::string newTopic) {
	this->topic = newTopic;
}

void	Channel::setTimeStamp(std::time_t newTimeStamp) {
	this->timestamp = newTimeStamp;
}

std::string	&Channel::getName() {
	return (name);
}

std::string	&Channel::getTopic() {
	return (topic);
}

std::time_t	&Channel::getTimeStamp() {
	return (timestamp);
}

std::string	&Channel::getPass() {
	return (pass);
}

std::vector<User> &Channel::getMembers() {
	return (members);
}

std::map<std::string, Role*>	&Channel::getRole() {
	return (role);
}

std::map<std::string, bool>	&Channel::getMode() {
	return (mode);
}