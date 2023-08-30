/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:53 by smessal           #+#    #+#             */
/*   Updated: 2023/08/30 18:20:27 by jsauvage         ###   ########.fr       */
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
		Role			&operator=(const Role &cpy);
		virtual std::string		getNickName() = 0;
		virtual	int				getSd() = 0;
	protected:
		User		*sender;
		std::string	channelName;
};

class Operator : public Role {
	public:
		Operator(User &sender, std::string channelName);
		~Operator();
		Operator();
		Operator	&operator=(const Operator &cpy);
		void	kick(const std::string& kicked, Channel& channel);
		void	invite(User receiver);
		void	topic(const std::string& topic, Channel& channel);
		// void	mode(std::string token);
		std::string	getNickName();
		int			getSd();
};
 
class Regular : public Role {
	public:
		Regular(User &sender, std::string channelName);
		~Regular();
		Regular();
		Regular	&operator=(const Regular &cpy);

		void	kick(const std::string& kicked, Channel& channel);
		void	invite(User receiver);
		void	topic(const std::string& topic, Channel& channel);
		// void	mode(std::string token);
		std::string	getNickName();
		int			getSd();
};

#endif