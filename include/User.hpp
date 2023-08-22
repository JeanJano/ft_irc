/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-kass <zel-kass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 17:05:46 by jsauvage          #+#    #+#             */
/*   Updated: 2023/08/22 11:19:07 by zel-kass         ###   ########.fr       */
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
		User&	operator=(const User &cpy);
		bool	operator==(const User &cmp) const;

		std::string	getUserName() const;
		std::string	getNickName() const;
		std::string	getRealName() const;
		std::string	getPassWord() const;
		std::string	getIp() const;
		int			getSd() const;
		bool		getStatus() const;

		void		setStatus(bool status);
		void		setIp(std::string ip);

		void		parseInput(std::string input);
		void		printInfos();
	private:
		std::string	userName;
		std::string nickName;
		std::string realName;
		std::string passWord;
		std::string	ip;
		bool		isConnected;
		int			sd;
};


#endif