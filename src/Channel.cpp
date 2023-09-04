/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/09/04 16:16:30 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel() : name("default") {
	pass = "default";
	topic = "default";
	timestamp = 0;
	topicSetter = NULL;
	limit = -1;
	mode['i'] = false;
	mode['k'] = false;
	mode['l'] = false;
	mode['o'] = false;
	mode['t'] = true;
}

Channel::Channel(std::string n, std::string p) : name(n), pass(p) {
	topic = "default";
	timestamp = 0;
	topicSetter = NULL;
	limit = -1;
	mode['i'] = false;
	mode['k'] = false;
	mode['l'] = false;
	mode['o'] = false;
	mode['t'] = true;
}

Channel::Channel(const Channel &cpy) {
	if (this != &cpy) {
		*this = cpy;
	}
}

Channel::~Channel() {
	for (std::map<std::string, Role *>::iterator it = role.begin(); it != role.end(); it++)
		delete it->second;
	role.clear();
	members.clear();
	banList.clear();
	mode.clear();
	if (topicSetter)
		delete topicSetter;
}

Channel	&Channel::operator=(const Channel &cpy) {
	name = cpy.name;
	pass = cpy.pass;
	topic = cpy.topic;
	timestamp = cpy.timestamp;
	members = cpy.members;
	banList = cpy.banList;
	role = cpy.role;
	limit = cpy.limit;
	mode = cpy.mode;
	topicSetter = NULL;
	if (cpy.topicSetter)
		topicSetter = new User(*cpy.topicSetter);
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

User	*Channel::getTopicSetter() {
	return (topicSetter);
}

void	Channel::setTopicSetter(User topicSet) {
	if (topicSetter)
		delete topicSetter;
	topicSetter = new User(topicSet);	
}

int	&Channel::getLimit() {
	return (limit);
}

void	Channel::modeManager(std::string input, User &sender) {
	std::istringstream	iss(input);
	std::string			channelName;
	char				operand;
	char				mode;
	std::string			param;
	
	iss >> channelName >> operand >> mode >> param;
	if (isOperator(role[sender.getNickName()]) == false) {
		reply(sender.getSd(), ERR_CHANOPRIVSNEEDED(sender.getNickName(), channelName));
		return ;
	}
	typedef void	(Channel::*functionPtr)(char, char, std::string, std::string, User&);

	char			modeArr[5] = {'i', 'k', 'l', 'o', 't'};
	functionPtr		modePtr[5] = {&Channel::inviteOnly, &Channel::channelKey, &Channel::userLimit, &Channel::operatorPriv, &Channel::topicRestr};
	for (int i = 0; i < 5; i++) {
		if (mode == modeArr[i])
			(this->*modePtr[i])(operand, mode, channelName, param, sender);
	}
}

void	Channel::changeStatus(char mod, char operand) {
	if (operand == '+')
		mode[mod] = true;
	else if (operand == '-')
		mode[mod] = false;
}

void	Channel::inviteOnly(char operand, char mod, std::string channelName, std::string param, User& sender) {
	(void)param;
	std::string	msg;

	msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getIp() + " MODE " + channelName + " " + operand + mod;
	for (size_t i = 0; i < members.size(); i++) {
		reply(members[i].getSd(), msg);
	}
	changeStatus(mod, operand);
}

void	Channel::channelKey(char operand, char mod, std::string channelName, std::string param, User& sender) {
	std::string	msg;

	msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getIp() + " MODE " + channelName + " " + operand + mod + " " + param;
	for (size_t i = 0; i < members.size(); i++) {
		reply(members[i].getSd(), msg);
	}
	changeStatus(mod, operand);
	if (operand == '+')
		pass = param;
	else if (operand == '-')
		pass = "";
}

void	Channel::userLimit(char operand, char mod, std::string channelName, std::string param, User& sender) {
	std::string	msg;

	msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getIp() + " MODE " + channelName + " " + operand + mod + " " + param;
	for (size_t i = 0; i < members.size(); i++) {
		reply(members[i].getSd(), msg);
	}
	changeStatus(mod, operand);
	if (operand == '+')
		limit = atoi(param.c_str());
	else if (operand == '-')
		limit = -1;
}

void	Channel::operatorPriv(char operand, char mod, std::string channelName,  std::string param, User& sender) {
	(void)mod;
	User	usr;
	for (size_t i = 0; i < members.size(); i++) {
		if (members[i].getNickName() == param)
			usr = members[i];
	}
	changeRole(usr, operand, channelName);
	std::string msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getIp() + " MODE " + channelName + " " + operand + mod + " " + param;
	for (size_t i = 0; i < members.size(); i++)
		reply(members[i].getSd(), msg);
}

void	Channel::topicRestr(char operand, char mod, std::string channelName,  std::string param, User& sender) {
	(void)param;
	changeStatus(mod, operand);
	std::string msg = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.getIp() + " MODE " + channelName + " " + operand + mod;
	for (size_t i = 0; i < members.size(); i++)
		reply(members[i].getSd(), msg);
}

bool	Channel::isOperator(Role* role) {
	if (dynamic_cast<Operator*>(role))
		return (true);
	return (false);
}

void	Channel::changeRole(User &user, char operand, std::string channelName) {
	if (operand == '+') {
		Operator *newOp = new Operator(user, channelName);
		std::map<std::string, Role*>::iterator it = role.find(user.getNickName());
		if (it != role.end()) {
			delete it->second;
			it->second = newOp;
		}
	} else if (operand == '-') {
		Regular *newReg = new Regular(user, channelName);
		std::map<std::string, Role*>::iterator it = role.find(user.getNickName());
		if (it != role.end()) {
			delete it->second;
			it->second = newReg;
		}
	}
}
