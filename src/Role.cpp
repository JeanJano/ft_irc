/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:43 by smessal           #+#    #+#             */
/*   Updated: 2023/08/29 15:07:00 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Role.hpp"

Role::~Role() {
	return ;
}

Role	&Role::operator=(const Role &cpy)
{
	sender = new User (*cpy.sender);
	channelName = channelName;
	return *this;
}