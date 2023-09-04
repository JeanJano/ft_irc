/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:30:00 by zel-kass          #+#    #+#             */
/*   Updated: 2023/09/04 16:34:08 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool	run = true;

void	signalHandler(int num) {
	(void)num;
	run = false;
}

int main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << "wrong number of arguments" << std::endl;
		return (1);
	}
	IRCServer server(av);
	return (0);
}