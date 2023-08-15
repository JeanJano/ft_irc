/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:35 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/15 14:30:47 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include "irc.hpp"

class IRCServer {
	public:
		IRCServer(char **av); // init server
		~IRCServer();
		void	createChannel(std::string name);
		Channel	*getChannel();
		
		void servConnectClient(char **av);

	private:
		sockaddr_in						servAddr;
		int								serverSd;
		fd_set							readfds;
		int								newSd;
		std::string						password;
		std::vector<struct pollfd>		fds;
		std::map<std::string, Channel*>	channels;
		std::map<std::string, User*>	users;
};


#endif