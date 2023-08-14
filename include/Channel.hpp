#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"
# include "User.hpp"

class Channel
{
	public:
		Channel(std::string n);
		Channel(const Channel &cpy);
		~Channel();
		Channel& operator=(const Channel &cpy);

		void	addUser(User *user);
		void	removeUser(User *user);
	private:
		std::string			name;
		std::vector<User>	members;
};

#endif