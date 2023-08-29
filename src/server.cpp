/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:30:00 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/29 12:29:40 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << "wrong number of arguments" << std::endl;
		return (1);
	}
	IRCServer server(av);
	return (0);
}