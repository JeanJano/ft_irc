/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:17:58 by jsauvage          #+#    #+#             */
/*   Updated: 2023/08/15 14:30:51 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <string>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/poll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <netdb.h>
# include <sys/uio.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <fstream>
# include <sstream>
# include <deque>
# include <vector>
# include <map>
# include "User.hpp"
# include "Channel.hpp"

# define BUFFER_SIZE 1024

class User;
class Channel;

typedef struct	s_server
{
	sockaddr_in	servAddr;
	int			serverSd;
	fd_set		readfds;
	int			newSd;
	std::string	password;
	std::vector<struct pollfd>	fds;
	std::vector<User> users;
	std::vector<Channel> channels;
}				t_server;

void		initServer(t_server *server, char **av);
void		welcomeMsg(int sd, t_server *server);
std::string	getCompleteMsg(int sd);

#endif