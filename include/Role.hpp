/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Role.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 18:18:53 by smessal           #+#    #+#             */
/*   Updated: 2023/08/24 13:48:19 by zel-kass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROLE_HPP
# define ROLE_HPP

# include "irc.hpp"
# include "User.hpp"
class User;

class Role
{
	public:
		virtual ~Role();

		virtual	void	kick() = 0;
		// virtual	void	invite(std::string nick) = 0;
		// virtual	void	topic(std::string topic) = 0;
		// virtual	void	mode(std::string token) = 0;
	protected:
		std::string nickname;
		int			sd;
		std::string	channelName;
};

class Operator : public Role
{
	public:
		Operator(User *client, std::string channelName);
		~Operator();
		void	kick();
		// void	invite(std::string nick);
		// void	topic(std::string topic);
		// void	mode(std::string token);	
};
 
class Regular : public Role
{
	public:
		Regular(std::string n, int s, std::string c);
		~Regular();
		Regular();

		std::string	getNickName();

		void	kick();
		// void	invite(std::string nick);
		// void	topic(std::string topic);
		// void	mode(std::string token);
};

#endif