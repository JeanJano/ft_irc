/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:05:39 by jsauvage          #+#    #+#             */
/*   Updated: 2023/08/16 19:41:31 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() {}

User::User(int newSd) : sd(newSd) {
	userName = "default";
	nickName = "default";
	realName = "default";
	passWord = "default";
	isConnected = false;
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
		sd = cpy.sd;
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

int	User::getSd() const {
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
	std::cout << std::endl;
	std::cout << "Username:	" << userName << std::endl;
	std::cout << "Nickname:	" << nickName << std::endl;
	std::cout << "Realname:	" << realName << std::endl;
	std::cout << "Password:	" << passWord << std::endl;
	std::cout << "SocketDe:	" << sd << std::endl;
	std::cout << std::endl;
}

User::~User() {}