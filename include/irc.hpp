/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:17:58 by jsauvage          #+#    #+#             */
/*   Updated: 2023/09/05 12:13:35 by smessal          ###   ########.fr       */
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
# include <ctime>
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
# include "Role.hpp"
# include "IRCServer.hpp"
# include "replies.hpp"
# include <netinet/tcp.h>
# include <algorithm>
# include <csignal>
# include <cerrno>
# define BUFFER_SIZE 1024

extern bool run;
void	reply(int sd, std::string msg);
void	signalHandler(int num);

#endif