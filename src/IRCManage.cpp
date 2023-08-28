/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCManage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:29:21 by smessal           #+#    #+#             */
/*   Updated: 2023/08/28 14:01:17 by smessal          ###   ########.fr       */
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
			std::string buf = getCompleteMsg(fds[i].fd, &i);
			if (!buf.empty())
			{
				std::cout << buf << std::endl;
				parseCmd(buf);
				while (cmd.size() > 0)
					treatCmd(fds[i].fd);
			}
		}
	}
}

void IRCServer::newConnexionMsg(int sd, sockaddr_in addr, User &usr)
{
	std::cout << "New client connected" << std::endl;
	usr.setIp(inet_ntoa(addr.sin_addr));
	std::cout << "addr ip: " << usr.getIp() << std::endl;
	users[usr.getNickName()] = usr;
	reply(sd, RPL_WELCOME(usr.getNickName(), usr.getNickName()));
}

bool IRCServer::checkNewClient(int sd, User client)
{
	std::string msg;
	if (client.getPassWord().compare(password) != 0)
	{
		reply(sd, ERR_PASSWDMISMATCH(client.getNickName()));
		return (false);
	}
	if (nickIsUsed(client.getNickName()))
	{
		std::cout << "client try to connect with used nickname" << std::endl;
		reply(sd, ERR_NICKNAMEINUSE(client.getNickName(), client.getNickName()));
		return (false);
	}
	return (true);
}

std::string IRCServer::getCompleteMsg(int sd, size_t *i)
{
	char msg[1500];
	int bytesread;
	std::string received;

	struct timeval tv;
	tv.tv_sec = 1; // 5 seconds timeout
	tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
	while (true)
	{
		memset(&msg, 0, sizeof(msg));
		bytesread = recv(sd, msg, sizeof(msg), 0);
		if (bytesread < 0)
			break;
		msg[bytesread] = '\0';
		received += msg;
		if (received.length() >= 2 && received.substr(received.length() - 2) == "/r/n")
			break;
	}
	return (received);
}
