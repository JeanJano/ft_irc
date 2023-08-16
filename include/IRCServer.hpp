/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:35 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/16 17:38:54 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include "irc.hpp"

class Channel;

typedef struct    s_cmd
{
    std::string    typeCmd;
    std::string    text;
}                t_cmd;

class IRCServer {
	public:
		IRCServer(char **av); // init server
		~IRCServer();
		void	createChannel(std::string name);
		Channel	*getChannel();
		
		void		init(char **av);
		void		serverManager(char **av);
		bool		connectClient();
		void		handleEvents();
		void		newConnexionMsg(int sd);
		bool		checkpassword(int sd, User client);
		t_cmd    	parseCmd(std::string buf);
		std::string	getCompleteMsg(int sd, size_t *i);
		void		checkCmd(t_cmd cmd);

		// events
		void	join(std::string intput);
		// void	privmsg();
		void		privateMsg(t_cmd msg);
		User		*findUser(std::string nick);
		User		*findUser(int sd);

	private:
		sockaddr_in						servAddr;
		int								serverSd;
		fd_set							readfds;
		int								newSd;
		size_t							*nowFd;
		std::string						password;
		std::vector<struct pollfd>		fds;
		std::map<std::string, Channel>	channels;
		std::map<std::string, User>	users;
};


#endif