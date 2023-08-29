/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:05:46 by jsauvage          #+#    #+#             */
/*   Updated: 2023/08/29 11:22:16 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User
{
	public:
		User();
		User(int newSd);
		User(const User &cpy);
		~User();
		User& operator=(const User &cpy);
		bool		operator==(const User &cmp) const;
		std::string	getUserName() const;
		std::string	getNickName() const;
		std::string	getRealName() const;
		std::string	getPassWord() const;
		std::string	getIp() const;
		int			getSd() const;

		void		setIp(std::string ip);

		void		parseInput(std::string input);
		void		printInfos();
	private:
		std::string	userName;
		std::string nickName;
		std::string realName;
		std::string passWord;
		std::string	ip;
		int			sd;
};

#endif