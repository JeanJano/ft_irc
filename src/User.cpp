/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:05:39 by jsauvage          #+#    #+#             */
/*   Updated: 2023/08/15 18:28:11 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(size_t sd)
{
	userName = "default";
	nickName = "default";
	realName = "default";
	passWord = "default";
	isConnected = false;
	sd = sd;
}

User::User() {
	userName = "default";
	nickName = "default";
	realName = "default";
	passWord = "default";
	isConnected = false;
	sd = -1;
}

User::User(const User &cpy) {
	if (this != &cpy)	
		*this = cpy;
}

User	&User::operator=(const User &cpy) {
	if (this != &cpy)
	{
		userName = cpy.userName;
		nickName = cpy.nickName;
		realName = cpy.realName;
		passWord = cpy.passWord;
	}
	return (*this);
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

bool	User::getStatus() const {
	return (isConnected);
}

int	User::getSd() const
{
	return (sd);
}

void	User::setStatus(bool status) {}

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
	std::cout << "Username: " << userName << std::endl;
	std::cout << "Nickname: " << nickName << std::endl;
	std::cout << "Realname: " << realName << std::endl;
	std::cout << "Password: " << passWord << std::endl;
}

User::~User() {}