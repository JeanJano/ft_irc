/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/28 18:26:16 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : name("default") {
	pass = "default";
	topic = "default";
	timestamp = 0;
}

Channel::Channel(std::string n, std::string p) : name(n), pass(p) {
	topic = "default";
	timestamp = 0;
}

Channel::Channel(const Channel &cpy) {
	if (this != &cpy)
	{
		*this = cpy;
	}
}

Channel::~Channel() {
	// Free Mode Operators
}

Channel	&Channel::operator=(const Channel &cpy) {
	name = cpy.name;
	pass = cpy.pass;
	topic = cpy.topic;
	timestamp = cpy.timestamp;
	members = cpy.members;
	banList = cpy.banList;
	mode = cpy.mode;
	return (*this);
}

void	Channel::addUser(User user) {
	members.push_back(user);
	if (members.size() == 1)
		mode[user.getNickName()] = new Operator(user, name); 
	else
		mode[user.getNickName()] = new Regular(user, name);
}

void	Channel::removeUser(User &user) {
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

std::map<std::string, Role*>	&Channel::getMode() {
	return (mode);
}
