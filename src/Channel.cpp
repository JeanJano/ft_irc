/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:45 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/14 20:49:33 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(std::string n) : name(n) {}

Channel::Channel(const Channel &cpy) : name(cpy.name), members(cpy.members) {}

Channel::~Channel() {}

Channel	&Channel::operator=(const Channel &cpy) {
	name = cpy.name;
	members = cpy.members;
	return (*this);
}

// void	Channel::addUser(User *user) {
// 	if (user->getStatus() == true) {
// 		members.push_back(*user);
// 	}
// }