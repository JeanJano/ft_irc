#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class User;

class Channel
{
	public:
		Channel();
		Channel(std::string n, std::string p);
		Channel(const Channel &cpy);
		~Channel();
		Channel& operator=(const Channel &cpy);

		void	addUser(User *user);
		void	removeUser(User *user);
		void	addMessage(std::string msg);

		std::string					getName();
		std::string					getPass();
		std::vector<User>			getMembers();
		std::vector<std::string>	getMessages();

	private:
		std::string					name;
		std::string					pass;
		std::vector<User>			members;
		std::vector<std::string>	messages;
};

#endif