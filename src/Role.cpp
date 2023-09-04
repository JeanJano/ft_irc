/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:43 by smessal           #+#    #+#             */
/*   Updated: 2023/09/04 16:16:10 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"

Role::~Role() {
	return ;
}

Role	&Role::operator=(const Role &cpy)
{
	if (sender)
		delete sender;
	sender = new User (*cpy.sender);
	channelName = channelName;
	return *this;
}