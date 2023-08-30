/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/30 15:12:32 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : name("default") {
	pass = "default";
	topic = "default";
	timestamp = 0;
	mode['i'] = false;
	mode['k'] = false;
	mode['l'] = false;
	mode['o'] = false;
	mode['t'] = false;
}

Channel::Channel(std::string n, std::string p) : name(n), pass(p) {
	topic = "default";
	timestamp = 0;
	mode['i'] = false;
	mode['k'] = false;
	mode['l'] = false;
	mode['o'] = false;
	mode['t'] = false;
}

Channel::Channel(const Channel &cpy) {
	if (this != &cpy) {
		*this = cpy;
	}
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
	return (*this);
}

void	Channel::addUser(User user) {
	members.push_back(user);
	if (members.size() == 1)
		role[user.getNickName()] = new Operator(user, name); 
	else
		role[user.getNickName()] = new Regular(user, name);
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

std::map<std::string, Role*>	&Channel::getRole() {
	return (role);
}

std::map<char, bool>	&Channel::getMode() {
	return (mode);
}

void	Channel::modeManager(std::string input, User &sender) {
	std::istringstream	iss(input);
	std::string			channelName;
	char				operand;
	char				mode;
	std::string			param;
	
	iss >> channelName >> operand >> mode >> param;
	
	typedef void	(Channel::*functionPtr)(char, char, std::string, std::string, User&);

	char			modeArr[5] = {'i', 'k', 'l', 'o', 't'};
	functionPtr		modePtr[5] = {&Channel::inviteOnly, &Channel::channelKey, &Channel::userLimit, &Channel::operatorPriv, &Channel::topicRestr};
	for (int i = 0; i < 5; i++) {
		if (mode == modeArr[i])
			(this->*modePtr[i])(operand, mode, channelName, param, sender);
	}
}

void	Channel::changeStatut(char mod, char operand) {
	if (operand == '+')
		mode[mod] = true;
	else if (operand == '-')
		mode[mod] = false;
}

void	Channel::inviteOnly(char operand, char mod, std::string channelName, std::string param, User& sender) {
	std::string	msg;

	msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getIp() + " MODE " + channelName + " " + operand + mod;
	for (size_t i = 0; i < members.size(); i++) {
		reply(members[i].getSd(), msg);
	}
	changeStatut(mod, operand);
}

void	Channel::channelKey(char operand, char mod, std::string channelName, std::string param, User& sender) {
	std::string	msg;
	msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getIp() + " MODE " + channelName + " " + operand + mod + " " + param;
	for (size_t i = 0; i < members.size(); i++) {
		reply(members[i].getSd(), msg);
	}
	changeStatut(mod, operand);
	if (operand == '+')
		pass = param;
	else if (operand == '-')
		pass = "";
}

void	Channel::userLimit(char operand, char mod, std::string channelName, std::string param, User& sender) {
	
}

void	Channel::operatorPriv(char operand, char mod, std::string channelName,  std::string param, User& sender) {
	
}

void	Channel::topicRestr(char operand, char mod, std::string channelName,  std::string param, User& sender) {
	
}

bool	Channel::isOperator(Role* role) {
	if (dynamic_cast<Operator*>(role))
		return (true);
	return (false);
}
