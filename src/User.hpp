#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User
{
	public:
		User();
		User(const User& p);
		~User();
		User& operator=(const User& p);

		

	private:
		std::string nickname;
		std::string	usernme;
};


#endif