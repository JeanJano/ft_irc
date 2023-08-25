/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:53 by smessal           #+#    #+#             */
/*   Updated: 2023/08/25 14:44:01 by smessal          ###   ########.fr       */
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
		// virtual	void	invite(std::string nick) = 0;
		// virtual	void	topic(std::string topic) = 0;
		// virtual	void	mode(std::string token) = 0;
	protected:
		User		*sender;
		std::string	channelName;
};

class Operator : public Role {
	public:
		Operator(User *sender, std::string c);
		~Operator();
		void	kick(const std::string& kicked, Channel& channel);
		// void	invite(std::string nick);
		// void	topic(std::string topic);
		// void	mode(std::string token);	
};
 
class Regular : public Role {
	public:
		Regular(User *sender, std::string c);
		~Regular();
		Regular();

		std::string	getNickName();

		void	kick(const std::string& kicked, Channel& channel);
		// void	invite(std::string nick);
		// void	topic(std::string topic);
		// void	mode(std::string token);
};

#endif