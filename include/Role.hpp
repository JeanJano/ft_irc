/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:53 by smessal           #+#    #+#             */
/*   Updated: 2023/08/28 15:41:24 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROLE_HPP
# define ROLE_HPP

# include "irc.hpp"
# include "User.hpp"

// class User;
class Channel;

class Role {
	public:
		virtual ~Role();

		virtual	void	kick(const std::string& kicked, Channel& channel) = 0;
		virtual	void	invite(User receiver) = 0;
		virtual	void	topic(const std::string& topic, Channel& channel) = 0;
		// virtual	void	mode(std::string token) = 0;
	protected:
		User		*sender;
		std::string	channelName;
};

class Operator : public Role {
	public:
		Operator(User *sender, std::string channelName);
		~Operator();
		Operator();
		void	kick(const std::string& kicked, Channel& channel);
		void	invite(User receiver);
		void	topic(const std::string& topic, Channel& channel);
		// void	mode(std::string token);	
};
 
class Regular : public Role {
	public:
		Regular(User *sender, std::string channelName);
		~Regular();
		Regular();

		std::string	getNickName();

		void	kick(const std::string& kicked, Channel& channel);
		void	invite(User receiver);
		void	topic(const std::string& topic, Channel& channel);
		// void	mode(std::string token);
};

#endif