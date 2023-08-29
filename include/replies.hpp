/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 10:31:55 by smessal           #+#    #+#             */
/*   Updated: 2023/08/28 18:53:52 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <iostream>

//ERROR REPLIES
#define ERR_NEEDMOREPARAMS(source, command)				std::string(":server 461 ") + source + " " + command + " :Not enough parameters"
#define ERR_NOTONCHANNEL(source, channel)				std::string(":server 442 ") + source + " " + channel + " :You're not on that channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)	std::string(":server 441 ") + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANNELISFULL(source, channel)				std::string(":server 471 ") + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_BADCHANNELKEY(source, channel)				std::string(":server 475 ") + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_INVITONLYCHAN(source, channel)				std::string(":server 473 ") + source + " " + channel + " :Cannot join channel (+i)"
#define ERR_NOSUCHCHANNEL(source, channel)				std::string(":server 403 ") + source + " " + channel + " :No such channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)			std::string(":server 482 ") + source + " " + channel + " :You're not channel operator"
#define ERR_NONICKNAMEGIVEN(source)						std::string(":server 431 ") + source + " :Nickname not given"
#define ERR_NICKNAMEINUSE(source, nickname)				std::string(":server 433 ") + source + " " + nickname + " :Nickname is already in use"
#define ERR_ALREADYREGISTERED(source)					std::string(":server 462 ") + source + " :You may not reregister"
#define ERR_PASSWDMISMATCH(source)						std::string(":server 464 ") + source + " :Password incorrect"
#define ERR_NOTREGISTERED(source)						std::string(":server 451 ") + source + " :You have not registered"
#define ERR_UNKNOWNCOMMAND(source, command)				std::string(":server 421 ") + source + " " + command + " :Unknown command"
#define ERR_USERONCHANNEL(source, target, channel)		std::string(":server 443 ") + source + " " + target + " " + channel + " :is already on channel"
#define ERR_NOSUCHNICK(source, name)					std::string(":server 401 ") + source + " " + name + " :No such nick/channel"
#define ERR_INVITEONLYCHAN(source, channel)				std::string(":server 473 ") + source + " " + channel + " :Cannot join channel (+i)"

// NUMERIC REPLIES
#define RPL_WELCOME(source, prefix)										std::string(":server 001 ") + source + " :Welcome to the Internet Relay Network " + prefix
#define RPL_YOURHOST(source, servername, version)						std::string(":server 002 ") + source + " :Your host is " + servername + ", running version " + version
#define RPL_CREATED(source, date)										std::string(":server 003 ") + source + " :This server was created " + date
#define RPL_MYINFO(source, servername, version, usermodes, chanmodes)	std::string(":server 004 ") + source + " :" + servername + " " + version + " " + usermodes + " " + chanmodes

#define RPL_NAMREPLY(source, channel, users)			std::string(":server 353 ") + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)					std::string(":server 366 ") + source + " " + channel + " :End of /NAMES list."
#define RPL_INVITING(source, channel, target)			std::string(":server 341 ") + source + " " + channel + " " + target

#define RPL_NOTOPIC(source, channel)					std::string(":server 331 ") + source + " " + channel + " :No topic is set"
#define RPL_TOPIC(source, channel, topic)				std::string(":server 332 ") + source + " " + channel + " :" + topic

#define RPL_WHOREPLY(source, channel, username, hostname, serverhostname, nickname, realname)	std::string(":server 352 ") + source + " " + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname
#define RPL_ENDOFWHO(source, channel)					std::string(":server 315 ") + source + " " + channel + " :End of WHO list"

#define RPL_LIST(source, channel, nbUsers, topic)		std::string(":server 322 ") + source + " " + channel + " " + nbUsers + " :" + topic
#define RPL_LISTEND(source)					std::string(":server 323 ") + source + " :End of LIST"

// COMMAND REPLIES

#define RPL_MODE(source, channel, modes, args)		std::string(":") +source + " MODE " + channel + " " + modes + " " + args
#define RPL_PING(source, token)						std::string(":") + source + " PONG :" + token
#define RPL_QUIT(source, message)					std::string(":") + source + " QUIT :" + message
#define RPL_JOIN(source, channel)					std::string(":") + source + " JOIN :" + channel
#define RPL_PART(source, channel)					std::string(":") + source + " PART " + channel
#define RPL_PART_REASON(source, channel, reason)	std::string(":") + source + " PART " + channel + " :" + reason
#define RPL_KICK(source, channel, target, reason)	std::string(":") + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_PRIVMSG(source, target, message)		std::string(":") + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)			std::string(":") + source + " NOTICE " + target + " :" + message
#define RPL_INVITE(source, target, channel)			std::string(":") + source + " INVITE " + target + " :" + channel

;
#endif