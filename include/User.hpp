/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsauvage <jsauvage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:05:46 by jsauvage          #+#    #+#             */
/*   Updated: 2023/08/15 13:24:34 by jsauvage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User
{
	public:
		User();
		User(const User &cpy);
		~User();
		User& operator=(const User &cpy);

		std::string	getUserName() const;
		std::string	getNickName() const;
		std::string	getRealName() const;
		std::string	getPassWord() const;
		bool		getStatus() const;

		void		setStatus(bool status);

		void		parseInput(std::string input);
		void		printInfos();
	private:
		std::string	userName;
		std::string nickName;
		std::string realName;
		std::string passWord;
		bool		isConnected;
};


#endif