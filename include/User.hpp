/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smessal <smessal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:05:46 by jsauvage          #+#    #+#             */
/*   Updated: 2023/08/15 18:27:39 by smessal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User
{
	public:
		User();
		User(size_t sd);
		User(const User &cpy);
		~User();
		User& operator=(const User &cpy);

		std::string	getUserName() const;
		std::string	getNickName() const;
		std::string	getRealName() const;
		std::string	getPassWord() const;
		int			getSd() const;
		bool		getStatus() const;

		void		setStatus(bool status);

		void		parseInput(std::string input);
		void		printInfos();
	private:
		std::string	userName;
		std::string nickName;
		std::string realName;
		std::string passWord;
		int			sd;
		bool		isConnected;
};


#endif