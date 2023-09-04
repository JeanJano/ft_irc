/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 10:58:04 by smessal           #+#    #+#             */
/*   Updated: 2023/09/04 16:24:46 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	reply(int sd, std::string msg) {
	msg += "\r\n";
	int val = send(sd, msg.c_str(), msg.size(), 0);
	if (val < 0)
		std::cout << "Message not complete" << std::endl;
	std::cout << "Sent: " << msg;
}
