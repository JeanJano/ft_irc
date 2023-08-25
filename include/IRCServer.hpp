/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:35 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/25 16:13:37 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include "irc.hpp"
# include <queue>

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
		void		newConnexionMsg(int sd, sockaddr_in addr, User &usr);
		bool		checkNewClient(int sd, User client);
		void    	parseCmd(std::string buf);
		std::string	getCompleteMsg(int sd, size_t *i);
		void		treatCmd(int sd);

		// events
		void		join(std::string, int);
		void		privmsg(std::string, int);
		void		ping(std::string, int);
		void		quit(std::string, int);
		void		kick(std::string, int);
		void		topic(std::string, int);
		
		void		privateMsg(t_cmd msg);
		User		&findUserInstance(int sd);
		std::string	findUserNickName(int sd);
		bool		nickIsUsed(std::string nickname);
		std::vector<User>	&getChannelMembers(std::string name, std::string sender);
		std::vector<User>	getPrivateMember(std::string name);

	private:
		sockaddr_in						servAddr;
		int								serverSd;
		fd_set							readfds;
		int								newSd;
		size_t							*nowFd;
		std::string						password;
		std::vector<struct pollfd>		fds;
		std::map<std::string, Channel>	channels;
		std::map<std::string, User>		users;
		std::queue<t_cmd>				cmd;
};


#endif