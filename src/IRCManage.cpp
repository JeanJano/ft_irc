/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCManage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:29:21 by smessal           #+#    #+#             */
/*   Updated: 2023/09/14 15:54:12 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

void IRCServer::handleEvents()
{
	for (size_t i = 1; i < fds.size(); ++i)
	{
		nowFd = &i;
		if (fds[i].revents & POLLIN)
		{
			std::string buf = getCompleteMsg(fds[i].fd);
			if (!buf.empty())
			{
				parseCmd(buf);
				while (cmd.size() > 0)
					treatCmd(fds[i].fd);
			}
		}
	}
}

void IRCServer::newConnexionMsg(int sd, sockaddr_in addr, User usr)
{
	usr.setIp(inet_ntoa(addr.sin_addr));
	users[usr.getNickName()] = usr;
	reply(sd, RPL_WELCOME(usr.getNickName(), usr.getNickName()));
	reply(sd, RPL_YOURHOST(usr.getNickName(), "server", "0.1"));
	reply(sd, RPL_CREATED(usr.getNickName(), creation));
}

bool IRCServer::checkNewClient(int sd, User client)
{
	std::string msg;
	// if (client.getPassWord().compare(password) != 0)
	// {
	// 	reply(sd, ERR_PASSWDMISMATCH(client.getNickName()));
	// 	return (false);
	// }
	if (nickIsUsed(client.getNickName()))
	{
		reply(sd, ERR_NICKNAMEINUSE(client.getNickName(), client.getNickName()));
		return (false);
	}
	return (true);
}

std::string IRCServer::getCompleteMsg(int sd)
{
	char msg[1500];
	int bytesread;
	std::string received;

	struct timeval tv;
	tv.tv_sec = 1; // 5 seconds timeout
	tv.tv_usec = 0;
	User	&sender = findUserInstance(sd);
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
	while (true)
	{
		memset(&msg, 0, sizeof(msg));
		bytesread = recv(sd, msg, sizeof(msg), 0);
		// if (bytesread < 0 && received.length() >= 2 && received.substr(received.length() - 2) == "\r\n")
		// {
		// 	break;
		// }
		if (bytesread < 0) {
			if (!sender.getFullMsg().empty())
			{
				std::cout << "fullMsg erased" << std::endl;
				sender.setFullMsg(received);
				received = sender.getFullMsg();
				sender.eraseFullMsg();
			}
			std::cout << "test" << std::endl;
			break;
		}
		if (bytesread == 0)
		{
			quit("", sd);
			std::cout << "Out because bytesread == 0" << std::endl;
			received = "";
			break ;
		}
		msg[bytesread] = '\0';
		received += msg;
		if (received[received.size() - 1] != '\n') {
			std::cout << "EOT" << std::endl;
			sender.setFullMsg(received);
			std::cout << "Full msg: " << sender.getFullMsg() << std::endl;
			received = "";
			break ;
		}
	}
	std::cout << "Received Complete: " << received << std::endl;
	return (received);
}

std::string IRCServer::getWelcomeMsg(int sd)
{
	char msg[1500];
	int bytesread;
	std::string received;

	struct timeval tv;
	tv.tv_sec = 5; // 5 seconds timeout
	tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
	usleep(1000);
	while (true)
	{
		memset(&msg, 0, sizeof(msg));
		bytesread = recv(sd, msg, sizeof(msg), 0);
		if (bytesread < 0) {
			std::cout << "exit 0" << std::endl;
			break;
		}
		if (bytesread == 0)
		{
			quit("", sd);
			std::cout << "Out because bytesread == 0" << std::endl;
			break ;
		}
		msg[bytesread] = '\0';
		received += msg;
		// if (bytesread < 0 && received.length() >= 2 && received.substr(received.length() - 2) == "\r\n")
		// 	break;
	}
	std::cout << "Received Welcome: " << received << std::endl;
	return (received);
}