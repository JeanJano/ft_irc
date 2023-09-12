/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:05:39 by jsauvage          #+#    #+#             */
/*   Updated: 2023/09/12 13:42:15 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() {
	userName = "default";
	nickName = "default";
	realName = "default";
	passWord = "default";
	fullMsg = "";
	ip = "default";
	sd = 0;
}

User::User(int newSd) : sd(newSd) {
	userName = "default";
	nickName = "default";
	realName = "default";
	passWord = "default";
	ip = "default";
	fullMsg = "";
}

User::User(const User &cpy) {
	if (this != &cpy)	
		*this = cpy;
}

User	&User::operator=(const User &cpy) {
	if (this != &cpy)
	{
		this->userName = cpy.userName;
		this->nickName = cpy.nickName;
		this->realName = cpy.realName;
		this->passWord = cpy.passWord;
		this->sd = cpy.sd;
		this->ip = cpy.ip;
		this->invit = cpy.invit;
		this->fullMsg = cpy.fullMsg;
	}
	return (*this);
}

bool	User::operator==(const User &cmp) const {
	return this->getNickName() == cmp.getNickName();
}

std::string	User::getUserName() const {
	return (userName);
}

std::string	User::getNickName() const {
	return (nickName);
}

std::string	User::getRealName() const {
	return (realName);
}

std::string	User::getPassWord() const {
	return (passWord);
}

std::string	User::getFullMsg() const {
	return (fullMsg);
}

void	User::setFullMsg(std::string msg) {
	if (msg.find('\n') != std::string::npos)
		msg.erase(msg.find('\n'), 1);
	fullMsg += msg;
	return ;
}

void	User::eraseFullMsg() {
	fullMsg.clear();
}
int	User::getSd() const {
	return (sd);
}

std::string	User::getIp() const {
	return (ip);
}

std::vector<std::string> User::getInvit() const {
	return (invit);
}

void	User::setIp(std::string ip) {
	this->ip = ip;
}

void	User::parseInput(std::string input) {
	std::stringstream	ss(input);
	std::string			line;

	while (std::getline(ss, line))
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
    		line.resize(line.size() - 1);
		if (line.substr(0, 5) == "NICK ")
			nickName = line.substr(5);
		else if (line.substr(0, 5) == "PASS ")
			passWord = line.substr(5);
		else if (line.substr(0, 5) == "USER ") {
			std::stringstream ustream(line.substr(5));
			ustream >> userName;
			ustream.ignore(256, ':');
			std::getline(ustream, realName);
		}
	}
}

void	User::printInfos() {
	std::cout << std::endl;
	std::cout << "Username:	" << userName << std::endl;
	std::cout << "Nickname:	" << nickName << std::endl;
	std::cout << "Realname:	" << realName << std::endl;
	std::cout << "Password:	" << passWord << std::endl;
	std::cout << "SocketDe:	" << sd << std::endl;
	std::cout << std::endl;
}

void	User::addInvit(std::string channelName) {
	invit.push_back(channelName);
}

void	User::removeInvit(std::string channelName) {
	size_t i = 0;
	for (; i < invit.size(); i++) {
		std::cout << "Invit: " << invit[i] << std::endl;
	}
	invit.erase(std::remove(invit.begin(), invit.end(), channelName), invit.end());
}

bool	User::isInvit(std::string channelName) {
	for (size_t i = 0; i < invit.size(); i++) {
		if (invit[i] == channelName)
			return (true);
	}
	return (false);
}

User::~User() {
	invit.clear();
}