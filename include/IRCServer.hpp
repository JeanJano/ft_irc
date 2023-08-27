/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 20:27:35 by zel-kass          #+#    #+#             */
/*   Updated: 2023/08/27 16:03:50 by smessal          ###   ########.fr       */
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
		void		topic(std::string input, int sd);
		void		invite(std::string input, int sd);
		// errors
		
		void		privateMsg(t_cmd msg);
		User		&findUserInstance(int sd);
		User 		findUserInstance(std::string nick);
		std::string	findUserNickName(int sd);
		bool		nickIsUsed(std::string nickname);
		std::vector<User>	getChannelMembers(std::string name, std::string sender);
		std::vector<User>	getPrivateMember(std::string name);
		bool				userInChannel(std::vector<User> &members, std::string nick);

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